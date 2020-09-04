#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "../include/process.h"

using std::string;
using std::to_string;
using std::vector;

// Return this process's ID
int Process::Pid() const 
{ 
  return _pid; 
}

void Process::setID(int id)
{
  _pid = id;
}

// Return this process's CPU utilization - interesting
float Process::CpuUtilization() const
{ 
  return LinuxParser::getProcessCPU(Process::Pid());
}

// Return the command that generated this process
string Process::Command() 
{ 
 return LinuxParser::Command(Process::Pid()); 
}

// Return this process's memory utilization
string Process::Ram() 
{ 
  return LinuxParser::Ram(Process::Pid()); 
}

// Return the user (name) that generated this process
string Process::User() 
{ 
  return LinuxParser::User(Process::Pid()); 
}

// Return the age of this process (in seconds)
long int Process::UpTime() 
{ 
  return LinuxParser::UpTime(Process::Pid()) / 100; 
}

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& otherProcess) const 
{
  return CpuUtilization() > otherProcess.CpuUtilization();
}