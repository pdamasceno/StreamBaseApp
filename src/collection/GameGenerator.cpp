#include "GameGenerator.h"
#include "Shooter.h"
#include "RPG.h"
#include "Racing.h"
#include <iostream>


namespace collection
{
   std::shared_ptr<collection::GameGenre> GameGenerator::CreateGame(GameGenre::Genre genre, std::string gameName)
   {
      // Create a game object based on genre and return shared_ptr of game object
      switch (genre)
      {
         case GameGenre::Shooter:
         {
            return std::make_shared<collection::Shooter>(gameName);
         }
         case GameGenre::RPG:
         {
            return std::make_shared<collection::RPG>(gameName);
         }
         case GameGenre::Racing:
         {
            return std::make_shared<collection::Racing>(gameName);
         }
         default:
         {
            // Returns nullptr if game genre does not exists
            return nullptr;
         }
      }
   }
}