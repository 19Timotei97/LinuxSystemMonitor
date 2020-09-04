#include "../include/processor.h"

// Return the aggregate CPU utilization
float Processor::Utilization() 
{
  return (float) LinuxParser::ActiveJiffies() / LinuxParser::Jiffies(); 
}