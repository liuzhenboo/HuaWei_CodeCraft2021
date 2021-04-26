#ifndef CODECRAFT_2021_INPUTS_H
#define CODECRAFT_2021_INPUTS_H


#include "data_type.h"

struct InputS {
public:

    int serverTypeNum;
    int virtualHostNum;
    int day_number;
    int day_number_constraint;
    int requestNum;

    int cpuNum_max;
    int memorySize_max;
    int hardwareCost_max;
    int dailyCost_max;

    InputS();
    void read_all_Server_Types(); 
    void read_all_VirtualHosts();
    void read_all_Int();
    void read_all_Requests();
    std::vector<std::string> split(std::string tempStr, char c);
    void addOutput(std::string s);

    std::vector<ServerType> serverTypes;
    std::unordered_map<std::string,ServerType> serverTypeMap;
    std::unordered_map<std::string,VirtualHost> virtualHosts;
    // map requestID to VM name
    //通过id 找到VM的名字
    std::unordered_map<std::string, std::string> requestIDMap;

    std::vector<std::vector<Request>> allRequests;

    std::vector<std::string> outputs; //最后所有的输出都储存在这个string里面

};


#endif //CODECRAFT_2021_INPUTS_H
