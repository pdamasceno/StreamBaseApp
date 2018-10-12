#include <iostream>
#include <Windows.h>
#include <namedpipeapi.h>
#include <string>
#include <memory>
#include <algorithm>
#include <array>

#ifndef STREAMBASEAPP_CLIENT_H
#define STREAMBASEAPP_CLIENT_H

namespace sbaclient
{
   class Client
   {
   public:
      enum
      {
         BufferSize = 1024
      };

      Client();
      ~Client() = default;

      // Piper functions
      bool ConnectToPipe();
      void CloseConnection();
      bool ReadFromPipe();
      bool WriteToPipe(std::string message);
      std::string GetBufferValue();
      void ClearBuffer();
      std::string HandleClientInput();

      // Communication functions
      std::string GetCommonMessageContent(std::string message);
      void PrintHelp();
      bool IsPreviousCommand();

   private:
      HANDLE m_pipeHandle;
      DWORD m_bytesRead;
      DWORD m_bytesWritten;
      std::array<char, BufferSize> m_buffer;
      bool m_isPreviousCommand;
   };
}

#endif //STREAMBASEAPP_CLIENT_H
