#include "MainNode.h"

double MainNode::getServerWeight(ServerType &serverType)
{
    return serverType.cpuNum * 0.75 + serverType.memorySize * 0.22 + serverType.hardwareCost * 0.01 + serverType.dailyCost * 0.02 * (totalDays - currentDay);
}

void MainNode::purchase_Ser(Request &request)
{

    VirtualHost &virtualHost = inputs->virtualHosts[request.virtualHostName];
    //读取当前的虚拟机名称代表的信息

    double k1 = -1.0,
           k2 = -1.0,
           k3 = 10.0,
           k4 = 0.001;

    /*
    if (currentDay >= totalDays-2)
    {
        k1 = 1; k2 = 1;
    }
    */

    std::vector<ServerType> fitableServer;
    std::vector<double> score_total;
    std::vector<double> score_1;
    std::vector<double> score_2;
    std::vector<double> score_3;
    std::vector<double> score_4;

    double qk = 0.7;

    if (virtualHost.nodeNum) // 双节点分布
    {
        for (ui i = 0; i < inputs->serverTypes.size(); i++)
        {
            if (score_1.size() >= inputs->serverTypes.size() * qk)
                break;

            if (virtualHost.cpuNum / 2 <= inputs->serverTypes[i].cpuNum / 2 && virtualHost.memorySize / 2 <= inputs->serverTypes[i].memorySize / 2)
            {
                fitableServer.emplace_back(inputs->serverTypes[i]);
                score_1.emplace_back(2 * std::fabs(virtualHost.cpuNum / 2 - inputs->serverTypes[i].cpuNum / 2));
                score_2.emplace_back(2 * std::fabs(virtualHost.memorySize / 2 - inputs->serverTypes[i].memorySize / 2));
                score_3.emplace_back(std::fabs(inputs->serverTypes[i].hardwareCost));
                score_4.emplace_back(std::fabs(inputs->serverTypes[i].dailyCost));
            }
        }
    }
    else
    {
        for (ui i = 0; i < inputs->serverTypes.size(); i++)
        {
            if (score_1.size() >= inputs->serverTypes.size() * qk)
                break;

            if (virtualHost.cpuNum <= inputs->serverTypes[i].cpuNum / 2 && virtualHost.memorySize <= inputs->serverTypes[i].memorySize / 2)
            {
                fitableServer.emplace_back(inputs->serverTypes[i]);
                score_1.emplace_back(std::fabs(virtualHost.cpuNum - inputs->serverTypes[i].cpuNum));
                score_2.emplace_back(std::fabs(virtualHost.memorySize - inputs->serverTypes[i].memorySize));
                score_3.emplace_back(std::fabs(inputs->serverTypes[i].hardwareCost));
                score_4.emplace_back(std::fabs(inputs->serverTypes[i].dailyCost));
            }
        }
    }

    double score_1_max = *max_element(score_1.begin(), score_1.end());
    double score_2_max = *max_element(score_2.begin(), score_2.end());
    double score_3_max = *max_element(score_3.begin(), score_3.end());
    double score_4_max = *max_element(score_4.begin(), score_4.end());

    for (ui i = 0; i < fitableServer.size(); i++)
    {
        double score = k1 * score_1[i] / score_1_max + k2 * score_2[i] / score_2_max + k3 * score_3[i] / score_3_max + k4 * score_4[i] / score_4_max;
        score_total.emplace_back(score);
    }

    int best_server_index = min_element(score_total.begin(), score_total.end()) - score_total.begin();

    // ServerType &serverType = inputs->serverTypes[20];
    ServerType &serverType = fitableServer[best_server_index];
    serversToBuy[serverType.name]++;

    currentServers.emplace_back(serverType, currentServers.size());

    // int ret = fit_Server(currentServers[currentServers.size() - 1], request);
}

void MainNode::purchase_SerOne(Request &request)
{
    VirtualHost &virtualHost = inputs->virtualHosts[request.virtualHostName];
    //读取当前的虚拟机名称代表的信息
    std::vector<ServerType> fitableServer;
    std::vector<double> score_total;
    std::vector<double> score_1;
    std::vector<double> score_2;
    std::vector<double> score_3;
    std::vector<double> score_4;

    if (virtualHost.nodeNum) // 双节点分布
    {
        for (ui i = 0; i < inputs->serverTypes.size(); i++)
        {
            if (virtualHost.cpuNum / 2 <= inputs->serverTypes[i].cpuNum / 2 && virtualHost.memorySize / 2 <= inputs->serverTypes[i].memorySize / 2)
            {
                fitableServer.emplace_back(inputs->serverTypes[i]);
                score_1.emplace_back(2 * std::fabs(virtualHost.cpuNum / 2 - inputs->serverTypes[i].cpuNum / 2));
                score_2.emplace_back(2 * std::fabs(virtualHost.memorySize / 2 - inputs->serverTypes[i].memorySize / 2));
                score_3.emplace_back(std::fabs(inputs->serverTypes[i].hardwareCost));
                score_4.emplace_back(std::fabs(inputs->serverTypes[i].dailyCost));
            }
        }
    }
    else
    {
        for (ui i = 0; i < inputs->serverTypes.size(); i++)
        {
            if (virtualHost.cpuNum <= inputs->serverTypes[i].cpuNum / 2 && virtualHost.memorySize <= inputs->serverTypes[i].memorySize / 2)
            {
                fitableServer.emplace_back(inputs->serverTypes[i]);
                score_1.emplace_back(std::fabs(virtualHost.cpuNum - inputs->serverTypes[i].cpuNum));
                score_2.emplace_back(std::fabs(virtualHost.memorySize - inputs->serverTypes[i].memorySize));
                score_3.emplace_back(std::fabs(inputs->serverTypes[i].hardwareCost));
                score_4.emplace_back(std::fabs(inputs->serverTypes[i].dailyCost));
            }
        }
    }

    double score_1_max = *max_element(score_1.begin(), score_1.end());
    double score_2_max = *max_element(score_2.begin(), score_2.end());
    double score_3_max = *max_element(score_3.begin(), score_3.end());
    double score_4_max = *max_element(score_4.begin(), score_4.end());

    double k1 = -1.0, k2 = -1.0, k3 = 10.0, k4 = 0.001;
    for (ui i = 0; i < fitableServer.size(); i++)
    {
        double score = k1 * score_1[i] / score_1_max + k2 * score_2[i] / score_2_max + k3 * score_3[i] / score_3_max + k4 * score_4[i] / score_4_max;
        score_total.emplace_back(score);
    }

    int best_server_index = min_element(score_total.begin(), score_total.end()) - score_total.begin();

    // ServerType &serverType = inputs->serverTypes[20];
    ServerType &serverType = fitableServer[best_server_index];
    serversToBuy[serverType.name]++;

    currentServers.emplace_back(serverType, currentServers.size());
}

//函数：往服务器里面装虚拟机
//输入：要装的服务器server 和 对应的request的虚拟机
//输出：合适 不合适 装在AB 哪个节点上
int MainNode::fit_Server(Server &server, Request &request)
{
    VirtualHost &virtualHost = inputs->virtualHosts[request.virtualHostName];
    bool fit = false;
    int coreCost = virtualHost.cpuNum, memoryCost = virtualHost.memorySize;
    /*
    if(virtualHost.nodeNum)  // 双节点分布
    {  
        coreCost /= 2; memoryCost /= 2;
        fit = server.fit(server.A, coreCost, memoryCost) && server.fit(server.B,coreCost, memoryCost);
        if(fit)   //经过上面的判断  如果放的下
        {            
            server.A.first -= coreCost;
            server.B.first -= coreCost;
            server.A.second -= memoryCost;
            server.B.second -= memoryCost;
            return DEPLOY_AB;
        }
    }else  // 单节点分布
    {

        bool fitA = server.fit(server.A, coreCost, memoryCost);
        bool fitB = server.fit(server.B, coreCost, memoryCost);
        if (fitA && fitB)
        {
            int A_left = server.A.first + server.A.second - (coreCost+memoryCost);
            int B_left = server.B.first + server.B.second - (coreCost+memoryCost);
            if (A_left <= B_left)
            {
                server.B.first -= coreCost;
                server.B.second -= memoryCost;
                return DEPLOY_B;
            }
            else
            {
                server.A.first -= coreCost;
                server.A.second -= memoryCost;
                return DEPLOY_A;
            }

        }
        else if (fitA && !fitB)
        {
            server.A.first -= coreCost;
            server.A.second -= memoryCost;
            return DEPLOY_A;
        }
        else if (!fitA && fitB)
        {
            server.B.first -= coreCost;
            server.B.second -= memoryCost;
            return DEPLOY_B;
        }

    }
    return UNFIT;
    */
    if (virtualHost.nodeNum) // 双节点分布
    {
        coreCost /= 2;
        memoryCost /= 2;
        fit = server.fit(server.A, coreCost, memoryCost) && server.fit(server.B, coreCost, memoryCost);
        if (fit) //经过上面的判断  如果放的下
        {
            server.A.first -= coreCost;
            server.B.first -= coreCost;
            server.A.second -= memoryCost;
            server.B.second -= memoryCost;
            return DEPLOY_AB;
        }
    }
    else // 单节点分布
    {
        fit |= server.fit(server.A, coreCost, memoryCost);
        if (fit)
        {
            server.A.first -= coreCost;
            server.A.second -= memoryCost;
            return DEPLOY_A;
        }
        else
        {
            fit |= server.fit(server.B, coreCost, memoryCost);
            if (fit)
            {
                server.B.first -= coreCost;
                server.B.second -= memoryCost;
                return DEPLOY_B;
            }
        }
    }
    return UNFIT;
}

