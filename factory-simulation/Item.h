// -----------------------------------------------------------------------------
/// \file                  Item.h
/// \brief                 Item class
/// \author                Sreejith.Naarakathil@gmail.com
/// \date                  23/1/2021
// -----------------------------------------------------------------------------

#ifndef _Item
#define _Item

#include "SimulationDataTypes.h"

// -----------------------------------------------------------------------------
/// \class                 Item
/// \brief                 Item class represents an item managed in 
///                        assembly line
// -----------------------------------------------------------------------------
class Item
{
 public:
   Type Name;
   static int CreatedCount;
   static int DestroyedCount;

 Item(Type type)
    :Name(type)
   {
      CreatedCount++;
   }
   ~Item()
      {
         DestroyedCount++;
         //cout<<static_cast<underlying_type<Type>::type>(Name)<<" Item destructor is called"<<endl;
      }
};

#endif // _Item
