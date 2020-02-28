#include "processor.h"
#include "linux_parser.h"


using State = LinuxParser::CPUStates;

float Processor::Utilization() { 
    auto cpu = LinuxParser::CpuUtilization();

    float idle = std::stof(cpu[State::kIdle_]) + std::stof(cpu[State::kIOwait_]);

    auto non_idle_states = {State::kUser_, State::kUser_, State::kNice_, State::kSystem_,
                           State::kIRQ_, State::kSoftIRQ_, State::kSteal_};

    float non_idle{0};
    for(auto key : non_idle_states){
        non_idle += std::stof(cpu[key]);
    }

    float total = idle + non_idle;
    float total_d = total - m_prev_total;
    float idle_d = idle - m_prev_idle;

    m_prev_total = total;
    m_prev_idle = idle;
    return (total_d - idle_d)/total_d;
}