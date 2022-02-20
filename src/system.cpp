#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"
#include "format.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;
/*You need to complete the mentioned TODOs in order to satisfy the rubric criteria "The student will be able to extract and display basic data about the system."

You need to properly format the uptime. Refer to the comments mentioned in format. cpp for formatting the uptime.*/

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

bool CpuUtilizationSort(Process p1, Process p2) {
    return p1 < p2;
}

//Return a container composed of the system's processes
vector<Process>& System::Processes() 
{ 
    vector<int> pids = LinuxParser::Pids();
    processes_.clear();
    for (long unsigned int i = 0; i < pids.size(); i++)
    {
        if(!LinuxParser::Ram(pids[i]).empty())
        {
            Process process(pids[i]);
            processes_.push_back(process);
        }
    }
    std::sort(processes_.rbegin(),processes_.rend(),CpuUtilizationSort);
    return processes_;
}

std::string System::Kernel() 
{
    return LinuxParser::Kernel();
}

float System::MemoryUtilization() 
{ 
    return LinuxParser::MemoryUtilization();
}

std::string System::OperatingSystem() 
{
    return LinuxParser::OperatingSystem(); 
}

int System::RunningProcesses()
{
    return LinuxParser::RunningProcesses(); 
}

int System::TotalProcesses() 
{ 
    return LinuxParser::TotalProcesses(); 
}

long int System::UpTime()
{
    return LinuxParser::UpTime();
}
