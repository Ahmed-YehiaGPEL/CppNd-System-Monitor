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

int Process::Pid() { return mPid; }


float Process::CpuUtilization() const { return mUtilization; }


string Process::Command() { return LinuxParser::Command(mPid); }


string Process::Ram() { return LinuxParser::Ram(mPid); }


string Process::User() { return LinuxParser::User(mPid); }


long int Process::UpTime() { return LinuxParser::UpTime(mPid); }

bool Process::operator<(const Process& a) const {
  return CpuUtilization() < a.CpuUtilization();
}

bool Process::operator>(const Process& a) const {
  return CpuUtilization() > a.CpuUtilization();
}

void Process::CalculateUtlization() { mUtilization = 0.0f; }