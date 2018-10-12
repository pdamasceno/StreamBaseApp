#include "Command.h"

namespace communication
{
   Command::Command() :
      Header(Header::MessageType::Command),
      m_commandType(Command::CommandType::InvalidCommand),
      m_gameGenre(""),
      m_gameName(""),
      m_totalTimePlayed(0)
   {
   }

   Command::Command(Command::CommandType commandType) :
      Header(Header::MessageType::Command),
      m_commandType(commandType),
      m_gameGenre(""),
      m_gameName(""),
      m_totalTimePlayed(0)
   {
   }

   Command::CommandType Command::GetCommandType()
   {
      return m_commandType;
   }

   void Command::SetGameGenre(std::string gameGenre)
   {
      m_gameGenre = gameGenre;
   }

   std::string Command::GetGameGenre() const&
   {
      return m_gameGenre;
   }

   void Command::SetGameName(std::string gameName)
   {
      m_gameName = gameName;
   }

   std::string Command::GetGameName() const&
   {
      return m_gameName;
   }

   void Command::SetTotalTimePlayed(int totalTimePlayed)
   {
      m_totalTimePlayed = totalTimePlayed;
   }

   int Command::GetTotalTimePlayed() const
   {
      return m_totalTimePlayed;
   }
}