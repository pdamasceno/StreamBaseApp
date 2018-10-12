#include "Client.h"
#include "Header.h"
#include "Command.h"
#include "CommonMessage.h"
#include "Reader.h"
#include "Writer.h"

namespace sbaclient
{
   Client::Client() :
      m_pipeHandle(nullptr),
      m_bytesRead(0),
      m_bytesWritten(0),
      m_buffer(),
      m_isPreviousCommand(false)
   {
   }

   bool Client::ConnectToPipe()
   {
      m_pipeHandle = CreateFileW(
         L"\\\\.\\pipe\\server_pipe",
         GENERIC_READ | GENERIC_WRITE,
         0,
         nullptr,
         OPEN_EXISTING,
         0,
         nullptr
      );

      if (m_pipeHandle == INVALID_HANDLE_VALUE)
      {
         std::cout << "Failed to connect to pipe." << std::endl;
         return false;
      }

      return true;
   }

   void Client::CloseConnection()
   {
      CloseHandle(m_pipeHandle);
   }

   bool Client::ReadFromPipe()
   {
      if (!ReadFile(
         m_pipeHandle,
         &m_buffer,
         BufferSize,
         &m_bytesRead,
         nullptr))
      {
         if (GetLastError() == ERROR_BROKEN_PIPE)
         {
            // Client down
            return false;
         }

         std::cout << "Error occurred while reading from the server: " << GetLastError() << std::endl;
         return false;
      }
      else if (m_bytesRead == 0)
      {
         std::cout << "Client read 0 bytes from the server. Something is wrong!" << std::endl;
         return false;
      }

      return true;
   }

   bool Client::WriteToPipe(std::string message)
   {
      std::copy(message.begin(), message.end(), m_buffer.begin());

      if (!WriteFile(
         m_pipeHandle,
         m_buffer.data(),
         strlen(m_buffer.data()) + 1,
         &m_bytesWritten,
         nullptr))
      {
         if (GetLastError() == ERROR_BROKEN_PIPE)
         {
            // Server is down
            return false;
         }

         std::cout << "Error occurred while writing to the server: " << GetLastError() << std::endl;
         return false;
      }
      else if (m_bytesWritten != (strlen(m_buffer.data()) + 1))
      {
         std::cout << "Server has written more than or less than it should!" << std::endl;
         return false;
      }

      return true;
   }

   std::string Client::GetBufferValue()
   {
      std::string message(m_buffer.data());
      return message;
   }

   void Client::ClearBuffer()
   {
      m_buffer.fill('\0');
   }

