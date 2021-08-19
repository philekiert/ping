#include <string>
#include <windows.h>
#include <iphlpapi.h>
#include <icmpapi.h>
#pragma comment(lib, "IPHLPAPI.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "user32.lib")

#include "Ping.hpp"

bool initialisedPing = false;

HANDLE icmpFile;
unsigned long addr = INADDR_NONE;
DWORD dwRetVal = 0;
char SendData[32] = "Data buffer";
LPVOID ReplyBuffer = NULL;
DWORD ReplySize = 0;

/* Return values
----------------
1 Successful
2 Failure
3 Other       */

unsigned Ping::Ping(const std::string &ip)
{
  return Ping(ip, 1000);
}

unsigned Ping::Ping(const std::string &ip, const unsigned &timeout)
{
  if (!initialisedPing)
  {
    icmpFile = IcmpCreateFile();
    if (icmpFile == INVALID_HANDLE_VALUE) { } // On failure to create handle.
  }

  // Convert the address string into a usable IP address.
  char convert[16];
  strcpy(convert, ip.c_str());
  addr = inet_addr(convert); // inet_addr converts a string into an IP address.
  if (addr == -1)
    return 4; // inet_addr() returns -1 on invalid IP.
  ReplySize = sizeof(ICMP_ECHO_REPLY) + sizeof(SendData);
  ReplyBuffer = (VOID*) malloc(ReplySize); // Reserve memory for replyBuffer.
  if (ReplyBuffer == NULL) { } // On failure to allocate memory.
  else
  {
    if (addr != INADDR_NONE)
      dwRetVal = IcmpSendEcho(icmpFile, addr, SendData, sizeof(SendData),
                NULL, ReplyBuffer, ReplySize, timeout); // Timeout value is DWORD.
  }

  PICMP_ECHO_REPLY echo = (PICMP_ECHO_REPLY)ReplyBuffer;
  ULONG status = echo->Status;

  // Values not included:
  // 11001 Reply buffer was too small.
  // 11006 Insufficient IP resources were available.
  // 11007 A bad IP option was specified.
  // 11008 Hardware error.
  // 11009 Packet was too big.
  // 11011 Bad request.
  // 11012 Bad route.
  // 11015 Parametre problem.
  // 11017 An IP option was too big.
  // 11018 Bad destination.
  // 11050 General failure.

  switch (status)
  {
    case 0:     // Successful.
      return 1;
    case 11016: // Datagrams arrived too quickly to be processed, some may have been missed.
      return 3; // Listing as 3 for now, as it does show the endpoint is live, it's just acting up in some way.
    case 11002: // Destination net unreachable.
      return 2;
    case 11003: // Destination host unreachable.
      return 2;
    case 11004: // Destination protocol unreachable.
      return 2;
    case 11005: // Destination port unreachable.
      return 2;
    case 11010: // Request timed out.
      return 2;
    case 11013: // TTL expired in transit.
      return 2;
    case 11014: // TTL expired during fragment assembly.
      return 2;
    default:
      return 3;
  }
}

void Ping::Close()
{
  if (initialisedPing)
    IcmpCloseHandle(icmpFile);
}
