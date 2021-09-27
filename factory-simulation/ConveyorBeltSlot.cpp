// -----------------------------------------------------------------------------
/// \file                  ConveyorBeltSlot.cpp
/// \brief                 ConveyorBeltSlot class implementation
/// \author                Sreejith.Naarakathil@gmail.com
/// \date                  23/1/2021
// -----------------------------------------------------------------------------

#include "ConveyorBeltSlot.h"
#include "Item.h"

#include <utility>

// -----------------------------------------------------------------------------
/// \brief                 Load an item to the conveyor slot 
/// \param                 ptr a unique pointer to the item being loaded 
/// \return                Nothing
// -----------------------------------------------------------------------------
void ConveyorBeltSlot::LoadItem(unique_ptr<Item> ptr)
{
   pCurrentItem = move(ptr);
}
