#include "Writer.h"
#include "Command.h"
#include "CommonMessage.h"
#include <sstream>

namespace communication
{
   std::string Writer::WriteAll(std::shared_ptr<communication::Header> base)
   {
      std::string separator = ";";
      std::stringstream message;

      // Serialize communication object based on message type
      switch (base->GetMessageType())
      {
         case Header::MessageType::CommonMessage:
         {
            auto derived = std::dynamic_pointer_cast<communication::CommonMessage>(base);

            if (derived == nullptr)
            {
               // dynamic_pointer_cast returned nullptr
               throw;
            }

            // Serialization
            message << derived->GetMessageType() << separator << derived->GetContent();

            return message.str();
         }
         case Header::MessageType::Command:
         {
            auto derived = std::dynamic_pointer_cast<communication::Command>(base);

            if (derived == nullptr)
            {
               // dynamic_pointer_cast returned nullptr
               throw;
            }

            // Serialization
            message << derived->GetMessageType() << separator << derived->GetCommandType() << separator << derived->GetGameGenre() << separator
                    << derived->GetGameName() << separator << derived->GetTotalTimePlayed();

            return message.str();
         }
         default:
         {
            return "Invalid";
         }
      }
   }
}