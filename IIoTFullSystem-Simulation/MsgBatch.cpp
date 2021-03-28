// -----------------------------------------------------------------------------
/// \file                  MsgBatch.cpp
/// \brief                 MsgBatch class implementation
/// \author                Sreejith.Naarakathil@gmail.com
/// \date                  27/3/2021
// -----------------------------------------------------------------------------

#include "MsgBatch.h"
#include "Item.h"

// -----------------------------------------------------------------------------
/// \brief                 Constructor
///                        Creates a message batch with a predefined order.
///                        Later it is randomized if configured so.
/// \param                 None
/// \return                None
// -----------------------------------------------------------------------------
MsgBatch::MsgBatch()
{
   // Model the required messaging protocol and field bus behaviour
   
   // C _ A B A ->
   
   Msgs.emplace_back(make_unique<Item>(Type::C));
   Msgs.emplace_back(make_unique<Item>(Type::X));
   Msgs[1].reset(); // Make it nullptr to represet contains nothing. Used to simulate a simple bus arbitration logic.
   Msgs.emplace_back(make_unique<Item>(Type::A));
   Msgs.emplace_back(make_unique<Item>(Type::B));
   Msgs.emplace_back(make_unique<Item>(Type::A));


   // C A _ B A ->
/*   
   Msgs.emplace_back(make_unique<Item>(Type::C));
   Msgs.emplace_back(make_unique<Item>(Type::A));
   Msgs.emplace_back(make_unique<Item>(Type::X));
   Msgs[2].reset(); // Make it nullptr to represet contains nothing. Used to simulate a simple bus arbitration logic.
   Msgs.emplace_back(make_unique<Item>(Type::B));
   Msgs.emplace_back(make_unique<Item>(Type::A));
*/
   
   // _ C _ B _ A ->
/*
   Msgs.emplace_back(make_unique<Item>(Type::X));
   Msgs[0].reset(); // Make it nullptr to represet contains nothing. Used to simulate a simple bus arbitration logic.
   Msgs.emplace_back(make_unique<Item>(Type::C));
   Msgs.emplace_back(make_unique<Item>(Type::X));
   Msgs[2].reset(); // Make it nullptr to represet contains nothing. Used to simulate a simple bus arbitration logic.
   Msgs.emplace_back(make_unique<Item>(Type::B));
   Msgs.emplace_back(make_unique<Item>(Type::X));
   Msgs[4].reset(); // Make it nullptr to represet contains nothing. Used to simulate a simple bus arbitration logic.
   Msgs.emplace_back(make_unique<Item>(Type::A));
*/
}
