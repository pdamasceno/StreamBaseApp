#include <string>

#ifndef STREAMBASEAPP_GAMEGENRE_H
#define STREAMBASEAPP_GAMEGENRE_H

namespace collection
{
   class GameGenre
   {
   public:
      enum Genre
      {
         Shooter,
         RPG,
         Racing
      };

      GameGenre() = default;
      GameGenre(std::string gameName);
      virtual ~GameGenre() = default;

      std::string GetGameName() const;

      void SetTotalTimePlayed(int totalTimePlayed);
      int GetTotalTimePlayed() const;

   private:
      std::string m_gameName;
      int m_totalTimePlayed; // in minutes
   };
}

#endif //STREAMBASEAPP_GAMEGENRE_H
