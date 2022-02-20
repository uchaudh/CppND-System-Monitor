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

Process::Process(int pid)
{
    pid_ = pid;
    // cpu_ = LinuxParser::CpuUtilization(pid);
    command_ = LinuxParser::Command(pid);
    ram_ = LinuxParser::Ram(pid);
    user_ = LinuxParser::User(pid);
    uptime_ = LinuxParser::UpTime(pid);

    long seconds = LinuxParser::UpTime() - uptime_;
    long totaltime = LinuxParser::ActiveJiffies(pid);
    cpu_ = float(totaltime) / float(seconds);

}

// Return this process's ID
int Process::Pid() { return pid_; }

// Return this process's CPU utilization
float Process::CpuUtilization() const { return cpu_; }
 
// Return the command that generated this process
string Process::Command() { return command_; }

// Return this process's memory utilization
string Process::Ram() { return ram_; }

// Return the user (name) that generated this process
string Process::User() { return user_; }

// Return the age of this process (in seconds)
long int Process::UpTime() { return uptime_; }

// TODO: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const 
{ return CpuUtilization() < a.CpuUtilization(); }