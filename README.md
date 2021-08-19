# Ping
A simple ping facility for C++ programs for Windows.

The source code provides two methods, Ping::Ping(IP) and Ping::Ping(IP, timeout).
- Ping(IP) sets a default timeout of 1000ms.
- The timeout parametre in the overloaded function must be given in ms.
- Ping::Close() must be called before the program exits.

That's all there is to it.
