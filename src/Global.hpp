#include <string>
#include <vector>

#ifndef GLOBAL_HPP
#define GLOBAL_HPP

using namespace std;

double alpha;  //history parameter
double tao;  //initial burst estimate
int* dtrack;  //array keep track numbers of each disk
//dtrack[N-1] is the track num of disk N
int* curtrackhead;  //keep track of current track head
double tcputotal=0;  //total cpu time of completed proc
int comprocnum=0;  //number of completed processes
unsigned long Memsize;  //totoal memory size
unsigned long Pagesize;  //the size of a page
unsigned long Maxsize;  //Maximum size of a process
int framenum;  //number of frames in system


//data structure PCB process control block
struct PCB{
	int PID;  //process id
	PCB *prev=NULL;  //pointer to previous PCB
	PCB *next=NULL;  //pointer to next PCB
	string filename;  //filename related to this process
	unsigned long startloc;  //starting location in memory
	char rorw;  //read or write
	long filesize;  //how long the file is
	int cylnum;  //No. of cylinder to access
	double taonext; //estimated next burst time
	double tprev;  //previous burst time actual
	int burstnum;  //total number of cpu burst
	double tave;  //average burst time
	double ttotal;  //total burst time
	double lotime;  //left over time
	unsigned long psize; //process size
	int pageNum;  //number of pages needed
	int* pagetable;  //page table array entrance
	
};
//queue data structure containing a head and a tail pointer
struct queue{
	PCB *head=NULL;
	PCB *tail=NULL;
};
queue *printer;
queue *disk;
queue *cdrw;



int pidtrack=0;
vector<PCB*> prolist;
queue jobpool;

#endif