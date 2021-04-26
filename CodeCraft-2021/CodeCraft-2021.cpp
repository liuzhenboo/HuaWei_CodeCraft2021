#include "InputS.h"
#include "MainNode.h"
#include "OfflineJudger.h"

int main(){

#ifdef compute_time
    clock_t start,end;
    start = clock();
#endif

    InputS inputs;

    inputs.read_all_Server_Types();

    inputs.read_all_VirtualHosts();

    inputs.read_all_Int();

    //cout<<"day = "<<inputs.day_number<<endl;
    //cout<<"cons = "<<inputs.day_number_constraint<<endl;

    int T = inputs.day_number;

    MainNode MainNode(&inputs,T);

#ifdef Try
    // MainNode.displayCost();
    std::freopen("output.txt","w",stdout);
#endif

    for(int ti = 0; ti < T; ti++)
    {
#ifdef PRINTMORE
        if(ti % 100 == 0)
            printf("process day %d\n",ti);
#endif
        inputs.read_all_Requests();

        MainNode.dailyRoutine(ti);


        for(auto &s:inputs.outputs)
        {
            std::cout << s << "\n";
        }
        inputs.outputs.clear();
        //fflush(stdout);

        //fflush(stdout);
    }

    fflush(stdout);


#ifdef compute_time
    end = clock();
    cout<<"time1="<<MainNode.time1<<endl;
    cout<<"time2="<<MainNode.time2<<endl;
    cout<<"time_sum = "<<double(end-start)/CLOCKS_PER_SEC<<"s"<<endl;
#endif



    // [3] write standard output
    /*
    for(auto &s:inputs.outputs){
        std::cout<<s<<"\n";
    }
    */
    // [4] flush stdout


#ifdef Try
    freopen("CON", "w", stdout);
    std::cout<<"========= Here begins Judger's output =========\n";
    runJudger(tryPathFile,"output.txt");
#endif
	return 0;
}
