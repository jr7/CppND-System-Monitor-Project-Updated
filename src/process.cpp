#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid){
    m_pid = pid;
    m_command = LinuxParser::Command(m_pid);
    m_ram = LinuxParser::Ram(m_pid);
    m_user = LinuxParser::User(m_pid);
    m_cpu_util = 0;
    m_uptime = 0;
}

int Process::Pid() {return m_pid;}
string Process::Command() {return m_command;}
string Process::Ram() {return m_ram;}
string Process::User() {return m_user;}


// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { return 0; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return 0; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a[[maybe_unused]]) const { return true; }