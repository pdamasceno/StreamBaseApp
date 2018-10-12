#include "Reader.h"
#include "Command.h"
#include "CommonMessage.h"
#include <sstream>
#include <vector>

namespace communication
{
   std::shared_ptr<communication::Header> Reader::ReadAll(std::string const& message)
   {
      std::vector<std::string> attributes;
      std::string attribute;
      std::istringstream attributeStream(message);

      // Split serialized message based on ';' separator
      while (std::getline(attributeStream, attribute, ';'))
      {
         attributes.push_back(attribute);
      }

      // Communication protocol always starts with message type
      int messageType = std::stoi(attributes.at(0));

      // Instantiate communication object based on message type and fill it
      switch (static_cast<Header::MessageType >(messageType))
      {
         case Header::MessageType::CommonMessage:
         {
            CommonMessage packet;

            // Only set content if client sent any content
            if (attributes.size() > 1)
            {
               packet.SetContent(attributes.at(1));
            }

            return std::make_shared<communication::CommonMessage>(packet);
         }
         case Header::MessageType::Command:
         {
            // Get command type to know how to fill the command object
            auto commandType = static_cast<Command::CommandType>(std::stoi(attributes.at(1)));
            Command packet(commandType);

            if (commandType == Command::CommandType::CreateGame)
            {
               packet.SetGameGenre(attributes.at(2));
               packet.SetGameName(attributes.at(3));
            }

            if (commandType == Command::CommandType::GetGame)
            {
               packet.SetGameName(attributes.at(3));
            }

            if (commandType == Command::CommandType::DeleteGame)
            {
               packet.SetGameName(attributes.at(3));
            }

            if (commandType == Command::CommandType::UpdateTotalTimePlayed)
            {
               packet.SetGameName(attributes.at(3));
               packet.SetTotalTimePlayed(std::stoi(attributes.at(4)));
            }

            // For GetGameCollection there is only need for the command type attribute to be filled

            return std::make_shared<communication::Command>(packet);
         }
         default:
         {
            return nullptr;
         }
      }
   }
}