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
  filestream.close();
  
  long Idle = values[3] + values[4];

  long NonIdle = values[0] + values[1] + values[2] + values[5] + values[6] + values[7];

  long Total = Idle + NonIdle;

  long Total_diff = Total - _prevTotal;
  long Idle_diff = Idle - _prevIdle;
  _prevTotal = Total;
  _prevIdle = Idle;

  float CPU_Percentage = ((float)Total_diff - (float)Idle_diff)/(float)Total_diff;

  return CPU_Percentage;
}