void MainNode::Be_putIn(Request &request, bool &result, int &id)
{

    std::multimap<int, int> left_compare;
    int left;

    for (Server &server : currentServers)
    { //如果能够装下
        //if (left_compare.size() >= currentServers.size()*2/3)
        //break;

        auto Vm = inputs->virtualHosts[request.virtualHostName];

        if (Vm.nodeNum == 0) //单节点
        {
            if (server.fit(server.A, Vm.cpuNum, Vm.memorySize) && server.fit(server.B, Vm.cpuNum, Vm.memorySize))
            {
                left = std::min((server.A.first - Vm.cpuNum + server.A.second - Vm.memorySize), (server.B.first - Vm.cpuNum + server.B.second - Vm.memorySize));
                left_compare.insert({left, server.id});
            }
            else if (!server.fit(server.A, Vm.cpuNum, Vm.memorySize) && server.fit(server.B, Vm.cpuNum, Vm.memorySize))
            {
                left = (server.B.first - Vm.cpuNum + server.B.second - Vm.memorySize);
                left_compare.insert({left, server.id});
            }
            else if (server.fit(server.A, Vm.cpuNum, Vm.memorySize) && !server.fit(server.B, Vm.cpuNum, Vm.memorySize))
            {

                left = (server.A.first - Vm.cpuNum + server.A.second - Vm.memorySize);
                left_compare.insert({left, server.id});
            }
        }
        else //双节点
        {
            if (server.fit(server.A, Vm.cpuNum / 2, Vm.memorySize / 2) && server.fit(server.B, Vm.cpuNum / 2, Vm.memorySize / 2))
            {
                left = (server.A.first + server.B.first - Vm.cpuNum + server.A.second + server.B.second - Vm.memorySize);
                left_compare.insert({left, server.id});
            }
        }
    }
    if (left_compare.empty())
    {
        result = false;
    }
    else
    {
        result = true;
        id = left_compare.begin()->second;
    }
}

void MainNode::distribute_now()
{

    migration_result.clear();

    migrate();
    //由于我们在处理请求的时候 是处理完了之后直接购买的
    //因此 我决定先迁移之前的 在迁移完了时候 再按照需求购买

    //裁判程序 1.购买操作 2.迁移操作 3.创建和删除操作

    std::vector<Request> &requests = inputs->allRequests[currentDay];
    //这一天所有的需求
    int previousServerNum = currentServers.size(); // 现在有哪些服务器 currentServers

    // 生成要购买的服务器序列
    for (Request &request : requests)
    { //遍历当天所有的请求
        if (request.operation == ADD)
        {
            bool flag = false;
            int ret;
            int ser_id;

            //当前有许多服务器都适合 放在那个剩余最小的服务器
            // 左int 剩多少 右int 服务器id less排序
            //std::multimap<int, int> left_compare;

            //函数 判断适合放在哪个服务器里面
            // 输入request 输出 最适合的服务器id true
            Be_putIn(request, flag, ser_id);

            if (flag)
            {
                auto server = &currentServers[ser_id];
                if ((ret = fit_Server(*server, request)) != UNFIT)
                {
                    server->requestIDs.insert(request.requestID);
                    vmOnserver[request.requestID] = {server->id, ret};
                    //每个虚拟机的ID 后面匹配的是 服务器的ID 配置到哪个节点上 AB 或者A,B
                    //break;
                }
            }

            if (!flag)
            { //如果所有的服务器都装不下
                purchase_Ser(request);
                Server &server = currentServers[currentServers.size() - 1];
                ret = fit_Server(server, request);
                server.requestIDs.insert(request.requestID);
                vmOnserver[request.requestID] = {server.id, ret};
            }
        }
        else
        { // 不是add 就是 delete
            // delete
            auto &ret = vmOnserver[request.requestID];                                // 服务器ID + Operation
            Server &server = currentServers[ret.first];                               // 通过服务器ID找到对应的服务器
            VirtualHost &virtualHost = inputs->virtualHosts[request.virtualHostName]; // 通过虚拟机名字找到虚拟机性质
            server.requestIDs.erase(request.requestID);

            vmOnserver.erase(request.requestID);

            if (ret.second == DEPLOY_AB)
            {
                server.A.first += virtualHost.cpuNum / 2;
                server.B.first += virtualHost.cpuNum / 2;
                server.A.second += virtualHost.memorySize / 2;
                server.B.second += virtualHost.memorySize / 2;
            }
            else
            {
                if (ret.second == DEPLOY_A)
                {
                    server.A.first += virtualHost.cpuNum;
                    server.A.second += virtualHost.memorySize;
                }
                else
                {
                    server.B.first += virtualHost.cpuNum;
                    server.B.second += virtualHost.memorySize;
                }
            }
        }
    }

    //保证在同一天买的相同的服务器，remappedID值是相连的
    inputs->addOutput("(purchase, " + std::to_string(serversToBuy.size()) + ")");

    int currentServerNum = currentServers.size();
    int currentID = previousServerNum;
    for (int i = previousServerNum; i < currentServerNum; i++)
    {
        Server &server = currentServers[i];
        if (server.remappedID == -1)
        {
            int num = serversToBuy[server.serverType.name];
            serverCost += server.serverType.hardwareCost * (ull)num;
            inputs->addOutput("(" + server.serverType.name + ", " + std::to_string(num) + ")");
            for (int j = i; j < currentServerNum; j++)
            {
                if (server.serverType.name == currentServers[j].serverType.name)
                {
                    // 同一类型服务器，只输出一个  名字 + 数量
                    currentServers[j].remappedID = currentID++;
                }
            }
        }
    }
    serversToBuy.clear();
    //migrate();
    /*
    if (migration_success)
    {
        //cout<<"migration success size= "<<migration_result.size()<<endl;
        int s = migration_result.size();
        inputs->addOutput("(migration," + std::to_string(s) +")");
        for(auto it:migration_result)
        {
            inputs->addOutput(it);

            //cout<<it<<" ";
        }
        //cout<<endl;

    } else
    {
        //cout<<"migration error"<<endl;
        inputs->addOutput("(migration, 0)");
    }
     */
    if (migration_result.size() == 0)
    {
        inputs->addOutput("(migration, 0)");
    }
    else
    {
        //cout<<"migration success size= "<<migration_result.size()<<endl;
        int s = migration_result.size();
        //cout<<"qianyi num="<<s<<endl;
        inputs->addOutput("(migration," + std::to_string(s) + ")");
        for (auto it : migration_result)
        {
            inputs->addOutput(it);
            //cout<<it<<" ";
        }
    }

    // 输出add操作对应的分配序列
    for (Request &request : requests)
    {
        if (request.operation == ADD)
        {
            auto &ret = vmOnserver[request.requestID];
            Server &server = currentServers[ret.first];
            std::string tmpStr = "(" + std::to_string(server.remappedID);
            if (ret.second == DEPLOY_AB)
            {
                inputs->addOutput(tmpStr + ")");
            }
            else if (ret.second == DEPLOY_A)
            {
                inputs->addOutput(tmpStr + ", A)");
            }
            else
                inputs->addOutput(tmpStr + ", B)");
        }
    }

    // compute daily cost
    for (Server &server : currentServers)
    {
        if (!server.requestIDs.empty())
            powerCost += server.serverType.dailyCost;
    }
}

