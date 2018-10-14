# StreamBaseApp

It is a simple named pipe Client/Server application with the theme of a video game collection management. You can create, search for individual games or the whole collection, update total time played and delete a game from the collection. To use, start the Server first and then the Client.

### 1. Communication Protocol

I created a simple communication protocol to exchange messages between Client and Server. The format of the procotol is as follows:

| Header  |  Message attributes  |
| ------- |----------------------|

**Header** is composed only of the message type.\
**Message attributes** are all the attribues that compose a message.

The only rules to the serialization of the packets used here are that each attribute is serialized in the order they are declared in their header files, that each attribute is separated by a semicolon and that the resulting serialized message is a simple string, there is no binary or hexadecimal conversion. Below is an example of a serialized message:

**101;1;rpg;diablo;0**

In the case of the example before, 101 represents the message type and from 1 to 0 are the atributes of the message of type 101.

### 1.1. Messages

Every message has its own header and source files. Each message derives from a base class called Header. The definitions of each message and their attributes are below:

**Message Type 100: Common Message**

| Attributes | Description                                                  |
|:----------:|:------------------------------------------------------------:|
| Content    | Stores the content of messages sent by the Client and Server |

Every other text sent by the Client that it is not a pre-determined command will be sent in the Content attribute of this message. Also every return message of a command is sent using this message by the Server.

**Message Type 101: Command**

| Attributes        | Description                                                  |
|:------------------|:-------------------------------------------------------------|
| Command Type      | Informs the type of command. (1=Create Game, 2=Get Game, 3=Get Game Collection, 4=Delete Game, 5=Update Total Time Played, 6=Invalid Command) |
| Game Genre        | Genre of the game to be created (Shooter, RPG and Racing available) |
| Game Name         | Name of the game to be created                               |
| Total Time Played | Game total time it was played                                |

The usage of the attributes will depend on wich command type is being used. For Create Game, Game Genre and Game Name. For Get Game and Delete Game only Game Name. For Update Total Time Played, Game Name and Total Time Played. For Get Game Collection, no attribute is used. The return message is a Common Message with the status message of the command in the Content attribute.

### 1.2. Writer

Writer is class that have the responsibility of serializing messages sent by the Client and by the Server, obeying the rules of the protocol. The function WriteAll is the one that does this and it receives a pointer to the base class of the messages that is the Header class and based on the message type information that Header holds, it downcasts the pointer to the respective derived class to access its attributes and generate the serialized message.

### 1.3. Reader

Reader is a class responsible for deserializing the messages sent by the Client and by the Server. The function ReadAll is the one that does this and it receivers a serialized message, splits the message based on the semicolon separator and store each splited value into a vector to access each attribute individually. After that it checks the first position of the vector that stores the message type and based on that information it instantiates a Common Message or Command and fill them accordingly. In the end a pointer to the messages is returned.

### 2. Client

The Client connects to the server via a named pipe and only make sync calls. The Client can write to the pipe and read from it. The Client also handles input typed in the terminal. Every normal message is sent via a Common Message and every game collection or game manipulation is sent via a Command.
The commands used by the client are below:

**create** - create game based on genre\
**get** - get game from collection\
**getall** - get all games from collection\
**update** - update total time played of a game\
**delete** - delete a game from collection

Every Common Message or Command is first serialized using Writer and the resulting serialized message is written in the pipe to be sent to the server. The Server only sends Common Message to the Client and after reading it from the pipe, the message is deserealized and its content is printed in the terminal.

### 3. Server

The Server creates a named pipe and waits for a connection by the client and can only receive sync calls. The Server handles all communication sent by the Client and respond to Command with a Common Message.

### 3.1. Game Generator

It is a simple factory of classes and is used to create games that later will be added to the collection. The base class used in the factory is called GameGenre and all genres available in this application are derived from it. For now there are only Shooter, RPG and Racing genres available but it is easy to add new ones if needed but the whole application would need to be recompiled. The static function CreateGame receives a base class GameGenre and the name of the game to be created. Then based on the type of the genre it returns a pointer to an instatiated derived genre.

### 3.2. How Server works

After reading a message from the client from the pipe, the Server uses the function HandleCommunication to treat the received message. It is in this function that the deserialization of messages is done based on the message type that arrived. First, the Server appends the date and hour that the serialized message using a stringstream and store it in a queue. When the application is closed it is created a log file with the information of this queue, with every entry in the file being like this: **YYY-MM-DD_HH:MM:SS-100;Test message**. Then it makes a call to ReadAll to get that pointer to base Header and based on the message type information, it downcasts the pointer to the respective derived class to be able to access its attributes and do what it is necessary with them.\
Every game created is stored in an unordered_map container with the key being the name of the game and the value the actual game object. I chose unordered_map because of its fast lookups and ease to manipulate using only the name of the game. For example, if I want to find a game called 'Diablo' I don't need to iterate over the whole container to find it. All commands do a type of operation on the container or the game object. Below is what which command do:

| Command Type             | Description (container = unordered_map)                                           |
|:-------------------------|:----------------------------------------------------------------------------------|
| Create Game              | Create a game object using the factory and stores it in the container             |
| Get Game                 | Search the game in the container and returns the game name and total time played  |
| Get Game Collection      | Returns the game names and total time played of all games stored in the containter|
| Delete Game              | Remove a game from the container                                                  |
| Update Total Time Played | Update the total time played of a game stored in the container                    |

The result of each operation is returned using a Common Message to the client.

### 4. Conclusion and Assumptions

It was a fun project to do. I tried to respect the total time of 8 hours expected to do this but ended up passing it by something between 4 to 6 hours if I'm correct. I'm not trying to make any excuses here but all of this is because my background developing with C++ is all in Linux. I never programmed in Windows before or used cmake to build projects and I struggled a little bit to get things right with cmake. My CMakeLists file is not great but gets the work done. Another reason for the delay is my inexperience with named pipes, getting the sync calls between server and client took some time but it was ok to figure it out but I was having a difficult time to get the async calls. I was able the create the pipe right but was having trouble with the async read and write. I know that for async calls maybe it is better to use threads but as I said it was taking too much time and I didn't want to extrapolate that 8 hours by more than I already did. Anyway, I know I can get my head around it with more time. Now for the assumptions I did for each feature:

- **The client should be able to connect to the server through a NamedPipe**\
Used Named Pipes API to create the pipe and the read and write to pipe works as expected in the Server and in the Client.
- **The client should be able to make both sync and async calls to the server**\
I was only able to make the sync calls as explained above. No async calls, sorry.
- **The client should be able to send trivial data (strings, numbers) to the server**\
The Client can send any information using a Common Message, the information is then printed to the Server terminal.
- **The client should be able to create objects on the server (based on req-7 below), retrieve them, their attributes and call methods on them**\
All this is done by the Client via the Command message. 
- **The server should be able to receive both sync/async connection requests from clients**\
Same as above, only sync calls and no async calls. Sorry again.
- **The server should be able to store data provided by the client via NamedPipe in a reasonable data structure**\
All messages sent by the Client are stored in a queue and later this queue is used to generate a log file.
- **The server should be able to register a custom class (w/ related functions, attributes) which can be used by the client (see req-4)**\
I did this by using the class factory Game Generator.
- **The server should be able to store the custom objects created by the client for the custom class created in req-7**\
The game objects are stored in a unordered_map container called gameCollection.

That's all folks. I hope that what I done with this little application is enough to let me work with you all.