   std::string Client::HandleClientInput()
   {
      // Serializer
      communication::Writer writer;

      // Get message or command from terminal
      std::string input;
      std::getline(std::cin, input);

      // Treat create command
      if (input == "create")
      {
         m_isPreviousCommand = true;
         communication::Command createGame(communication::Command::CommandType::CreateGame);

         // Set game genre
         std::cout << "Game genre: ";
         std::getline(std::cin, input);
         createGame.SetGameGenre(input);

         // Set game name
         std::cout << "Game name: ";
         std::getline(std::cin, input);

         // Game name can't be empty
         while (input.empty())
         {
            std::cout << "Game name can't be empty!" << std::endl;
            std::cout << "Game genre: ";
            std::getline(std::cin, input);
         }

         createGame.SetGameName(input);

         // Return serialized create command
         return writer.WriteAll(std::make_shared<communication::Command>(createGame));
      }

      // Treat get command
      if (input == "get")
      {
         m_isPreviousCommand = true;
         communication::Command getGame(communication::Command::CommandType::GetGame);

         // Set game name
         std::cout << "Game name: ";
         std::getline(std::cin, input);
         getGame.SetGameName(input);

         // Return serialized get command
         return writer.WriteAll(std::make_shared<communication::Command>(getGame));
      }

      // Treat getall command
      if (input == "getall")
      {
         m_isPreviousCommand = true;
         communication::Command getGameCollection(communication::Command::CommandType::GetGameCollection);

         // Return serialized get game collection command
         return writer.WriteAll(std::make_shared<communication::Command>(getGameCollection));
      }

      // Treat update command
      if (input == "update")
      {
         m_isPreviousCommand = true;
         communication::Command updateGame(communication::Command::CommandType::UpdateTotalTimePlayed);

         // Set game name
         std::cout << "Game name: ";
         std::getline(std::cin, input);
         updateGame.SetGameName(input);

         // Set total time played
         std::cout << "Total time played: ";
         std::getline(std::cin, input);
         updateGame.SetTotalTimePlayed(std::stoi(input));

         // Return serialized update total time played command
         return writer.WriteAll(std::make_shared<communication::Command>(updateGame));
      }

      // Treat delete command
      if (input == "delete")
      {
         m_isPreviousCommand = true;
         communication::Command deleteGame(communication::Command::CommandType::DeleteGame);

         //Set game name
         std::cout << "Game name: ";
         std::getline(std::cin, input);
         deleteGame.SetGameName(input);

         // Return serialized delete game command
         return writer.WriteAll(std::make_shared<communication::Command>(deleteGame));
      }

      if (input == "help")
      {
         return input;
      }

      // Treat quit command
      if (input == "quit")
      {
         return input;
      }

      m_isPreviousCommand = false;

      communication::CommonMessage commonMessage;
      commonMessage.SetContent(input);

      // Return serialized common message
      return writer.WriteAll(std::make_shared<communication::CommonMessage>(commonMessage));
   }

   // Return the content of common message
   std::string Client::GetCommonMessageContent(std::string message)
   {
      communication::Reader reader;

      auto basePtr = reader.ReadAll(message);
      auto derivedPtr = std::dynamic_pointer_cast<communication::CommonMessage>(basePtr);
      if (derivedPtr == nullptr)
      {
         return "nullptr";
      }

      return derivedPtr->GetContent();
   }

   void Client::PrintHelp()
   {
      std::cout << "Game collection management" << std::endl;
      std::cout << "Usage: type one of the commands below and follow the prompts, every other text is sent as a normal message" << std::endl;
      std::cout << "create  | Create game and add to collection  | prompts: game genre (Shooter, RPG and Racing available) and game name" << std::endl;
      std::cout << "get     | Get a game from collection         | prompts: game name" << std::endl;
      std::cout << "getall  | Get all games in the               | no prompts" << std::endl;
      std::cout << "update  | Update total time played of a game | prompts: game name and total time played" << std::endl;
      std::cout << "delete  | Remove a game from collection      | prompts: game name" << std::endl;
      std::cout << "help    | Prints this help" << std::endl;
      std::cout << "quit    | Close pipe connection and quit application" << std::endl << std::endl;
   }

   bool Client::IsPreviousCommand()
   {
      return m_isPreviousCommand;
   }
}

int main()
{
   std::cout << "StreamBase Client" << std::endl << std::endl;

   sbaclient::Client client;
   client.PrintHelp();

   if (!client.ConnectToPipe())
   {
      system("pause");
      return 1;
   }

   // Communication loop
   for (;;)
   {
      std::string messageToSend;

      try
      {
         // Get input from console
         messageToSend = client.HandleClientInput();
      } catch (...)
      {
         // Error serializing message or command
         break;
      }

      if (messageToSend == "help")
      {
         client.PrintHelp();
      }
      else if (messageToSend == "quit")
      {
         break;
      }
      else
      {
         // Send serialized message
         if (!client.WriteToPipe(messageToSend))
         {
            break;
         }

         // Clear buffer to prevent possible leftover characters from previous message
         client.ClearBuffer();

         if (!client.ReadFromPipe())
         {
            break;
         }

         // Only print messages that came as a result of a command
         if (client.IsPreviousCommand())
         {
            std::cout << client.GetCommonMessageContent(client.GetBufferValue());
         }
      }

      client.ClearBuffer();
   }

   client.CloseConnection();

   return 0;
}