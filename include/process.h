#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid);
  int Pid();
  std::string User();
  std::string Command();
  float CpuUtilization() const;
  std::string Ram();
  long int UpTime();
  bool operator<(Process const& a) const;

  // Private members
 private:
 
 int pid_;
 std::string ram_;
 float cpu_;
 long uptime_;
 std::string command_;
 std::string user_;

};

#endif