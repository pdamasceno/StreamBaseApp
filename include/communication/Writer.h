#include "Header.h"
#include <string>
#include <memory>

#ifndef STREAMBASEAPP_WRITER_H
#define STREAMBASEAPP_WRITER_H

namespace communication
{
   class Writer
   {
   public:
      Writer() = default;
      ~Writer() = default;

      // Serialize function
      std::string WriteAll(std::shared_ptr<communication::Header> base);
   };
}

#endif //STREAMBASEAPP_WRITER_H
