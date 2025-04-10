#include <bits/stdc++.h>
//#include "check.h"
using namespace std;
vector<int> Waiting_time;
vector<int> TurnAround_time;
vector<int> Finish_time;

const static bool cmp(const tuple<string, int, int> &t1, const tuple<string, int, int> &t2) {
    return get<1>(t1) < get<1>(t2);
}
const static bool comp(const tuple<int,double, int> &t1, const tuple<int,double,int> &t2) {
    return get<1>(t1) > get<1>(t2);
}
const static bool comp1(const tuple<int,int> &t1, const tuple<int,int> &t2) {
    return get<1>(t1) > get<1>(t2);
}
void sortByArrivalTime(vector<tuple<string, int, int>> &input_tuple) {
    sort(input_tuple.begin(), input_tuple.end(), cmp);
}
void sortByResponseRatio(vector<tuple<int,double,int>> &input_tuple){
    sort(input_tuple.begin(),input_tuple.end(),comp);
}
void sortByPriority(vector<tuple<int,int>> &input_tuple){
    sort(input_tuple.begin(),input_tuple.end(),comp1);
}
int getArrivalTime(const tuple<string, int, int> &t1) {
    return get<1>(t1);
}

int getBurstTime(const tuple<string, int, int> &t1) {
    return get<2>(t1);
}
double Get_response_ratio(int Waiting_T, int Burst_T){
    return (double)(Waiting_T+Burst_T)/(double)Burst_T;
}
void getFCFS(vector<tuple<string, int, int>> &input_tuple, int process_count, int time_span) {
    Waiting_time.resize(process_count);
    TurnAround_time.resize(process_count);
    Finish_time.resize(process_count);
    
    sortByArrivalTime(input_tuple);

    int time = 0;
    for (int i = 0; i < process_count; i++) {
        int AT = getArrivalTime(input_tuple[i]);
        int BT = getBurstTime(input_tuple[i]);
        if (time < AT) {
            time = AT;  // If the CPU is idle, skip to the arrival time of the next process
        }
        time += BT;
        if(time>time_span){
            time=time_span;
        
        Finish_time[i] = time;
        TurnAround_time[i] = Finish_time[i] - AT;
        Waiting_time[i] = TurnAround_time[i] - BT;
        break;
        }
        Finish_time[i] = time;
        TurnAround_time[i] = Finish_time[i] - AT;
        Waiting_time[i] = TurnAround_time[i] - BT;
    }
}

