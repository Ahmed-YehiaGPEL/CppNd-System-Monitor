#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process();
  Process(int pid);
  int Pid();
  std::string User();
  std::string Command();
  float CpuUtilization() const;
  std::string Ram();
  long int UpTime();

  bool operator<(const Process& a) const;
  bool operator>(const Process& a) const;

 private:
  int mPid;
  float mUtilization;

  void CalculateUtlization();
};

#endif