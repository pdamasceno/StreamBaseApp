#include "GameGenre.h"

#ifndef STREAMBASEAPP_RACING_H
#define STREAMBASEAPP_RACING_H

namespace collection
{
   class Racing : public GameGenre
   {
   public:
      Racing(std::string gameName);
      ~Racing() = default;
   };
}

#endif //STREAMBASEAPP_RACING_H
