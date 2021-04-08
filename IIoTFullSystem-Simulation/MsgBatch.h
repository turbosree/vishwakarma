// -----------------------------------------------------------------------------
/// \file                  MsgBatch.h
/// \brief                 MsgBatch class
/// \author                Sreejith.Naarakathil@gmail.com
/// \date                  27/3/2021
// -----------------------------------------------------------------------------

#ifndef _MsgBatch
#define _MsgBatch

#include <memory>
#include <vector>

class Item;

// -----------------------------------------------------------------------------
/// \class                 MsgBatch
/// \brief                 MsgBatch class represents a batch of messages 
///                        to represent a protocol or a message sequence
// -----------------------------------------------------------------------------
class MsgBatch
{
 public:
   std::vector<std::unique_ptr<Item>> Msgs;
   MsgBatch();
};

#endif // _MsgBatch
