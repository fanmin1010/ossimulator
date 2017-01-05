#include <string>
#include <iostream>
#include <limits>
#include "Mod2.hpp"

#ifndef MOD_HPP
#define MOD_HPP

using namespace std;

void InputInt(int & temp, int lowlimit, string instruct);
void InputInt(int & temp, int lowlimit, int uplimit, string instruct);
void InputDouble(double & temp, double lowlimit, double uplimit, string instruct);
//C-LOOK disk scheduling alg
int SortDiskQ(queue& qtem, PCB& temp,  int TNCylinder, int curhead);
void QueryTime(PCB& temp); //time query when Interrupt or SC
//Fisrt part "sys gen" section function
int* sysgen(int & pnumber, int & dnumber, int & cdnumber)
{
	cout<<"Please specify how many devices of each type:"<<endl;
	cout<<"Printer: ";
	int temp, temp2, temp3;
	temp=temp2=temp3=0;
	InputInt(temp, 0, 100, "Printer: ");
	pnumber=temp;
	cout<<"Disk: ";
	InputInt(temp2, 0, 100, "Disk: ");
	dnumber=temp2;
	int* dtracktemp= new int[temp2];
	curtrackhead=new int[temp2];  //initialize track head int array
	for(int count=0; count<temp2; count++)
	{
		cout<<"Number of cylinders for Disk "<<count+1<<":";
		InputInt(dtracktemp[count], 0, "Number of cylinders: ");
		curtrackhead[count]=0;
	}
	cout<<"CD/RW: ";
	InputInt(temp3, 0, 100, "CD/RW: ");
	cdnumber=temp3;
	cout<<"Input history parameter alpha:";
	InputDouble(alpha, 0, 1, "aplpha: ");
	cout<<"Input initial burst estimate:";
	InputDouble(tao, 0, 30000, "initial burst estimate: ");
	
	MemSetup(Memsize, Maxsize, Pagesize); //Set up MMU part
	
	return dtracktemp;
}

void InputInt(int & temp, int lowlimit, string instruct)
{
	while(true)  //check input
	{
		if(cin>>temp&&temp>=lowlimit)
		{
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			break;
		}
		cout<<"Error! Please enter the right number."<<endl;
		cout<<instruct;
		cin.clear();
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
}
void InputInt(int & temp, int lowlimit, int uplimit, string instruct)
{
	while(true)  //check input
	{
		if(cin>>temp&&temp>=lowlimit&&temp<=uplimit)
		{
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			break;
		}
		cout<<"Error! Please enter the right number."<<endl;
		cout<<instruct;
		cin.clear();
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
}

void InputDouble(double & temp, double lowlimit, double uplimit, string instruct)
{
	while(true)  //check input
	{
		if(cin>>temp&&temp>=lowlimit&&temp<=uplimit) 
		{
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			break;
		}
		cout<<"Error! Please enter the right float number."<<endl;
		cout<<instruct;
		cin.clear();
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
}

//sort disk queue using C-LOOK
int SortDiskQ(queue& qtem, PCB& newtemp, int TNCylinder, int curhead) 
{
	bool  status=false;  //to see if the comparison reaches the end
	PCB* comtemp=qtem.head;  // used for loop comparison
	if(qtem.head==NULL)
	{
		qtem.head=& newtemp;
		qtem.tail=& newtemp;
		newtemp.prev=NULL;
		newtemp.next=NULL;
		return 0;
	}
	//decide whether it goes to be the first pcb after head
	if(curhead<=comtemp->cylnum)
	{
		if(curhead<=newtemp.cylnum&&newtemp.cylnum<comtemp->cylnum)
		{
			//insert to the qtem head first one
			newtemp.next=qtem.head;
			qtem.head->prev=&newtemp;
			qtem.head=&newtemp;
			newtemp.prev=NULL;
			return 1; //insertion finished and stop func
		}
	}
	if(curhead>comtemp->cylnum)
	{
		bool bb1=curhead<=newtemp.cylnum&&newtemp.cylnum<=TNCylinder;
		bool bb2=0<=newtemp.cylnum&&newtemp.cylnum<comtemp->cylnum;
		if(bb1||bb2)
		{
			//insert to the qtem head first one
			newtemp.next=qtem.head;
			qtem.head->prev=&newtemp;
			qtem.head=&newtemp;
			newtemp.prev=NULL;
			return 1; //insertion finished and stop func
		}
	}
		
		
	while(comtemp!=NULL)
	{
		
		
		
		if(comtemp->next!=NULL)
		{
			if(comtemp->cylnum<=newtemp.cylnum&&newtemp.cylnum<comtemp->next->cylnum)
			{ 	 //if the new track is btw prev and next insert
				newtemp.prev=comtemp;  //PCB insert start
				newtemp.next=comtemp->next;
				comtemp->next=&newtemp;
				newtemp.next->prev=&newtemp;  //PCB insert finish
				status=true;
				break;
			}
			if(comtemp->cylnum>comtemp->next->cylnum)  //last one this direction
			{
				bool b1=comtemp->cylnum<=newtemp.cylnum&&newtemp.cylnum<=TNCylinder;
				bool b2=0<=newtemp.cylnum&&newtemp.cylnum<comtemp->next->cylnum;
				if(b1||b2)
				{
					newtemp.prev=comtemp;  //PCB insert start
					newtemp.next=comtemp->next;
					comtemp->next=&newtemp;
					newtemp.next->prev=&newtemp;  //PCB insert finish
					status=true;
					break;
				}
			}
		}
		if(comtemp->next==NULL)
		{
			if(status==false)
			{
				newtemp.prev=comtemp;  //PCB insert start
				newtemp.next=NULL;
				comtemp->next=&newtemp;  //PCB end insert finish
				qtem.tail=&newtemp;
				break;
			}
		}  //compiled
		comtemp=comtemp->next;  //iteration
	}
	return 2;
}
void QueryTime(PCB& temp)
{
	double tempd;
	cout<<"CPU time used:";
	InputDouble(tempd, 0, 30000, "burst time:");
	//temp.tprev=tempd;  // one burst is done
	// temp.burstnum++;  //burst num increase only when leaves cpu
	temp.ttotal=temp.ttotal+tempd;  //increase total burst time
	temp.lotime=temp.lotime-tempd;
	//to correct negative leftover time
	if(temp.lotime<0)
		temp.lotime=0;
}
void UpdateTime(PCB& temp)
{
	// only called when one burst is done
	temp.tprev=temp.ttotal-temp.tave*temp.burstnum;  
	temp.burstnum++;  //burst num increase only when leaves cpu
	
	//update estimated next burst time
	temp.taonext=alpha*temp.taonext+(1-alpha)*temp.tprev;
	temp.lotime=temp.taonext;
	if(temp.burstnum!=0)  //calculate ave time if num is non-zero
		temp.tave=temp.ttotal/temp.burstnum;
}

#endif