//CSCI 340 Spring 2015 Programming Assignment 3

#include <iostream>
#include <limits>
#include <string>
#include <vector>
#include "stdio.h"
#include "Mod.hpp"
#include "framelist.hpp"

using namespace std;

void Arrival(PCB& tempA);  //Arrival interrupt handling routine
PCB *CPU=NULL;  //pointer to the process in cpu

queue readyq;  //ready queue for cpu processing
int checkcpu();  //current process in cpu check
int checkinput(string& s);  //function to check correctness of input
void PDCinterrupt(queue& qtemp);  //P D C interrupt handling routine
void pdcSC(queue& qtem, int pornot);  //p d c system call handling routine
void Print(queue *temp, int size, char a);  //S interrupt routine
void sortReadyQ(PCB& temp);  //sort readyQ whenever pcb adds using preem SJF
void ArrivalnJobPool(framelist& fltem);  //func for arrival
PCB* CheckJobPool(framelist& fltem);  //get proc from jobpool
PCB* findProc(int pidtem, int pnum, int dnum, int cnum); //for K interrupt
int main()
{
	int pnum, dnum, cnum;
	dtrack=sysgen(pnum, dnum, cnum);  //Sys gen section calling sysgen function
	framelist fl;
	string input;  //for storing input from keyboard
	
	printer=new queue[pnum];  //constuct queues for printers
	disk=new queue[dnum];  //queues for disks
	cdrw=new queue[cnum];  //queues for cd/rws

	
	//while loop for handling system calls and interrupts
	while(true)
	{
		cout<<"Command:";
		if(readyq.head==NULL)
			readyq.tail=readyq.head;
		if(!(cin>>input))  //check whether the input is successful
		{
			cout<<"Error: Wrong input!"<<endl;
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			continue;
		}
		CPU=readyq.head;  //Pair with checkcpu()
		
		
		if(input[0]=='A')
		{
			if(checkinput(input)!=0) continue;
			//if(CPU!=NULL) QueryTime(*CPU);
			ArrivalnJobPool(fl);
			
			
		}
		
		else if(input[0]=='P')  //Printer interrupt handling routine
		{
			int incheck=checkinput(input);
			//if wrong format wait for next command
			if(incheck==-1) continue;
			else if(incheck>pnum||incheck<=0) 
			{ cout<<"No such device"<<endl; continue;}
			PDCinterrupt(printer[incheck-1]);
		
		}
		else if(input[0]=='D')  //Disk interrupt handling routine
		{
			int incheck=checkinput(input);
			//if wrong format wait for next command
			if(incheck==-1) continue;
			else if(incheck>dnum||incheck<=0) 
			{ cout<<"No such device"<<endl; continue;}
			if(disk[incheck-1].head!=NULL)
			{
				if(disk[incheck-1].head->next!=NULL)
					curtrackhead[incheck-1]=disk[incheck-1].head->next->cylnum;
			}
			PDCinterrupt(disk[incheck-1]);
		
		}
		else if(input[0]=='C')  //CDRW interrupt handling routine
		{
			int incheck=checkinput(input);
			//if wrong format wait for next command
			if(incheck==-1) continue;
			else if(incheck>cnum||incheck<=0) 
			{ cout<<"No such device"<<endl; continue;}
			PDCinterrupt(cdrw[incheck-1]);
		
		}
		else if(input[0]=='p')
		{
			int incheck=checkinput(input);
			//if wrong format wait for next command
			if(incheck==-1) continue;
			else if(incheck>pnum||incheck<=0) 
			{ cout<<"No such device"<<endl; continue;}
			int cpusta=checkcpu();  //checkcpu() should be called only once
			if(cpusta==-1) continue;  //if no process in CPU
            if(cpusta==1) pdcSC(printer[incheck-1], 1);
		}
		else if(input[0]=='d')
		{
			int incheck=checkinput(input);
			//if wrong format wait for next command
			if(incheck==-1) continue;
			else if(incheck>dnum||incheck<=0) 
			{ cout<<"No such device"<<endl; continue;}
			int cpusta=checkcpu();  //checkcpu() should be called only once
			if(cpusta==-1) continue;  //if no process in CPU
			if(cpusta==1) pdcSC(disk[incheck-1], incheck+1);
		}	
		else if(input[0]=='c')
		{
			int incheck=checkinput(input);
			//if wrong format wait for next command
			if(incheck==-1) continue;
			else if(incheck>cnum||incheck<=0) 
			{ cout<<"No such device"<<endl; continue;}
			int cpusta=checkcpu();  //checkcpu() should be called only once
			if(cpusta==-1) continue;  //if no process in CPU
			if(cpusta==1) pdcSC(cdrw[incheck-1], 0);
		}
		else if(input[0]=='t')
		{
			if(checkinput(input)!=0) continue;
			int cpusta=checkcpu();  //checkcpu() should be called only once
			if(cpusta==-1) continue;  //no process in cpu
			if(cpusta==1) {
				//QueryTime(*CPU);  //ask for last cpu time
				                    //done in checkcpu
				
				UpdateTime(*CPU);
				                    
				cout<<"Terminated:"<<endl;
				//output pid
				printf("%-19s%-d\n", "PID:", CPU->PID);
				//output total
				printf("%-19s%-.1f\n", "total CPU time:", CPU->ttotal);
				//output avg
				printf("%-19s%-.1f\n", "avg burst time:",CPU->tave);
				//update total cpu completed time
				tcputotal=tcputotal+CPU->ttotal;
				comprocnum++;
				//new from here
				fl.setFrame(*CPU);
				delete CPU;  //release memory
				CPU=NULL;  //set cpu free
				PCB* tempJP;
				while(true){
					tempJP=CheckJobPool(fl);  //comp jobpool with fl
					if(tempJP==NULL) break;
					else {
						bool bt=fl.getFrame(tempJP->pageNum, *tempJP);
						if(bt==false) cout<<"bug"<<endl; //shouldbe true
						sortReadyQ(*tempJP);  //put the new proc in readyq
					}
				}
				
			}
		}
		else if(input[0]=='K'){
			int incheck=checkinput(input);
			//if wrong format wait for next command
			if(incheck==-1) continue;
			else if((incheck==0&&input[1]=='\0')||incheck>pidtrack)
				{ cout<<"No such process"<<endl; continue;}
			if(CPU!=NULL){
				if(incheck==CPU->PID){
					checkcpu();
					UpdateTime(*CPU);
				                    
				cout<<"Killed:"<<endl;
				//output pid
				printf("%-19s%-d\n", "PID:", CPU->PID);
				//output total
				printf("%-19s%-.1f\n", "total CPU time:", CPU->ttotal);
				//output avg
				printf("%-19s%-.1f\n", "avg burst time:",CPU->tave);
				//update total cpu completed time
				tcputotal=tcputotal+CPU->ttotal;
				comprocnum++;
				//new from here
				fl.setFrame(*CPU);
				delete CPU;  //release memory
				CPU=NULL;  //set cpu free
				/* if (readyq.head == NULL)  //no need readyq mod done in checkcpu
					readyq.tail = readyq.head;  
				else
					readyq.head = readyq.head->next; */ 
				PCB* tempJP;
				while(true){
					tempJP=CheckJobPool(fl);  //comp jobpool with fl
					if(tempJP==NULL) break;
					else {
						bool bt=fl.getFrame(tempJP->pageNum, *tempJP);
						if(bt==false) cout<<"bug"<<endl; //shouldbe true
						sortReadyQ(*tempJP);  //put the new proc in readyq
					}
				}
				continue;
				}
			}
			PCB* target=findProc(incheck, pnum, dnum, cnum);
			if(target==NULL)
				cout<<"Process not in memory, cannot be killed."<<endl;
			else {
				cout<<"Killed:"<<endl;
				//output pid
				printf("%-19s%-d\n", "PID:", target->PID);
				//output total
				printf("%-19s%-.1f\n", "total CPU time:", target->ttotal);
				//output avg
				printf("%-19s%-.1f\n", "avg burst time:", target->tave);
				//update total cpu completed time
				tcputotal=tcputotal+target->ttotal;
				comprocnum++;
				fl.setFrame(*target);  //release used frames
				delete target;  //release memory
				PCB* tempJP;
				while(true){
					tempJP=CheckJobPool(fl);  //comp jobpool with fl
					if(tempJP==NULL) break;
					else {
						bool bt=fl.getFrame(tempJP->pageNum, *tempJP);
						if(bt==false) cout<<"bug"<<endl; //shouldbe true
						sortReadyQ(*tempJP);  //put the new proc in readyq
					}
				}
				
			}
		}
		else if(input[0]=='S')
		{
			if(input[1]=='\0') {
				cout<<"Select p, d, c, r, m: ";
				cin>>input[1];
			}
			if(input[1]=='p'||input[1]=='P')  //printer queue
			{
				Print(printer, pnum, 'p');
			}
			else if(input[1]=='d'||input[1]=='D')  //disk queue
			{
				Print(disk, dnum, 'd');
			}
			else if(input[1]=='c'||input[1]=='C')  //cdrw queue
			{
				Print(cdrw, cnum, 'c');
			}
			else if(input[1]=='r'||input[1]=='R')  //ready queue
			{
				Print(&readyq, 1, 'r');
			}
			else if(input[1]=='m'||input[1]=='M')  //memory info
			{
				cout<<"Memory information:"<<endl;
				fl.Print();
			}
			else
			{
				cout<<"Wrong command."<<endl;
				//  PrintJP();  //for debugging
			}
				
		}
		else
			cout<<"Wrong command."<<endl;
		//checkcpu();
		//if(input[0]=='S') break;
		
		
		if(CPU==NULL) CPU=readyq.head;  //double check whether necessary
		//cin.ignore(1000,'\n');  //Discard unwanted input
	}
			
	
	
	delete [] printer;
	delete [] disk;
	delete [] cdrw;
	return 0;
}

