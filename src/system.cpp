#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

std::string System::Kernel() { return LinuxParser::Kernel();}
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }
long int System::UpTime() { return LinuxParser::UpTime();}
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization();}
int System::RunningProcesses() { return LinuxParser::RunningProcesses();}
int System::TotalProcesses() { return LinuxParser::TotalProcesses();}


// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { return processes_; }
