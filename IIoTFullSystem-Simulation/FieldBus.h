// -----------------------------------------------------------------------------
/// \file                  FieldBus.h
/// \brief                 FieldBus class
/// \author                Sreejith.Naarakathil@gmail.com
/// \date                  27/3/2021
// -----------------------------------------------------------------------------

#ifndef _FieldBus
#define _FieldBus

#include <memory>

class Item;

// -----------------------------------------------------------------------------
/// \class                 FieldBus
/// \brief                 FieldBus class represents a port on 
///                        the Fieldbus control network
// -----------------------------------------------------------------------------
class FieldBus
{
 public:
   std::unique_ptr<Item> pCurrentItem;
   void LoadItem(std::unique_ptr<Item> ptr);   
};


#endif // _FieldBus
