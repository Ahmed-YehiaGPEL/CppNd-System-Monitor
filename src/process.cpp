#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;
Process::Process():mPid(0){}
Process::Process(int pid) : mPid(pid) {}
// TODO: Return this process's ID
int Process::Pid() { return mPid; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() const { return mUtilization; }

// TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(mPid); }

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(mPid); }

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(mPid); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(mPid); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(const Process& a) const {
  return CpuUtilization() < a.CpuUtilization();
}

bool Process::operator>(const Process& a) const {
  return CpuUtilization() > a.CpuUtilization();
}

void Process::CalculateUtlization() { mUtilization = 0.0f; }