#include "Header.h"
#include <string>
#include <memory>

#ifndef STREAMBASEAPP_READER_H
#define STREAMBASEAPP_READER_H

namespace communication
{
   class Reader
   {
   public:
      Reader() = default;
      ~Reader() = default;

      // Deserialize function
      std::shared_ptr<communication::Header> ReadAll(std::string const& message);
   };
}

#endif //STREAMBASEAPP_READER_H
