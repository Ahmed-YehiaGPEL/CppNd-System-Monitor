#include <fstream>

#include "linux_parser.h"
#include "processor.h"
Processor::Processor() : previousProcessorState(10) {}
// DONE: Return the aggregate CPU utilization
float Processor::Utilization() {
  std::vector<float> currentProcessorState(10);
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

    // Idle
    float prevIdle = previousProcessorState[LinuxParser::kIdle_] +
                     previousProcessorState[LinuxParser::kIOwait_];
    float idle = currentProcessorState[LinuxParser::kIdle_] +
                 currentProcessorState[LinuxParser::kIOwait_];
    float idleDelta = idle - prevIdle;
    // NonIdle

    float prevNonIdle = previousProcessorState[LinuxParser::kUser_] +
                        previousProcessorState[LinuxParser::kNice_] +
                        previousProcessorState[LinuxParser::kSystem_] +
                        previousProcessorState[LinuxParser::kIRQ_] +
                        previousProcessorState[LinuxParser::kSoftIRQ_] +
                        previousProcessorState[LinuxParser::kSteal_];
    
    float nonIdle =     currentProcessorState[LinuxParser::kUser_] +
                        currentProcessorState[LinuxParser::kNice_] +
                        currentProcessorState[LinuxParser::kSystem_] +
                        currentProcessorState[LinuxParser::kIRQ_] +
                        currentProcessorState[LinuxParser::kSoftIRQ_] +
                        currentProcessorState[LinuxParser::kSteal_];
    // Total
    float prevTotal = prevIdle + prevNonIdle;
    float total = idle + nonIdle;
    float totalDelta = total - prevTotal;

    previousProcessorState = std::move(currentProcessorState);
    return (totalDelta - idleDelta) / totalDelta;
  }

  return 0.0;
}