void RoundRobin(vector<tuple<string, int, int>> &input_tuple, int process_count, int quantum, int time_span) {
    Waiting_time.resize(process_count);
    TurnAround_time.resize(process_count);
    Finish_time.resize(process_count);

    sortByArrivalTime(input_tuple);
    queue<pair<int, int>> q;
    int time = get<1>(input_tuple[0]);
    int CQ = quantum; // Current quantum
    q.push({0, get<2>(input_tuple[0])});

    for (int i = time; i < time_span; i++) {
        if (!q.empty()) {
            int Pid = q.front().first;
            q.front().second -= 1;
            // cout<<q.front().second<<" "<<Pid<<endl;
            // Add arriving processes to the queue
            for (int j = 1; j < process_count; j++) {
                if (getArrivalTime(input_tuple[j]) == i+1) {
                    q.push({j, get<2>(input_tuple[j])});
                }
            }

            int remaining_burst = q.front().second;
            int idx = q.front().first;
            int AT = getArrivalTime(input_tuple[idx]);
            int BT = getBurstTime(input_tuple[idx]);

            CQ--;

            if (remaining_burst == 0 || i==time_span-1) {
                Finish_time[idx] = i + 1; // Process completes at this time
                TurnAround_time[idx] = Finish_time[idx] - AT;
                Waiting_time[idx] = TurnAround_time[idx] - BT;
                q.pop();
                CQ = quantum;
            } else if (CQ == 0 ) {
                q.pop();
                // cout<<remaining_burst<<"$"<<endl;
                q.push({idx, remaining_burst});
                CQ = quantum;
            }
           
            continue;
            
        }

        // Add arriving processes to the queue (missed outside if-else)
        for (int j = 1; j < process_count; j++) {
            if (getArrivalTime(input_tuple[j]) == i+1) {
                q.push({j, get<2>(input_tuple[j])});
            }
        }
    }
}
void SJF(vector<tuple<string, int, int>> &input_tuple, int process_count, int time_span){
    Waiting_time.resize(process_count);
    TurnAround_time.resize(process_count);
    Finish_time.resize(process_count);
    sortByArrivalTime(input_tuple);
    priority_queue<pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>>> pq;
    for(int time=0;time<time_span;time++){
        for(int j=0;j<process_count;j++){
            if(getArrivalTime(input_tuple[j])==time){
                 pq.push({get<2>(input_tuple[j]),j});
            }
        }
        if(!pq.empty()){
            int remaining_time=pq.top().first;
            int Pid=pq.top().second;
            pq.pop();
            int AT=getArrivalTime(input_tuple[Pid]);
            Finish_time[Pid]=time+remaining_time;
            TurnAround_time[Pid]=Finish_time[Pid]-AT;
            Waiting_time[Pid]=time-AT;
           for(int j=0;j<process_count;j++){
            int AT=getArrivalTime(input_tuple[j]);
            if(AT>time && AT<time+remaining_time){
                 pq.push({get<2>(input_tuple[j]),j});
            }
        }
        time+=remaining_time-1;
        }
    }
}
void SRTF(vector<tuple<string, int, int>> &input_tuple,int process_count, int time_span){
     Waiting_time.resize(process_count);
    TurnAround_time.resize(process_count);
    Finish_time.resize(process_count);
    sortByArrivalTime(input_tuple);
    priority_queue<pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>>> q;
    int time = get<1>(input_tuple[0]);
    // int CQ = quantum; // Current quantum
    q.push({get<2>(input_tuple[0]),0});

    for (int i = time; i < time_span; i++) {
        if (!q.empty()) {
            int Pid = q.top().second;
            int remaining_burst = q.top().first;
            // cout<<q.front().second<<" "<<Pid<<endl;
            // Add arriving processes to the queue
            for (int j = 1; j < process_count; j++) {
                if (getArrivalTime(input_tuple[j]) == i) {
                    q.push({get<2>(input_tuple[j]),j});
                }
            }

            
            int idx = q.top().second;
            remaining_burst=q.top().first-1;
            int AT = getArrivalTime(input_tuple[idx]);
            int BT = getBurstTime(input_tuple[idx]);

            // CQ--;

            if (remaining_burst == 0 || i==time_span-1) {
                Finish_time[idx] = i + 1; // Process completes at this time
                TurnAround_time[idx] = Finish_time[idx] - AT;
                Waiting_time[idx] = TurnAround_time[idx] - BT;
                q.pop();
                // CQ = quantum;
            } 
            // else if (CQ == 0 ) {
            //     q.pop();
            //     // cout<<remaining_burst<<"$"<<endl;
            //     q.push({idx, remaining_burst});
            //     CQ = quantum;
            // }
            else{
                q.pop();
                q.push({remaining_burst,idx});
            }
            continue;
        }

        // Add arriving processes to the queue (missed outside if-else)
        for (int j = 1; j < process_count; j++) {
            if (getArrivalTime(input_tuple[j]) == i+1) {
                q.push({j, get<2>(input_tuple[j])});
            }
        }
    }

}
void highestResponseRatioNext(vector<tuple<string, int, int>> &input_tuple, int process_count, int time_span) {
    Waiting_time.resize(process_count);
    TurnAround_time.resize(process_count);
    Finish_time.resize(process_count);
    sortByArrivalTime(input_tuple);
    vector<tuple<int, double, int>> present_processes;
    int j = 0;

    for (int time = 0; time < time_span; time++) {
        // Add all processes that have arrived by the current time to the present_processes vector
        while (j < process_count && getArrivalTime(input_tuple[j]) <= time) {
            present_processes.push_back(make_tuple(j, 1.0, getBurstTime(input_tuple[j])));
            j++;
        }

        // Update response ratios for all processes in the present_processes vector
        for (auto &it : present_processes) {
            int Pid = get<0>(it);
            int BT = getBurstTime(input_tuple[Pid]);
            int AT = getArrivalTime(input_tuple[Pid]);
            int WT = time - AT;
            get<1>(it) = Get_response_ratio(WT, BT);
        }

        // Sort processes by response ratio
        sortByResponseRatio(present_processes);

        // Process the highest response ratio process
        if (!present_processes.empty()) {
            int Pid = get<0>(present_processes[0]);
            int BT = get<2>(present_processes[0]);  // Get the correct burst time
            int AT = getArrivalTime(input_tuple[Pid]);
            Waiting_time[Pid]=time-AT;
            time += BT;
            if (time > time_span) {
                time = time_span;
            }
            //  = TurnAround_time[Pid] - BT;
            Finish_time[Pid] = time;
            TurnAround_time[Pid] = Finish_time[Pid] - AT;
            

            // Remove the completed process
            present_processes.erase(present_processes.begin());

            // Decrement time to correctly handle the outer loop's increment
            time--;
        }
    }
}
void Ageing(vector<tuple<string, int, int>> &input_tuple, int process_count, int time_span){
    Waiting_time.resize(process_count);
    TurnAround_time.resize(process_count);
    Finish_time.resize(process_count);
    sortByArrivalTime(input_tuple);
    priority_queue<pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>>> pq;//priority queue,process id
    unordered_map<int,int>remainingServiceTime; 
    int j=0;
    if(getArrivalTime(input_tuple[0])==0){
        pq.push({0,0});
        remainingServiceTime[0]=getBurstTime(input_tuple[0]);
        j++;
    }
    for(int i=0;i<time_span;i++){
        if(!pq.empty()){
            int Pid=pq.top().second;
            int Priority_level=pq.top().first;
            pq.pop();
             while(j<process_count && getArrivalTime(input_tuple[j])==i+1){
                pq.push({0,j});
                remainingServiceTime[j]=getBurstTime(input_tuple[j]);
                j++;
         }
         
         remainingServiceTime[Pid]--;
         if(remainingServiceTime[Pid]==0){
            int AT=getArrivalTime(input_tuple[Pid]);
         int BT=getBurstTime(input_tuple[Pid]);
             Finish_time[Pid]=i+1;
             TurnAround_time[Pid]=Finish_time[Pid]-AT;
             Waiting_time[Pid]=TurnAround_time[Pid]-BT;
         }
         else{
            if(pq.size()>=1)
            pq.push({Priority_level+1,Pid});
            else
                pq.push({Priority_level,Pid});
         }
        }
        while(j<process_count && getArrivalTime(input_tuple[j])==i+1){
                pq.push({0,j});
                remainingServiceTime[j]=getBurstTime(input_tuple[j]);
                j++;
         }
    }
}
void Ageing2(vector<tuple<string,int,int>> &input_tuple,int process_count,int time_span){
    Waiting_time.resize(process_count);
    TurnAround_time.resize(process_count);
    Finish_time.resize(process_count);
    sortByArrivalTime(input_tuple);
    priority_queue<pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>>> pq;//priority queue,process id
    unordered_map<int,int> remainingServiceTime; 
    int j=0;
    if(getArrivalTime(input_tuple[j])==0){
        pq.push({0,0});
        remainingServiceTime[0]=getBurstTime(input_tuple[j]);
        j++;
    }
    for(int i=0;i<time_span;i++){
        if(!pq.empty()){
             int Pid=pq.top().second;
            int Priority_level=pq.top().first;
            pq.pop();
             while(j<process_count && getArrivalTime(input_tuple[j])==i+1){
                pq.push({0,j});
                remainingServiceTime[j]=getBurstTime(input_tuple[j]);
                j++;
         }
          int current_quantum=pow(2,Priority_level);
          while(current_quantum && remainingServiceTime[Pid]){
            current_quantum--;
            remainingServiceTime[Pid]--;
            i++;
          }
          if(remainingServiceTime[Pid]==0){
               int AT=getArrivalTime(input_tuple[Pid]);
         int BT=getBurstTime(input_tuple[Pid]);
             Finish_time[Pid]=i;
             TurnAround_time[Pid]=Finish_time[Pid]-AT;
             Waiting_time[Pid]=TurnAround_time[Pid]-BT;
          }
        else{
            if(pq.size()>=1)
            pq.push({Priority_level+1,Pid});
            else
                pq.push({Priority_level,Pid});
         }
         i--;
        }
        while(j<process_count && getArrivalTime(input_tuple[j])==i+1){
                pq.push({0,j});
                remainingServiceTime[j]=getBurstTime(input_tuple[j]);
                j++;
         }
    }
}
void printans(const vector<tuple<string, int, int>>& input_tuple, int process_count) {
    // Print ID row
    cout << "ID             ";
    for (int i = 0; i < process_count; i++) {
        cout << " | " << get<0>(input_tuple[i]);
    }
    cout <<" |"<< endl;

    // Print Arrival Time row
    cout << "Arrival Time   ";
    for (int i = 0; i < process_count; i++) {
        cout << " | " << get<1>(input_tuple[i]);
    }
    cout <<" |"<< endl;

    // Print Burst Time row
    cout << "Burst Time     ";
    for (int i = 0; i < process_count; i++) {
        cout << " | " << get<2>(input_tuple[i]);
    }
    cout <<" |"<< endl;

    // Print Waiting Time row
    cout << "Waiting Time   ";
    for (int i = 0; i < process_count; i++) {
        cout << " | " << Waiting_time[i];
    }
   cout <<" |"<< endl;

    // Print Turnaround Time row
    cout << "Turnaround Time";
    for (int i = 0; i < process_count; i++) {
        cout << " | " << TurnAround_time[i];
    }
    cout <<" |"<< endl;

    // Print Finish Time row
    cout << "Finish Time    ";
    for (int i = 0; i < process_count; i++) {
        cout << " | " << Finish_time[i];
    }
   cout <<" |"<< endl;
}