void MainNode::migrate()
{
    //inputs->addOutput("(migration, 0)");

    int migrate_constraint = int(vmOnserver.size()) * 3 / 100; //-1
    // if (currentDay == totalDays * 5 / 7)
    //     migrate_constraint = int(vmOnserver.size()) - 1;
    //cout<<"vmOnserver.size="<<vmOnserver.size()<<endl;
    //cout<<"migrate_constraint="<<migrate_constraint<<endl;
    //migrate_constraint = int(migrate_constraint)-1;
    if (migrate_constraint < 1)
    {
        migration_success = false;
        return;
    }

#ifdef Migration_print
    cout << "vmOnserver.size=" << vmOnserver.size() << endl;
    cout << "migrate_constraint=" << migrate_constraint << endl;
#endif
    //int migrate_number = 0;
    //int Servers_number = currentServers.size();
    //if (migrate_constraint >= 50)
    // migrate_constraint = 50;  //限制一下这个constraint 不能太大

    //std::map<int,int> Servers_balance_ratio; //左边的是占有率  右边的是服务器id
    std::vector<double> Servers_score;
    Compute_balance_ratio(Servers_score);

    //Adjustment(Servers_score,migrate_constraint);
    AdjustmentOne(Servers_score, migrate_constraint);
    //AdjustmentTwo(Servers_score,migrate_constraint);
    //如果调整的数量小于constraint 那么就可以完成调整
    //并再调整一次

    //如果调整之后发现大于constraint 那么就跳出调整的循环
}

void MainNode::To_sort_Vm(std::multimap<int, std::string, std::less<int>> &Vm_sort,
                          std::unordered_set<std::string> &Vm_here_all)
{
    int score;
    int knode;
    for (auto Vm : Vm_here_all)
    {
        auto vm_type = inputs->virtualHosts[inputs->requestIDMap[Vm]];

        /*
        if (vm_type.nodeNum == 0)
            knode = (vm_type.cpuNum + vm_type.memorySize)/4;
        else
            knode = 0;
        */

        score = vm_type.cpuNum + vm_type.memorySize + knode;

        Vm_sort.insert({score, Vm});
    }
}

void MainNode::AdjustmentOne(std::vector<double> &Servers_score, int migrate_constraint)
{
#ifdef Migration_print
    cout << "begin Adjustment" << endl;
#endif
    std::vector<int> Servers_need_adjustment; //储存相应的id
    int Wm_number = 0;

#ifdef Migration_print
    cout << "chose Servers" << endl;
#endif
    //while (Wm_number <= migrate_constraint*5)
    //while (ser <= currentServers.size()-2)
#ifdef compute_time
    clock_t start, end;
    start = clock();
#endif

    std::multimap<double, int, std::greater<double>> score_sort;
    for (int i = 0; i < currentServers.size() * 100 / 100; i++)
    {
        score_sort.insert({Servers_score[i], i});
        //score_sort.insert({Servers_score[currentServers.size()-1-i],currentServers.size()-1-i});
    }

    //for(int i=0;i<score_sort.size(); i++)
    //auto itL = score_sort.end();
    //itL--;

    for (auto it : score_sort)
    {
        Servers_need_adjustment.push_back(it.second);

        if (Servers_need_adjustment.size() >= currentServers.size() * 95 / 100)
            break;
    }
    /*
    int ser = 0;
    while (ser <= currentServers.size())
    //while (ser <= currentServers.size()/5*3)
    {
        int maxPos = max_element(Servers_score.begin(),Servers_score.end()) - Servers_score.begin();
        auto it  = &currentServers[maxPos];
        Wm_number = Wm_number + it->requestIDs.size();
        Servers_need_adjustment.push_back(it->id); //这里进去的是服务器的id
        Servers_score[maxPos] = 0;
        ser++;
    }
*/

#ifdef compute_time
    end = clock();
    time1 = time1 + double(end - start) / CLOCKS_PER_SEC;
#endif

    migration_result.clear();

    //这里暂时的想法是：进来两倍限制数量的服务器
    //对这些服务器里面的虚拟机 从分高的虚拟机开始
    //一个个从分低的开始塞 如果能塞下 就迁移
    //如果不能塞下 也就是指针 到自己这里了 那么就忽视这个虚拟机

#ifdef Migration_print
    printSerAndVm(Servers_need_adjustment);
    cout << "Servers_need_adjustment.size=" << Servers_need_adjustment.size() << endl;
#endif

#ifdef compute_time
    clock_t start1, end1;
    start1 = clock();
#endif

    int migration_number = 0;
    //for(int i=Servers_need_adjustment.size()-1;i>1;i--)
    //for(int i=0;i<Servers_need_adjustment.size()-1;i++)

    std::unordered_set<std::string> Vm_be_migrated;

    for (int i = 0; i < Servers_need_adjustment.size(); i++)
    {
        //if (migration_number >= migrate_constraint-1)
        if (migration_number >= migrate_constraint)
            break;

        if (i >= Servers_need_adjustment.size() * 20 / 100)
            break;

        auto server_i = &currentServers[Servers_need_adjustment[i]];
#ifdef Migration_print
        cout << "i=" << i << endl;
        printServerInfo(*server_i);
        printServerVmId(*server_i);
#endif
        //当前要拿出来的服务器
        auto Vm_here_all = server_i->requestIDs;

        //从大的开始塞 还是从小的开始塞 比较好？此处为乱序塞的

        //std::multimap<int, std::string, std::less<int>> Vm_sort;

        //To_sort_Vm(Vm_sort, Vm_here_all);

        // for(auto it = Vm_sort.begin(); it!= Vm_sort.end();it++)
        for (auto it = Vm_here_all.begin(); it != Vm_here_all.end(); it++)
        {

            if (migration_number >= migrate_constraint)
                break;

            //auto Vm_name = it->second;
            auto Vm_name = *it;

#ifdef Migration_print
            cout << "Vm name = " << Vm_name << endl;
            printVmInfo(Vm_name);
#endif
            VirtualHost &Vm_type = inputs->virtualHosts[inputs->requestIDMap[Vm_name]];

            if (Vm_be_migrated.find(Vm_name) != Vm_be_migrated.end())
                continue;

            //auto Vm_type = inputs->virtualHosts[Vm_name];
            int node = vmOnserver[Vm_name].second;
            bool be_put_in = false;

            //选最右边的服务器
            //for(int j=Servers_need_adjustment.size()-1;j>=0;j--)
            for (int j = Servers_need_adjustment.size() - 1; j >= 0; j--)
            {
                if (migration_number >= migrate_constraint)
                    break;

                if (j == i)
                    break;

                auto server_now = &currentServers[Servers_need_adjustment[j]];
#ifdef Migration_print
                cout << "j=" << j << "ser id=" << server_now->remappedID << endl;
                printServerInfo(*server_now);
#endif
                if (Vm_type.nodeNum == 0) //单节点
                {
                    if (server_now->fit(server_now->A, Vm_type.cpuNum, Vm_type.memorySize) &&
                        server_now->fit(server_now->B, Vm_type.cpuNum, Vm_type.memorySize))
                    {
#ifdef Migration_print
                        cout << "AB dou" << endl;
#endif
                        int resultA = server_now->A.first - Vm_type.cpuNum + server_now->A.second - Vm_type.memorySize;
                        int resultB = server_now->B.first - Vm_type.cpuNum + server_now->B.second - Vm_type.memorySize;
                        //放在剩的少的那里
                        if (resultA >= resultB)
                        {
                            server_now->B.first = server_now->B.first - Vm_type.cpuNum;
                            server_now->B.second = server_now->B.second - Vm_type.memorySize;
                            server_now->requestIDs.insert(Vm_name);

                            std::string s =
                                Output_Migration_single(Vm_name, *server_now, DEPLOY_B);

                            vmOnserver[Vm_name] = {server_now->id, DEPLOY_B};
                            Vm_be_migrated.insert(Vm_name);
                            //it++;
                            migration_number++;
                            if (s.size() != 0)
                            {
                                //inputs->addOutput(s);
                                migration_result.push_back(s);
#ifdef Migration_print
                                cout << s << endl;
#endif
                            }
                        }
                        else
                        {
                            server_now->A.first = server_now->A.first - Vm_type.cpuNum;
                            server_now->A.second = server_now->A.second - Vm_type.memorySize;
                            server_now->requestIDs.insert(Vm_name);
                            std::string s = Output_Migration_single(Vm_name, *server_now, DEPLOY_A);
                            vmOnserver[Vm_name] = {server_now->id, DEPLOY_A};
                            Vm_be_migrated.insert(Vm_name);
                            migration_number++;
                            if (s.size() != 0)
                            {
                                //inputs->addOutput(s);
                                migration_result.push_back(s);
#ifdef Migration_print
                                cout << s << endl;
#endif
                            }
                        }
                        be_put_in = true;
                    }

                    else if (server_now->fit(server_now->A, Vm_type.cpuNum, Vm_type.memorySize) &&
                             !server_now->fit(server_now->B, Vm_type.cpuNum, Vm_type.memorySize))
                    {
#ifdef Migration_print
                        cout << "A" << endl;
#endif
                        server_now->A.first = server_now->A.first - Vm_type.cpuNum;

                        server_now->A.second = server_now->A.second - Vm_type.memorySize;
                        server_now->requestIDs.insert(Vm_name);

                        be_put_in = true;

                        std::string s = Output_Migration_single(Vm_name, *server_now, DEPLOY_A);
                        migration_number++;
                        vmOnserver[Vm_name] = {server_now->id, DEPLOY_A};
                        Vm_be_migrated.insert(Vm_name);

                        if (s.size() != 0)
                        {
                            //inputs->addOutput(s);
                            migration_result.push_back(s);
#ifdef Migration_print
                            cout << s << endl;
#endif
                        }
                    }

                    else if (!server_now->fit(server_now->A, Vm_type.cpuNum, Vm_type.memorySize) &&
                             server_now->fit(server_now->B, Vm_type.cpuNum, Vm_type.memorySize))
                    {
#ifdef Migration_print
                        cout << "B" << endl;
#endif
                        server_now->B.first = server_now->B.first - Vm_type.cpuNum;
                        server_now->B.second = server_now->B.second - Vm_type.memorySize;
                        server_now->requestIDs.insert(Vm_name);
                        be_put_in = true;
                        migration_number++;
                        std::string s = Output_Migration_single(Vm_name, *server_now, DEPLOY_B);
                        vmOnserver[Vm_name] = {server_now->id, DEPLOY_B};
                        Vm_be_migrated.insert(Vm_name);
                        if (s.size() != 0)
                        {
                            //inputs->addOutput(s);
                            migration_result.push_back(s);
#ifdef Migration_print
                            cout << s << endl;
#endif
                        }
                    }

                    if (be_put_in)
                    {
                        //原来的放在哪个节点上
                        //migration_number++;
                        server_i->requestIDs.erase(Vm_name);
                        if (node == DEPLOY_A)
                        {
                            server_i->A.first = server_i->A.first + Vm_type.cpuNum;
                            server_i->A.second = server_i->A.second + Vm_type.memorySize;
                        }
                        if (node == DEPLOY_B)
                        {
                            server_i->B.first = server_i->B.first + Vm_type.cpuNum;
                            server_i->B.second = server_i->B.second + Vm_type.memorySize;
                        }
                        break;
                    }
                    else
                    {
                        continue;
                    }
                }
                else //双节点
                {

                    if (server_now->fit(server_now->A, Vm_type.cpuNum / 2, Vm_type.memorySize / 2) &&
                        server_now->fit(server_now->B, Vm_type.cpuNum / 2, Vm_type.memorySize / 2))
                    {
#ifdef Migration_print
                        cout << "AB shuang jie dian" << endl;
#endif
                        server_now->A.first = server_now->A.first - Vm_type.cpuNum / 2;
                        server_now->A.second = server_now->A.second - Vm_type.memorySize / 2;
                        server_now->B.first = server_now->B.first - Vm_type.cpuNum / 2;
                        server_now->B.second = server_now->B.second - Vm_type.memorySize / 2;

                        server_i->A.first = server_i->A.first + Vm_type.cpuNum / 2;
                        server_i->A.second = server_i->A.second + Vm_type.memorySize / 2;
                        server_i->B.first = server_i->B.first + Vm_type.cpuNum / 2;
                        server_i->B.second = server_i->B.second + Vm_type.memorySize / 2;

                        server_now->requestIDs.insert(Vm_name);
                        server_i->requestIDs.erase(Vm_name);

                        std::string s = Output_Migration_double(Vm_name, *server_now);

                        vmOnserver[Vm_name] = {server_now->id, DEPLOY_AB};
                        Vm_be_migrated.insert(Vm_name);
                        migration_number++;

                        if (s.size() != 0)
                        {
                            //inputs->addOutput(s);
                            migration_result.push_back(s);
#ifdef Migration_print
                            cout << s << endl;
#endif
                        }
                        break;
                    }
                    else
                    {
                        continue;
                    }
                }
            }
        }
    }

#ifdef compute_time
    end1 = clock();
    time2 = time2 + double(end1 - start1) / CLOCKS_PER_SEC;
#endif
}

