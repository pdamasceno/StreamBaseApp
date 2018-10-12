#include "Header.h"

namespace communication
{
   Header::Header() :
      m_messageType(Header::MessageType::InvalidMessage)
   {
   }

   Header::Header(Header::MessageType messageType) :
      m_messageType(messageType)
   {
   }

   Header::MessageType Header::GetMessageType() const
   {
      return m_messageType;
   }
}

