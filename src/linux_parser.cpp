#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string key;
  string value;
  string line;
  float memtotal{1}, memfree{0};
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> key >> value; 
    if (key == "MemTotal:" && std::all_of(value.begin(), value.end(), isdigit)){
       memtotal = stoi(value);
    }

    std::getline(filestream, line);
    std::istringstream linestream2(line);
    linestream2 >> key >> value;
    if (key == "MemFree:" && std::all_of(value.begin(), value.end(), isdigit)){
       memfree = stoi(value);
    }    
  }
  return (memtotal - memfree) / memtotal; 
}

//  : Read and return the system uptime
long LinuxParser::UpTime() { 
  std::string value1, value2;
  float suspend_sec{0}, idle_proc_sec{0}; 
  long uptime_sec{0};
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  string line;
  if (filestream.is_open()) {
      std::getline(filestream, line);
      std::istringstream linestream(line);
      linestream >> value1 >> value2;    
      suspend_sec = stoi(value1);
      idle_proc_sec = stoi(value2);
         
  }
  uptime_sec = (long)(suspend_sec + idle_proc_sec);
  return uptime_sec;
}

//  : Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  return {}; }

//  : Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line;
  string key;
  string value;
  int n_proc{0};
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes" && std::all_of(value.begin(), value.end(), isdigit)) {
          n_proc = stoi(value);
          return n_proc;
        }
      }
    }
  }
  return n_proc;
}

//  : Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string line;
  string key;
  string value;
  int n_run_proc{0};
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running" && std::all_of(value.begin(), value.end(), isdigit)) {
          n_run_proc = stoi(value);
          return n_run_proc;
        }
      }
    }
  }
  return n_run_proc;
}
