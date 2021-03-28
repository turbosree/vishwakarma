// -----------------------------------------------------------------------------
/// \file                  FieldBusDevice.h
/// \brief                 FieldBusDevice class
/// \author                Sreejith.Naarakathil@gmail.com
/// \date                  27/3/2021
// -----------------------------------------------------------------------------

#ifndef _FieldBusDevice
#define _FieldBusDevice

#include <memory>

using namespace std;

class Item;

// -----------------------------------------------------------------------------
/// \class                 FieldBusDevice
/// \brief                 FieldBusDevice class represents a bus device on 
///                        connected to the fieldbus
// -----------------------------------------------------------------------------
class FieldBusDevice
{
 public:
   enum class State{ReadyToProcess, ProcessedA, ProcessedB, ProcessedC, ReadyToSendP, ReadyToSendQ};
   State state = State::ReadyToProcess;

   // TODO: Place holder to model preconditions
   unique_ptr<Item> pItem1;
   unique_ptr<Item> pItem2;

   // Statemachine implementation representing the device/sensor function and it's method of operation. Different type of devices can create their own behaviours.
   // Eg: Create a response data message P or Q if both preconditions are met.
   // Return a response data message or a null pointer to inform a message is already consumed by this device.
   unique_ptr<Item> ProcessMessages(unique_ptr<Item> ptr);
};

#endif // _FieldBusDevice
