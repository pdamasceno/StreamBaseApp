#include "Server.h"
#include "GameGenerator.h"
#include "Header.h"
#include "Command.h"
#include "CommonMessage.h"
#include "Reader.h"
#include "Writer.h"
#include <iostream>
#include <memory>
#include <algorithm>
#include <sstream>
#include <filesystem>
#include <fstream>

namespace sbaserver
{
   Server::Server() :
      m_pipeHandle(nullptr),
      m_bytesRead(0),
      m_bytesWritten(0),
      m_buffer(),
      m_gameCollection(),
      m_communicationLog()
   {
   }

   bool Server::CreatePipe()
   {
      m_pipeHandle = CreateNamedPipeW(
         L"\\\\.\\pipe\\server_pipe",
         PIPE_ACCESS_DUPLEX,
         PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
         PIPE_UNLIMITED_INSTANCES,
         BufferSize,
         BufferSize,
         NMPWAIT_NOWAIT,
         nullptr
      );

      if (m_pipeHandle == nullptr ||
          m_pipeHandle == INVALID_HANDLE_VALUE)
      {
         std::cout << "Failed to create outbound pipe instance." << std::endl;
         return false;
      }

      return true;
   }

   bool Server::WaitClientConnection()
   {
      if (!ConnectNamedPipe(m_pipeHandle, nullptr))
      {
         std::cout << "Failed to make connection on named pipe." << std::endl;
         return false;
      }

      return true;
   }

   void Server::CloseConnection()
   {
      CloseHandle(m_pipeHandle);
   }

