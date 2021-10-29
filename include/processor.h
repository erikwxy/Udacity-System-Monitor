#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // Declare any necessary private members
 private:
  long _prevIdle{0};
  long _prevTotal{0};
};

#endif