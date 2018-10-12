#include "GameGenre.h"
#include <Windows.h>
#include <namedpipeapi.h>
#include <string>
#include <array>
#include <unordered_map>
#include <queue>

#ifndef STREAMBASEAPP_SERVER_H
#define STREAMBASEAPP_SERVER_H

namespace sbaserver
{
   class Server
   {
   public:
      enum
      {
         BufferSize = 1024
      };

      Server();
      ~Server() = default;

      // Pipe functions
      bool CreatePipe();
      bool WaitClientConnection();
      void CloseConnection();
      bool ReadFromPipe();
      bool WriteToPipe(std::string message);
      std::string GetBufferValue();
      void ClearBuffer();

      // Communication functions
      std::string GenerateCommonMessage(std::string content);
      std::string HandleCommunication(std::string message);
      std::string GetCurrentDateTime();
      void CreateLogFile();

   private:
      HANDLE m_pipeHandle;
      DWORD m_bytesRead;
      DWORD m_bytesWritten;
      std::array<char, BufferSize> m_buffer;
      std::unordered_map<std::string, collection::GameGenre> m_gameCollection;
      std::queue<std::string> m_communicationLog;
   };
}

#endif //STREAMBASEAPP_SERVER_H
