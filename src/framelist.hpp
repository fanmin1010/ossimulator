// frame list class header file

#include "Global.hpp"
#include <iostream>
#include "stdio.h"
#include <cmath>
#include <vector>

#ifndef FRAMELIST_HPP
#define FRAMELIST_HPP
using namespace std;

struct frame{
	int PID;
	int Pagenum;
};

class framelist{
public:
	frame* frametable;  //frame table
	bool* freeframe;  //keep track of free frame
	int ffnum;  //free frame num
	
public:
	framelist();  //default constructor
	void Print();  //print frame table and free frame list
	bool getFrame(int n, PCB& tempP);  //get n num of frames K or not
	int setFrame(PCB& tempP);  //when proc ends, set its frames free
	
};
// default constructor initialize two arrays
framelist::framelist(){
	frametable= new frame[framenum];  //initialize the arrays
	freeframe=new bool[framenum];
	for(int i=0; i<framenum; i++)
	{
		frametable[i].PID=-1;
		frametable[i].Pagenum=-1;
		freeframe[i]=true;  //set to true for free frame
		ffnum=framenum;  //free frame num initialization
		
	}
}

void framelist::Print(){
	//bf print creat a free frame list
	vector<frame> fframels;
	vector<int> fnumls;
	for (int se = 0; se < framenum; se++)
	{
		if (freeframe[se] == false){
			fframels.push_back(frametable[se]);
			fnumls.push_back(se);
		}
	}
	if (fframels.size() != (framenum - ffnum))
		cout << "create fframe list fail error" << endl;

	//cout<<ffnum<<endl;
	cout<<"frame table:"<<endl;
	double temp1=(framenum-ffnum)/18.0;  //used frame num
	int rownum=(int)ceil(temp1);
	int columnnum;
	if ((framenum - ffnum) == 18)
		columnnum = 18;  //columnnum not be 0 if one full row
	else
		columnnum = (framenum - ffnum) % 18;  //used frame num
	int temp3;  //for loop temp in column printing
	for(int temp2=0; temp2<rownum; temp2++)
	{
		if(temp2!=0)
			cout<<"continued..."<<endl;
		if(temp2==rownum-1) temp3=columnnum;
		else temp3=18;  //deciding how many columns to print
		
		printf("frame:");
		for(int temp4=0; temp4<temp3; temp4++)
		{
			int tempid=temp2*18+temp4;
			printf("%4d", fnumls[tempid]);  //only print out used frames
		}
		cout<<endl;
		printf("%-6s", "PID:");
		for(int temp4=0; temp4<temp3; temp4++)
		{
			int tempid=temp2*18+temp4;
			printf("%4d", fframels[tempid].PID);
			//else printf("    ");  //unused one nothing
		}
		cout<<endl;
		printf("%-6s", "page#:");
		for(int temp4=0; temp4<temp3; temp4++)
		{
			int tempid=temp2*18+temp4;	
			printf("%4d", fframels[tempid].Pagenum);
			//else printf("    ");  //unused one nothing
		}
		cout<<endl;
	}
	cout<<"free frame list:";
	for(int temp5=0; temp5<framenum; temp5++)
	{
		if(freeframe[temp5])
			printf(" %d", temp5);
	}
	cout<<endl;
}

bool framelist::getFrame(int n, PCB& tempP){
	if(n>ffnum)
		return false;
	else
	{
		ffnum=ffnum-n;  //reduce free frame num
		for(int m=0; m<n; m++){  //assign each page
			for(int m1=0; m1<framenum; m1++)  //find free frame and Mod
			{
				if(freeframe[m1]==true)  //find free frame
				{
					freeframe[m1]=false;  //mod status
					frametable[m1].PID=tempP.PID;  //mod frame table
					frametable[m1].Pagenum=m;
					tempP.pagetable[m]=m1;  //put the frame# to pagetable
					break;  //no need for more search
				}
			}
		}
	}
	return true;  //get frame evaluates to true
	
}
int framelist::setFrame(PCB& tempP){
	int status=0;  //for checking set success or not
	int tep1=tempP.pageNum;
	int tep2;
	ffnum=ffnum+tep1;
	if(ffnum>framenum) {status--; cout<<"error"<<endl;}
	for(int m=0; m<tep1; m++){
		tep2=tempP.pagetable[m];  //get frame num in p table
		freeframe[tep2]=true;  //mod status
		frametable[tep2].PID=-1;
		frametable[tep2].Pagenum=-1;
	}
	return status;
}

#endif