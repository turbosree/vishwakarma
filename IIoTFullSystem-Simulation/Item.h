// -----------------------------------------------------------------------------
/// \file                  Item.h
/// \brief                 Item class
/// \author                Sreejith.Naarakathil@gmail.com
/// \date                  27/3/2021
// -----------------------------------------------------------------------------

#ifndef _Item
#define _Item

#include <iostream>

#include "SimulationDataTypes.h"

using namespace std;

// -----------------------------------------------------------------------------
/// \class                 Item
/// \brief                 Item class represents a generic item managed in 
///                        the fieldbus control network. Eg: a message
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
