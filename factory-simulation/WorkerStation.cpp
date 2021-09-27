// -----------------------------------------------------------------------------
/// \file                  WorkerStation.cpp
/// \brief                 WorkerStation class implementation
/// \author                Sreejith.Naarakathil@gmail.com
/// \date                  23/1/2021
// -----------------------------------------------------------------------------

#include "WorkerStation.h"
#include "Item.h"

// -----------------------------------------------------------------------------
/// \brief                 Function to implement the assembly procedure statemachine
/// \param                 ptr is the unique pointer to the current item under consideration
/// \return                a unique pointer which is either an unsed component, 
///                        finished product or nullptr. nullptr represents that
///                        the component was consumed.
// -----------------------------------------------------------------------------
unique_ptr<Item> WorkerStation::AssembleProduct(unique_ptr<Item> ptr)
{
   State tmp = state; // Note current state
   if(ptr != nullptr)
   {
      switch(state)
      {
      case State::ReadyToPick:
         if(ptr->Name == Type::A)
         {
            if(pItem1 == nullptr)
            {
               pItem1 = move(ptr); ptr = make_unique<Item>(Type::X);
               state = State::PickedA;
            }
         }
         else if((ptr->Name == Type::B))
         {
            if(pItem2 == nullptr)
            {
               pItem2 =  move(ptr); ptr = make_unique<Item>(Type::X);
               state = State::PickedB;
            }
         }
         else if((ptr->Name == Type::C))
         {
            if(pItem2 == nullptr)
            {
               pItem2 =  move(ptr); ptr = make_unique<Item>(Type::X);
               state = State::PickedC;
            }
         }
         break;
      case State::PickedA:
         if((ptr->Name == Type::B))
         {
            if(pItem2 == nullptr)
            {
               pItem2 =  move(ptr); ptr = make_unique<Item>(Type::X);
               state = State::PickedB;
            }
         }
         else if((ptr->Name == Type::C))
         {
            if(pItem2 == nullptr)
            {
               pItem2 =  move(ptr); ptr = make_unique<Item>(Type::X);
               state = State::PickedC;
            }
         }
         if(pItem1 != nullptr && pItem2 != nullptr)
         {
            state = (pItem2->Name == Type::B) ? State::ReadyToPlaceP : State::ReadyToPlaceQ;
         }
         break;
      case State::PickedB:
         if((ptr->Name == Type::A))
         {
            if(pItem1 == nullptr)
            {
               pItem1 =  move(ptr); ptr = make_unique<Item>(Type::X);
               state = State::PickedA;
            }
         }
         if(pItem1 != nullptr && pItem2 != nullptr)
         {
            state = (pItem2->Name == Type::B) ? State::ReadyToPlaceP : State::ReadyToPlaceQ;
         }
         break;
      case State::PickedC:
         if((ptr->Name == Type::A))
         {
            if(pItem1 == nullptr)
            {
               pItem1 =  move(ptr); ptr = make_unique<Item>(Type::X);
               state = State::PickedA;
            }
         }
         if(pItem1 != nullptr && pItem2 != nullptr)
         {
            state = (pItem2->Name == Type::B) ? State::ReadyToPlaceP : State::ReadyToPlaceQ;
         }
         break;
      default:
         // It could be a assembled product, don't touch it!
         // Or it could be component, but statemachine is waiting for an empty slot to place P or Q. So don't touch it!
         break;
      }
   }
   else
   {
      // We have an empty slot now, is there a assembled product read?
      // If an assembled product is ready, we can delete the component objects (A and 'B or C') to represent they are consumed. 
      switch(state)
      {
      case State::ReadyToPlaceP:
         if(pItem1 != nullptr && pItem2 != nullptr)
         {
            ptr.reset();
            pItem1.reset();
            pItem2.reset();
            ptr = make_unique<Item>(Type::P);
            state = State::ReadyToPick;
         }
         break;
      case State::ReadyToPlaceQ:
         if(pItem1 != nullptr && pItem2 != nullptr)
         {
            ptr.reset();
            pItem1.reset();
            pItem2.reset();
            ptr = make_unique<Item>(Type::Q);
            state = State::ReadyToPick;
         }
         break;
      default:
         break;
      }
   }
   
   //Place assembled product on conveyor. If not ready, ptr will be nullptr, representing a component is already taken by this work station.
   return move(ptr); 
}
