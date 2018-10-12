#include "CommonMessage.h"

namespace communication
{
   CommonMessage::CommonMessage() :
      Header(Header::MessageType::CommonMessage),
      m_content("")
   {
   }

   void CommonMessage::SetContent(std::string content)
   {
      m_content = content;
   }

   std::string CommonMessage::GetContent() const&
   {
      return m_content;
   }
}