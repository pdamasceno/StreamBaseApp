#include <string>

#ifndef STREAMBASEAPP_HEADER_H
#define STREAMBASEAPP_HEADER_H

namespace communication
{
   class Header
   {
   public:
      enum MessageType
      {
         CommonMessage = 100,
         Command = 101,
         InvalidMessage = 102
      };
      Header();
      Header(Header::MessageType messageType);
      virtual ~Header() = default;

      Header::MessageType GetMessageType() const;
   private:
      Header::MessageType m_messageType;
   };
}

#endif //STREAMBASEAPP_HEADER_H