   bool Server::ReadFromPipe()
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
            // Client disconnected
            return false;
         }

         std::cout << "Error occurred while reading from the client: " << GetLastError() << std::endl;
         return false;
      }
      else if (m_bytesRead == 0)
      {
         std::cout << "Server read 0 bytes from the client. Something is wrong!" << std::endl;
         return false;
      }

      return true;
   }

   bool Server::WriteToPipe(std::string message)
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
            // Client disconnected
            return false;
         }

         std::cout << "Error occurred while writing to the client: " << GetLastError() << std::endl;
         return false;
      }
      else if (m_bytesWritten != (strlen(m_buffer.data()) + 1))
      {
         std::cout << "Server has written more than or less than it should!" << std::endl;
         return false;
      }

      return true;
   }

   std::string Server::GetBufferValue()
   {
      std::string message(m_buffer.data());
      return message;
   }

   void Server::ClearBuffer()
   {
      // Clear buffer to prevent characters left behind from previous uses
      m_buffer.fill('\0');
   }

   std::string Server::GenerateCommonMessage(std::string content)
   {
      communication::Writer writer; // Serializer

      // Fill common message with content
      communication::CommonMessage commonMessage;
      commonMessage.SetContent(content);

      // Return serialized common message
      return writer.WriteAll(std::make_shared<communication::CommonMessage>(commonMessage));
   }

   std::string Server::HandleCommunication(std::string message)
   {
      // Deserializer
      communication::Reader reader;

      // Stream to build return messages
      std::stringstream returnMessageStream;

      // Stream to build log entries
      std::stringstream logEntry;
      logEntry << GetCurrentDateTime() << "-" << message;

      // Store client message in communication log container
      m_communicationLog.push(logEntry.str());

      try
      {
         // Deserialize message
         auto basePtr = reader.ReadAll(message);

         // Treat message based on type
         switch (basePtr->GetMessageType())
         {
            case communication::Header::MessageType::CommonMessage:
            {
               // Downcast base packet to equivalent derived packet
               auto derivedPtr = std::dynamic_pointer_cast<communication::CommonMessage>(basePtr);

               // Print client message to server console
               std::cout << "[Client] " << derivedPtr->GetContent() << "\n";

               break;
            }
            case communication::Header::MessageType::Command:
            {
               // Downcast base message to equivalent derived message
               auto derivedPtr = std::dynamic_pointer_cast<communication::Command>(basePtr);

               // Treat create game command
               if (derivedPtr->GetCommandType() == communication::Command::CommandType::CreateGame)
               {
                  std::shared_ptr<collection::GameGenre> game;

                  // Create game based on genre
                  if (derivedPtr->GetGameGenre() == "Shooter" || derivedPtr->GetGameGenre() == "shooter")
                  {
                     game = collection::GameGenerator::CreateGame(collection::GameGenre::Shooter, derivedPtr->GetGameName());
                  }
                  else if (derivedPtr->GetGameGenre() == "RPG" || derivedPtr->GetGameGenre() == "rpg")
                  {
                     game = collection::GameGenerator::CreateGame(collection::GameGenre::RPG, derivedPtr->GetGameName());
                  }
                  else if (derivedPtr->GetGameGenre() == "Racing" || derivedPtr->GetGameGenre() == "racing")
                  {
                     game = collection::GameGenerator::CreateGame(collection::GameGenre::Racing, derivedPtr->GetGameName());
                  }

                  if (game == nullptr)
                  {
                     // Genre does not exist
                     returnMessageStream << "[Server] Game genre is not available\n";
                  }
                  else
                  {
                     //Store the game in collection container and prepare return message
                     m_gameCollection.insert(std::make_pair(derivedPtr->GetGameName(), *game));
                     returnMessageStream << "[Server] Game created successfully\n";
                  }

                  break;
               }

               // Treat get game command
               if (derivedPtr->GetCommandType() == communication::Command::CommandType::GetGame)
               {
                  // Try to find game
                  auto game = m_gameCollection.find(derivedPtr->GetGameName());

                  // If game exists in the game collection
                  if (game != m_gameCollection.end())
                  {
                     // Get name and total played time from game stored in collection container and prepare return message
                     returnMessageStream << "[Server] Game: " << game->second.GetGameName() << " | Total time played: " << game->second.GetTotalTimePlayed() << "\n";
                  }
                  else
                  {
                     // Preparing fail message
                     returnMessageStream << "[Server] " << derivedPtr->GetGameName() << " does not exist in game collection\n";
                  }

                  break;
               }

               // Treat get game collection command
               if (derivedPtr->GetCommandType() == communication::Command::CommandType::GetGameCollection)
               {
                  // If game collection is empty
                  if (m_gameCollection.empty())
                  {
                     // Prepare return message
                     returnMessageStream << "[Server] Game collection is empty\n";
                  }

                  // Get all games and respective total time played
                  for (auto game : m_gameCollection)
                  {
                     // Prepare return message
                     returnMessageStream << "[Server] Game: " << game.first << " | Total time played: " << game.second.GetTotalTimePlayed() << "\n";
                  }

                  break;
               }

               // Treat delete game command
               if (derivedPtr->GetCommandType() == communication::Command::CommandType::DeleteGame)
               {
                  // If game exists in collection
                  if (m_gameCollection.find(derivedPtr->GetGameName()) != m_gameCollection.end())
                  {
                     // Delete game from collection container and prepare return message
                     m_gameCollection.erase(derivedPtr->GetGameName());
                     returnMessageStream << "[Server] " << derivedPtr->GetGameName() << " removed from game collection\n";
                  }
                  else
                  {
                     // Prepare return message
                     returnMessageStream << "[Server] " << derivedPtr->GetGameName() << " does not exist in game collection\n";
                  }

                  break;
               }

               // Treat update total time played command
               if (derivedPtr->GetCommandType() == communication::Command::CommandType::UpdateTotalTimePlayed)
               {
                  // Try to find game
                  auto game = m_gameCollection.find(derivedPtr->GetGameName());

                  // If game exists in collection
                  if (game != m_gameCollection.end())
                  {
                     // Update total time played and prepare return message
                     game->second.SetTotalTimePlayed(derivedPtr->GetTotalTimePlayed());
                     returnMessageStream << "[Server] Total time played of " << derivedPtr->GetGameName() << " updated\n";
                  }
                  else
                  {
                     // Prepare return message
                     returnMessageStream << "[Server] " << derivedPtr->GetGameName() << " does not exist in game collection\n";
                  }

                  break;
               }
            }
            default:
            {
               std::cout << "Message type does not exist" << std::endl;
            }
         }
      }
      catch (...)
      {
         throw "Error handling communication";
      }

      return returnMessageStream.str();
   }

   std::string Server::GetCurrentDateTime()
   {
      // Get current calendar time
      auto time = std::time(nullptr);

      // Format calendar time
      std::stringstream dateTime;
      dateTime << std::put_time(std::localtime(&time), "%F_%T");

      return dateTime.str();
   }

   void Server::CreateLogFile()
   {
      std::ofstream file(R"(.\StreamBaseAppClientLog.txt)", std::ios::app);

      while (!m_communicationLog.empty())
      {
         file << m_communicationLog.front() << std::endl;
         m_communicationLog.pop();
      }

      file.close();
   }
}

int main()
{
   sbaserver::Server server;
   std::wcout << "StreamBase Server" << std::endl << std::endl;

   // Create named pipe
   if (!server.CreatePipe())
   {
      system("pause");
      return 1;
   }

   // Wait client connection
   if (!server.WaitClientConnection())
   {
      server.CloseConnection();
      system("pause");
      return 1;
   }

   // Communication loop
   for (;;)
   {
      if (!server.ReadFromPipe())
      {
         break;
      }

      std::string returnMessage;

      try
      {
         // Handle received message to generate corresponding return message
         returnMessage = server.HandleCommunication(server.GetBufferValue());
      } catch (...)
      {
         // Error while handling message
         break;
      }

      // Clear buffer to prevent possible leftover characters from previous messages
      server.ClearBuffer();

      if (!server.WriteToPipe(server.GenerateCommonMessage(returnMessage)))
      {
         break;
      }

      server.ClearBuffer();
   }

   // Create client communication log file
   server.CreateLogFile();

   // Close pipe
   server.CloseConnection();
   system("pause");

   return 0;
}

