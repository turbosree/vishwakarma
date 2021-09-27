// -----------------------------------------------------------------------------
/// \file                  ComponentBatch.cpp
/// \brief                 ComponentBatch class implementation
/// \author                Sreejith.Naarakathil@gmail.com
/// \date                  23/1/2021
// -----------------------------------------------------------------------------

#include "ComponentBatch.h"
#include "Item.h"

// -----------------------------------------------------------------------------
/// \brief                 Constructor
///                        Creates a component batch with a predefined order.
///                        Later it is randomized if configured so.
/// \param                 None
/// \return                None
// -----------------------------------------------------------------------------
ComponentBatch::ComponentBatch()
{
   Components.emplace_back(make_unique<Item>(Type::C));
   Components.emplace_back(make_unique<Item>(Type::A));
   Components[1].reset(); // Make it nullptr to represet contains nothing
   Components.emplace_back(make_unique<Item>(Type::A));
   Components.emplace_back(make_unique<Item>(Type::B));
   Components.emplace_back(make_unique<Item>(Type::A));
}
