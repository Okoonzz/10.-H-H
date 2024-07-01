#include <iostream>
#include <algorithm> 
#include <iomanip>
#include <queue> 
#include <string.h>
using namespace std;

struct process {
    int name;
    int arrival_time;
    int burst_time;
    int start_time;
    int completion_time;
    int waiting_time;
    int response_time;
    int burst_remaining;
};

bool compare1(process p1, process p2) 
{ 
    return p1.arrival_time < p2.arrival_time;
}

int main() {

    int n;
    int quantumTime;
    struct process p[100];
    float avg_completion_time;
    float avg_waiting_time;
    float avg_response_time;
    int total_completion_time = 0;
    int total_waiting_time = 0;
    int total_response_time = 0;
    int idx;

    cout << setprecision(2) << fixed;

    cout<<"Enter the number of processes: ";
    cin>>n;
    cout<<"Enter time quantum: ";
    cin>>quantumTime;

    for(int i = 0; i < n; i++) {
        cout<<"Enter ID Arrival Burst: "; cin >> p[i].name >>p[i].arrival_time >>p[i].burst_time;
        p[i].burst_remaining = p[i].burst_time;
    }

    sort(p,p+n,compare1);

    queue<int> q;
    int current_time = 0;
    q.push(0);
    int completed = 0;
    int mark[100];
    memset(mark,0,sizeof(mark));
    mark[0] = 1;

    while(completed != n) {
        idx = q.front();
        q.pop();

        //check process có phải lần đầu tiên được chạy hay không
        if(p[idx].burst_remaining == p[idx].burst_time) {
            p[idx].start_time = max(current_time,p[idx].arrival_time);
            current_time = p[idx].start_time;
            cout<<"P" <<p[idx].name<<" ";
        }

        //Kiểm tra xem sau khi chạy với quatum thì có hết burst hay chưa
        if(p[idx].burst_remaining-quantumTime > 0) {
            p[idx].burst_remaining -= quantumTime;
            current_time += quantumTime;
            cout<< "P" <<p[idx].name<<" ";
        }
        //nếu burst còn lại nhỏ hơn quantum time thì lấy current time là burst hiện tại
        else {
            current_time += p[idx].burst_remaining;
            p[idx].burst_remaining = 0;
            completed++;

            p[idx].completion_time = current_time - p[idx].arrival_time;
            p[idx].waiting_time = p[idx].completion_time - p[idx].burst_time;
            p[idx].response_time = p[idx].start_time - p[idx].arrival_time;

            total_completion_time += p[idx].completion_time;
            total_waiting_time += p[idx].waiting_time;
            total_response_time += p[idx].response_time;
            cout<< "P"<<p[idx].name<<" ";
        }

        //xét process kế tiếp xem trong quá trình chạy process trước đó thì có process nào được đưa vào 
        //hay không. Nếu có thì đưa vào đầu hàng đợi. Biến mark được sử dụng để chỉ đưa 1 process
        //vào hàng đợi, giúp tránh thêm vào nhiều lần. Ví dụ như đang xét P6 (đã ở hàng đợi), thì khi
        //duyệt qua ko thêm P6 vào hàng đợi nữa mà P6 sẽ là của p[idx] còn thêm vào hàng đợi là process khác
        //(process được thêm vào trong quá trình chạy P6)
        for(int i = 1; i < n; i++) {
            if(p[i].burst_remaining > 0 && p[i].arrival_time <= current_time && mark[i] == 0) {
                q.push(i);
                mark[i] = 1;
            }
        }

        //đưa process đang chạy xuống cuối hàng đợi.
        if(p[idx].burst_remaining > 0) {
            q.push(idx);
        }

        if(q.empty()) {
            for(int i = 1; i < n; i++) {
                if(p[i].burst_remaining > 0) {
                    q.push(i);
                    mark[i] = 1;
                    break;
                }
            }
        }


    }

    avg_completion_time = (float) total_completion_time / n;
    avg_waiting_time = (float) total_waiting_time / n;
    avg_response_time = (float) total_response_time / n;
    
    cout<<"\nThời gian hoàn thành trung bình: "<<avg_completion_time<<endl;
    cout<<"Thời gian đợi trung bình: "<<avg_waiting_time<<endl;
    cout<<"Thời gian đáp ứng trung bình: "<<avg_response_time<<endl;
}
