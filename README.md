# Ping
A simple ping facility for C++ programs for Windows.

The source code provides two methods, Ping::Ping(IP) and Ping::Ping(IP, timeout).
- Ping(IP) sets a default timeout of 1000ms.
- The timeout parametre in the overloaded function must be given in ms.
- Ping::Close() must be called before the program exits.

That's all there is to it.

If you want to expand the functionality beyond only reporting failure or success, look at the switch statement near the bottom of Ping.cc and adjust the return values as needed.
