# OS Kernel Simulator
The project was coded for the programming assignment of Hunter CSCI 340, which I took in spring 2015. The program is a simple operating system kernel simulator, which handles basic interrupts and system calls from processes. It also incorporates process management and memory management (paging). The disk scheduling is implemented with C-LOOK algorithm. For CPU scheduling, the short-term scheduling utilizes preemptive shortest-job-first and the long-term scheduling employs best-fit algorithm.
<hr/>
Compile and run:
g++ src/oshw3.cpp -o run.me
./run.me
<hr/>
The program has two stages of operation, the “sys gen” section, and the “running” section. During sys gen, the system installer specifies how many devices of each type (printer, disk, and CD/RW) are in the system (there is only one CPU implemented).<br/>
During the running section, capital letters are interrupts, lower case letters indicate system calls. Each non-CPU device has a “name” consisting of a letter and an integer. The process in the CPU can request “printer 1” by issuing a “p1” on the keyboard, and Printer 1 will signal an “interrupt” indicating completion of the task with a “P1” being entered at the keyboard.
Interrupts:
“A”: arrival of a process; a PCB and a PID are generated
“S”: a “Snapshot” interrupt; next input:
<div style = "padding-left: 5px">“r” - show the PIDs of the processes in the Ready Queue
“p” - show the PIDs and printer specific information
“d” - do the same for the disks
"c" - the CD/RW queues
</div>
“t”: process is terminating; the PCB is recycled