void MainNode::AdjustmentTwo(std::vector<double> &Servers_score, int migrate_constraint)
{

    //cout<<"AdjustmentTwo"<<endl;

    std::vector<int> Servers_need_adjustment; //储存相应的id
    int Wm_number = 0;

    std::multimap<double, int, std::greater<double>> score_sort;
    //进去的分数
    for (int i = 0; i < currentServers.size() * 100 / 100; i++)
    {
        score_sort.insert({Servers_score[i], i});
    }

    for (auto it : score_sort)
    {
        Servers_need_adjustment.push_back(it.second);
        //if (Servers_need_adjustment.size() >= currentServers.size()*70/100)
        //break;
    }
    /*
    一个新的思路：从比较饱满的服务器开始
    从前面所有的虚拟机集合中 开始挑选合适的来填满这个空间
    如果没有合适的虚拟机可以填满  那么就去计算下一个服务器
    */
    std::unordered_set<std::string> Vm_could_choose;

    //把当前的所有vm id更新到Vm_could_choose
    Vm_To_Vm_could_choose(vmOnserver, Vm_could_choose);

    int count = 0;
    int constraint = migrate_constraint;
    for (auto server_id : Servers_need_adjustment)
    { //从分低的开始计算
        auto server_now = &currentServers[server_id];
        //删除当前服务器的里面的vm id
        //cout<<"delete_ser_vm"<<endl;
        delete_ser_vm(Vm_could_choose, *server_now);

        //cout<<"Which_vm_to_choose"<<endl;
        if (server_now->A.first <= 10 || server_now->A.second <= 10 || server_now->B.first <= 10 || server_now->B.first <= 10)
            continue;

        Which_vm_to_choose(*server_now, Vm_could_choose, constraint);

        count++;

        if (constraint == 0)
            break;
        if (count >= Servers_need_adjustment.size() * 1 / 100)
            break;
    }
}

