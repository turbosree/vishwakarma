// -----------------------------------------------------------------------------
/// \file                  FieldBus.cpp
/// \brief                 FieldBus class implementation
/// \author                Sreejith.Naarakathil@gmail.com
/// \date                  27/3/2021
// -----------------------------------------------------------------------------

#include "FieldBus.h"
#include "Item.h"

#include <utility>

// -----------------------------------------------------------------------------
/// \brief                 Load an item (message) to the fieldbus system 
/// \param                 ptr a unique pointer to the item being loaded 
/// \return                Nothing
// -----------------------------------------------------------------------------
void FieldBus::LoadItem(unique_ptr<Item> ptr)
{
   pCurrentItem = move(ptr);
}
