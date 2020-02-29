#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <algorithm>

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
Processor& System::Cpu() { return cpu_; }

vector<Process>& System::Processes() { 

    m_processes.clear();
    for(auto pid : LinuxParser::Pids()){
        auto process = Process(pid);
        m_processes.push_back(process);
    }
    std::sort(m_processes.begin(), m_processes.end());
    return m_processes;
}