void MainNode::Which_vm_to_choose(Server &server,
                                  std::unordered_set<std::string> &Vm_could_choose, int &migrate_constraint)
{
    //把可以选择虚拟机排序后 依次往当前的服务器里面塞
    std::multimap<int, std::string, std::greater<int>> Vm_sort;

    //std::unordered_set<std::string> Vm_res;

    int sum;
    for (auto vm_id : Vm_could_choose)
    {
        auto vm_type = inputs->virtualHosts[inputs->requestIDMap[vm_id]];

        if (vm_type.nodeNum == 0)
            sum = vm_type.cpuNum + vm_type.memorySize;
        else
            sum = (vm_type.cpuNum + vm_type.memorySize) * 5 / 4;

        Vm_sort.insert({sum, vm_id});
    } //从分高的开始进行

    bool going_on = true;
    int A_cpu, A_memo;
    int B_cpu, B_memo;

    /*
    A_cpu = server.A.first;
    A_memo = server.A.second;
    B_cpu = server.B.first;
    B_memo = server.B.second;
    */
    auto server_now = &server;
    //这里开始往里面塞
    //直接装就完事了

    int migration_number = 0;

    //cout<<"--begin sai --"<<endl;

    for (auto vm_id : Vm_sort)
    {

        if (server_now->A.first <= 10 || server_now->A.second <= 10 || server_now->B.first <= 10 || server_now->B.first <= 10)
            break;

        auto Vm_type = inputs->virtualHosts[inputs->requestIDMap[vm_id.second]];
        auto Vm_name = vm_id.second;

        //当前虚拟机所在服务器的情况
        auto server_i = &currentServers[vmOnserver[vm_id.second].first];
        auto node = vmOnserver[vm_id.second].second;

        bool be_put_in = false;
        if (Vm_type.nodeNum == 0) //单节点
        {
            if (server_now->fit(server_now->A, Vm_type.cpuNum, Vm_type.memorySize) &&
                server_now->fit(server_now->B, Vm_type.cpuNum, Vm_type.memorySize))
            {
#ifdef Migration_print
                cout << "AB dou" << endl;
#endif
                int resultA = server_now->A.first - Vm_type.cpuNum + server_now->A.second - Vm_type.memorySize;
                int resultB = server_now->B.first - Vm_type.cpuNum + server_now->B.second - Vm_type.memorySize;
                //放在剩的少的那里
                if (resultA >= resultB)
                {
                    server_now->B.first = server_now->B.first - Vm_type.cpuNum;
                    server_now->B.second = server_now->B.second - Vm_type.memorySize;
                    server_now->requestIDs.insert(Vm_name);

                    std::string s =
                        Output_Migration_single(Vm_name, *server_now, DEPLOY_B);

                    vmOnserver[Vm_name] = {server_now->id, DEPLOY_B};

                    //Vm_be_migrated.insert(Vm_name);
                    //it++;
                    //migration_number++;

                    if (!s.empty())
                    {
                        //inputs->addOutput(s);
                        migration_result.push_back(s);
#ifdef Migration_print
                        cout << s << endl;
#endif
                    }

                    migrate_constraint--;
                    if (migrate_constraint == 0)
                        break;
                }
                else
                {
                    server_now->A.first = server_now->A.first - Vm_type.cpuNum;
                    server_now->A.second = server_now->A.second - Vm_type.memorySize;
                    server_now->requestIDs.insert(Vm_name);
                    std::string s = Output_Migration_single(Vm_name, *server_now, DEPLOY_A);
                    vmOnserver[Vm_name] = {server_now->id, DEPLOY_A};

                    //Vm_be_migrated.insert(Vm_name);
                    //migration_number++;

                    if (!s.empty())
                    {
                        //inputs->addOutput(s);
                        migration_result.push_back(s);
#ifdef Migration_print
                        cout << s << endl;
#endif
                    }
                    migrate_constraint--;
                    if (migrate_constraint == 0)
                        break;
                }

                be_put_in = true;
            }
            else if (server_now->fit(server_now->A, Vm_type.cpuNum, Vm_type.memorySize) &&
                     !server_now->fit(server_now->B, Vm_type.cpuNum, Vm_type.memorySize))
            {
#ifdef Migration_print
                cout << "A" << endl;
#endif
                server_now->A.first = server_now->A.first - Vm_type.cpuNum;

                server_now->A.second = server_now->A.second - Vm_type.memorySize;
                server_now->requestIDs.insert(Vm_name);

                be_put_in = true;

                std::string s = Output_Migration_single(Vm_name, *server_now, DEPLOY_A);

                //migration_number++;

                vmOnserver[Vm_name] = {server_now->id, DEPLOY_A};

                //Vm_be_migrated.insert(Vm_name);

                if (!s.empty())
                {
                    //inputs->addOutput(s);
                    migration_result.push_back(s);
#ifdef Migration_print
                    cout << s << endl;
#endif
                }

                migrate_constraint--;
                if (migrate_constraint == 0)
                    break;
            }
            else if (!server_now->fit(server_now->A, Vm_type.cpuNum, Vm_type.memorySize) &&
                     server_now->fit(server_now->B, Vm_type.cpuNum, Vm_type.memorySize))
            {
#ifdef Migration_print
                cout << "B" << endl;
#endif
                server_now->B.first = server_now->B.first - Vm_type.cpuNum;
                server_now->B.second = server_now->B.second - Vm_type.memorySize;
                server_now->requestIDs.insert(Vm_name);

                be_put_in = true;

                //migration_number++;

                std::string s = Output_Migration_single(Vm_name, *server_now, DEPLOY_B);
                vmOnserver[Vm_name] = {server_now->id, DEPLOY_B};
                //Vm_be_migrated.insert(Vm_name);
                if (!s.empty())
                {
                    //inputs->addOutput(s);
                    migration_result.push_back(s);
#ifdef Migration_print
                    cout << s << endl;
#endif
                }
                migrate_constraint--;
                if (migrate_constraint == 0)
                    break;
            }

            if (be_put_in)
            {
                //原来的放在哪个节点上
                //migration_number++;
                server_i->requestIDs.erase(Vm_name);
                if (node == DEPLOY_A)
                {
                    server_i->A.first = server_i->A.first + Vm_type.cpuNum;
                    server_i->A.second = server_i->A.second + Vm_type.memorySize;
                }
                if (node == DEPLOY_B)
                {
                    server_i->B.first = server_i->B.first + Vm_type.cpuNum;
                    server_i->B.second = server_i->B.second + Vm_type.memorySize;
                }
                Vm_could_choose.erase(Vm_name);
                //break;
            }
            else
            {
                continue;
            }
        }
        else //双节点
        {

            if (server_now->fit(server_now->A, Vm_type.cpuNum / 2, Vm_type.memorySize / 2) &&
                server_now->fit(server_now->B, Vm_type.cpuNum / 2, Vm_type.memorySize / 2))
            {
#ifdef Migration_print
                cout << "AB shuang jie dian" << endl;
#endif
                server_now->A.first = server_now->A.first - Vm_type.cpuNum / 2;
                server_now->A.second = server_now->A.second - Vm_type.memorySize / 2;
                server_now->B.first = server_now->B.first - Vm_type.cpuNum / 2;
                server_now->B.second = server_now->B.second - Vm_type.memorySize / 2;

                server_i->A.first = server_i->A.first + Vm_type.cpuNum / 2;
                server_i->A.second = server_i->A.second + Vm_type.memorySize / 2;
                server_i->B.first = server_i->B.first + Vm_type.cpuNum / 2;
                server_i->B.second = server_i->B.second + Vm_type.memorySize / 2;

                server_now->requestIDs.insert(Vm_name);
                server_i->requestIDs.erase(Vm_name);

                std::string s = Output_Migration_double(Vm_name, *server_now);

                vmOnserver[Vm_name] = {server_now->id, DEPLOY_AB};

                //Vm_be_migrated.insert(Vm_name);
                //migration_number++;

                if (!s.empty())
                {
                    //inputs->addOutput(s);
                    migration_result.push_back(s);
#ifdef Migration_print
                    cout << s << endl;
#endif
                }
                migrate_constraint--;
                Vm_could_choose.erase(Vm_name);
                if (migrate_constraint == 0)
                    break;
                //break;
            }
            else
            {
                continue;
            }
            //双节点结束
        }
    }
}

void MainNode::Vm_To_Vm_could_choose(std::unordered_map<std::string, std::pair<int, int>> &vmOnserver,
                                     std::unordered_set<std::string> &Vm_could_choose)
{
    //把当前的所有vm id更新到Vm_could_choose
    for (auto it : vmOnserver)
    {
        Vm_could_choose.insert(it.first);
    }
}

void MainNode::delete_ser_vm(std::unordered_set<std::string> &Vm_could_choose, Server &server)
{
    //删除这个服务器上的vm id 在可以选择的服务器中
    auto vm_id = &server.requestIDs;
    for (auto it : *vm_id)
    {
        Vm_could_choose.erase(it);
    }
}

void MainNode::Adjustment(std::vector<int> &Servers_score, int migrate_constraint)
{
#ifdef Migration_print
    cout << "begin Adjustment" << endl;
#endif
    std::vector<int> Servers_need_adjustment; //储存相应的id
    int Wm_number = 0;

#ifdef Migration_print
    cout << "chose Servers" << endl;
#endif
    while (Wm_number <= migrate_constraint)
    {
        int maxPos = max_element(Servers_score.begin(), Servers_score.end()) - Servers_score.begin();
        auto it = &currentServers[maxPos];
        Wm_number = Wm_number + it->requestIDs.size();
        Servers_need_adjustment.push_back(it->id); //这里进去的是服务器的id
        Servers_score[maxPos] = 0;
    }
    //printSerAndVm(Servers_need_adjustment);
    //储存所有之前的信息  重分配失败后可以回去
    std::vector<Server> Store = Store_Servers_Info(Servers_need_adjustment);

    //此时我们已经知道 有哪些服务器需要重规划了
    //接下来我们就重规划这些服务器
    std::vector<std::string> Vm_double; //储存双节点虚拟机id
    std::vector<std::string> Vm_single; //储存单节点虚拟机id
    std::vector<std::string> Vm_all;    //储存所有的虚拟机id

#ifdef Migration_print
    cout << "next we refresh all servers" << endl;
#endif
    for (auto si : Servers_need_adjustment)
    {
        auto server = &currentServers[si];
        auto it = server->requestIDs; //当前虚拟机的所有id
        for (auto id : it)
        {
            std::string Vm_name = inputs->requestIDMap[id];
            Vm_all.push_back(id);
            int node_num = inputs->virtualHosts[Vm_name].nodeNum;
            if (node_num == 0)
                Vm_single.push_back(id);
            else
                Vm_double.push_back(id);
        }
        //把这个服务器刷新一下 全部清零
        server->requestIDs.clear();
        server->A.first = server->serverType.cpuNum / 2;
        server->A.second = server->serverType.memorySize / 2;
        server->B.first = server->serverType.cpuNum / 2;
        server->B.second = server->serverType.memorySize / 2;
    }
    //接下来重新分配
#ifdef Migration_print
    cout << "Server need ad num=" << Servers_need_adjustment.size() << endl;
    for (ui i = 0; i < Servers_need_adjustment.size(); i++)
    {
        auto it = &currentServers[Servers_need_adjustment[i]];
        cout << "id=" << it->remappedID << " ";
    }
    cout << endl;
    cout << "Vm_single num=" << Vm_single.size() << endl;
    cout << "Vm_double num=" << Vm_double.size() << endl;
#endif

    bool Jud = Redistribute(Servers_need_adjustment, Vm_single, Vm_double, Vm_all);
#ifdef Migration_print
    cout << "-Jud Redistribute over-" << endl;
#endif
    if (Jud)
    {
        migration_success = true;
        //cout<<"success in adjust "<<endl;
        //printSerAndVm(Servers_need_adjustment);
#ifdef Migration_print
        cout << "-----Adjustment is done ----" << endl;
#endif
    }
    else
    {
        migration_success = false;
        Return_server_Info(Store, Servers_need_adjustment);
        //cout<<" failure in adjust"<<endl;
#ifdef Migration_print
        cout << endl;
        cout << "--error--Adjustment return back ----" << endl;
        cout << endl;
#endif
    }
}

