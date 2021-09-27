// -----------------------------------------------------------------------------
/// \file                  ComponentBatch.h
/// \brief                 ComponentBatch class
/// \author                Sreejith.Naarakathil@gmail.com
/// \date                  23/1/2021
// -----------------------------------------------------------------------------

#ifndef _ComponentBatch
#define _ComponentBatch

#include <memory>
#include <vector>

using namespace std;

class Item;

// -----------------------------------------------------------------------------
/// \class                 ComponentBatch
/// \brief                 ComponentBatch class represents a batch of component 
///                        used to load at the start of assembly line
// -----------------------------------------------------------------------------
class ComponentBatch
{
 public:
   vector<unique_ptr<Item>> Components;
   ComponentBatch();
};

#endif // _ComponentBatch
