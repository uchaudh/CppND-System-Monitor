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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() 
{ 
  string key,line,val;
  float memtotal, memfree, buffers,cached, sreclaim,shmem;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if(stream.is_open())
  {
    while(std::getline(stream,line))
    {
      std::istringstream linestream(line);
      linestream >> key >> val;
      if(key == "MemTotal:") {memtotal = std::stof(val);}
      else if(key == "MemFree:") {memfree = std::stof(val);}
      else if(key == "Buffers:") {buffers = std::stof(val);}
      else if(key == "Cached:") {cached = std::stof(val);}
      else if(key == "SReclaimable:") {sreclaim = std::stof(val);}
      else if(key == "Shmem:") {shmem = std::stof(val);}
    }
  }
  
  return ((memtotal - memfree)-(buffers+(cached+sreclaim-shmem)))/memtotal;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() 
{ 
  string wholetime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if(stream.is_open())
  {
    while(std::getline(stream,line))
    {
      std::istringstream linestream(line);
      while(linestream >> wholetime)
      {
        return std::stol(wholetime);
      }
    }
  }
  return 0;
 }

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization()
{ 
  string line,val;
  std::vector<string> values;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open())
  {
    while(std::getline(stream,line))
    {
      std::istringstream linestream(line);
      while(linestream >> val)
      {
        values.push_back(val);
      }

      if (values[0] == "cpu")
      {
        return values;
      }
    }
  }
  
  return {};
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses()
{
  string line, key, val;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open())
  {
    while(std::getline(stream,line))
    {
      std::istringstream linestream(line);
      linestream >> key >> val;
      if(key == "processes") 
      {
        return std::stoi(val);
      }
    }
  }
  return 0;
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses()
{
  string line, key, val;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open())
  {
    while(std::getline(stream,line))
    {
      std::istringstream linestream(line);
      linestream >> key >> val;
      if(key == "procs_running") 
      {
        return std::stoi(val);
      }
    }
  }
  return 0;
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) 
{ 
  string command;

  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if(stream.is_open())
  {
    std::getline(stream, command);
  }
  
  return command; 
  
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) 
{ 
  string key, ram, line;
  float tempram;

  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if(stream.is_open())
  {
    while (std::getline(stream,line))
    {
      std::istringstream linestream(line);
      linestream >> key >> ram;
      if(key == "VmSize")
      {
        tempram = std::stoi(ram);
        tempram = tempram / 1000;
        ram = std::to_string(tempram);
        break;
      }

    }
  }

  return ram;

}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) 
{ 
  string key, uid,line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + "/status");
  if(stream.is_open())
  {
    while(std::getline(stream,line))
    {
      std::istringstream linestream(line);
      linestream >> key;
      if(key == "Uid:")
      {
        linestream >> uid;
        break;
      }
    }
  }

  return uid;
}

// Read and return the user associated with a process
string LinuxParser::User(int pid)
{ 
  string user,line,id,tempuser,temp, uid;
  uid = Uid(pid);
  std::ifstream stream(kPasswordPath);
  if(stream.is_open())
  {
    while(std::getline(stream, line))
    {
      std::istringstream linestream(line);
      std::replace(line.begin(),line.end(), ':', ' ');
      linestream >> tempuser >> temp >> id;
      if(id == uid)
      {
        user = tempuser;
        break;
      }
    }
  }
  return user;
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) 
{ 
  long uptime;

  return uptime;
}
