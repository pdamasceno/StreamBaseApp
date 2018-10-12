#include "Header.h"

#ifndef STREAMBASEAPP_COMMAND_H
#define STREAMBASEAPP_COMMAND_H

namespace communication
{
   class Command : public Header
   {
   public:
      enum CommandType
      {
         CreateGame = 1,
         GetGame = 2,
         GetGameCollection = 3,
         DeleteGame = 4,
         UpdateTotalTimePlayed = 5,
         InvalidCommand = 6
      };

      Command();
      Command(Command::CommandType commandType);
      ~Command() = default;

      Command::CommandType GetCommandType();

      void SetGameGenre(std::string gameGenre);
      std::string GetGameGenre() const&;

      void SetGameName(std::string gameName);
      std::string GetGameName() const&;

      void SetTotalTimePlayed(int totalTimePlayed);
      int GetTotalTimePlayed() const;

   private:
      Command::CommandType m_commandType;
      std::string m_gameGenre;
      std::string m_gameName;
      unsigned int m_totalTimePlayed;
   };
}

#endif //STREAMBASEAPP_COMMAND_H
