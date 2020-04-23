#include <fstream>

#include "linux_parser.h"
#include "processor.h"
// DONE: Return the aggregate CPU utilization
float Processor::Utilization() {
  ProcessorState state;
  std::ifstream procFile(LinuxParser::kProcDirectory +
                         LinuxParser::kStatFilename);
  if (procFile.is_open()) {
    std::string line, k, v;
    while (getline(procFile, line)) {
      std::stringstream cpuValues(line);
      cpuValues >> k;
      if (k == "cpu") {
        cpuValues >> state.User >> state.Nice >> state.System >> state.Idle
        >> state.IOWait >> state.Irq >> state.SoftIrq >> state.Steal >>
        state.Guest >> state.GuestNice;
        cpuValues.str();
        break;
      }
    }

    // Idle
    float prevIdle =
        previousProcessorState.Idle + previousProcessorState.IOWait;
    float idle = state.Idle + state.IOWait;
    float idleDelta = idle - prevIdle;
    // NonIdle
    auto& pr = previousProcessorState;
    float prevNonIdle =
        pr.User + pr.Nice + pr.System + pr.Irq + pr.SoftIrq + pr.Steal;
    float nonIdle = state.User + state.Nice + state.System + state.Irq +
                    state.SoftIrq + state.Steal;
    // Total
    float prevTotal = prevIdle + prevNonIdle;
    float total = idle + nonIdle;
    float totalDelta = total - prevTotal;

    previousProcessorState = state;
    return (totalDelta - idleDelta) / totalDelta;
  }

  return 0.0;
}