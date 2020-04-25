#include <dirent.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <vector>

#include "linux_parser.h"
#define TICKS_PER_SEC sysconf(_SC_CLK_TCK)

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
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

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  float memUtil;
  std::ifstream memFile(kProcDirectory + kMeminfoFilename);
  std::string k, v, line;
  float totalMemory = -1, freeMemory = -1, bufferMemory = -1;
  if (memFile.is_open()) {
    while (getline(memFile, line)) {
      std::stringstream stream(line);
      stream >> k >> v;
      if (k == "MemTotal:") {
        totalMemory = std::stof(v);
      } else if (k == "MemFree:") {
        freeMemory = std::stof(v);
      } else if (k == "Buffers:") {
        bufferMemory = std::stof(v);
      }
      if (totalMemory != -1 && freeMemory != -1 && bufferMemory != -1) break;
    }
    // Two Ways of implementing:
    // 1- Get free memory and subtract the final percentile value from 1, since
    // we need to calculate used
    // 2- Calculate Used Memory (we need total mem,
    // free mem, buffer, cached, SReclaimable, Shmem)
    // We apply 1 for being easier, since we don't need more info.
    auto usableMemory = (totalMemory - bufferMemory);
    memUtil = freeMemory / usableMemory;
    memUtil = 1 - memUtil;
  }

  return memUtil;
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime() {
  long systemUpTime = 0;
  std::ifstream sysUpTimeFile(kProcDirectory + kUptimeFilename);
  std::string systemUpTimeString;

  if (sysUpTimeFile.is_open()) {
    std::string line;
    getline(sysUpTimeFile, line);
    std::istringstream stream(line);
    stream >> systemUpTimeString;
  }
  systemUpTime = std::stold(systemUpTimeString);
  return systemUpTime;
}

// DONE: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  auto cpuUtil = CpuUtilization();
  long systemJiffies =   cpuUtil[LinuxParser::kUser_] +
                      cpuUtil[LinuxParser::kNice_] +
                      cpuUtil[LinuxParser::kSystem_] +
                      cpuUtil[LinuxParser::kIRQ_] +
                      cpuUtil[LinuxParser::kSoftIRQ_] +
                      cpuUtil[LinuxParser::kSteal_] +
                      
                      cpuUtil[LinuxParser::kIdle_] + 
                      cpuUtil[LinuxParser::kIOwait_];

  return systemJiffies; 
    //return UpTime() * TICKS_PER_SEC; 
  }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// Done: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  auto cpuUtil = CpuUtilization();
  long activeTime =   cpuUtil[LinuxParser::kUser_] +
                      cpuUtil[LinuxParser::kNice_] +
                      cpuUtil[LinuxParser::kSystem_] +
                      cpuUtil[LinuxParser::kIRQ_] +
                      cpuUtil[LinuxParser::kSoftIRQ_] +
                      cpuUtil[LinuxParser::kSteal_];

  return activeTime; 
  }

// Done: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  auto cpuUtilization = CpuUtilization();
  return cpuUtilization[CPUStates::kIdle_] +
         cpuUtilization[CPUStates::kIOwait_];
}

// Done: Read and return CPU utilization
vector<long> LinuxParser::CpuUtilization() {
  std::vector<long> currentProcessorState(10);
  std::ifstream procFile(LinuxParser::kProcDirectory +
                         LinuxParser::kStatFilename);
  if (procFile.is_open()) {
    std::string line, k, v;
    while (getline(procFile, line)) {
      std::stringstream cpuValues(line);
      cpuValues >> k;
      if (k == "cpu") {
        cpuValues >> currentProcessorState[LinuxParser::CPUStates::kUser_] >>
            currentProcessorState[LinuxParser::CPUStates::kNice_] >>
            currentProcessorState[LinuxParser::CPUStates::kSystem_] >>
            currentProcessorState[LinuxParser::CPUStates::kIdle_] >>
            currentProcessorState[LinuxParser::CPUStates::kIOwait_] >>
            currentProcessorState[LinuxParser::CPUStates::kIRQ_] >>
            currentProcessorState[LinuxParser::CPUStates::kSoftIRQ_] >>
            currentProcessorState[LinuxParser::CPUStates::kSteal_] >>
            currentProcessorState[LinuxParser::CPUStates::kGuest_] >>
            currentProcessorState[LinuxParser::CPUStates::kGuestNice_];
        break;
      }
    }
  }
  return currentProcessorState;
}
// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  std::ifstream memInfoFile(kProcDirectory + kStatFilename);
  if (memInfoFile.is_open()) {
    std::string line, k, v;
    while (getline(memInfoFile, line)) {
      std::stringstream stream(line);
      stream >> k >> v;
      if (k == "processes") {
        return std::stoi(v);
      }
    }
  }

  return 0;
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  std::ifstream memInfoFile(kProcDirectory + kStatFilename);
  if (memInfoFile.is_open()) {
    std::string line, k, v;
    while (getline(memInfoFile, line)) {
      std::stringstream stream(line);
      stream >> k >> v;
      if (k == "procs_running") {
        return std::stoi(v);
      }
    }
  }

  return 0;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]]) { return 0; }