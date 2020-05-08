#include <unistd.h>
#include <cctype>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process() : mPid(0) {}
Process::Process(int pid) : mPid(pid) { CalculateUtlization(); }

int Process::Pid() { return mPid; }

float Process::CpuUtilization() {
  CalculateUtlization();
  return mUtilization;
}

string Process::Command() { return LinuxParser::Command(mPid); }

string Process::Ram() { return LinuxParser::Ram(mPid); }

string Process::User() { return LinuxParser::User(mPid); }

long int Process::UpTime() { return LinuxParser::UpTime(mPid); }

bool Process::operator<(Process& a) {
  return CpuUtilization() < a.CpuUtilization();
}

bool Process::operator>(Process& a) {
  return CpuUtilization() > a.CpuUtilization();
}

void Process::CalculateUtlization() {
  long upTimeSeconds = LinuxParser::UpTime();  // Sys Uptime in Seconds
  long processStartTimeSeconds = UpTime();
  float activeTimeSeconds =
      (LinuxParser::ActiveJiffies(mPid) / sysconf(_SC_CLK_TCK));
  float totalProcessTimeSeconds = upTimeSeconds - processStartTimeSeconds;
  mUtilization = 100 * (activeTimeSeconds / totalProcessTimeSeconds);
}