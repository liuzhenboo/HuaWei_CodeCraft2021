#include "InputS.h"

InputS::InputS() {
    #ifdef Try
        std::freopen(tryPathFile.c_str(),"r",stdin);
    #endif
    cpuNum_max = 0;
    memorySize_max = 0;
    hardwareCost_max = 0;
    dailyCost_max = 0;
}

void InputS::read_all_Server_Types() {
    std::cin>>serverTypeNum;
    std::cin.ignore(1024, '\n');
    //read server types
    for (int i = 0; i < serverTypeNum; ++i) {
        std::cin.ignore();
        std::string tempStr;
        getline(std::cin, tempStr);
        tempStr.pop_back();

        auto fields = split(tempStr,',');

        std::string name = fields[0];
        int cpuNum = std::stoi(fields[1]);
        int memorySize = std::stoi(fields[2]);
        int hardwareCost = std::stoi(fields[3]);
        int dailyCost = std::stoi(fields[4]);
        serverTypes.push_back({name, cpuNum, memorySize, hardwareCost, dailyCost});
        serverTypeMap[name] = serverTypes.back(); //建立hash map  将name和后面的信息联系起来
        //我们把所有的信息存在vector里面
        //根据name找信息 可以使用serverTypeMap 直接查找

        cpuNum_max = std::max(cpuNum_max, cpuNum);
        memorySize_max = std::max(memorySize_max, memorySize);
        hardwareCost_max = std::max(hardwareCost_max, hardwareCost);
        dailyCost_max = std::max(dailyCost_max, dailyCost);

#ifdef PRINTMORE
        std::cout<<"Server-" <<i+1<<" "<<serverTypeMap[name].toString()<<std::endl;
#endif
    }
}

void InputS::read_all_VirtualHosts() {
    
    std::cin>>virtualHostNum;
    std::cin.ignore(1024, '\n');
    //read virtual hosts types
    for (int i = 0; i < virtualHostNum; ++i) {
        std::cin.ignore();
        std::string tempStr;
        getline(std::cin, tempStr);
        tempStr.pop_back();

        auto fields = split(tempStr,',');

        std::string name = fields[0];
        int cpuNum = std::stoi(fields[1]);
        int memorySize = std::stoi(fields[2]);
        int nodeNum = std::stoi(fields[3]);
        virtualHosts[name] = {name, cpuNum, memorySize, nodeNum};
        //此处根据 不同的虚拟机 在hash map中建立联系
#ifdef PRINTMORE
        std::cout<<"VirtualHost-"<<i+1<<" "<<virtualHosts[name].toString()<<std::endl;
#endif
    }
}

void InputS::read_all_Int() {
    std::cin>>day_number;
    std::cin>>day_number_constraint;
}

void InputS::read_all_Requests() {
    std::vector<Request> requests;
    std::cin>>requestNum;
    std::cin.ignore(1024, '\n');
    for (int i = 0; i < requestNum; ++i) {
        std::cin.ignore();
        std::string tempStr;
        getline(std::cin, tempStr);
        tempStr.pop_back();

        auto fields = split(tempStr,',');

        //对于删除操作，保证ID对应的虚拟机一定存在
        int operation = (fields[0]=="add")? ADD:DEL;
        std::string virtualHostName;
        std::string requestID;
        if(operation == ADD){
            virtualHostName = fields[1].substr(1);
            requestID = fields[2].substr(1);
            requestIDMap[requestID] = virtualHostName;
            //每一个需求都对应ID  删除的时候直接删除相应ID就可以了
            //ID 对应的都是string 相应的名字
        }else{
            //delete
            requestID = fields[1].substr(1);
            virtualHostName = requestIDMap[requestID];
        }
        requests.push_back({operation,virtualHostName,requestID});
#ifdef PRINTMORE
        std::cout<<"Request-"<<i+1<<" "<<requests.back().toString()<<std::endl;
#endif
    }
    allRequests.emplace_back(requests);
}

std::vector<std::string> InputS::split(std::string tempStr, char c) {
    auto ret = std::vector<std::string>();
    size_t pos;
    while((pos = tempStr.find(c))!=std::string::npos){
        ret.emplace_back(tempStr.substr(0, pos));
        tempStr = tempStr.substr(pos + 1);
    }
    ret.emplace_back(tempStr);
    return ret;
}

void InputS::addOutput(std::string s) {
#ifdef PRINTMORE
//    std::cout<<s<<std::endl;
#endif
    outputs.emplace_back(s);
}

