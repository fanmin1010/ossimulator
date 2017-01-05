#include <string>
#include <iostream>
#include <limits>
#include <vector>
#include "stdio.h"
#include <cmath>
#include "Global.hpp"

#ifndef MOD2_HPP
#define MOD2_HPP

using namespace std;

//new stuff for HW3 starts here

void InputULong(unsigned long & temp, unsigned long lowlimit, 
	unsigned long uplimit, string instruct)
{
	
	while(true)  //check input
	{
		if(uplimit==0)
		{
			if(cin>>temp&&temp>=lowlimit)
			{
				cin.clear();
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				break;
			}
		}
		else {
			if(cin>>temp&&temp>=lowlimit&&temp<=uplimit)
			{
				cin.clear();
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				break;
			}
		}
		cout<<"Error! Please enter the right number."<<endl;
		cout<<instruct;
		cin.clear();
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
}

bool isPowerOfTwo(unsigned long x)  //check whether a num is power of 2
{
	return ((x!=0)&&((x&(~x+1))==x));
}

void MemSetup(unsigned long & memsize, unsigned long &maxsize,
	unsigned long & pagesize)
{
	while(true){
		cout<<"Total memory size:";
		InputULong(memsize, 0, 0, "Total memory size:");
		while(true){
			cout<<"The size of a page:";
			InputULong(pagesize, 0, memsize, "Page size:");
			if(isPowerOfTwo(pagesize)) break;
			else cout<<"Error: page size is not a power of 2!"<<endl;
		}
		if(memsize % pagesize==0) break;
		else cout<<"Error, page size should divide up memory size."<<endl;
	}
	while(true){
		cout<<"Maximum size of a process:";
		InputULong(maxsize, 0, 0, "Max process size:");
		if(maxsize<=memsize) break;
		else cout<<"Error: max process size must be less than memory size!"
			<<endl;
	}
	framenum=memsize/pagesize;  //initialization of framenum
}

unsigned long getHexinput(unsigned long & testn, 
	unsigned long  uplimit, PCB& tempPro){
	while(true)
	{
		if(cin>>hex>>testn&&testn<=uplimit)
			{
				cin.clear();
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				cin>>dec;
				break;
			}
		cout<<"Error! Memory address illegal."<<endl;
		cout<<"Starting location:";
		cin.clear();
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
	//int offsetlen=(int) (log((double)Pagesize)/log(2.0));
	unsigned long offset=testn%Pagesize;
	int temppagenum=testn/Pagesize;  //get page num
	int tempframenum=tempPro.pagetable[temppagenum];  //find frame num
	unsigned long physadd=tempframenum*Pagesize+offset;  //physical address
	cout<<"Physical address:";  //print out
	cout<<hex<<physadd<<endl;  //hex out
	cout<<dec;
	tempPro.startloc=physadd;  //add to startloc
	return physadd;
}    //tested and compiled
	

void sortJobPool(PCB & temp)  //add to jobpool's right pos
{
	//if job pool is empty then pcb is head & tail
	if(jobpool.head==NULL)
	{
		jobpool.head=&temp;
		jobpool.tail=&temp;  //tail pointer too
	}
	else
	{
		PCB* comtemp=jobpool.head;  //used for loop comparison
		bool inserted=false;
		while(comtemp!=NULL)
		{
			//in loop when bigger than size, insert
			if(temp.psize>comtemp->psize)
			{
				inserted=true;
				temp.prev=comtemp->prev;
				temp.next=comtemp;
				comtemp->prev=&temp;  //insertion completed
				if(temp.prev==NULL)  //if new head for jobpool
					jobpool.head=&temp;
				else  //otherwise set the prev pcb's next pointer
					(temp.prev)->next=&temp;
				break;
			}
			comtemp=comtemp->next;  //iteration
		}
		if(inserted==false)  //if reached the end
		{
			temp.prev=jobpool.tail;
			jobpool.tail->next=&temp;
			jobpool.tail=&temp;
			temp.next=NULL;
		}
	}
}
PCB* removePCB(PCB& temp, queue& qtem){
	if(temp.prev==NULL){  //head of the queue
		qtem.head=temp.next;
		if(temp.next==NULL)
			qtem.tail=NULL;
		else
			temp.next->prev=NULL;
		temp.next=NULL;
	}
	else if(temp.next==NULL){  //endl of queue
		qtem.tail=temp.prev;
		if(temp.prev==NULL)
			qtem.head=NULL;
		else
			temp.prev->next=NULL;
		temp.prev=NULL;
	}
	else {
		temp.prev->next=temp.next;
		temp.next->prev=temp.prev;
		temp.prev=NULL;
		temp.next=NULL;
	}
	return &temp;
}


int ptPrint(queue& qtem){  //print page table info in each queue
	PCB* iter=qtem.head;
	int protrack=0;
	while(true){
		if (iter == NULL) break;
		printf("%-4d%-4d", iter->PID, iter->pageNum);
		protrack++;
		for(int i=0; i<iter->pageNum; i++)
			cout<<iter->pagetable[i]<<" ";
		cout<<endl;
		if(iter->next==NULL||iter==qtem.tail)
			break;
		iter=iter->next;
	}
	return protrack;
}

void PrintJP(){  //job pool printing func
	PCB* iter=jobpool.head;
	cout<<"job pool list:"<<endl;
	while(iter!=NULL){
		cout<<iter->PID<<" ";
		iter=iter->next;
	}
	cout<<endl;
}


#endif