std::vector<Server> MainNode::Store_Servers_Info(std::vector<int> &Servers_need_adjustment)
{
    std::vector<Server> Store;
    for (auto id : Servers_need_adjustment)
    {
        Store.push_back(currentServers[id]);
    }

    return Store;
}

void MainNode::Return_server_Info(std::vector<Server> &Store, std::vector<int> &Servers_need_adjustment)
{
#ifdef Migration_print
    cout << " Return_server_Info " << endl;
#endif
    //重分配失败 返回之前的所有服务器信息 相当于没刷新
    int i = 0;
    for (auto id : Servers_need_adjustment)
    {
        currentServers[id].A = Store[i].A;
        currentServers[id].B = Store[i].B;
        currentServers[id].requestIDs = Store[i].requestIDs;
    }
}

bool MainNode::Redistribute(std::vector<int> &Servers_need_adjustment,
                            std::vector<std::string> &Vm_single, std::vector<std::string> &Vm_double, std::vector<std::string> &Vm_all)
{
#ifdef Migration_print
    cout << "Redistributing" << endl;
#endif
    //对所有的Vm 打分 从大到小进行扔
    int ser_i = 0;
    int Vm_left = -1;
    while (Vm_all.size() != 0)
    {
        if (Vm_left != Vm_all.size() && ser_i != Servers_need_adjustment.size())
        {
            Vm_left = Vm_all.size();
        }
        else
        {
#ifdef Migration_print
            cout << "return false" << endl;
#endif
            return false;
        }
#ifdef Migration_print
        cout << "distributing all" << endl;
        cout << "Vm all number left = " << Vm_all.size() << endl;
#endif
        int Vm_sign = 0;
        auto server = &currentServers[Servers_need_adjustment[ser_i]];

        while (Vm_sign != Vm_left)
        {
            std::vector<std::pair<int, std::pair<int, std::string>>> Vm_all_score;
            for (auto Vm : Vm_all) //每次都会计算当前分配的分数 然后选择最高分的那个
            {
                auto Vm_type = inputs->virtualHosts[inputs->requestIDMap[Vm]];
                auto r = Compute_Vm_score_all(*server, Vm_type, Vm);
                Vm_all_score.push_back(r);
            }

            //cout<<"--all vm value--"<<endl;

            if (JudgeAllzero(Vm_all_score))
            {
                //cout<<"no room for it"<<endl;
                break;
            }

            sort(Vm_all_score.begin(), Vm_all_score.end(), Judge);
#ifdef Migration_print
            /*
            //打印这个评分
            cout<<"server state id="<<server->remappedID<<endl;
            cout<<"A ="<<server->A.first<<" "<<server->A.second;
            cout<<"B ="<<server->B.first<<" "<<server->B.second<<endl;
            cout<<"All scores"<<endl;
            for (auto it:Vm_all_score) {
                cout<<"id="<<it.second.second<<" score="<<it.first<<" AB="<<it.second.first<<endl;
            }
             */
#endif
            auto VmS = Vm_all_score.back();

            if (VmS.first == 0 && VmS.second.first == UNFIT) //如果判断这个装不下
            {
                Vm_sign++;
            }
            else if (VmS.second.first == DEPLOY_A || VmS.second.first == DEPLOY_B)
            {
                Vm_sign++;
                auto Vm_now = inputs->virtualHosts[inputs->requestIDMap[VmS.second.second]];
                int itq;
                for (ui i = 0; i < Vm_single.size(); i++)
                {
                    if (Vm_all[i] == VmS.second.second)
                        itq = i;
                }
                if (Vm_all.size() != 0)
                {
                    Vm_all.erase(Vm_all.begin() + itq);
                } //删除这个元素

                if (VmS.second.first == DEPLOY_A)
                {
                    server->requestIDs.insert(VmS.second.second);
                    server->A.first = server->A.first - Vm_now.cpuNum;
                    server->A.second = server->A.second - Vm_now.memorySize;
                    //cout<<"server map id"<<server.remappedID<<endl;
                    std::string s = Output_Migration_single(VmS.second.second, *server, VmS.second.first);
                    if (s.size() != 0)
                    {
                        //inputs->addOutput(s);
                        migration_result.push_back(s);
#ifdef Migration_print
                        cout << s << endl;
#endif
                        vmOnserver[VmS.second.second] = {server->remappedID, DEPLOY_A};
                    }
                    else
                    {
#ifdef Migration_print
                        cout << "Vm " << VmS.second.second << " bu bian" << endl;
#endif
                    }
                }

                if (VmS.second.first == DEPLOY_B)
                {
                    server->requestIDs.insert(VmS.second.second);
                    server->B.first = server->B.first - Vm_now.cpuNum;
                    server->B.second = server->B.second - Vm_now.memorySize;

                    //cout<<"server map id"<<server.remappedID<<endl;

                    std::string s = Output_Migration_single(VmS.second.second, *server, VmS.second.first);

                    if (s.size() != 0)
                    {
                        //inputs->addOutput(s);
                        migration_result.push_back(s);
#ifdef Migration_print
                        cout << s << endl;
#endif
                        vmOnserver[VmS.second.second] = {server->remappedID, DEPLOY_B};
                    }
                    else
                    {
#ifdef Migration_print
                        cout << "Vm " << VmS.second.second << " bu bian" << endl;
#endif
                    }
                }
            }
            else //双节点
            {
                Vm_sign++;
                auto Vm_now = inputs->virtualHosts[inputs->requestIDMap[VmS.second.second]];
                int itq;
                for (ui i = 0; i < Vm_single.size(); i++)
                {
                    if (Vm_all[i] == VmS.second.second)
                        itq = i;
                }
                if (Vm_all.size() != 0)
                {
                    Vm_all.erase(Vm_all.begin() + itq);
                } //删除这个元素

                //把对应这个id的装到当前的服务器里面去
                server->requestIDs.insert(VmS.second.second);
                server->A.first = server->A.first - Vm_now.cpuNum / 2;
                server->B.first = server->B.first - Vm_now.cpuNum / 2;
                server->A.second = server->A.second - Vm_now.memorySize / 2;
                server->B.second = server->B.second - Vm_now.memorySize / 2;
                std::string s = Output_Migration_double(VmS.second.second, currentServers[Servers_need_adjustment[ser_i]]);
                if (s.size() != 0)
                { //如果位置被改变了
#ifdef Migration_print
                    cout << s << endl;
#endif
                    migration_result.push_back(s);
                    //inputs->addOutput(s);
                    vmOnserver[VmS.second.second] = {server->remappedID, DEPLOY_AB};
                    //塞进去之后需要输出migration 双节点 (Vm_id, Ser_id)
                }
            }
        }
        ser_i++;
    }
    /*

    //是先扔双节点 再扔单节点
    //但是会出现bug问题
    ser_i=0;
    while (Vm_double.size() != 0)
    {//先把双节点都装完了
        cout<<"distributing double "<<endl;
        cout<<"Vm double number left = "<<Vm_double.size()<<endl;

        int Vm_sign = 0;
        auto server = &currentServers[Servers_need_adjustment[ser_i]];
        int Vm_left = Vm_double.size();

        while (Vm_sign != Vm_left)
        {
            std::vector<int> Vm_double_score;
            for (auto Vm:Vm_double) //每次都会计算当前分配的分数 然后选择最高分的那个
            {
                auto Vm_type = inputs->virtualHosts[inputs->requestIDMap[Vm]];
                double r
                =Compute_Vm_score_double(currentServers[Servers_need_adjustment[ser_i]], Vm_type);
                Vm_double_score.push_back(r);
            }
            cout<<"--all vm value--"<<endl;

            if(JudgeAllzeroOne(Vm_double_score))
                break;

            for(ui i=0;i<Vm_double_score.size();i++)
            {
                cout<<"i="<<i<<" "<<Vm_double_score[i]<<" ";
            }
            cout<<endl;
            auto max_val = max_element(Vm_double_score.begin(), Vm_double_score.end());
            cout<<"max val = "<<*max_val<<endl;

            if (*max_val == 0)
            {   //如果当前最大的值是零 那么意味着没有可以分配的虚拟机 即可执行下一个服务器
                Vm_sign++;
            }
            else
            {

                int max_pos = max_val - Vm_double_score.begin(); //算出最大值的顺序
                auto id = Vm_double[max_pos];
                cout<<"----max_pos="<<max_pos<<endl;
                Vm_double.erase(Vm_double.begin()+max_pos);//在Vm中删除掉这个最大值
                auto Vm_now = inputs->virtualHosts[inputs->requestIDMap[id]];
                //把对应这个id的装到当前的服务器里面去
                server->requestIDs.insert(id);
                server->A.first = server->A.first-Vm_now.cpuNum/2;
                server->B.first = server->B.first-Vm_now.cpuNum/2;
                server->A.second = server->A.second-Vm_now.memorySize/2;
                server->B.second = server->B.second-Vm_now.memorySize/2;
                std::string s =  Output_Migration_double(id, currentServers[Servers_need_adjustment[ser_i]]);
                if(s.size()!=0)
                {   //如果位置被改变了
                    cout<<s<<endl;
                    //inputs->addOutput(s);
                    vmOnserver[id] = {server->remappedID, DEPLOY_AB};
                    //塞进去之后需要输出migration 双节点 (Vm_id, Ser_id)
                }
            }
        }
        ser_i++;

    }

    //装单节点的
    /*
    cout<<"qwe---"<<endl;
    for(ui i=0;i<Servers_need_adjustment.size();i++)
    {
        cout<<"id="<<Servers_need_adjustment[i].remappedID<<" ";
    }
    cout<<endl;
     */

    /*
    ser_i=0;
    while (Vm_single.size() != 0)
    {
        if (ser_i == Servers_need_adjustment.size() && Vm_single.size() !=0)
        {
            cout<<"error in single q"<<endl;
        }

        cout << "distribute single  ing" << endl;
        cout << "Vm single num left = " << Vm_single.size() << endl;

        auto server = &currentServers[Servers_need_adjustment[ser_i]];
        int Vm_left = Vm_single.size();

        int VM_sign = 0; //0  如果等于 Vm_left 那么都尝试过了

        while (VM_sign != Vm_left)//是否每个Vm都尝试了 如果尝试了换下一个服务器
        {
            std::vector<std::pair<int, std::pair<int, std::string>>> Vm_single_score;

            for (auto Vm:Vm_single)
            {
                auto Vm_type = inputs->virtualHosts[inputs->requestIDMap[Vm]];
                auto r = Compute_Vm_score_single(*server, Vm_type, Vm);
                Vm_single_score.push_back(r);
            }
            sort(Vm_single_score.begin(), Vm_single_score.end(), Judge);

            if(JudgeAllzero(Vm_single_score))
                break;
            //打印这个评分
            cout<<"server state id="<<server->remappedID<<endl;
            cout<<"A ="<<server->A.first<<" "<<server->A.second;
            cout<<"B ="<<server->B.first<<" "<<server->B.second<<endl;
            cout<<"All scores"<<endl;
            for (auto it:Vm_single_score) {
                cout<<"id="<<it.second.second<<" score="<<it.first<<" AB="<<it.second.first<<endl;
            }


            auto VmS = Vm_single_score.back();

            if (VmS.first == 0 && VmS.second.first == UNFIT) //如果判断这个装不下
            {
                VM_sign++;
            }
            else
            {
                VM_sign++;
                auto Vm_now = inputs->virtualHosts[inputs->requestIDMap[VmS.second.second]];
                int itq;
                for (ui i = 0; i < Vm_single.size(); i++)
                {
                    if (Vm_single[i] == VmS.second.second)
                        itq = i;
                }
                if (Vm_single.size() != 0)
                { Vm_single.erase(Vm_single.begin() + itq); }//删除这个元素

                if (VmS.second.first == DEPLOY_A)
                {
                    server->requestIDs.insert(VmS.second.second);
                    server->A.first = server->A.first - Vm_now.cpuNum;
                    server->A.second = server->A.second - Vm_now.memorySize;

                    //cout<<"server map id"<<server.remappedID<<endl;

                    std::string s
                            = Output_Migration_single(VmS.second.second, *server, VmS.second.first);
                    if (s.size() != 0)
                    {
                        //inputs->addOutput(s);
                        cout << s << endl;
                        vmOnserver[VmS.second.second] = {server->remappedID, DEPLOY_A};
                    }
                    else
                        {
                        cout << "Vm "<<VmS.second.second <<" bu bian" << endl;
                    }
                }

                if (VmS.second.first == DEPLOY_B)
                {
                    server->requestIDs.insert(VmS.second.second);
                    server->B.first = server->B.first - Vm_now.cpuNum;
                    server->B.second = server->B.second - Vm_now.memorySize;

                    //cout<<"server map id"<<server.remappedID<<endl;

                    std::string s
                            = Output_Migration_single(VmS.second.second, *server, VmS.second.first);

                    if (s.size() != 0)
                    {
                        //inputs->addOutput(s);
                        cout << s << endl;
                        vmOnserver[VmS.second.second] = {server->remappedID, DEPLOY_B};
                    }
                    else
                    {
                        cout << "Vm "<<VmS.second.second<<" bu bian" << endl;
                    }
                }
            }
        }
        ser_i++;
    }
    */
#ifdef Migration_print
    cout << "Redistribute is done!!!" << endl;
#endif
    return true;
}

