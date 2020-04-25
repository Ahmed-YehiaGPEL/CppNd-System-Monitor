#include <fstream>

#include "linux_parser.h"
#include "processor.h"
Processor::Processor() : previousProcessorState(10) {}
// DONE: Return the aggregate CPU utilization
float Processor::Utilization() {
  std::vector<float> currentProcessorState = LinuxParser::CpuUtilization();

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

  float nonIdle = currentProcessorState[LinuxParser::kUser_] +
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