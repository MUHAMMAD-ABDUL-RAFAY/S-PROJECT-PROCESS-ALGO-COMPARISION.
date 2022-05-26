#include <iostream>
#include <algorithm> 
#include <iomanip>
#include <string.h>
#include <queue>
using namespace std;
struct process {
    int pid;
    int arrival_time;
    int burst_time;
    int start_time;
    int priority;
    int completion_time;
    int turnaround_time;
    int waiting_time;
    int response_time;
};
bool compareArrival(process p1, process p2) 
{ 
    return p1.arrival_time < p2.arrival_time;
}

bool compareID(process p1, process p2) 
{  
    return p1.pid < p2.pid;
}
void fcfs_algo(int num_of_proc,int at[],int bt[],int prior[],int prid[])
{
	cout<<"FCFS ALGO"<<endl;
	cout<<"----------------------------------------------------------"<<endl;
	struct process p1[100];
	float avg_turnaround_time;
	float avg_waiting_time;
    float avg_response_time;
    float cpu_utilisation;
    int total_turnaround_time = 0;
    int total_waiting_time = 0;
    int total_response_time = 0;
    int total_idle_time = 0;
    int temp[num_of_proc];
    cout << setprecision(2) << fixed;
    for(int a = 0; a < num_of_proc; a++)
	{
        p1[a].arrival_time = at[a];
        p1[a].burst_time = bt[a];
        p1[a].pid = prid[a];
        temp[a] = at[a];
    }
	sort(temp,temp+num_of_proc);	
    sort(p1,p1+num_of_proc,compareArrival);
    cout<<"GANTT CHART"<<endl;
    cout<<"----------------------------------------------------------"<<endl;
	for(int j=0;j<num_of_proc;j++)
    {
    	for(int l=0;l<num_of_proc;l++)
    	{
    		if(temp[j] == at[l])
    		{
    			cout<<"P"<<prid[l]<<" ";
    			break;
			}
		}
	}
    for(int a = 0; a < num_of_proc; a++) {
        p1[a].start_time = (a == 0)?p1[a].arrival_time:max(p1[a-1].completion_time,p1[a].arrival_time);
        p1[a].completion_time = p1[a].start_time + p1[a].burst_time;
        p1[a].turnaround_time = p1[a].completion_time - p1[a].arrival_time;
        p1[a].waiting_time = p1[a].turnaround_time - p1[a].burst_time;
        p1[a].response_time = p1[a].start_time - p1[a].arrival_time;			
        total_turnaround_time += p1[a].turnaround_time;
        total_waiting_time += p1[a].waiting_time;
        total_response_time += p1[a].response_time;
        total_idle_time += (a == 0)?(p1[a].arrival_time):(p1[a].start_time - p1[a-1].completion_time);
    }
    avg_turnaround_time = (float) total_turnaround_time / num_of_proc;
    avg_waiting_time = (float) total_waiting_time / num_of_proc;
    avg_response_time = (float) total_response_time / num_of_proc;
    cpu_utilisation = ((p1[num_of_proc-1].completion_time - total_idle_time) / (float) p1[num_of_proc-1].completion_time)*100;
    sort(p1,p1+num_of_proc,compareID);
    cout<<endl;
    cout<<endl;
    cout<<"#P\t"<<"AT\t"<<"BT\t"<<"ST\t"<<"CT\t"<<"TAT\t"<<"WT\t"<<"RT\t"<<endl;
    for(int a = 0; a < num_of_proc; a++) {
    	cout<<"----------------------------------------------------------"<<endl;
        cout<<p1[a].pid<<"\t"<<p1[a].arrival_time<<"\t"<<p1[a].burst_time<<"\t"<<p1[a].start_time<<"\t"<<p1[a].completion_time<<"\t"<<p1[a].turnaround_time<<"\t"<<p1[a].waiting_time<<"\t"<<p1[a].response_time<<"\t"<<endl;
    }
    cout<<endl;
    cout<<"Average Turnaround Time = "<<avg_turnaround_time<<endl;
    cout<<"Average Waiting Time = "<<avg_waiting_time<<endl;
    cout<<"Average Response Time = "<<avg_response_time<<endl;
    cout<<"CPU Utilization = "<<cpu_utilisation<<"%"<<endl;
    cout<<"----------------------------------------------------------"<<endl;
}
void priority_pre_algo(int num_of_proc,int at[],int bt[],int prior[],int prid[])
{
	cout<<"PRIORITTY ALGO PREEMPTIVE VERSION"<<endl;
	cout<<"----------------------------------------------------------"<<endl;
	struct process p2[100];
    float avg_turnaround_time;
    float avg_waiting_time;
    float avg_response_time;
    float cpu_utilisation;
    int total_turnaround_time = 0;
    int total_waiting_time = 0;
    int total_response_time = 0;
    int total_idle_time = 0;
    int burst_remaining[100];
    int is_completed[100];
    memset(is_completed,0,sizeof(is_completed));
    cout << setprecision(2) << fixed;
    for(int a = 0; a < num_of_proc; a++) {
        p2[a].arrival_time = at[a];
        p2[a].burst_time = bt[a];
        p2[a].priority = prior[a];
        p2[a].pid = prid[a];
        burst_remaining[a] = p2[a].burst_time;
    }
    int current_time = 0;
    int completed = 0;
    int prev = 0;
    cout<<"GANTT CHART"<<endl;
    cout<<"----------------------------------------------------------"<<endl;
    while(completed != num_of_proc) {
        int idx = -1;
        int mx = -1;
        for(int a = 0; a < num_of_proc; a++) {
            if(p2[a].arrival_time <= current_time && is_completed[a] == 0) {
                if(p2[a].priority > mx) {
                    mx = p2[a].priority;
                    idx = a;
                }
                if(p2[a].priority == mx) {
                    if(p2[a].arrival_time < p2[idx].arrival_time) {
                        mx = p2[a].priority;
                        idx = a;
                    }
                }
            }
        }
        if(idx != -1) {
        	cout<<"P"<<idx+1<<" ";
            if(burst_remaining[idx] == p2[idx].burst_time) {
                p2[idx].start_time = current_time;
                total_idle_time += p2[idx].start_time - prev;
                
            }
            burst_remaining[idx] -= 1;
            current_time++;
            prev = current_time;   
            if(burst_remaining[idx] == 0) {
                p2[idx].completion_time = current_time;
        		p2[idx].turnaround_time = p2[idx].completion_time - p2[idx].arrival_time;
        		p2[idx].waiting_time = p2[idx].turnaround_time - p2[idx].burst_time;
                p2[idx].response_time = p2[idx].start_time - p2[idx].arrival_time;
                total_turnaround_time += p2[idx].turnaround_time;
                total_waiting_time += p2[idx].waiting_time;
                total_response_time += p2[idx].response_time;
                is_completed[idx] = 1;
                completed++;
			}
		}
    	else {
         	current_time++;
    	}  
	}
    int min_arrival_time = 10000000;
    int max_completion_time = -1;
    for(int a = 0; a < num_of_proc; a++) {
        min_arrival_time = min(min_arrival_time,p2[a].arrival_time);
        max_completion_time = max(max_completion_time,p2[a].completion_time);
    }
    avg_turnaround_time = (float) total_turnaround_time / num_of_proc;
    avg_waiting_time = (float) total_waiting_time / num_of_proc;
    avg_response_time = (float) total_response_time / num_of_proc;
    cpu_utilisation = ((max_completion_time - total_idle_time) / (float) max_completion_time )*100;
    cout<<endl;
    cout<<endl;
    cout<<"#P\t"<<"AT\t"<<"BT\t"<<"PRI\t"<<"ST\t"<<"CT\t"<<"TAT\t"<<"WT\t"<<"RT\t"<<endl;
    for(int a = 0; a < num_of_proc; a++) {
    	cout<<"------------------------------------------------------------------"<<endl;
        cout<<p2[a].pid<<"\t"<<p2[a].arrival_time<<"\t"<<p2[a].burst_time<<"\t"<<p2[a].priority<<"\t"<<p2[a].start_time<<"\t"<<p2[a].completion_time<<"\t"<<p2[a].turnaround_time<<"\t"<<p2[a].waiting_time<<"\t"<<p2[a].response_time<<"\t"<<endl;
    }
    cout<<endl;
    cout<<"Average Turnaround Time = "<<avg_turnaround_time<<endl;
    cout<<"Average Waiting Time = "<<avg_waiting_time<<endl;
    cout<<"Average Response Time = "<<avg_response_time<<endl;
    cout<<"CPU Utilization = "<<cpu_utilisation<<"%"<<endl;
    cout<<"----------------------------------------------------------"<<endl;
}
void priority_nonpre_algo(int num_of_proc,int at[],int bt[],int prior[],int prid[])
{
	cout<<"PRIORITTY ALGO NON-PREEMPTIVE VERSION"<<endl;
	cout<<"----------------------------------------------------------"<<endl;
    struct process p3[100];
    float avg_turnaround_time;
    float avg_waiting_time;
    float avg_response_time;
    float cpu_utilisation;
    int total_turnaround_time = 0;
    int total_waiting_time = 0;
    int total_response_time = 0;
    int total_idle_time = 0;
    int is_completed[100];
    memset(is_completed,0,sizeof(is_completed));
    cout << setprecision(2) << fixed;
    for(int a = 0; a < num_of_proc; a++) {
        p3[a].arrival_time = at[a];
        p3[a].burst_time = bt[a];
        p3[a].priority = prior[a];
        p3[a].pid = prid[a];
    }
    int current_time = 0;
    int completed = 0;
    int prev = 0;
    cout<<"GANTT CHART"<<endl;
    cout<<"----------------------------------------------------------"<<endl;
	while(completed != num_of_proc) {
        int idx = -1;
        int mx = -1;
        for(int a = 0; a < num_of_proc; a++) {
            if(p3[a].arrival_time <= current_time && is_completed[a] == 0) {
                if(p3[a].priority > mx) {
                    mx = p3[a].priority;
                    idx = a;
                }
                if(p3[a].priority == mx) {
                    if(p3[a].arrival_time < p3[idx].arrival_time) {
                        mx = p3[a].priority;
                        idx = a;
                    }
                }
            }
        }
        if(idx != -1) {
            p3[idx].start_time = current_time;
            p3[idx].completion_time = p3[idx].start_time + p3[idx].burst_time;
            p3[idx].turnaround_time = p3[idx].completion_time - p3[idx].arrival_time;
            p3[idx].waiting_time = p3[idx].turnaround_time - p3[idx].burst_time;
            p3[idx].response_time = p3[idx].start_time - p3[idx].arrival_time;
            total_turnaround_time += p3[idx].turnaround_time;
            total_waiting_time += p3[idx].waiting_time;
            total_response_time += p3[idx].response_time;
            total_idle_time += p3[idx].start_time - prev;	
            is_completed[idx] = 1;
            completed++;
            cout<<"P"<<idx+1<<" ";
            current_time = p3[idx].completion_time;
            prev = current_time;
        }
        else {
            current_time++;
        }		        
    }
    int min_arrival_time = 10000000;
    int max_completion_time = -1;
    for(int a = 0; a < num_of_proc; a++) {
        min_arrival_time = min(min_arrival_time,p3[a].arrival_time);
        max_completion_time = max(max_completion_time,p3[a].completion_time);
    }
    avg_turnaround_time = (float) total_turnaround_time / num_of_proc;
    avg_waiting_time = (float) total_waiting_time / num_of_proc;
    avg_response_time = (float) total_response_time / num_of_proc;
    cpu_utilisation = ((max_completion_time - total_idle_time) / (float) max_completion_time )*100;
    cout<<endl;
    cout<<endl;
    cout<<"#P\t"<<"AT\t"<<"BT\t"<<"PRI\t"<<"ST\t"<<"CT\t"<<"TAT\t"<<"WT\t"<<"RT\t"<<endl;
    for(int a = 0; a < num_of_proc; a++) {
    	cout<<"------------------------------------------------------------------"<<endl;
        cout<<p3[a].pid<<"\t"<<p3[a].arrival_time<<"\t"<<p3[a].burst_time<<"\t"<<p3[a].priority<<"\t"<<p3[a].start_time<<"\t"<<p3[a].completion_time<<"\t"<<p3[a].turnaround_time<<"\t"<<p3[a].waiting_time<<"\t"<<p3[a].response_time<<"\t"<<endl;
    }
    cout<<endl;
    cout<<"Average Turnaround Time = "<<avg_turnaround_time<<endl;
    cout<<"Average Waiting Time = "<<avg_waiting_time<<endl;
    cout<<"Average Response Time = "<<avg_response_time<<endl;
    cout<<"CPU Utilization = "<<cpu_utilisation<<"%"<<endl;
    cout<<"----------------------------------------------------------"<<endl;
}
void sjf_algo(int num_of_proc,int at[],int bt[],int prior[],int prid[])
{
	cout<<"SJF ALGO"<<endl;
	cout<<"----------------------------------------------------------"<<endl;
    struct process p4[100];
    float avg_turnaround_time;
    float avg_waiting_time;
    float avg_response_time;
    float cpu_utilisation;
    int total_turnaround_time = 0;
    int total_waiting_time = 0;
    int total_response_time = 0;
    int total_idle_time = 0;
    float throughput;
    int is_completed[100];
    memset(is_completed,0,sizeof(is_completed));
    cout << setprecision(2) << fixed;
    for(int a = 0; a < num_of_proc; a++) {
        p4[a].arrival_time = at[a];
        p4[a].burst_time = bt[a];
        p4[a].pid =prid[a];
    }
    int current_time = 0;
    int completed = 0;
    int prev = 0;
    cout<<"GANTT CHART"<<endl;
    cout<<"----------------------------------------------------------"<<endl;
    while(completed != num_of_proc) {
        int idx = -1;
        int mn = 10000000;
        for(int a = 0; a < num_of_proc; a++) {
            if(p4[a].arrival_time <= current_time && is_completed[a] == 0) {
                if(p4[a].burst_time < mn) {
                    mn = p4[a].burst_time;
                    idx = a;
                }
                if(p4[a].burst_time == mn) {
                    if(p4[a].arrival_time < p4[idx].arrival_time) {
                        mn = p4[a].burst_time;
                        idx = a;
                    }
                }
            }
        }
        if(idx != -1) {
            p4[idx].start_time = current_time;
            p4[idx].completion_time = p4[idx].start_time + p4[idx].burst_time;
            p4[idx].turnaround_time = p4[idx].completion_time - p4[idx].arrival_time;
            p4[idx].waiting_time = p4[idx].turnaround_time - p4[idx].burst_time;
            p4[idx].response_time = p4[idx].start_time - p4[idx].arrival_time;
            total_turnaround_time += p4[idx].turnaround_time;
            total_waiting_time += p4[idx].waiting_time;
            total_response_time += p4[idx].response_time;
            total_idle_time += p4[idx].start_time - prev;
            is_completed[idx] = 1;
            completed++;
            current_time = p4[idx].completion_time;
            prev = current_time;
            cout<<"P"<<idx+1<<" ";
        }
        else {
            current_time++;
        }
    }
    int min_arrival_time = 10000000;
    int max_completion_time = -1;
    for(int a = 0; a < num_of_proc; a++) {
        min_arrival_time = min(min_arrival_time,p4[a].arrival_time);
        max_completion_time = max(max_completion_time,p4[a].completion_time);
    }
    avg_turnaround_time = (float) total_turnaround_time / num_of_proc;
    avg_waiting_time = (float) total_waiting_time / num_of_proc;
    avg_response_time = (float) total_response_time / num_of_proc;
    cpu_utilisation = ((max_completion_time - total_idle_time) / (float) max_completion_time )*100;
    cout<<endl;
    cout<<endl;
    cout<<"#P\t"<<"AT\t"<<"BT\t"<<"ST\t"<<"CT\t"<<"TAT\t"<<"WT\t"<<"RT\t"<<endl;
    for(int a = 0; a < num_of_proc; a++) {
    	cout<<"----------------------------------------------------------"<<endl;
        cout<<p4[a].pid<<"\t"<<p4[a].arrival_time<<"\t"<<p4[a].burst_time<<"\t"<<p4[a].start_time<<"\t"<<p4[a].completion_time<<"\t"<<p4[a].turnaround_time<<"\t"<<p4[a].waiting_time<<"\t"<<p4[a].response_time<<"\t"<<endl;
    }
    cout<<endl;
    cout<<"Average Turnaround Time = "<<avg_turnaround_time<<endl;
    cout<<"Average Waiting Time = "<<avg_waiting_time<<endl;
    cout<<"Average Response Time = "<<avg_response_time<<endl;
    cout<<"CPU Utilization = "<<cpu_utilisation<<"%"<<endl;			
	cout<<"----------------------------------------------------------"<<endl;
}
void srtf_algo(int num_of_proc,int at[],int bt[],int prior[],int prid[])
{
	cout<<"SRTF ALGO"<<endl;
	cout<<"----------------------------------------------------------"<<endl;
    struct process p5[100];
    float avg_turnaround_time;
    float avg_waiting_time;
    float avg_response_time;
    float cpu_utilisation;
    int total_turnaround_time = 0;
    int total_waiting_time = 0;
    int total_response_time = 0;
    int total_idle_time = 0;
    int burst_remaining[100];
    int is_completed[100];
    memset(is_completed,0,sizeof(is_completed));
    cout << setprecision(2) << fixed;
    for(int a = 0; a < num_of_proc; a++) {
        p5[a].arrival_time = at[a];
        p5[a].burst_time = bt[a];
        p5[a].pid = prid[a];
        burst_remaining[a] = bt[a];
    }
    int current_time = 0;
    int completed = 0;
    int prev = 0;
    cout<<"GANTT CHART"<<endl;
    cout<<"----------------------------------------------------------"<<endl;
    while(completed != num_of_proc) {
        int idx = -1;
        int mn = 10000000;
        for(int a = 0; a < num_of_proc; a++) {
            if(p5[a].arrival_time <= current_time && is_completed[a] == 0) {
                if(burst_remaining[a] < mn) {
                    mn = burst_remaining[a];
                    idx = a;
                }
                if(burst_remaining[a] == mn) {
                    if(p5[a].arrival_time < p5[idx].arrival_time) {
                        mn = burst_remaining[a];
                        idx = a;
                    }
                }
            }
        }
        if(idx != -1) {
        	cout<<"P"<<idx+1<<" ";
            if(burst_remaining[idx] == p5[idx].burst_time) {
                p5[idx].start_time = current_time;
                total_idle_time += p5[idx].start_time - prev;
            }
            burst_remaining[idx] -= 1;
            current_time++;
            prev = current_time;
            if(burst_remaining[idx] == 0) {
                p5[idx].completion_time = current_time;
                p5[idx].turnaround_time = p5[idx].completion_time - p5[idx].arrival_time;
                p5[idx].waiting_time = p5[idx].turnaround_time - p5[idx].burst_time;
                p5[idx].response_time = p5[idx].start_time - p5[idx].arrival_time;
                total_turnaround_time += p5[idx].turnaround_time;
                total_waiting_time += p5[idx].waiting_time;
                total_response_time += p5[idx].response_time;
                is_completed[idx] = 1;
                completed++;
            }
        }
        else {
             current_time++;
        }  
    }
    int min_arrival_time = 10000000;
    int max_completion_time = -1;
    for(int a = 0; a < num_of_proc; a++) {
        min_arrival_time = min(min_arrival_time,p5[a].arrival_time);
        max_completion_time = max(max_completion_time,p5[a].completion_time);
    }
    avg_turnaround_time = (float) total_turnaround_time / num_of_proc;
    avg_waiting_time = (float) total_waiting_time / num_of_proc;
    avg_response_time = (float) total_response_time / num_of_proc;
    cpu_utilisation = ((max_completion_time - total_idle_time) / (float) max_completion_time )*100;
    cout<<endl;
    cout<<endl;
    cout<<"#P\t"<<"AT\t"<<"BT\t"<<"ST\t"<<"CT\t"<<"TAT\t"<<"WT\t"<<"RT\t"<<endl;
    for(int a = 0; a < num_of_proc; a++) {
    	cout<<"----------------------------------------------------------"<<endl;
        cout<<p5[a].pid<<"\t"<<p5[a].arrival_time<<"\t"<<p5[a].burst_time<<"\t"<<p5[a].start_time<<"\t"<<p5[a].completion_time<<"\t"<<p5[a].turnaround_time<<"\t"<<p5[a].waiting_time<<"\t"<<p5[a].response_time<<"\t"<<endl;
    }
    cout<<endl;
    cout<<"Average Turnaround Time = "<<avg_turnaround_time<<endl;
    cout<<"Average Waiting Time = "<<avg_waiting_time<<endl;
    cout<<"Average Response Time = "<<avg_response_time<<endl;
    cout<<"CPU Utilization = "<<cpu_utilisation<<"%"<<endl;
    cout<<"----------------------------------------------------------"<<endl;
}
void round_robin_algo(int num_of_proc,int at[],int bt[],int prior[],int prid[])
{
	cout<<"ROUND-ROBIN ALGO"<<endl;
	cout<<"----------------------------------------------------------"<<endl;
	int tq;
	struct process p6[100];
	float avg_turnaround_time;
	float avg_waiting_time;
	float avg_response_time;
	float cpu_utilisation;
	int total_turnaround_time = 0;
	int total_waiting_time = 0;
	int total_response_time = 0;
	int total_idle_time = 0;
	int burst_remaining[100];
	int idx;
	cout << setprecision(2) << fixed;
	cout<<"ENTER TIME QUANTUM : ";
	cin>>tq;
	for(int a = 0; a < num_of_proc; a++) {
	    p6[a].arrival_time = at[a];
	    p6[a].burst_time = bt[a];
	    burst_remaining[a] = p6[a].burst_time;
	    p6[a].pid = prid[a];
	}
	sort(p6,p6+num_of_proc,compareArrival);
	queue<int> q;
	int current_time = 0;
	q.push(0);
	int completed = 0;
	int mark[100];
	memset(mark,0,sizeof(mark));
	mark[0] = 1;
	while(completed != num_of_proc) {
	    idx = q.front();
	    q.pop();
	    if(burst_remaining[idx] == p6[idx].burst_time) {
	        p6[idx].start_time = max(current_time,p6[idx].arrival_time);
	        total_idle_time += p6[idx].start_time - current_time;
	        current_time = p6[idx].start_time;
	    }
	    if(burst_remaining[idx]-tq > 0) {
	        burst_remaining[idx] -= tq;
	        current_time += tq;
	    }
	    else {
	        current_time += burst_remaining[idx];
	        burst_remaining[idx] = 0;
	        completed++;
	        p6[idx].completion_time = current_time;
	        p6[idx].turnaround_time = p6[idx].completion_time - p6[idx].arrival_time;
	        p6[idx].waiting_time = p6[idx].turnaround_time - p6[idx].burst_time;
	        p6[idx].response_time = p6[idx].start_time - p6[idx].arrival_time;
	        total_turnaround_time += p6[idx].turnaround_time;
	        total_waiting_time += p6[idx].waiting_time;
	        total_response_time += p6[idx].response_time;
	    }
	    for(int a = 1; a < num_of_proc; a++) {
	        if(burst_remaining[a] > 0 && p6[a].arrival_time <= current_time && mark[a] == 0) {
	            q.push(a);
	            mark[a] = 1;
	        }
	    }
	    if(burst_remaining[idx] > 0) {
	        q.push(idx);
	    }
	    if(q.empty()) {
	        for(int a = 1; a < num_of_proc; a++) {
	            if(burst_remaining[a] > 0) {
	                q.push(a);
	                mark[a] = 1;
	                break;
	            }
	        }
	    }
	}
	avg_turnaround_time = (float) total_turnaround_time / num_of_proc;
	avg_waiting_time = (float) total_waiting_time / num_of_proc;
	avg_response_time = (float) total_response_time / num_of_proc;
	cpu_utilisation = ((p6[num_of_proc-1].completion_time - total_idle_time) / (float) p6[num_of_proc-1].completion_time)*100;			
	sort(p6,p6+num_of_proc,compareID);
	cout<<endl;
	cout<<"#P\t"<<"AT\t"<<"BT\t"<<"ST\t"<<"CT\t"<<"TAT\t"<<"WT\t"<<"RT\t"<<endl;
	for(int a = 0; a < num_of_proc; a++) {
		cout<<"----------------------------------------------------------"<<endl;
	    cout<<p6[a].pid<<"\t"<<p6[a].arrival_time<<"\t"<<p6[a].burst_time<<"\t"<<p6[a].start_time<<"\t"<<p6[a].completion_time<<"\t"<<p6[a].turnaround_time<<"\t"<<p6[a].waiting_time<<"\t"<<p6[a].response_time<<"\t"<<endl;
	}
	cout<<endl;
	cout<<"Average Turnaround Time = "<<avg_turnaround_time<<endl;
	cout<<"Average Waiting Time = "<<avg_waiting_time<<endl;
	cout<<"Average Response Time = "<<avg_response_time<<endl;
	cout<<"CPU Utilization = "<<cpu_utilisation<<"%"<<endl;
	cout<<"----------------------------------------------------------"<<endl;
}
int main()
{
	int n_algo;
	int algo_choice;
	cout<<"ENTER NUMBER OF ALGOS YOU WANT TO RUN ? ";
	cin>>n_algo;
	cout<<"PRESS 1 FOR FCFS ALGO"<<endl;
	cout<<"PRESS 2 FOR PRIORITY ALGO (PREEMPTIVE)"<<endl;
	cout<<"PRESS 3 FOR PRIORITY ALGO (NON-PREEMPTIVE)"<<endl;
	cout<<"PRESS 4 FOR SJF ALGO"<<endl;
	cout<<"PRESS 5 FOR SRTF ALGO"<<endl;
	cout<<"PRESS 6 FOR ROUND-ROBIN ALGO"<<endl;
	cout<<endl<<endl;
	int arr[6] = {0,0,0,0,0,0};
	for(int i=0;i<n_algo;i++)
	{
		while(true)
		{
			cout<<"ENTER CHOICE OF ALGO "<<i+1<<" : ";
			cin>>algo_choice;
			if(algo_choice <= 6 && algo_choice > 0)
			{
				if(arr[algo_choice-1] == 0)
				{
					arr[algo_choice-1] = 1;
					break;
				}
				cout<<"YOU HAVE ALREADY CHOOSE THAT ALGO TO RUN PLEASE SELECT ANOTHER ALGO"<<endl;
			}
			else
			{
				cout<<"YOU HAVE ENTERED THE WRONG CHOICE"<<endl;
			}
		}
	}
	cout<<endl<<endl;
	int num_of_proc;
	cout<<"ENTER NUMBER OF PROCESSES : ";
	cin>>num_of_proc;
	int at[num_of_proc];
	int bt[num_of_proc];
	int prior[num_of_proc];
	int prid[num_of_proc];
	cout<<endl;
	for(int i = 0; i < num_of_proc; i++) {
        cout<<"ENTER ARRIVAL TIME OF PROCESS "<<i+1<<" : ";
        cin>>at[i];
        cout<<"ENTER BURST TIME OF PROCESS "<<i+1<<" : ";
        cin>>bt[i];
        cout<<"ENTER PRIORITY OF PROCESS "<<i+1<<" : ";
        cin>>prior[i];
        prid[i] = i+1;
        cout<<endl;
    }
	for(int i=0;i<6;i++)
	{
		if(arr[i] == 0)
		{
			continue;
		}
		else
		{
			if(i == 0)
			{
				fcfs_algo(num_of_proc,at,bt,prior,prid);
			}	
			
			if(i == 1)
			{
				priority_pre_algo(num_of_proc,at,bt,prior,prid);
			}				
			
			if(i == 2)
			{
				priority_nonpre_algo(num_of_proc,at,bt,prior,prid);			
			}
			
			if(i == 3)
			{
				sjf_algo(num_of_proc,at,bt,prior,prid);
			}
			
			if(i == 4)
			{
				srtf_algo(num_of_proc,at,bt,prior,prid);
		    }
			if(i == 5)
			{
				round_robin_algo(num_of_proc,at,bt,prior,prid);
			}
		}
	}
}