bool MainNode::JudgeAllzero(std::vector<std::pair<int, std::pair<int, std::string>>> &Vm_single_score)
{
    for (auto it : Vm_single_score)
    {
        if (it.first != 0)
            return false;
    }
    return true;
}

bool MainNode::JudgeAllzeroOne(std::vector<int> &Vm_double_score)
{
    for (auto it : Vm_double_score)
    {
        if (it != 0)
            return false;
    }
    return true;
}

std::string MainNode::Output_Migration_double(std::string &Vm_id, Server &server)
{
    auto previous_info = vmOnserver[Vm_id];
    int Ser_num = currentServers[previous_info.first].remappedID; //通过服务器的id 读取map id
    std::string s;
    if (Ser_num != server.remappedID)
    {
        s = "(" + Vm_id + "," + std::to_string(server.remappedID) + ")";
        return s;
    }
    else
    {
        return s;
    }
}

std::string MainNode::Output_Migration_single(std::string &Vm_id, Server &server, int node_now)
{
    //cout<<"server id jin ru"<<server.remappedID<<endl;
    auto previous_info = vmOnserver[Vm_id];

    int node_p = previous_info.second;

    int Ser_num = currentServers[previous_info.first].remappedID;
    std::string s1;
    if (node_now == DEPLOY_A)
        s1 = "A";
    if (node_now == DEPLOY_B)
        s1 = "B";
    std::string s;
    if (Ser_num != server.remappedID)
    {

        s = "(" + Vm_id + "," + std::to_string(server.remappedID) + "," + s1 + ")";
        return s;
    }
    if (Ser_num == server.remappedID && node_p != node_now)
    {

        s = "(" + Vm_id + "," + std::to_string(server.remappedID) + "," + s1 + ")";
        return s;
    }
    return s;
}

double MainNode::Compute_Vm_score_double(Server &server, VirtualHost &Vm)
{
    //double cpu_A_use=0, cpu_B_use=0;
    //double memo_A_use=0, memo_B_use=0;
    int res;
    if (server.fit(server.A, Vm.cpuNum / 2, Vm.memorySize / 2) && server.fit(server.B, Vm.cpuNum / 2, Vm.memorySize / 2))
    {
        res = Vm.cpuNum + Vm.memorySize;
        return res;
    }
    else
    {
        return 0; //如果放不下 那么给0
    }
}

bool Judge(std::pair<int, std::pair<int, std::string>> a, std::pair<int, std::pair<int, std::string>> b)
{
    return a.first < b.first;
}

void MainNode::printSerAndVm(std::vector<int> &Servers_need_adjustment)
{
    cout << "----All server infor----" << endl;
    for (auto it : Servers_need_adjustment)
    {
        auto server = currentServers[it];
        /*
        double ausec = (server.serverType.cpuNum/2-server.A.first)/(server.serverType.cpuNum/2);
        double busec = (server.serverType.cpuNum/2-server.B.first)/(server.serverType.cpuNum/2);
        double ausem = (server.serverType.memorySize/2-server.A.second)/(server.serverType.memorySize/2);
        double busem = (server.serverType.memorySize/2-server.B.second)/(server.serverType.memorySize/2);
        */
        cout << "server id=" << server.remappedID << endl;
        cout << "cpu=" << server.serverType.cpuNum << endl;
        cout << "memo=" << server.serverType.memorySize << endl;
        cout << "cpu AB left " << server.A.first << " " << server.B.first << endl;
        cout << "memo AB left " << server.A.second << " " << server.B.second << endl;
        //cout<<"cpu A B use="<<ausec<<" "<<busec<<endl;
        //cout<<"memo A B use="<<ausem<<" "<<busem<<endl;

        for (auto it : server.requestIDs)
        {
            VirtualHost &vm = inputs->virtualHosts[inputs->requestIDMap[it]];
            cout << it << " ";
            cout << "Vm_cpu=" << vm.cpuNum << " Vm_memo=" << vm.memorySize;
            cout << " node=" << vm.nodeNum << endl;
        }
        cout << endl;
        cout << "--------" << endl;
    }
}

