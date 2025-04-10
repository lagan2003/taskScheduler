# TaskScheduler
An implementation of various CPU scheduling algorithms in C++. The algorithms included are First Come First Serve (FCFS), Round Robin (RR), Shortest Job First (SJF), Shortest Remaining Time (SRTF), Highest Response Ratio Next (HRRN), Aging.
## First Come First Serve (FCFS)
First Come First Serve (FCFS) is a scheduling algorithm where the process that arrives first is executed first. It is simple and easy to understand, but it can lead to poor performance if there are processes with long burst times. FCFS does not prioritize processes, making it a non-preemptive algorithm. This means that once a process starts executing, it runs until completion. FCFS is commonly used in batch systems where the order of processes is important.

## Round Robin with Varying Time Quantum (RR)
Round Robin (RR) with variable quantum is a scheduling algorithm that uses time-sharing to divide CPU time among processes. In this version of RR, the quantum (time slice) can be adjusted depending on the requirements of the processes. Shorter processes can be given smaller quanta and longer processes larger quanta. The algorithm maintains a queue of processes, where each process is given a quantum of time to execute. When a process's quantum expires, it is moved to the back of the queue. This helps to minimize the average waiting time and avoid starvation, where long processes prevent others from executing.

## Shortest Job First (SJF)
Shortest Process Next (SJF) is a scheduling algorithm that executes processes based on their burst time, or the time needed to complete their task. It is a non-preemptive algorithm, meaning that once a process starts executing, it runs until completion. The process with the shortest burst time is executed first. This minimizes the average waiting time for processes but can cause longer running processes to be blocked by shorter ones.

## Shortest Remaining Time First (SRTF)
Shortest Remaining Time Next (SRTF) is similar to SPN but is a preemptive algorithm. This means that a new process can interrupt a running process with a shorter remaining time. The process with the shortest remaining time is executed first. SRT adapts to changes in remaining time as processes execute, making it useful when burst times are not known in advance. This helps to minimize the average waiting time for processes.

## Highest Response Ratio Next (HRRN)
Highest Response Ratio Next (HRRN) is a scheduling algorithm that prioritizes processes based on their response ratio. The response ratio is calculated as (waiting time + burst time) / burst time. The process with the highest response ratio is executed first. HRRN is non-preemptive, meaning that once a process starts executing, it runs until completion. This algorithm helps to minimize the average waiting time, especially for processes with long waiting times.

## Aging
Aging is a technique used in process scheduling to prevent starvation of low-priority processes. Starvation occurs when high-priority processes continuously prevent low-priority processes from getting CPU time. Aging gradually increases the priority of a waiting process, ensuring it eventually gets CPU time.

## Aging with a Quantum in Power
In some systems, a more nuanced approach to aging involves using a quantum, a fixed time slice, and adjusting the aging factor in a more controlled manner. This approach combines the benefits of aging and time-slicing to optimize process scheduling.
