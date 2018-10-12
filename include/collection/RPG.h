#include "GameGenre.h"

#ifndef STREAMBASEAPP_RPG_H
#define STREAMBASEAPP_RPG_H

namespace collection
{
   class RPG : public GameGenre
   {
   public:
      RPG(std::string gameName);
      ~RPG() = default;
   };
}

#endif //STREAMBASEAPP_RPG_H