int main() {
    int process_count;
    int time_span;

    cout << "Enter Time Span: ";
    cin >> time_span;

    cout << "Enter Number of Processes: ";
    cin >> process_count;

    unordered_map<string, int> process_types;
    process_types["fcfs"] = 1;

    string process_name;
    cout << "Enter the process name: ";
    cin >> process_name;
    vector<tuple<string, int, int>> input_tuple;
     for (int i = 0; i < process_count; i++) {
        string process_id;
        int AT, BT;
        cout << "Enter the Process ID, Arrival Time, Burst Time: ";
        cin >> process_id >> AT >> BT;
        input_tuple.emplace_back(process_id, AT, BT);
    }
    if (process_name == "fcfs" || process_name=="FCFS") {
        getFCFS(input_tuple, process_count, time_span);
        printans(input_tuple, process_count);
    } else if (process_name == "rr" || process_name=="RR") {
        int quantum;
        cout << "Enter Quantum: ";
        cin >> quantum;
        RoundRobin(input_tuple, process_count, quantum, time_span);
        printans(input_tuple, process_count);
    }
    else if(process_name=="sjf" || process_name=="SJF"){
        SJF(input_tuple,process_count,time_span);
        printans(input_tuple, process_count);
    }
    else if(process_name=="srtf" || process_name=="SRTF"){
        SRTF(input_tuple,process_count,time_span);
        printans(input_tuple, process_count);
    }
    else if(process_name=="hrrn" || process_name=="HRRN"){
        highestResponseRatioNext(input_tuple,process_count,time_span);
        printans(input_tuple,process_count);
    }
    else if(process_name=="MLQ" || process_name=="mlq"){
        Ageing(input_tuple,process_count,time_span);
        printans(input_tuple,process_count);
    }
    else if(process_name=="MLQ2" || process_name=="mlq2"){
        Ageing2(input_tuple,process_count,time_span);
        printans(input_tuple,process_count);
    }
    // else if(process_name=="ageing"){
    //     ageing(input_tuple,process_count,time_span);
    //     printans(input_tuple,process_count);
    // }
    else{
        cout<<"Invalid Process"<<endl;
    }
    return 0;
}
