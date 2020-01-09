/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of dialog for creating/editing a combo
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "../Group/GroupDialog.h"
#include "ComboDialog.h"
#include "ComboManager.h"
#include <XMiLib/Exception.h>
#include <XMiLib/XMiLibConstants.h>
#include "PreferencesManager.h"


namespace {


//**********************************************************************************************************************
/// \param[in] keyword The keyword.
/// \param[in] parent The parent widget of the dialog.
/// \return true if and only if the user decided to proceed with the short keyword.
//**********************************************************************************************************************
bool showShortKeywordConfirmationDialog(QString const& keyword, QWidget* parent = nullptr)
{
   QMessageBox msgBox(parent);
   msgBox.setText(QObject::tr("The keyword is very short. Are you sure you want to use the keyword '%1'?")
      .arg(keyword));
   msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
   msgBox.setDefaultButton(QMessageBox::No);
   msgBox.setEscapeButton(QMessageBox::No);
   QCheckBox* check = new QCheckBox(QObject::tr("Do not show this warning again."), &msgBox);
   msgBox.setCheckBox(check);
   qint32 const button = msgBox.exec();
   PreferencesManager::instance().setWarnAboutShortComboKeywords(!check->isChecked());
   return (QMessageBox::Yes == button);
}


}


//**********************************************************************************************************************
/// \param[in] combo The combo
/// \param[in] title The title to display in the dialog's title bar
/// \param[in] parent The parent widget of the dialog
/// \return true if and only if the user validated the dialog
//**********************************************************************************************************************
bool ComboDialog::run(SpCombo& combo, QString const& title, QWidget* parent)
{
   return Accepted == ComboDialog(combo, title, parent).exec();
}


//**********************************************************************************************************************
/// \param[in] combo The combo
/// \param[in] title The title to display in the dialog's title bar
/// \param[in] parent The parent widget of the dialog
//**********************************************************************************************************************
ComboDialog::ComboDialog(SpCombo const& combo, QString const& title, QWidget* parent)
   : QDialog(parent, xmilib::constants::kDefaultDialogFlags | Qt::WindowMaximizeButtonHint)
   , combo_(combo)
{
   ComboManager::instance().groupListRef().ensureNotEmpty();
   if (!combo)
      throw xmilib::Exception("%1(): combo is null.");
   ui_.setupUi(this);
   this->setWindowTitle(title);
   ui_.editName->setText(combo->name());
   ui_.comboGroup->setContent(ComboManager::instance().groupListRef());
   ui_.comboGroup->setCurrentGroup(combo_->group());
   this->setMatchingComboValue(combo->useLooseMatching());
   ui_.editKeyword->setText(combo->keyword());
   ui_.editKeyword->setValidator(&validator_);
   bool const useHtml = combo->useHtml();
   ui_.comboEditor->setRichTextMode(useHtml);
   this->setUseHtmlComboValue(useHtml);
   if (useHtml)
      ui_.comboEditor->snippetEdit()->setHtml(combo->snippet());
   else
      ui_.comboEditor->snippetEdit()->setPlainText(combo->snippet());
   this->updateGui();
}


//**********************************************************************************************************************
/// \return true if and only if the combo is valid
//**********************************************************************************************************************
bool ComboDialog::checkAndReportInvalidCombo()
{
   if (ui_.comboEditor->plainText().isEmpty())
   {
      QMessageBox::critical(this, tr("Error"), tr("The snippet text is empty."));
      return false;
   }
   QString text = ui_.editKeyword->text();
   if (QValidator::Acceptable != validator_.validate(text))
   {
      QMessageBox::critical(this, tr("Error"), tr("The keyword is invalid."));
      return false;
   }

   SpGroup const group = ui_.comboGroup->currentGroup();
   if (!group)
   {
      QMessageBox::critical(this, tr("Error"), tr("The group is invalid."));
      return false;
   }

   // we check that the keyword is not already in use
   QString const newKeyword = ui_.editKeyword->text();
   ComboList const& comboList = ComboManager::instance().comboListRef();
   if (comboList.end() != std::find_if(comboList.begin(), comboList.end(), [&](SpCombo const& existing) -> bool
      { return (existing != combo_) && (existing->keyword() == newKeyword);}))
      return 0 == QMessageBox::information(this, tr("Duplicate keyword"), tr("This keyword is already in use. \n\n"
         "You can have multiple combos with the same keyword, Beeftext will pick one of the matching combos "
         "randomly."), tr("&Continue"), tr("C&ancel"), QString());

   // we check for conflicts that would make some combo 'unreachable'
   qint32 const conflictCount = std::count_if(comboList.begin(), comboList.end(), [&](SpCombo const& existing) -> bool 
   { return (existing != combo_) && (existing->keyword().startsWith(newKeyword) || 
      newKeyword.startsWith(existing->keyword())); });
   if (!conflictCount)
      return true;
   QString const singleConflictStr = tr("An existing combo is creating a conflict with this combo.");
   QString const multipleConflictStr = tr("%1 existing combos are creating conflicts with this combo.")
      .arg(conflictCount);
   return QMessageBox::Yes == QMessageBox::question(this, tr("Conflict"), tr("%1 If you use automatic substitution, "
      "conflicts make some combos impossible to trigger.\n\nDo you want to continue anymay?")
      .arg(conflictCount > 1 ? multipleConflictStr : singleConflictStr), QMessageBox::Yes | QMessageBox::No, 
      QMessageBox::No);
}


