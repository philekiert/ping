#include <string>

namespace Ping
{
  /* Return values
  ----------------
  1 Successful
  2 Failure
  3 Other, e.g. invalid.  */

  unsigned Ping(const std::string &ip);
  unsigned Ping(const std::string &ip, const unsigned &timeout);

  // Must be Close()d before program exits.
  void Close();
}
