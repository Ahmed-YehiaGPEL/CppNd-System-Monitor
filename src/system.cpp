#include "system.h"

#include <unistd.h>

#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

System::System()
    : mKernel(LinuxParser::Kernel()),
      mOperatingSystem(LinuxParser::OperatingSystem()) {
  SampleProcesses();
}
// DONE: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// DONE: Return a container composed of the system's processes
vector<Process>& System::Processes() {
  SampleProcesses();
  return processes_;
}

// DONE: Return the system's kernel identifier (string)
std::string System::Kernel() { return mKernel; }

// DONE: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// DPME: Return the operating system name
std::string System::OperatingSystem() { return mOperatingSystem; }

// DONE: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// DONE: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// DONE: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }

void System::SampleProcesses() {
  vector<int> currentPids = LinuxParser::Pids();
  vector<int> oldPids;
  for (auto p : processes_) {
    if (std::find(currentPids.begin(), currentPids.end(), p.Pid()) !=
        currentPids.end()) {
      oldPids.push_back(p.Pid());
    }
  }
  for (auto p : currentPids) {
    if (std::find(oldPids.begin(), oldPids.end(), p) == oldPids.end()) {
      processes_.push_back(Process(p));
    }
  }
  std::sort(processes_.begin(), processes_.end());
}