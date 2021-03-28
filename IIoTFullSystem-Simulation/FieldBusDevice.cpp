// -----------------------------------------------------------------------------
/// \file                  FieldBusDevice.cpp
/// \brief                 FieldBusDevice class implementation
/// \author                Sreejith.Naarakathil@gmail.com
/// \date                  27/3/2021
// -----------------------------------------------------------------------------

#include "FieldBusDevice.h"
#include "Item.h"

// -----------------------------------------------------------------------------
/// \brief                 Function to implement the device/sensor statemachine
/// \param                 ptr is the unique pointer to the current item (Eg: message) under consideration
/// \return                a unique pointer which is either an unsed message, 
///                        response message or nullptr. nullptr represents that
///                        the message was consumed.
// -----------------------------------------------------------------------------
unique_ptr<Item> FieldBusDevice::ProcessMessages(unique_ptr<Item> ptr)
{
   State tmp = state; // Note current state
   if(ptr != nullptr)
   {
      switch(state)
      {
      case State::ReadyToProcess:
         if(ptr->Name == Type::A)
         {
            if(pItem1 == nullptr)
            {
               pItem1 = move(ptr); ptr = make_unique<Item>(Type::X);
               state = State::ProcessedA;
            }
         }
         else if((ptr->Name == Type::B))
         {
            if(pItem2 == nullptr)
            {
               pItem2 =  move(ptr); ptr = make_unique<Item>(Type::X);
               state = State::ProcessedB;
            }
         }
         else if((ptr->Name == Type::C))
         {
            if(pItem2 == nullptr)
            {
               pItem2 =  move(ptr); ptr = make_unique<Item>(Type::X);
               state = State::ProcessedC;
            }
         }
         break;
      case State::ProcessedA:
         if((ptr->Name == Type::B))
         {
            if(pItem2 == nullptr)
            {
               pItem2 =  move(ptr); ptr = make_unique<Item>(Type::X);
               state = State::ProcessedB;
            }
         }
         else if((ptr->Name == Type::C))
         {
            if(pItem2 == nullptr)
            {
               pItem2 =  move(ptr); ptr = make_unique<Item>(Type::X);
               state = State::ProcessedC;
            }
         }
         if(pItem1 != nullptr && pItem2 != nullptr)
         {
            state = (pItem2->Name == Type::B) ? State::ReadyToSendP : State::ReadyToSendQ;
         }
         break;
      case State::ProcessedB:
         if((ptr->Name == Type::A))
         {
            if(pItem1 == nullptr)
            {
               pItem1 =  move(ptr); ptr = make_unique<Item>(Type::X);
               state = State::ProcessedA;
            }
         }
         if(pItem1 != nullptr && pItem2 != nullptr)
         {
            state = (pItem2->Name == Type::B) ? State::ReadyToSendP : State::ReadyToSendQ;
         }
         break;
      case State::ProcessedC:
         if((ptr->Name == Type::A))
         {
            if(pItem1 == nullptr)
            {
               pItem1 =  move(ptr); ptr = make_unique<Item>(Type::X);
               state = State::ProcessedA;
            }
         }
         if(pItem1 != nullptr && pItem2 != nullptr)
         {
            state = (pItem2->Name == Type::B) ? State::ReadyToSendP : State::ReadyToSendQ;
         }
         break;
      default:
         // TODO
         break;
      }
   }
   else
   {
      // We have an time window now to transmit response, is there a response data message ready?
      // If a response data message is ready, we can delete the messages received (A and 'B or C') to represent they are processed. 
      switch(state)
      {
      case State::ReadyToSendP:
         if(pItem1 != nullptr && pItem2 != nullptr)
         {
            ptr.reset();
            pItem1.reset();
            pItem2.reset();
            ptr = make_unique<Item>(Type::P);
            state = State::ReadyToProcess;
         }
         break;
      case State::ReadyToSendQ:
         if(pItem1 != nullptr && pItem2 != nullptr)
         {
            ptr.reset();
            pItem1.reset();
            pItem2.reset();
            ptr = make_unique<Item>(Type::Q);
            state = State::ReadyToProcess;
         }
         break;
      default:
         break;
      }
   }
   
   //Send a response message to the bus. If not ready, ptr will be nullptr, representing a message is already taken by this node. Eg: The message was destined for this devide using a destination address in a protocol message.
   return move(ptr); 
}
