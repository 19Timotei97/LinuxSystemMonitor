#include <string>

#include "../include/format.h"

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) 
{ 
  string hH, mM;

  mM = std::to_string((seconds / 60) % 60);

  hH = std::to_string(seconds / 3600);

  return hH.append(":").append(mM).append(":").append(std::to_string(seconds % 60)); 
}