//**********************************************************************************************************************
/// \param[in] useLooseMatching true if the combo uses loose matching
//**********************************************************************************************************************
void ComboDialog::setMatchingComboValue(bool const useLooseMatching) const
{
   ui_.comboMatching->setCurrentIndex(useLooseMatching ? 1 : 0);
}


//**********************************************************************************************************************
/// \return true if the 'Loose' item is selected
//**********************************************************************************************************************
bool ComboDialog::matchingComboValue() const
{
   return 1 == ui_.comboMatching->currentIndex();
}


//**********************************************************************************************************************
/// \param[in] useHtml The value of the 'Use HTML' combo.
//**********************************************************************************************************************
void ComboDialog::setUseHtmlComboValue(bool useHtml) const
{
   ui_.comboUseHtml->setCurrentIndex(useHtml ? 1 : 0);
}


//**********************************************************************************************************************
/// \return The value of the 'Use Html' combo.
//**********************************************************************************************************************
bool ComboDialog::useHtmlComboValue() const
{
   return 0 != ui_.comboUseHtml->currentIndex();
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ComboDialog::onActionOk()
{
   Q_ASSERT(combo_);
   if (!checkAndReportInvalidCombo())
      return;
   QString const keyword = ui_.editKeyword->text().trimmed();
   if (PreferencesManager::instance().warnAboutShortComboKeywords() && (keyword.size() < 3) 
      && (!showShortKeywordConfirmationDialog(keyword, this)))
      return;
   combo_->setName(ui_.editName->text().trimmed());
   combo_->setGroup(ui_.comboGroup->currentGroup());
   combo_->setUseLooseMatching(this->matchingComboValue());
   combo_->setKeyword(keyword);
   bool const useHtml = this->useHtmlComboValue();
   combo_->setUseHtml(useHtml);
   combo_->setSnippet(useHtml ? ui_.comboEditor->html() : ui_.comboEditor->plainText());
   this->accept();
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ComboDialog::onActionNewGroup()
{
   SpGroup group = std::make_shared<Group>(QString());
   if ((!GroupDialog::run(group, tr("New Group"), this)) || !group)
      return;
   ComboManager::instance().comboListRef().groupListRef().append(group);
   ui_.comboGroup->setContent(ComboManager::instance().groupListRef());
   ui_.comboGroup->setCurrentGroup(group);
}


//**********************************************************************************************************************
// 
//**********************************************************************************************************************
void ComboDialog::updateGui() const
{
   QString keyword = ui_.editKeyword->text();
   bool const canAccept = (QValidator::Acceptable == validator_.validate(keyword)) &&
      (!ui_.comboEditor->plainText().isEmpty()) && ui_.comboGroup->currentGroup();
   ui_.buttonOk->setEnabled(canAccept);
   ui_.labelEditor->setVisible(this->useHtmlComboValue());
}


//**********************************************************************************************************************
//
//**********************************************************************************************************************
void ComboDialog::onUseHtmlChanged() const
{
   bool const useHtml = this->useHtmlComboValue();
   ui_.comboEditor->setRichTextMode(useHtml);
   if (!useHtml)
   {
      QTextEdit& edit = *ui_.comboEditor->snippetEdit();
      // note: normally, the code above could simply be edit.setPlainText(edit.toPlainText(), but a Qt issue
      // causes the format not to be properly removed in some cases.
      edit.document()->setPlainText(edit.toPlainText());
   }
   this->updateGui();
}


