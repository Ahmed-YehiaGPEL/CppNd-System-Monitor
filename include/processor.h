#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 private:
  struct ProcessorState {
    float Idle, IOWait, User, Nice, System, Irq, SoftIrq, Steal, Guest,
        GuestNice;
  };

 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
  ProcessorState previousProcessorState;
};

#endif