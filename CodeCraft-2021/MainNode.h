#ifndef HWCODECRAFT2021_MainNode_H
#define HWCODECRAFT2021_MainNode_H

#include "InputS.h"
#include <cmath>
#include <algorithm>

class MainNode {

public:
    MainNode(InputS *inputs, int totalDays);

    void dailyRoutine(int day);

    void displayCost();

    double getServerWeight(ServerType &serverType);


    double time1 = 0;
    double time2 = 0;

private:
    InputS *inputs;

    // current bought servers
    std::vector<Server> currentServers;

    // map request ID to Server instance and insert type
    // to resolve corresponding VM,
    // use virtualHosts[requestIDMap[requestID]] or virtualHosts[request.virtualHostName]
    //string 是 id 两个int分别是 哪个服务器和 AB节点
    std::unordered_map<std::string ,std::pair<int, int>> vmOnserver;

    int currentDay,totalDays;
    std::unordered_map<std::string, int> serversToBuy;
    ull serverCost,powerCost;

    std::vector<std::string> migration_result;
    bool migration_success= false;



    void Vm_To_Vm_could_choose(std::unordered_map<std::string ,std::pair<int, int>>& vmOnserver,
                               std::unordered_set<std::string> &Vm_could_choose);

    void delete_ser_vm(std::unordered_set<std::string> &Vm_could_choose, Server &server);

    void Which_vm_to_choose(Server &server,
                            std::unordered_set<std::string>& Vm_could_choose, int& migrate_constraint);

    void purchase_Ser(Request& request);

    void purchase_SerOne(Request& request);

    void distribute_now();

    void Be_putIn(Request& request, bool& result, int& id);

    void migrate();

    int fit_Server(Server &server, Request &request);

    void Compute_balance_ratio(std::vector<double> &Servers_score);

    void Adjustment(std::vector<int> &Servers_score, int migrate_constraint);

    bool Redistribute(std::vector<int>& Servers_need_adjustment,
                      std::vector<std::string>& Vm_single,std::vector<std::string>& Vm_double,
                      std::vector<std::string>& Vm_all);

    double Compute_Vm_score_double(Server &server, VirtualHost & Vm);

    std::pair<int,std::pair<int,std::string>>
    Compute_Vm_score_single(Server &server, VirtualHost & Vm, std::string& id);

    std::pair<int,std::pair<int,std::string>>
    Compute_Vm_score_all(Server &server, VirtualHost &Vm, std::string& id);

    std::string Output_Migration_double(std::string& Vm_id,Server &server);

    std::string Output_Migration_single(std::string& Vm_id,Server &server, int node_now);

    bool JudgeAllzero(std::vector<std::pair<int, std::pair<int, std::string>>>& Vm_single_score);

    bool JudgeAllzeroOne(std::vector<int>& Vm_double_score);

    std::vector<Server> Store_Servers_Info(std::vector<int>& Servers_need_adjustment);

    void Return_server_Info(std::vector<Server>& Store, std::vector<int>& Servers_need_adjustment);

    void AdjustmentOne(std::vector<double> &Servers_score, int migrate_constraint);

    void AdjustmentTwo(std::vector<double> &Servers_score, int migrate_constraint);

    void To_sort_Vm(std::multimap<int, std::string, std::less<int>>& Vm_sort,
                    std::unordered_set<std::string>& Vm_here_all);

    void printSerAndVm(std::vector<int> &Servers_need_adjustment);

    void printServerInfo(Server & server);

    void printServerVmId(Server & server);

    void printVmInfo(std::string Vm_id);

};


#endif //HWCODECRAFT2021_MainNode_H
