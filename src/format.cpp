#include <iomanip>
#include <sstream>
#include <string>

#include "format.h"

using std::string;

// DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  int h, m, s;
  m = seconds / 60;
  s = seconds % 60;
  h = (m / 60);
  m %= 60;
  std::stringstream stream;
  stream << std::setw(2) << std::setfill('0') << h << ":" << std::setw(2)
         << std::setfill('0') << m << ":" << std::setw(2) << std::setfill('0')
         << s;
  return stream.str();
}