#pragma once
#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>

namespace LinuxParser {
// Paths needed inside the methods
const std::string kProcDirectory{"/proc/"};
const std::string kCmdlineFilename{"/cmdline"};
const std::string kCpuinfoFilename{"/cpuinfo"};
const std::string kStatusFilename{"/status"};
const std::string kStatFilename{"/stat"};
const std::string kUptimeFilename{"/uptime"};
const std::string kMeminfoFilename{"/meminfo"};
const std::string kVersionFilename{"/version"};
const std::string kOSPath{"/etc/os-release"};
const std::string kPasswordPath{"/etc/passwd"};

// System methods
float MemoryUtilization();

long UpTime();

std::vector<int> Pids();

int TotalProcesses();

int RunningProcesses();

std::string OperatingSystem();

std::string Kernel();

// CPU methods
enum CPUStates {
  kUser_ = 0,
  kNice_,
  kSystem_,
  kIdle_,
  kIOwait_,
  kIRQ_,
  kSoftIRQ_,
  kSteal_,
  kGuest_,
  kGuestNice_
};

std::vector<std::string> CpuUtilization();

long Jiffies();

long ActiveJiffies();

// Not used / defined, TODO: find an implementation
long ActiveJiffies(int);

long IdleJiffies();

float getProcessCPU(int );

// Processes methods
std::string Command(int);
std::string Ram(int);
std::string Uid(int);
std::string User(int);
long int UpTime(int);
};

#endif