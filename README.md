# OS Kernel Simulator
The project was coded for the programming assignment of Hunter CSCI 340, which I took in spring 2015. The program is a simple operating system kernel simulator, which handles basic interrupts and system calls from processes. It also incorporates process management and memory management (paging). The disk scheduling is implemented with C-LOOK algorithm. For CPU scheduling, the short-term scheduling utilizes preemptive shortest-job-first and the long-term scheduling employs best-fit algorithm.
<hr/>
Compile and run:<br/>
g++ src/oshw3.cpp -o run.me<br/>
./run.me
<hr/>
The program has two stages of operation, the “sys gen” section, and the “running” section. During sys gen, the system installer specifies how many devices of each type (printer, disk, and CD/RW) are in the system (there is only one CPU implemented). Specify history parameter, α (0 ≤ α ≤ 1) the initial burst estimate.<br/>
During the running section, capital letters are interrupts, lower case letters indicate system calls. Each non-CPU device has a “name” consisting of a letter and an integer. The process in the CPU can request “printer 1” by issuing a “p1” on the keyboard, and Printer 1 will signal an “interrupt” indicating completion of the task with a “P1” being entered at the keyboard.<br/>
Interrupts:<br/>
“A”: arrival of a process; a PCB and a PID are generated<br/>
“S”: a “Snapshot” interrupt; next input:<br/>
&nbsp;&nbsp;&nbsp;&nbsp;“r” - show the PIDs of the processes in the Ready Queue
<br/>&nbsp;&nbsp;&nbsp;&nbsp;“p” - show the PIDs and printer specific information
<br/>&nbsp;&nbsp;&nbsp;&nbsp;“d” - do the same for the disks
<br/>&nbsp;&nbsp;&nbsp;&nbsp;"c" - the CD/RW queues<br/>
&nbsp;&nbsp;&nbsp;&nbsp;“m” - system memory information<br/>
"K": kill a process with its pid<br/>
"P": printer interrupt with printer number<br/>
"D": disk interrupt with disk number<br/>
"C": cd/rw interrupt with cd/rw number<br/>
System calls:<br/>
“t”: process is terminating; the PCB is recycled<br/>
"p": printer call with printer number<br/>
"d": disk call with disk number<br/>
"c": cd/rw call with cd/rw number<br/>
<hr/>
Have fun!
