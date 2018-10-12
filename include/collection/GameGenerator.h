#include "GameGenre.h"
#include <memory>

#ifndef STREAMBASEAPP_GAMEGENERATOR_H
#define STREAMBASEAPP_GAMEGENERATOR_H

namespace collection
{
   class GameGenerator
   {
   public:
      // Factory function
      static std::shared_ptr<collection::GameGenre> CreateGame(GameGenre::Genre genre, std::string gameName);
   };
}

#endif //STREAMBASEAPP_GAMEGENERATOR_H
