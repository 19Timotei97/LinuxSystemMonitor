#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include "linux_parser.h"
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process 
{
public:
  // Relevant constructors
  Process() : _pid {0} {}
  Process(int pid) : _pid {pid} {}

  // Getter, setter
  int Pid() const;
  void setID (int);
  
  // Needed to be defined methods
  std::string User();

  std::string Command();                   
  
  float CpuUtilization() const;                  
  
  std::string Ram();                       
  
  long int UpTime();                       
  
  bool operator<(Process const&) const;

private:
  int _pid;
};

#endif