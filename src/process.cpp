#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid): pid_(pid){};

// Return this process's ID
int Process::Pid() { return pid_; }

// Return this process's CPU utilization
float Process::CpuUtilization() { 
  string line;
  string value;
  long int utime{0}, stime{0}, cutime{0}, cstime{0};
  string pid_string = to_string(Pid());
  int counter{0};
  std::ifstream filestream(LinuxParser::kProcDirectory + pid_string + LinuxParser::kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line); 
    std::istringstream linestream(line);
    while(linestream >> value){
      counter++;
      if (counter == 14 && std::all_of(value.begin(), value.end(), isdigit)){
        utime = stoi(value);
      }
      if (counter == 15 && std::all_of(value.begin(), value.end(), isdigit)){
        stime = stoi(value);
      }
      if (counter == 16 && std::all_of(value.begin(), value.end(), isdigit)){
        cutime = stoi(value);
      }
      if (counter == 17 && std::all_of(value.begin(), value.end(), isdigit)){
        cstime = stoi(value);
        break;
      }
    }
  }
  long int total_time = utime + stime + cutime + cstime;
  long int seconds =  LinuxParser::UpTime() - UpTime();
  float cpu_usage = 100 * ((float)total_time / sysconf(_SC_CLK_TCK) / (float)seconds);
  return cpu_usage; 
}

// Return the command that generated this process
string Process::Command() { 
  string line{0};
  string pid_string = to_string(Pid());
  std::ifstream filestream(LinuxParser::kProcDirectory + pid_string + LinuxParser::kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    return line;
  }
  return line;
}

// Return this process's memory utilization
string Process::Ram() { 
  string line;
  string key;
  string value;
  long mem;
  string pid_string = to_string(Pid());
  std::ifstream filestream(LinuxParser::kProcDirectory + pid_string + LinuxParser::kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "VmSize:" && std::all_of(value.begin(), value.end(), isdigit)) {
        mem = stoi(value);
        return to_string(mem/1000);
      }
    }
  }
  return to_string(mem/1000);
}

// Return the user (name) that generated this process
string Process::User() { 
  string line;
  string key;
  string uid;
  string pid_string = to_string(Pid());
  std::ifstream filestream(LinuxParser::kProcDirectory + pid_string + LinuxParser::kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> uid;
      if (key == "Uid:") {
          break;
      }
    }
  }
  string user{"Unknown"};
  string str, uid2;
  std::ifstream filestream2(LinuxParser::kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream2, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> user >> str >> uid2;
      if (uid2 == uid){
        return user;
      }
  
    }
  }
  return user; 
}

// Return the age of this process (in seconds)
long int Process::UpTime() { 
  string line;
  string value;
  long int uptime{0};
  string pid_string = to_string(Pid());
  int counter{0};
  std::ifstream filestream(LinuxParser::kProcDirectory + pid_string + LinuxParser::kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line); 
    std::istringstream linestream(line);
    while(linestream >> value){
      counter++;
      if (counter == 22 && std::all_of(value.begin(), value.end(), isdigit)){
        uptime = stoi(value) / sysconf(_SC_CLK_TCK);
      }
    }
  }
  return uptime; 
}
