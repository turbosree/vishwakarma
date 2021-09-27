// -----------------------------------------------------------------------------
/// \file                  WorkerStation.h
/// \brief                 WorkerStation class
/// \author                Sreejith.Naarakathil@gmail.com
/// \date                  23/1/2021
// -----------------------------------------------------------------------------

#ifndef _WorkerStation
#define _WorkerStation

#include <memory>

using namespace std;

class Item;

// -----------------------------------------------------------------------------
/// \class                 WorkerStation
/// \brief                 WorkerStation class represents a workstation on 
///                        either sides of the conveyor belt
// -----------------------------------------------------------------------------
class WorkerStation
{
 public:
   enum class State{ReadyToPick, PickedA, PickedB, PickedC, ReadyToPlaceP, ReadyToPlaceQ};
   State state = State::ReadyToPick;
   unique_ptr<Item> pItem1;
   unique_ptr<Item> pItem2;

   // Statemachine implementation representing the assembling/manufacturing method.
   // Assemble products if both components are available.
   // Return assembled product or a null pointer to inform a component is already consumed by this work station.
   unique_ptr<Item> AssembleProduct(unique_ptr<Item> ptr);
};

#endif // _WorkerStation
