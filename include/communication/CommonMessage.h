#include "Header.h"

#ifndef STREAMBASEAPP_MESSAGE_H
#define STREAMBASEAPP_MESSAGE_H

namespace communication
{
   class CommonMessage : public Header
   {
   public:
      CommonMessage();
      ~CommonMessage() = default;

      void SetContent(std::string content);
      std::string GetContent() const&;

   private:
      std::string m_content;
   };
}

#endif //STREAMBASEAPP_MESSAGE_H
