/// \file
/// \author Xavier Michelon
///
/// \brief Declaration of combo table frame
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#ifndef BEEFTEXT__COMBO__TABLE__FRAME__H
#define BEEFTEXT__COMBO__TABLE__FRAME__H


#include "ui_ComboTableFrame.h"


//**********************************************************************************************************************
/// \brief A class for the frame containing the combo table and associated controls
//**********************************************************************************************************************
class ComboTableFrame: public QFrame
{
   Q_OBJECT
public: // member functions
	ComboTableFrame(QWidget* parent = nullptr); ///< Default constructor
	~ComboTableFrame() = default; ///< Default destructor
	
private: // member functions
	ComboTableFrame(ComboTableFrame const&) = delete; ///< Disabled copy constructor
	ComboTableFrame(ComboTableFrame&&) = delete; ///< Disabled move constructor
	ComboTableFrame& operator=(ComboTableFrame const&) = delete; ///< Disabled assignment operator
	ComboTableFrame& operator=(ComboTableFrame&&) = delete; ///< Disabled move assignment operator
   void setupTable(); ///< Setup the combo table
   qint32 getSelectedComboCount() const; ///< Get the number of selected combo in the combo table
   QList<qint32> getSelectedComboIndexes() const; ///< Retrieve the list indexes of the selected combos

private slots:
   void updateGui(); ///< Update the GUI state
   void onActionAddCombo(); ///< Slot for the 'Add Combo' action
   void onActionDeleteCombo(); ///< Slot for the 'Delete Combo' action
   void onActionEditCombo(); ///< Slot for the 'Edit Combo' action

private: // data members
   Ui::ComboTableFrame ui_; ///< The GUI for the frame
   QSortFilterProxyModel proxyModel_; ///< The proxy model for sorting/filtering the combo table

};

#endif // #ifndef BEEFTEXT__COMBO__TABLE__FRAME__H
