#include <iostream>
#include <algorithm> 
#include <iomanip>
#include <string.h> 
#include <vector>
using namespace std;
struct Process {
    int name;
    int arrival_time;
    int burst_time;
    int start_time;
    int completion_time;
    int response_time;
    int waiting_time;
    int check_first = 0;
    int burst_remaining;
    int is_complete = 0;
};


void SRTF(Process *process, int numProcess){
    float avg_completion_time;
    float avg_waiting_time;
    float avg_response_time;
    int total_completion_time = 0;
    int total_waiting_time = 0;
    int total_respone_time = 0;

    int current_time = 0;
    int completed = 0;
    vector<int> execution_order;

    while(completed != numProcess) {
        int idx = -1;

        //khai báo giá trị init
        int PrevBurstRemain = 10000000;

        //chọn 1 process
        for(int i = 0; i < numProcess; i++) {
            if(process[i].arrival_time <= current_time && process[i].is_complete == 0) {
                if(process[i].burst_remaining < PrevBurstRemain) {
                    PrevBurstRemain = process[i].burst_remaining;
                    idx = i;
                }
            }
        }

        //bắt đầu chạy process được chọn
        if(idx != -1) {
            //check lần đầu tiên process được đưa vào chạy
            if(!process[idx].check_first) {
                process[idx].start_time = current_time;
                process[idx].check_first = 1;
            }

            //cập nhật lại burst sau khi chạy
            process[idx].burst_remaining--;
            current_time++;
            
            //process hoan thanh
            if(process[idx].burst_remaining == 0) {
                process[idx].completion_time = current_time - process[idx].arrival_time;
                process[idx].waiting_time = process[idx].completion_time - process[idx].burst_time;
                process[idx].response_time = process[idx].start_time - process[idx].arrival_time;

                total_completion_time += process[idx].completion_time;
                total_waiting_time += process[idx].waiting_time;
                total_respone_time += process[idx].response_time;

                process[idx].is_complete = 1;
                completed++;
            }
            execution_order.push_back(process[idx].name);
        }
        else {
             current_time++;
        }  
    }


    avg_completion_time = (float) total_completion_time / numProcess;
    avg_waiting_time = (float) total_waiting_time / numProcess;
    avg_response_time = (float) total_respone_time / numProcess;



    cout<<"Thời gian hoàn thành trung bình: "<<avg_completion_time<<endl;
    cout<<"Thời gian đợi trung bình:"<<avg_waiting_time<<endl;
    cout << "thời gian đáp ứng trung bình: " << avg_response_time<<endl;


    cout << "Tiến trình: ";
    for(int i = 0; i < execution_order.size(); i++) {
        cout << "P" << execution_order[i];
        if(i != execution_order.size() - 1) cout << " -> ";
    }
    cout << endl;

}

int main() {

    int numProcess;
    Process *process = new Process[100];

    cout << setprecision(2) << fixed;

    cout<<"Enter the number of processes: ";
    cin>>numProcess;

    for(int i = 0; i < numProcess; i++) {
        cout<<"Enter ID Arrival Burst: "; cin >> process[i].name >> process[i].arrival_time >> process[i].burst_time;
        process[i].burst_remaining = process[i].burst_time;
    }

    SRTF(process, numProcess);
}
