# MonopolyDeal
Akshay's Summer 2019 project.
C telnet/netcat server for a (command-line based) game of (a modified version of) HasBro Studios' Monopoly Deal.
Along with Monopoly Deal, users can communicate via the chat system.

Note: 
- This is a very early beta version. This project contains incomplete / OS-specific source code, incomplete game functionality, and a lot of potential for bugs.
- The server only works on POSIX based computers. The client (netcat/telnet) works on all computers but is very buggy on Windows.

## Getting Started

#### Installing

Clone the repo, run the Makefile:

```
git clone https://github.com/SnootierMoon/MonopolyDeal.git
cd MonopolyDeal
make 
```

If it runs a bunch of (`gcc`/`clang`) commands and then says "Done compiling files", it worked. If anything else happened, it did not work. If it did not work, then it could be possible that some libraries used by the program have not been linked correctly for the computer. In that case, you may have to wait for a later and better release.

#### Running the server

The compiled binary should be `MonopolyDeal/out/main`. It can be run by simply executing the binary file or typing `make run` from the root direcory of the repository. 

On execution, the program should print out the IP(s) and port of the socket. This feature is a little buggy. While it does report the IP(s) and port correctly, it reports all possible IPs (WiFi, localhost, ethernet), and attempts to determine the most convenient IP for the machine by trying to figure out which one is WiFi. This feature may not work, so the user might have to use intuition or trial/error to figure out which IP to use.

#### Connecting a client

On a Linux/Unix system, both netcat and telnet should be available. I would advise to use netcat because telnet can be funky. On Windows, only telnet is available. 

Obviously, the server and client must be connected to the same network.

##### Linux/Unix (POSIX)

If telnet/netcat is available, you can run `[telnet|netcat] [ip] [port]`. If a prompt for your Monopoly Deal username appears, it worked. If anything else happened (it gets stuck, it ends the command prematurely, it explicitly said that it did not work, etc.), it did not work.

##### Windows

On windows, only `telnet` should be available. You can run it by either entering the command prompt (do: windows-key "cmd" enter-key) and type `telnet [ip] [port]` just like Linux, or you can open the windows telnet (do: windows-key "telnet" enter-key) application which will bring you directly to a prompt that should look like:

```
telnet>
```

Here, you need to type `open [ip] [port]`.

## TODO

There's a long list of things remaining to be done, so I'll shorten this list to things I have to do for now:

1. Finish the game functionality
2. Fix the IP print at the beginning of the server startup
3. Clean up code from bad to good (error handling, general C safety among many other things)