void Arrival(PCB& tempA)
{
	//initialization
	tempA.PID=pidtrack;
	tempA.filename = "";
	tempA.startloc = 0;
	tempA.rorw = ' ';
	tempA.filesize = 0;
	tempA.cylnum=0;
	tempA.taonext=tao;
	tempA.tprev=0;
	tempA.burstnum=0;
	tempA.tave=0;
	tempA.ttotal=0;
	tempA.lotime=tao;
	pidtrack++;
	prolist.push_back(&tempA);
	
	
}
int checkcpu()  //should only be used in system calls
{
	if(CPU==NULL)  //if no process in cpu return -1
		{cout<<"Currently no process is in CPU."<<endl; return -1;}
    //modify head of ready queue
	readyq.head=readyq.head->next;
	if(readyq.head==NULL)  //Empty readyq change tail pointer too.
		readyq.tail=readyq.head;
	CPU->next=NULL;  //move process to cpu
    CPU->prev=NULL;
    if(readyq.head!=NULL)
        readyq.head->prev=NULL;
    QueryTime(*CPU);
    //sortReadyQ(*CPU);  //should not sort cpu into ready q
                         //if it were a SC
	return 1;  //if successful return 1
}
int checkinput(string& s)   //if the input is wrong format return -1
{
	int temp0, temp1, temp2=0;
	if (s[0] == '\0')  //first check whether it is empty string
	{
		cout << "Wrong input format!" << endl;
		return -1;
	}
	else if (s[1] == '\0')  //check second char
		return 0;
	else if(s[2]!='\0')  //check whether its two digits
	{
		temp0=s[2]-'0';
		if(temp0<0||temp0>9)
		{
			cout<<"Wrong input format!"<<endl;
			return -1;
		}
	}
	else
	{
		temp0 = -1;  //if it only has one digit
		temp1 = s[1] - '0';
	}
	if(temp1<0||temp1>9)
		{
			cout<<"Wrong input format!"<<endl;
			return -1;
		}
	if(temp0==-1) temp2=temp1;
	if(temp0!=-1) temp2=temp1*10+temp0;
	return temp2;
}
void PDCinterrupt(queue& qtemp)
{
	if(qtemp.head==NULL)
	{
		cout<<"No process in this device queue."<<endl;
	}
	if(qtemp.head!=NULL)
	{
		int cpusta1=checkcpu();
		sortReadyQ(*CPU);  //add cpu pcb to ready q first
		//save head-next PCB pointer in qtem
		PCB* qheadtemp=qtemp.head->next; 
		sortReadyQ(*qtemp.head);  //add qtem head pcb to readyQ
		qtemp.head=qheadtemp;
		
		if(qtemp.head!=NULL)
			qtemp.head->prev=NULL;
		
		if(qtemp.head==NULL)  //qtemp is empty
			qtemp.tail=qtemp.head;
	}
}
void pdcSC(queue& qtemp, int pornot)
{
	UpdateTime(*CPU);
	cout<<"Please input file name:";  //ask for input
	cin>>CPU->filename;
	cout<<"Starting location in memory:"; //ask for parameters
	unsigned long tempstartloc;
	getHexinput(tempstartloc, CPU->psize, *CPU); //get ox input, call func
	
	if (pornot!=1){      //check whether it is a printer or not
		cout<<"Read(r) or Write(w):";
		while(true)  //Check whether input is right or not
		{
		if(cin>>(CPU->rorw)&&(CPU->rorw=='r'||CPU->rorw=='w')) break;
		cout<<"Error! Enter 'r'(read) or 'w'(write):";
		cin.clear();
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
	}
	if(pornot==1) CPU->rorw='w';
	if(CPU->rorw=='w'||CPU->rorw=='W'){   //if it is a write, ask for size
		cout<<"File size is:";
		while(!(cin>>CPU->filesize))  //check successful input or not
	{
		cout<<"Error! Enter filesize in integer number:";
		cin.clear();
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}

	}
	if(pornot<2){
	//put the current process in CPU into the end of requested queue
	if(qtemp.tail==NULL){
		qtemp.head=CPU;
		qtemp.tail=CPU;
	}
	else {
		qtemp.tail->next=CPU;
		CPU->prev=qtemp.tail;
		qtemp.tail=qtemp.tail->next;
	}
	}
	if(pornot>=2)  //if it was a disk queue
	{
		int ntem=pornot-2;
		cout<<"Entry cylinder is:";
		InputInt(CPU->cylnum, 0, dtrack[ntem], "Entry cylinder:");
		if(qtemp.head==NULL)
			curtrackhead[ntem]=CPU->cylnum;
		SortDiskQ(qtemp, *CPU, dtrack[ntem], curtrackhead[ntem]);
	}
		
	// make sure to move current process out of cpu
	CPU=NULL;
}

//print function for S interrupt
void Print(queue *temp, int size, char a)  //compliled
{
	string str;  //for display
	if(a=='p') str="printer";
	if(a=='r') str="ready queue";
	if(a=='d') str="disk";
	if(a=='c') str="cdrw";
	int temn;
	PCB* iter;  //iterator
	PCB* itercpu;  //proc in cpu
	//output average total cpu time of completed processes
	if(comprocnum!=0)
		printf("\naverage CPU time of completed processes: %.1f\n",
			tcputotal/comprocnum);
	if(comprocnum==0)
		printf("\nNo process has been completed.\n");
	
	//formating with printf function
	if(a!='r') {
	printf("%-5s%-20s%-10s%-5s%-10s%-8s%-8s%-10s\n", " ", " ",
		" ", " ", " ", "total", "avg.", "disk");
	printf("%-5s%-20s%-10s%-5s%-10s%-8s%-8s%-10s\n", "PID", "Filename",
	"MEMstart", "R/W", "Length", "time", "burst", "cylinder");}
	if(a=='r') {
	printf("%-5s%-20s%-10s%-5s%-10s%-8s%-8s%-10s\n", " ", " ",
		" ", " ", " ", "total", "avg.", "next");
	printf("%-5s%-20s%-10s%-5s%-10s%-8s%-8s%-10s\n", "PID", "Filename",
	"MEMstart", "R/W", "Length", "time", "burst", "burst");}
	if(a!='r'&&a!='d') {
	for(temn=1; temn<=size; temn++)
	{
		printf("---%s%d\n", str.c_str(), temn);
		if(temp[temn-1].head==NULL) printf("%-s\n", "Empty"); //empty queue
		else
		{
			iter=temp[temn-1].head;
			while(true){
				printf("%-5d%-20s%-10lx%-5c%-10ld%-8.1f%-8.1f\n", iter->PID,
					iter->filename.c_str(), iter->startloc, iter->rorw,
					iter->filesize, iter->ttotal, iter->tave);
				if(iter==temp[temn-1].tail||iter==NULL)
					break; //when reaching the end
				else iter=iter->next;
			}
		}
	}
	cout<<"page table:"<<endl;
	printf("%-4s%-4s%-s", "PID", "Pg#", "frame list (to ascending page#)\n");
	int sem = 0;
	for(int sem1=0; sem1<size; sem1++)	
	{
		int sem2=ptPrint(temp[sem1]);
		sem = sem + sem2;
	}
	if (sem == 0) cout << "Empty" << endl;
	}
	if(a=='r'){
		if (temp->head == NULL) 
		{
			printf("---CPU:\n");  //print PCB in cpu
			printf("%-s\n", "Empty"); //empty queue
			printf("---%s\n", str.c_str());
			printf("%-s\n", "Empty"); //empty queue
		}
		else if((temp->head)->next==NULL) 
		{
			itercpu=temp->head;
			printf("---CPU:\n");  //print PCB in cpu
			printf("%-5d%-20s%-10lx%-5c%-10ld%-8.1f%-8.1f\n", itercpu->PID,
					itercpu->filename.c_str(), itercpu->startloc, itercpu->rorw,
					itercpu->filesize, itercpu->ttotal, itercpu->tave);
			//taonext is meaningless bc this burst is not over
			printf("---%s\n", str.c_str());
			printf("%-s\n", "Empty"); //still empty
		}
		else {
			iter=(temp->head)->next;
			itercpu=temp->head;
			printf("---CPU:\n");  //print PCB in cpu
			printf("%-5d%-20s%-10lx%-5c%-10ld%-8.1f%-8.1f\n", itercpu->PID,
					itercpu->filename.c_str(), itercpu->startloc, itercpu->rorw,
					itercpu->filesize, itercpu->ttotal, itercpu->tave);
			printf("---%s\n", str.c_str());
			while(true){
				printf("%-5d%-20s%-10lx%-5c%-10ld%-8.1f%-8.1f%-10.1f\n", iter->PID,
					iter->filename.c_str(), iter->startloc, iter->rorw,
					iter->filesize, iter->ttotal, iter->tave, iter->lotime);
				if(iter==temp->tail||iter==NULL)
					break; //when reaching the end
				else iter=iter->next;
			}
		}
		cout<<"page table:"<<endl;
		printf("%-4s%-4s%-s", "PID", "Pg#", "frame list (to ascending page#)\n");
		int sem=ptPrint(readyq);
		if(sem==0) cout<<"Empty"<<endl;
	}
	if(a=='d') {
	for(temn=1; temn<=size; temn++)
	{
		printf("---%s%d\n", str.c_str(), temn);
		cout<<"current head position: "<<curtrackhead[temn-1]<<endl;
		if(temp[temn-1].head==NULL) printf("%-s\n", "Empty"); //empty queue
		else
		{
			iter=temp[temn-1].head;
			while(true){
				printf("%-5d%-20s%-10lx%-5c%-10ld%-8.1f%-8.1f%-10d\n", iter->PID,
					iter->filename.c_str(), iter->startloc, iter->rorw,
					iter->filesize, iter->ttotal, iter->tave, iter->cylnum);
				if(iter==temp[temn-1].tail||iter==NULL)
					break; //when reaching the end
				else iter=iter->next;
			}
		}
	}
	cout<<"page table:"<<endl;
	printf("%-4s%-4s%-s", "PID", "Pg#", "frame list (to ascending page#)\n");
	int sem = 0;
	for (int sem1 = 0; sem1 < size; sem1++)
	{
		int sem2 = ptPrint(temp[sem1]);
		sem = sem + sem2;
	}
	if (sem == 0) cout << "Empty" << endl;
	}
}
void sortReadyQ(PCB& temp)
{
	//if ready queue is empty pcb is head & tail
	if(readyq.head==NULL)
	{
		readyq.head=&temp;
		readyq.tail=&temp;  //change tail pointer
	}
	if(readyq.head!=NULL){
	PCB* comtemp=readyq.head;  //used for loop comparison
	bool inserted=false;
	while(comtemp!=NULL)
	{
		
		//in loop when burst < current burst, insert
		if(temp.lotime<comtemp->lotime)
		{
			inserted=true;
			temp.prev=comtemp->prev;
			temp.next=comtemp;
			comtemp->prev=&temp;  //insertion completed
			
			if(temp.prev==NULL)  //if temp became the queue head
				readyq.head=&temp;
			else  //otherwise set the next pointer of prev PCB
				(temp.prev)->next=&temp;
			break;
		}
		
		comtemp=comtemp->next;  //iteration
	}
	if(inserted==false)  //if reached the end
		{
			temp.prev=readyq.tail;
			readyq.tail->next=&temp;
			readyq.tail=&temp;
			temp.next=NULL;
		}
	CPU=readyq.head;  //after sort make sure cpu is head
	}
}
//function for arrival of new pcb and deciding JobPool or not
void ArrivalnJobPool(framelist& fltem){
	unsigned long tempsize;
	cout<<"Size of the new process:";
	InputULong(tempsize, 0, 0, "Size of the process:");
	if(tempsize>Maxsize){
		cout<<"Error! Size of the new process is beyond the maximum size."
		<<endl;
	}
	else {
		PCB *temp=new PCB();
		Arrival(*temp);
		temp->psize=tempsize;  //add to new pcb's psize
		double temp1=((double) tempsize)/((double) Pagesize);
		temp->pageNum=(int)ceil(temp1);
		temp->pagetable=new int[temp->pageNum];  //set up page table
		bool getff=fltem.getFrame(temp->pageNum, *temp);
		if(getff==true){    //add new pcb to system/readyq
			int cpusta0=checkcpu();
			sortReadyQ(*CPU);
			sortReadyQ(*temp);  //insert the newcomer
		}
		else {
			sortJobPool(*temp);
		}
	}
}
PCB* CheckJobPool(framelist& fltem){
	PCB* comtemp=jobpool.head;  //used for loop comparison
	PCB* temp1=NULL;  //used to store the one to return
	unsigned long asize=fltem.ffnum*Pagesize;  //used for comparing
	bool getted=false;
	if(jobpool.head==NULL)
		return temp1;
	while(comtemp!=NULL)
	{
		if(comtemp->psize<=asize)
		{
			temp1=removePCB(*comtemp, jobpool);
			getted=true;
			break;   //single extraction from the job pool
		}
		comtemp=comtemp->next;
	}
	return temp1;  //if cant find, return NULL
	
}
PCB* findProc(int pidtem, int pnum, int dnum, int cnum){
	PCB* iter;  //for iteration
	PCB* target=NULL;  //for storing target PCB
	bool found=false;  //found status
	iter=readyq.head;
	while(iter!=NULL){
		if(pidtem==iter->PID){
			target=removePCB(*iter, readyq);
			found=true;
			break;
		}
		iter=iter->next;
	}
	if(found==true) return target;
	for(int i=0; i<pnum; i++){
		iter=printer[i].head;
		while(iter!=NULL){
			if(pidtem==iter->PID){
				target=removePCB(*iter, printer[i]);
				found=true;
				break;
			}
			iter=iter->next;
		}
		if(found==true) break;
	}
	if(found==true) return target;
	for(int i=0; i<dnum; i++){
		iter=disk[i].head;
		while(iter!=NULL){
			if(pidtem==iter->PID){
				target=removePCB(*iter, disk[i]);
				found=true;
				break;
			}
			iter=iter->next;
		}
		if(found==true) break;
	}
	if(found==true) return target;
	for(int i=0; i<cnum; i++){
		iter=cdrw[i].head;
		while(iter!=NULL){
			if(pidtem==iter->PID){
				target=removePCB(*iter, cdrw[i]);
				found=true;
				break;
			}
			iter=iter->next;
		}
		if(found==true) break;
	}
	return target;
}
	
	