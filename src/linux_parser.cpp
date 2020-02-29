#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iterator>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

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

string LinuxParser::Kernel() {
  string os, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel >> kernel;
  }
  return kernel;
}

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


long LinuxParser::UpTime() {
  string line, _, uptime;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line); 
    std::istringstream linestream(line);
    linestream >> uptime >> _;
  }
  return stol(uptime);
}

vector<string> LinuxParser::CpuUtilization() { 
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line); 
    std::istringstream linestream(line);
    std::vector<std::string> output;
    std::string val;

    while (linestream >> val){
      if(val != "cpu"){ output.push_back(val);}
    }
    return output;
  } 
}

string LinuxParser::FindLineByKey(string path, string key){
  std::ifstream filestream(path);
  string line;

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      if (line.find(key) != string::npos)  return line; 
    }
  }
  return string();
}

vector<string> LinuxParser::SplitLine(string line){
  std::istringstream stream(line);
  vector<string> output(std::istream_iterator<string>{stream},
                                  std::istream_iterator<string>());
  return output;
}

string LinuxParser::GetElementByIndex(string line, int index){
  vector<string> out = LinuxParser::SplitLine(line);
  return out[index];
}

float LinuxParser::MemoryUtilization() { 
  string path = kProcDirectory + kMeminfoFilename;
  string memtot_str = LinuxParser::FindLineByKey(path, "MemTotal");
  string memfree_str = LinuxParser::FindLineByKey(path, "MemFree");

  float memtotal = std::stof(LinuxParser::GetElementByIndex(memtot_str, 1));
  float memfree = std::stof(LinuxParser::GetElementByIndex(memfree_str, 1)); 

  return (memtotal - memfree)/memtotal;
}

int LinuxParser::TotalProcesses() { 
  string path = kProcDirectory + kStatFilename;
  string process_str = LinuxParser::FindLineByKey(path, "processes");
  return std::stol(LinuxParser::GetElementByIndex(process_str, 1));
}

int LinuxParser::RunningProcesses() { 
  string path = kProcDirectory + kStatFilename;
  string process_str = LinuxParser::FindLineByKey(path, "procs_running");
  return std::stol(LinuxParser::GetElementByIndex(process_str, 1));
}

string LinuxParser::Command(int pid) { 
  std::string path = kProcDirectory + std::to_string(pid) + kCmdlineFilename;
  string line;

  std::ifstream stream(path);
  if (stream.is_open()) {
    std::getline(stream, line); 
    return line;
  }
}

string LinuxParser::Ram(int pid) {
  std::string path = kProcDirectory + std::to_string(pid) + kStatusFilename;
  string process_str = LinuxParser::FindLineByKey(path, "VmSize");

  if (process_str.size() > 0) {
    float ram_usg =  std::stof(LinuxParser::GetElementByIndex(process_str, 1));
    return std::to_string(int(ram_usg/1000.));
  }
  return "0";
}

string LinuxParser::Uid(int pid) { 
  std::string path = kProcDirectory + std::to_string(pid) + kStatusFilename;
  string uid_line = LinuxParser::FindLineByKey(path, "Uid");
  return LinuxParser::GetElementByIndex(uid_line, 1);
}

string LinuxParser::User(int pid) { 
  string uid = LinuxParser::Uid(pid);
  string user_line = LinuxParser::FindLineByKey(kPasswordPath, uid);
  std::replace(user_line.begin(), user_line.end(), ':', ' ');

  if (user_line.size() > 0){
    return  LinuxParser::GetElementByIndex(user_line, 0);
  }
  return string();
}

vector<string> LinuxParser::ProcessUtilizaton(int pid) { 
  std::string path = kProcDirectory + std::to_string(pid) + kStatFilename;
  std::ifstream filestream(path);
  string line;

  if (filestream.is_open()) { std::getline(filestream, line); }
  std::vector<int> ids{13, 14, 15, 16, 21};

  vector<string> out;
  for (auto i : ids){
    string elem = LinuxParser::GetElementByIndex(line, i);
    if (elem.size() > 0){ out.push_back(elem); }
  }
  return out;
}


long LinuxParser::UpTime(int pid[[maybe_unused]]) { return 0; }

///////////////////
///////////////////
//   NOT USED    //
///////////////////
///////////////////

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }