// -----------------------------------------------------------------------------
/// \file                  ConveyorBeltSlot.h
/// \brief                 ConveyorBeltSlot class
/// \author                Sreejith.Naarakathil@gmail.com
/// \date                  23/1/2021
// -----------------------------------------------------------------------------

#ifndef _ConveyorBeltSlot
#define _ConveyorBeltSlot

#include <memory>

using namespace std;

class Item;

// -----------------------------------------------------------------------------
/// \class                 ConveyorBeltSlot
/// \brief                 ConveyorBeltSlot class represents a slot on 
///                        the conveyor belt
// -----------------------------------------------------------------------------
class ConveyorBeltSlot
{
 public:
   unique_ptr<Item> pCurrentItem;
   void LoadItem(unique_ptr<Item> ptr);   
};


#endif // _ConveyorBeltSlot
