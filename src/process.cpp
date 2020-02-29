#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>

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
    m_cpu_util = this->m_ComputeUtilization(m_pid);
    m_uptime = 0;
}

int Process::Pid() {return m_pid;}
string Process::Command() {return m_command;}
string Process::Ram() {return m_ram;}
string Process::User() {return m_user;}

float Process::CpuUtilization() const {return m_cpu_util;}

float Process::m_ComputeUtilization(int pid){
    auto lines = LinuxParser::ProcessUtilizaton(pid);
    long utime = stol(lines[0]);
    long stime = stol(lines[1]);
    long cutime = stol(lines[2]);
    long cstime = stol(lines[3]);
    long starttime = stol(lines[4]);
    long uptime = LinuxParser::UpTime();
    long total_time = utime + stime + cutime + cstime;
    long hertz = sysconf(_SC_CLK_TCK);

    float seconds = uptime - (starttime / hertz);
    float output = ((total_time / hertz) / seconds);

    if(std::isnan(output)){return 0;}
    return output;
}



bool Process::operator<(Process const& a) const { 
    if (a.CpuUtilization() < m_cpu_util){
        return true;
    } 
    return false;
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return 0; }