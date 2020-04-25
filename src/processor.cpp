#include <fstream>

#include "linux_parser.h"
#include "processor.h"
Processor::Processor() : previousProcessorState(10) {}
// DONE: Return the aggregate CPU utilization
float Processor::Utilization() {
  std::vector<long> currentProcessorState = LinuxParser::CpuUtilization();

  // Idle
  long prevIdle = previousProcessorState[LinuxParser::kIdle_] +
                   previousProcessorState[LinuxParser::kIOwait_];
  long idle = currentProcessorState[LinuxParser::kIdle_] +
               currentProcessorState[LinuxParser::kIOwait_];
  long idleDelta = idle - prevIdle;
  // NonIdle

  long prevNonIdle = previousProcessorState[LinuxParser::kUser_] +
                      previousProcessorState[LinuxParser::kNice_] +
                      previousProcessorState[LinuxParser::kSystem_] +
                      previousProcessorState[LinuxParser::kIRQ_] +
                      previousProcessorState[LinuxParser::kSoftIRQ_] +
                      previousProcessorState[LinuxParser::kSteal_];

  long nonIdle = currentProcessorState[LinuxParser::kUser_] +
                  currentProcessorState[LinuxParser::kNice_] +
                  currentProcessorState[LinuxParser::kSystem_] +
                  currentProcessorState[LinuxParser::kIRQ_] +
                  currentProcessorState[LinuxParser::kSoftIRQ_] +
                  currentProcessorState[LinuxParser::kSteal_];
  // Total
  long prevTotal = prevIdle + prevNonIdle;
  long total = idle + nonIdle;
  long totalDelta = total - prevTotal;

  previousProcessorState = std::move(currentProcessorState);
  return (totalDelta - idleDelta) / (1.0f * totalDelta);
}