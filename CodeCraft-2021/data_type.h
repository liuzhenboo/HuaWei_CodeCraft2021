#ifndef HWCODECRAFT2021_DATATYPE_H
#define HWCODECRAFT2021_DATATYPE_H

#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <time.h>
#include <algorithm>

//#define Try
//#define PRINTMORE
//#define VER1
//#define Migration_print
//#define compute_time

const std::string tryPathFile = "../training-2.txt";
using std::cout;
using std::endl;

typedef uint64_t ull;
typedef int64_t ll;
typedef uint32_t ui;

enum Operation
{
    ADD,
    DEL,
    PURCHASE,
    MIGRATE,
    DEPLOY_A,
    DEPLOY_B,
    DEPLOY_AB,
    UNFIT
};

struct VirtualHost
{
    std::string name;
    int cpuNum, memorySize, nodeNum;
};

struct ServerType
{
    std::string name;
    int cpuNum, memorySize, hardwareCost, dailyCost;
};

bool Judge(std::pair<int, std::pair<int, std::string>> a, std::pair<int, std::pair<int, std::string>> b);

struct Request
{
    int operation;
    std::string virtualHostName;
    std::string requestID; // 创建请求的虚拟机ID唯一

    // std::string toString(){
    //     char buf[1024]={0};
    //     sprintf(buf,"Operation:[%s] virtualHostName:[%s] requestID:[%s]",operation==ADD? "add":"del",virtualHostName.c_str(),requestID.c_str());
    //     return buf;
    // }
};

// 具体的服务器实例
struct Server
{
    int id;         //在服务器集合中的下标
    int remappedID; //实际购买序列中的id

    ServerType serverType;

    //current resource
    std::pair<int, int> A, B;

    //map server to request ID
    std::unordered_set<std::string> requestIDs; //储存当前服务器里面有哪些虚拟机的ID

    Server() = default;
    Server(const ServerType &_s, int _id)
    {
        id = _id;
        remappedID = -1;
        serverType = _s;
        A = {_s.cpuNum / 2, _s.memorySize / 2};
        B = {_s.cpuNum / 2, _s.memorySize / 2};
    }

    bool fit(std::pair<int, int> &C, int cpuNum, int memorySize)
    {
        return C.first >= cpuNum && C.second >= memorySize;
    }
};
#endif //HWCODECRAFT2021_GLOBALDEFINITION_H
