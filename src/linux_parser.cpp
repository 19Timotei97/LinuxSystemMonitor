#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <algorithm>

#include "../include/linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// Retrieve the operating system of the computer
string LinuxParser::OperatingSystem() {
  
  string line, key, value;
  
  std::ifstream filestream(kOSPath);

  if (filestream.is_open()) {
    
    while (std::getline(filestream, line)) {
      
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      
      std::istringstream linestream(line);
      
      while (linestream >> key >> value) {
        
        if (key == "PRETTY_NAME") {
          
          std::replace(value.begin(), value.end(), '_', ' ');
          
          return value;
        }
      }
    }
  }
  return value;
}

// Retrieve the Kernel of the system
string LinuxParser::Kernel() 
{
  string os, kernel, line;
  
  std::ifstream stream(kProcDirectory + kVersionFilename);
  
  if (stream.is_open()) 
  {  
    std::getline(stream, line);
    
    std::istringstream linestream(line);
    
    linestream >> os >> kernel;
  }
  
  return kernel;
}

// TOD: Update this to use std::filesystem
std::vector<int> LinuxParser::Pids() {
  
  std::vector<int> pids;
  
  DIR* directory = opendir(kProcDirectory.c_str());
  
  struct dirent* file;
  
  while ((file = readdir(directory)) != nullptr) 
  {
    // Is this a directory?
    if (file->d_type == DT_DIR) 
    {
      // Is every character of the name a digit?
      string filename(file->d_name);
      
      if (std::all_of(filename.begin(), filename.end(), isdigit)) 
      {
        
        int pid = stoi(filename);
        
        pids.push_back(pid);
      }
    }
  }
  
  closedir(directory);
  
  return pids;
}

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() 
{
  string line, key, value;
  
  float memTotal = 0.0f, memFree = 0.0f;
  
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  
  if (filestream.is_open()) 
  {
    while (std::getline(filestream, line)) 
    {  
      /*
       * https://stackoverflow.com/questions/83439/remove-spaces-from-stdstring-in-c 
       * Remove all the spaces from a container (in this case, a string).
       * The algorithm remove_if can't change the container(only modify the values), so it actually shuffles the values around 
       * and returns a pointer to where the end now should be. 
       * So a call to string::erase is needed to actually modify the length of the container 
      */
//       line.erase(remove_if(line.begin(), line.end(), isspace), line.end());
//       std::replace(line.begin(), line.end(), ':', ' ');
      
      std::istringstream linestream(line);
      
      while (linestream >> key)
      {
        if (0 == key.compare("MemTotal"))
        {
          linestream >> value;
          memTotal = std::stof(value);
        }
            
        if (0 == key.compare("MemFree"))
        {
          linestream >> value;
          memFree = std::stof(value);
        }
      }
    }
  }
  
  return (memTotal - memFree);
}

// Read and return the system uptime
long LinuxParser::UpTime() 
{
  string line;
  
  long upTime = 0, upTimeAndIdle = 0;

  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  
  if (filestream.is_open()) 
  {
    while (std::getline(filestream, line)) 
    {
      std::istringstream linestream(line);
      linestream >> upTime >> upTimeAndIdle;
    }
  }

  return upTime; 
}


// Read and return the number of jiffies for the system
long LinuxParser::Jiffies()
{
  string line, key, value;
  long jiffies = 0;

  std::ifstream filestream(kProcDirectory + kStatFilename);

  if (filestream.is_open())
  {
    while (std::getline(filestream, line))
    {
      std::istringstream linestream(line);

      while (linestream >> key)
      {
        if (key.compare("cpu") == 0)
        {
          while (linestream >> value)
          {
            jiffies += std::stol(value);
          }
        }
      }
    }
  }

  return jiffies; 
}


// Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) 
{
  // TODO: implement in the future 
  return 0; 
}

// Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() 
{
  string line, key, value;
  long activeJiffies = 0;

  std::ifstream filestream (kProcDirectory + kStatFilename);

  if (filestream.is_open())
  {
    while(std::getline(filestream, line))
    {
      std::istringstream linestream(line);

      while (linestream >> key)
      {
        if (0 == key.compare("cpu"))
        {
          for (auto localIndex = 0; localIndex < 3; localIndex++)
          {
            linestream >> value;
            activeJiffies += stol(value);
          }
        }
      }
    }
  }

   return activeJiffies; 
}

// Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() 
{ 
  string line, key, value;

  long idleJiffies = 1;

  std::ifstream filestream(kProcDirectory + kStatFilename);

  if (filestream.is_open())
  {
    while (std::getline(filestream, line))
    {
      std::istringstream linestream(line);

      while (linestream >> key)
      {
        if (0 == key.compare("cpu"))
        {
          for (auto localIndex = 0; localIndex < 3; localIndex++)
          {
            linestream >> value;
          }

          // storing iddle jiffies
          linestream >> value;

          idleJiffies = std::stol(value);

          // storing iowait
          linestream >> value;
          idleJiffies += std::stol(value);
        }
      }
    }
  }
 
  return idleJiffies;
}

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() 
{ 
  long user{0}, nice{0}, syst{0}, idle{0}, iowait{0}, irq{0};
  long softirq{0}, steal{0}, guest{0}, guest_nice{0};

  string line, cpu;

  std::ifstream filestream (kProcDirectory + kStatFilename);

  if (filestream.is_open())
  {
    std::getline(filestream, line);

    std::istringstream linestream (line);

    linestream >> cpu >> user >> nice >> syst >> idle;
    linestream >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
  }
  
  std::vector<long> cpuUsage {user, nice, syst, idle, iowait, 
                              irq, softirq, steal, guest, guest_nice};
  
  std::vector<string> cpuUsageString;
  std::vector<string> errorOption;
  
  for (auto& localIndex : cpuUsage) 
  {
    string element;
    
    std::stringstream strStream;
    
    strStream << localIndex;
    strStream >> element;
    
    cpuUsageString.push_back(element);
  }
  
  return (0 != cpuUsageString.size()) ? cpuUsageString : errorOption;  
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() 
{ 
  string line, key, value;

  int totalProcs = 1;

  std::ifstream filestream (kProcDirectory + kStatFilename);

  if (filestream.is_open())
  {
    while (std::getline(filestream, line))
    {
      std::istringstream linestream (line);

      while (linestream >> key)
      {
        if (0 == key.compare("processes"))
        {
          linestream >> totalProcs;
        }
      }
    }
  }
  
  return totalProcs; 
}

// Read and return the process's CPU time
float LinuxParser::getProcessCPU(int pid)
{
  std::string line, notRelevant;
  
  long utime, stime, cutime, cstime, starttime;
  float processCpuTime = 0;;
  
  std::ifstream filestream (kProcDirectory + std::to_string(pid) + kStatFilename);
  
  if (filestream.is_open())
  {
    std::getline(filestream,line);
    
    std::istringstream linestream(line);
    
    for (auto localIndex = 0; localIndex < 13; localIndex++)
    {
      linestream >> notRelevant;
    }
    
    linestream >> utime >> stime >> cutime >> cstime;
    
    for (auto localIndex = 0; localIndex < 4; localIndex++)
    {
      linestream >> notRelevant;
    }
    
    linestream >> starttime;
    
    long seconds = LinuxParser::UpTime(pid) - (starttime)/100;
    
    processCpuTime = (float) (((utime + stime + cutime + cstime)/100)/seconds);
  }
  
  return processCpuTime;
}


// Read and return the number of running processes
int LinuxParser::RunningProcesses() 
{ 
  string line, key, value;

  int runningProcs = 1;

  std::ifstream filestream (kProcDirectory + kStatFilename);

  if (filestream.is_open())
  {
    while (std::getline(filestream, line))
    {
      std::istringstream linestream (line);

      while (linestream >> key)
      {
        if (0 == key.compare("procs_running"))
        {
          linestream >> runningProcs;
        }
      }
    }
  }
  
  return runningProcs; 
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) 
{ 
  string line, command = "";
  
  std::ifstream filestream (kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  
  if (filestream.is_open())
  {
    while (std::getline(filestream, line))
    {
      std::istringstream linestream (line);
      linestream >> command;
    }
  }

  return command;
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) 
{ 
  string line, key, ramUsage = "";
  
  std::ifstream filestream (kProcDirectory + std::to_string(pid) + kStatusFilename);
  
  if (filestream.is_open())
  {
    while (std::getline(filestream, line))
    {
      std::istringstream linestream (line);
      
      while (linestream >> key)
      {
        if (0 == key.compare("VmSize:"))
        {
          linestream >> ramUsage;
        }
      }
    }
  }
  
  return ramUsage; 
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) 
{
  string line, key, uid = "";
  
  std::ifstream filestream (kProcDirectory + std::to_string(pid) + kStatusFilename);
  
  if (filestream.is_open())
  {
    while (std::getline(filestream, line))
    {
      std::istringstream linestream (line);
      
      while (linestream >> key)
      {
        if (0 == key.compare("Uid:"))
        {
          linestream >> uid;
        }  
      }
    }
  }  
  
  return uid; 
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) 
{ 
  string line, key, user, group, id;
  string uid = Uid(pid);
  
  std::ifstream filestream (kPasswordPath);
  
  if (filestream.is_open())
  {
    while (std::getline(filestream, line))
    {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream (line);
      
      linestream >> user >> group >> id;
      
      if (0 == id.compare(uid))
      {
        return user;
      }
      else
      {
        return string();
      } 
    }
  }
  
  return user;
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) 
{ 
  string line, notRelevant, key;
  long upTime = 0;
  
  std::ifstream filestream (kProcDirectory + std::to_string(pid) + kStatFilename);
  
  if (filestream.is_open())
  {
    std::getline(filestream, line);
    std::istringstream linestream (line);
    
    for (auto localIndex = 0; localIndex < 21; localIndex++)
    {
      linestream >> notRelevant;
    }
    
    linestream >> key;
    upTime = std::stol(key);
  }
  
  return upTime; 
}
