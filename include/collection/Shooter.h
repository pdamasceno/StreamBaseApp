#include "GameGenre.h"

#ifndef STREAMBASEAPP_SHOOTER_H
#define STREAMBASEAPP_SHOOTER_H

namespace collection
{
   class Shooter : public GameGenre
   {
   public:
      Shooter(std::string gameName);
      ~Shooter() = default;
   };
}

#endif //STREAMBASEAPP_SHOOTER_H