std::pair<int, std::pair<int, std::string>>
MainNode::Compute_Vm_score_all(Server &server, VirtualHost &Vm, std::string &id)
{
    if (Vm.nodeNum == 0)
    {
        double cpu_A_use = 0, cpu_B_use = 0;
        double memo_A_use = 0, memo_B_use = 0;
        double resultA, resultB;

        int score = Vm.memorySize + Vm.cpuNum;
        if (!server.fit(server.A, Vm.cpuNum, Vm.memorySize) && !server.fit(server.B, Vm.cpuNum, Vm.memorySize))
            return {0, {UNFIT, id}};

        if (server.fit(server.A, Vm.cpuNum, Vm.memorySize) && server.fit(server.B, Vm.cpuNum, Vm.memorySize))
        {
            //装下的话 A B 哪个更好
            cpu_A_use = server.A.first - Vm.cpuNum;
            memo_A_use = server.A.second - Vm.memorySize;
            cpu_B_use = server.B.first - Vm.cpuNum;
            memo_B_use = server.B.second - Vm.memorySize;
            ;
            resultA = cpu_A_use + memo_A_use;
            resultB = cpu_B_use + memo_B_use;
            //分数直接就是cpu+memo 这样大的分就高
            if (resultA <= resultB)
                return {score, {DEPLOY_A, id}};
            else
                return {score, {DEPLOY_B, id}};
        }

        if (server.fit(server.A, Vm.cpuNum, Vm.memorySize) && !server.fit(server.B, Vm.cpuNum, Vm.memorySize))
        {
            return {score, {DEPLOY_A, id}};
        }

        if (!server.fit(server.A, Vm.cpuNum, Vm.memorySize) && server.fit(server.B, Vm.cpuNum, Vm.memorySize))
        {
            return {score, {DEPLOY_B, id}};
        }

        return {0, {UNFIT, id}};
    }
    else
    {
        double score;
        if (server.fit(server.A, Vm.cpuNum / 2, Vm.memorySize / 2) && server.fit(server.B, Vm.cpuNum / 2, Vm.memorySize / 2))
        {
            score = Vm.cpuNum + Vm.memorySize;
            return {score, {DEPLOY_AB, id}};
        }
        else
        {
            return {0, {UNFIT, id}};
            ; //如果放不下 那么给0
        }
    }
}

std::pair<int, std::pair<int, std::string>>
MainNode::Compute_Vm_score_single(Server &server, VirtualHost &Vm, std::string &id)
{ //单节点
    double cpu_A_use = 0, cpu_B_use = 0;
    double memo_A_use = 0, memo_B_use = 0;
    double resultA, resultB;

    int score = Vm.memorySize + Vm.cpuNum;

    if (!server.fit(server.A, Vm.cpuNum, Vm.memorySize) && !server.fit(server.B, Vm.cpuNum, Vm.memorySize))
        return {0, {UNFIT, id}};

    if (server.fit(server.A, Vm.cpuNum, Vm.memorySize) && server.fit(server.B, Vm.cpuNum, Vm.memorySize))
    {
        //装下的话 A B 哪个更好
        cpu_A_use = server.A.first - Vm.cpuNum;
        memo_A_use = server.A.second - Vm.memorySize;
        cpu_B_use = server.B.first - Vm.cpuNum;
        memo_B_use = server.B.second - Vm.memorySize;
        ;
        resultA = cpu_A_use + memo_A_use;
        resultB = cpu_B_use + memo_B_use;

        //分数直接就是cpu+memo 这样大的分就高

        if (resultA <= resultB)
            return {score, {DEPLOY_A, id}};
        else
            return {score, {DEPLOY_B, id}};
    }

    if (server.fit(server.A, Vm.cpuNum, Vm.memorySize) && !server.fit(server.B, Vm.cpuNum, Vm.memorySize))
    {
        return {score, {DEPLOY_A, id}};
    }

    if (!server.fit(server.A, Vm.cpuNum, Vm.memorySize) && server.fit(server.B, Vm.cpuNum, Vm.memorySize))
    {
        return {score, {DEPLOY_B, id}};
    }

    return {0, {UNFIT, id}};
}

void MainNode::Compute_balance_ratio(std::vector<double> &Servers_score)
{
    //int Servers_number = currentServers.size();

    /*
    std::cout<<"---qwe---"<<std::endl;
    for(int i=0;i<currentServers.size();i++)
    {
        std::cout<<"i="<<i<<" id="<<currentServers[i].id<<std::endl;
    }
    */
    double balance_score, usage_score, sum_score;
    double left_sum;
    double cpu_k, memo_k;
    double cpu_use, memo_use;
    int k1 = 20, k2 = 150, k3 = 1, k4 = 0;

    //以下分别对平衡和占有率进行打分
    //越不平衡 占有率越低 打分越高 就越需要调整
    for (ui i = 0; i < currentServers.size(); i++)
    {

        Server *S_now = &currentServers[i];
        int sums_S = (S_now->serverType.cpuNum + S_now->serverType.memorySize);

        cpu_k = std::fabs(S_now->A.first - S_now->B.first) / S_now->serverType.cpuNum;
        memo_k = std::fabs(S_now->A.second - S_now->B.second) / S_now->serverType.memorySize;
        balance_score = (cpu_k + memo_k) / 2;

        cpu_use = double(S_now->A.first + S_now->B.first) / S_now->serverType.cpuNum;
        memo_use = double(S_now->A.second + S_now->B.second) / S_now->serverType.memorySize;
        usage_score = ((cpu_use) + (memo_use)) / 2;

        left_sum = (S_now->A.first + S_now->A.second + S_now->B.first + S_now->B.second);

        auto vm_id = &S_now->requestIDs;
        double r_sum;
        int vm_sum = vm_id->size();
        r_sum = vm_sum;

        sum_score = k1 * balance_score + k2 * usage_score + k3 * left_sum + k4 * r_sum;

        Servers_score.push_back(sum_score);

        //Servers_balance_ratio.insert(std::pair<int,int>(sum_score, currentServers[i].id));
        //insert(pair<int,int>(key,value));
    }
#ifdef VER1
    std::cout << "--begin--Compute_balance_ratio--" << std::endl;
    int i = 1;

    std::cout << "size=" << Servers_score.size() << std::endl;
    for (auto iter = Servers_score.begin(); iter != Servers_score.end(); iter++)
    {
        std::cout << "i=" << i << " score=" << *iter << std::endl;
        i++;
    }
#endif
}

void MainNode::dailyRoutine(int day)
{
    currentDay = day;

    distribute_now();
}

MainNode::MainNode(InputS *inputs, int totalDays) : inputs(inputs), totalDays(totalDays)
{
    serverCost = powerCost = 0;
}

void MainNode::displayCost()
{
    std::cout << "ServerCost: " << serverCost << "\nPowerCost: " << powerCost << "\nTotal: " << serverCost + powerCost << std::endl;
}

void MainNode::printServerInfo(Server &server)
{
    cout << "ser id=" << server.remappedID << endl;
    cout << "A CPU MEMO left " << server.A.first << " " << server.A.second << endl;
    cout << "B CPU MEMO left " << server.B.first << " " << server.B.second << endl;
}

void MainNode::printServerVmId(Server &server)
{
    cout << "server all Vm " << endl;
    for (auto it : server.requestIDs)
    {
        VirtualHost &i = inputs->virtualHosts[inputs->requestIDMap[it]];
        //auto i = &inputs->virtualHosts[it];
        cout << "Vm id=" << it << " ";
        cout << " cpu=" << i.cpuNum << " ";
        cout << " memo=" << i.memorySize << " ";
    }
    cout << endl;
}

void MainNode::printVmInfo(std::string Vm_id)
{
    cout << "Vm id=" << Vm_id << " ";
    VirtualHost &i = inputs->virtualHosts[inputs->requestIDMap[Vm_id]];
    auto it = vmOnserver[Vm_id];
    cout << "cpu=" << i.cpuNum << " ";
    cout << "memo=" << i.memorySize << " ";
    cout << " node=" << i.nodeNum << " ";
    cout << endl;
}
