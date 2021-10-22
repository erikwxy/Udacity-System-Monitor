#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>

#include "processor.h"
#include "linux_parser.h"

using std::string;

// Return the aggregate CPU utilization
float Processor::Utilization() { 
  string line;
  string key;
  long integers;
  string string_integer;
  std::vector<long> values;

  std::ifstream filestream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> key;
    while (linestream >> string_integer){
      if (std::all_of(string_integer.begin(), string_integer.end(), isdigit)) {
          integers = stoi(string_integer);
          values.push_back(integers);
      }
    }
  }
  
  long Idle = values[3] + values[4];
  long NonIdle = values[0] + values[1] + values[2] + values[5] + values[6] + values[7];

  long Total = Idle + NonIdle;

  float CPU_Percentage = ((float)Total - (float)NonIdle)/(float)Total;

  return CPU_Percentage;
}