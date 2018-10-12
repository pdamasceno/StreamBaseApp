#include "GameGenre.h"

namespace collection
{
   GameGenre::GameGenre(std::string gameName) :
      m_gameName(gameName),
      m_totalTimePlayed(0)
   {
   }

   std::string GameGenre::GetGameName() const
   {
      return m_gameName;
   }

   void GameGenre::SetTotalTimePlayed(int totalTimePlayed)
   {
      m_totalTimePlayed = totalTimePlayed;
   }

   int GameGenre::GetTotalTimePlayed() const
   {
      return m_totalTimePlayed;
   }
}

