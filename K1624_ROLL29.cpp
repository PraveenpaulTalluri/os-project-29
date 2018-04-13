#include<iostream>
#include<queue>

using namespace std;

int no_of_pr;
const int threshold=5; //if(bt>=threshold) it is batch process
const int RRTimeslice=2;
struct process{
int pid;
int at; int bt; int et; int rt; int wt; int tt; //et - executed time //bt- bursttime //tt- turnaround
};

void print_process(process t){
	cout<<"pid:"<<t.pid;
	cout<<" at:"<<t.at;
	cout<<" bt:"<<t.bt;
	cout<<" wt:"<<t.wt;
	cout<<" et:"<<t.et;	
	cout<<" rt:"<<t.rt;
	cout<<" tt:"<<t.tt;
}

void print_queue(std::queue<process> q){
  process t; queue<process> tq;
  while (!q.empty())
  {
	t=q.front();
	cout<<"\n";
	print_process(t);
	tq.push(t);
    q.pop();	
  }
  std::cout << std::endl;
}

int main(){
queue<process> q1,q2;


cout<<"give number of processes : ";
cin>>no_of_pr;

process p[no_of_pr];

int max_at=0;

for(int i=0;i<no_of_pr;i++){ 
cout<<"enter arrival time of p"<<i<<"  :";
cin>>p[i].at;
cout<<"enter burst time of p"<<i<<"    :";
cin>>p[i].bt;

p[i].pid=i;
p[i].et=0;	//process did not start execution, so executed time=0	
p[i].rt=p[i].bt; //initially remaining time = burst time
p[i].wt=0;
p[i].tt=0;

if(p[i].at>max_at) max_at=p[i].at;

}

int turnaround_time=0,tot_wait_time=0;

int lTimeslice=0;

int t=0,i;
queue<process> tempq;
do{//time counter 
cout<<"\n time :"<<t<<endl;

for(i=0;i<no_of_pr;i++) if(t==p[i].at){//a process has arrived
	
cout<<"\nprocess "<<p[i].pid<<" arrived";
if(p[i].bt<threshold){ //interactive process - SRTF
	//queue tempq;
	if(!q1.empty()){
	while(!q1.empty() && q1.front().rt <= p[i].rt){
		process tmp=q1.front();  tempq.push(tmp);	q1.pop();
	}
	tempq.push(p[i]);
	while(!q1.empty()) { 
		process tmp=q1.front(); q1.pop();
		tempq.push(tmp);
		}
	while(!tempq.empty()){
		process tmp=tempq.front(); tempq.pop();
		q1.push(tmp);
	}
	}
	else q1.push(p[i]);
}
else//batch process - RR
	q2.push(p[i]);
	
}

if(!q1.empty()){//give processor to q1
//except q.front() increase wt for all in q1

process tmp=q1.front(); q1.pop();
		tempq.push(tmp);
while(!q1.empty()) { 
		tmp=q1.front(); tmp.wt++; //increase wt then push again
		q1.pop();
		tempq.push(tmp);
		}
	while(!tempq.empty()){
		process tmp=tempq.front(); tempq.pop();
		q1.push(tmp);
	}

if(q1.front().rt>0)
{	q1.front().et++; q1.front().rt--; }
if(q1.front().rt==0)//et=bt process completed
{
	process finp=q1.front();
	finp.tt=finp.et+finp.wt;
	cout<<"\nFINISHED ";
	print_process(finp);
	turnaround_time+=finp.tt; tot_wait_time+=finp.wt;
	q1.pop();
}

	//increase wt for all processes in q2
	while(!q2.empty()) { 
		process tmp=q2.front(); tmp.wt++; //increase wt then push again
		q2.pop();
		tempq.push(tmp);
		}
	while(!tempq.empty()){
		process tmp=tempq.front(); tempq.pop();
		q2.push(tmp);
	}

}
else if(!q2.empty()){//give processor to q2
	if(q2.front().rt>0){
	//increase wt for all other processes in q2
	//queue tempq;
	process tmp=q2.front(); q2.pop();
		tempq.push(tmp);
	while(!q2.empty()) { 
		tmp=q2.front(); tmp.wt++; //increase wt then push again
		q2.pop();
		tempq.push(tmp);
		}
	while(!tempq.empty()){
		process tmp=tempq.front(); tempq.pop();
		q2.push(tmp);
	}

		if(lTimeslice<RRTimeslice) {	lTimeslice++;
						q2.front().et++;  q2.front().rt--;
					}
		else{	process tmp=q2.front();
			q2.pop();
			q2.push(tmp);
			lTimeslice=0;
			}	
	}
	if(q2.front().rt==0)//et=bt process completed
	{ 	process finp=q2.front();
		finp.tt=finp.et+finp.wt;
		cout<<"\nFINISHED ";
		print_process(finp);
		turnaround_time+=finp.tt; tot_wait_time+=finp.wt;
		q2.pop(); lTimeslice=0; }

}

if(!q1.empty()){ cout<<"\nque1";print_queue(q1);}
if(!q2.empty()){ cout<<"\nque2"; print_queue(q2); }
t++;
}while((!q1.empty() || !q2.empty() ) || t<=max_at);

cout<<"\nTotal Turnaround time is "<<turnaround_time;
cout<<"\n Average waiting time is "<<tot_wait_time/no_of_pr;

return 0;
}
