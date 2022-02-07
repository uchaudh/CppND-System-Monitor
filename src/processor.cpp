#include <string>

#include "processor.h"
#include "linux_parser.h"

using std::string;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() 
{
    std::vector<float> valuesf;
    float idle, nonidel,total;

    valuesf.push_back(0.0);

    std::vector<string> values= LinuxParser::CpuUtilization();

    if (values[0] == "cpu")
    {
        for (int i =1; i < values.size(); i++)
        {
        valuesf.push_back(std::stof(values[i]));
        }

        nonidel = valuesf[1]+valuesf[2]+valuesf[3]+valuesf[6]+valuesf[7]+valuesf[8];
        idle = valuesf[4]+valuesf[5];
        total = idle + nonidel;

        return nonidel/total;
    }

    else
    {
        return 0.0;
    }
}