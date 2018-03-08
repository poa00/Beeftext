/// \file
/// \author Xavier Michelon
///
/// \brief Implementation of sort and filter proxy model for combos
///  
/// Copyright (c) Xavier Michelon. All rights reserved.  
/// Licensed under the MIT License. See LICENSE file in the project root for full license information.  


#include "stdafx.h"
#include "ComboSortFilterProxyModel.h"
#include "ComboList.h"


//**********************************************************************************************************************
/// \param[in] parent The parent object of the proxy model
//**********************************************************************************************************************
ComboSortFilterProxyModel::ComboSortFilterProxyModel(QObject* parent)
   : QSortFilterProxyModel(parent)
{
   this->setSortCaseSensitivity(Qt::CaseInsensitive);
   this->setFilterCaseSensitivity(Qt::CaseInsensitive);
}


//**********************************************************************************************************************
/// \param[in] group
//**********************************************************************************************************************
void ComboSortFilterProxyModel::setGroup(SPGroup const& group)
{
   group_ = group;
   this->invalidateFilter();
}


//**********************************************************************************************************************
/// \param[in] sourceRow The row index in the source model
/// \param[in] index Unused
//**********************************************************************************************************************
bool ComboSortFilterProxyModel::filterAcceptsRow(int sourceRow, QModelIndex const& index) const
{
   ComboList* combos = dynamic_cast<ComboList*>(sourceModel());
   Q_ASSERT(combos);
   for (int col = 0; col < combos->columnCount(); ++col)
   {
      SPCombo combo = (*combos)[sourceRow];
      if (combo->group() != group_)
         return false;
      QString const str = combos->data(combos->index(sourceRow, col)).toString();
      if (str.contains(this->filterRegExp()))
         return true;
   }
   return false;
}
