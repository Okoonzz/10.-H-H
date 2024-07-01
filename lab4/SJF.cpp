#include <stdio.h>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Process{
    int name;
    int Burst_Time;
    int Arrival_Time;
    int Waiting_Time;
    int Completion_Time;
};

bool compareArrival(Process p1, Process p2) 
{ 
    return p1.Arrival_Time < p2.Arrival_Time;
}

bool compareBurst(Process p1, Process p2) 
{ 
    return p1.Burst_Time < p2.Burst_Time;
}

void SJF(Process *process, int numProcess){
    int current_Time = 0;
    vector<Process> tmpProc;
    double total_Waiting_Time = 0, total_Completion_Time = 0;

    sort(process, process + numProcess, compareArrival); // Sort by arrival time

    int i = 0;
    while(i < numProcess || !tmpProc.empty()){
        while(i < numProcess && process[i].Arrival_Time <= current_Time){
            tmpProc.push_back(process[i]);
            i++;
        }
        if(!tmpProc.empty()){
            sort(tmpProc.begin(), tmpProc.end(), compareBurst); // Sort by burst time
            Process shortestJob = tmpProc.front();
            cout << shortestJob.name << " ";
            shortestJob.Waiting_Time = current_Time - shortestJob.Arrival_Time;
            current_Time += shortestJob.Burst_Time;
            shortestJob.Completion_Time = current_Time - shortestJob.Arrival_Time;
            total_Waiting_Time += shortestJob.Waiting_Time;
            total_Completion_Time += shortestJob.Completion_Time;
            tmpProc.erase(tmpProc.begin());
        } else {
            if(i < numProcess){
                current_Time = process[i].Arrival_Time;
            }
        }
    }

    double average_Waiting_Time = total_Waiting_Time / numProcess;
    double average_Completion_Time = total_Completion_Time / numProcess;
    cout << "\nThời gian đợi trung bình: " << average_Waiting_Time << "\n";
    cout << "Thời gian hoàn thành trung bình: " << average_Completion_Time << "\n";
}

int main(){
    int numProcess;
    Process *process = new Process[200];

    cout << "Enter number process: ";
    
    cin >> numProcess;
    for(int i = 0; i < numProcess; i++){
        cout << "Enter ID Arrival Burst: "; cin >> process[i].name >> process[i].Arrival_Time >> process[i].Burst_Time;
    }
    SJF(process, numProcess);

}