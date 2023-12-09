## MLFQ
### MLFQ算法简述
> 注：本节对MLFQ算法的介绍主要参考Operating Systems: Three Easy Pieces Version 1.10中的Scheduling: The Multi-Level Feedback Queue一节。

多级反馈队列（MLFQ）是一种著名的调度方法，它尝试解决的基本问题有两方面：首先，它希望优化turnaround time，通过优先运行较短的任务来实现；其次，MLFQ希望使系统对交互用户感觉响应迅速，即最小化响应时间。

顾名思义，MLFQ有一系列队列，每个队列都有自己独特的优先级，尽管不同系统中对MLFQ的实现存在差异，但我们可以引入MLFQ的两个基本原则：
- **Rule 1**: If Priority(A) > Priority(B), A runs (B doesn’t).
- **Rule 2**: If Priority(A) = Priority(B), A & B run in RR.

有了这两条基本规则后，自然地，主要问题就集中在如何设置和动态调整优先级的问题上了。为了解决这个问题，我们可以引入一个所谓的allotment，也就是一个job在特定优先级队列上可以花费的时间块。由此展开新的几条规则：
- **Rule 3**: When a job enters the system, it is placed at the highest priority (the topmost queue).
- **Rule 4a**: If a job uses up its allotment while running, its priority is reduced (i.e., it moves down one queue).
- **Rule 4b**: If a job gives up the CPU (for example, by performing an I/O operation) before the allotment is up, it stays at the same priority level (i.e., its allotment is reset).

有了这几条规则之后，长任务执行过程中会逐渐降低优先级，而新的任务进来之后会被“认为”是短任务（放在高优先级）从而优先执行，如果它确实是短任务，那很快就会优先执行完，之后回到低优先级的长任务，如果它不是短任务，那一段时间后它也会降低到低优先级，从而开始RR，交互执行。

这样一来，对于交互和周转的基本要求就得到了一定的实现，但仍然存在一些问题，比如如果短任务很多或交互任务很多，长任务很可能会陷入饥饿状态，再比如一些开发者可以利用这种新任务优先机制来对系统进行攻击等等。

所以为了解决这些问题，进一步对算法进行升级：加一个优先级boost，也就是一段时间后对低优先级任务进行优先级升高操作，添加下面这个新规则：
- **Rule 5**: After some time period S, move all the jobs in the system to the topmost queue.

而这又出现了一个新问题，就是如何设置一个合理的升级间隔，这是一个比较tricky的问题，没有很定式的解决方式，而为了一定程度上解决这个问题，可以将之前的Rule 4部分进行改动，得到下面这条规则：
- **Rule 4**: Once a job uses up its time allotment at a given level (regardless of how many times it has given up the CPU), its priority is reduced (i.e., it moves down one queue).

经过上述一系列发现问题再改进之后，我们可以得到一个refined版本的MLFQ规则，总结如下：
- **Rule 1**: If Priority(A) > Priority(B), A runs (B doesn’t).
- **Rule 2**: If Priority(A) = Priority(B), A & B run in round-robin fashion using the time slice (quantum length) of the given queue.
- **Rule 3**: When a job enters the system, it is placed at the highest
priority (the topmost queue).
- **Rule 4**: Once a job uses up its time allotment at a given level (regardless of how many times it has given up the CPU), its priority is reduced (i.e., it moves down one queue).
- **Rule 5**: After some time period S, move all the jobs in the system
to the topmost queue.

### MLFQ算法实现
本次实验中使用C语言实现了一个MLFQ调度算法的模拟，详见my_mlfq.c文件。

首先对几个关键结构体以及一些配置相关宏进行了定义：
~~~c
#define MAX_PRIORITY_LEVELS 3 // 优先级队列深度
#define TIME_SLICE 2 // 队列上时间片
#define BOOST_TIME 8 // 更新（升级）低优先级队列作业的时间间隔

// 进程结构
typedef struct {
    int pid;
    int priority; //当前优先级
    int burst_time; // 总执行时间
    int start_time; // 开始时间（相对模拟开始时间而言）
    bool is_completed; // 当前是否结束
} Process;

// 队列节点
typedef struct Node {
    Process process;
    struct Node* next;
} Node;

// 队列结构
typedef struct {
    Node* front;
    Node* rear;
    int size;
} Queue;
~~~
其中Queue是每一级队列的结构体，Process结构体是对进程的抽象。

一些重要函数的定义如下：
~~~c
// 创建队列
Queue* createQueue();
// 入队列
void enqueue(Queue* q, Process process);
// 出队列
Process dequeue(Queue* q);
// 提高队列内作业优先级
void boostPriority(Queue* queues[]);
// 运行进程
void runProcess(Queue* queues[], Process* process, int quantum, int total_time);
// MLFQ调度函数
void sched(Queue* queues[]);
// 初始化模拟进程
void initProcesses(Queue* queues[]);
// 初始化优先级队列
void initQueues(Queue* queues[]);
// 释放队列资源
void freeQueues(Queue* queues[])
~~~

模拟的主流程十分简洁，就是初始化后进行调度：
~~~c
int main() {
    Queue* queues[MAX_PRIORITY_LEVELS];
    initQueues(queues);
    initProcesses(queues); 
    sched(queues); // 调度执行
    freeQueues(queues);

    return 0;
}
~~~
重点看sched函数，也就是调度算法：
~~~c
void sched(Queue* queues[]) {
    int current_time = 0;
    int total_time = 0;
    while (true) {
        bool all_done = true;
        // 每个时间片开始时进行调度
        for (int i = 0; i < MAX_PRIORITY_LEVELS; i++) {
            // 优先从高优先级队列找process进行调度
            if (!isEmpty(queues[i])) {
                all_done = false;
                Process process = dequeue(queues[i]);
                if (process.start_time > total_time) {
                    enqueue(queues[i], process);
                    continue;
                }
                runProcess(queues, &process, TIME_SLICE, total_time);
                current_time += TIME_SLICE;
                total_time += TIME_SLICE;
                if (!process.is_completed) {
                    if (process.priority < MAX_PRIORITY_LEVELS - 1) {
                        process.priority++;
                    }
                    enqueue(queues[process.priority], process);
                }
                break;
            }
        }
        // 所有进程模拟执行完成，结束模拟
        if (all_done) {
            break;
        }
        // 超过升级间隔，进行优先级升级
        if (current_time >= BOOST_TIME) {
            boostPriority(queues);
            current_time = 0;
        }
    }
}
~~~
核心思路就是拿去当前最高非空优先级队列中的作业进行RR调度执行，然后在时间片到达后降低优先级，每次时间更新也同步更新升级时间片，当BOOST_TIME触发时，就对低优先级队列进行升级操作，然后重复上述调度，直到任务全部完成。

### 效果测试
在本次实验中，首先使用如下5个线程进行模拟测试：
~~~
1 20 0
2 2 4
3 15 4
4 3 10
5 10 11
~~~
也就是第0秒进来一个长任务，第4秒加入一个短任务一个长任务，第10秒加入一个短任务，第11秒加入一个长任务。

保持优先级队列最大深度为5，队列内时间片为1的情况下(为了显示方便)，改动BOOST_TIME也就是升级时间间隔，进行如下测试：

1. BOOST_TIME = 120（测试内不会升级）
~~~
Time 0: Process 1 is running at level 0
Time 1: Process 1 is running at level 1
Time 2: Process 1 is running at level 2
Time 3: Process 1 is running at level 3
Time 4: Process 1 is running at level 4
Time 5: Process 2 is running at level 0
Time 6: Process 3 is running at level 0
Time 7: Process 2 is running at level 1
Process 2 completed
Time 8: Process 3 is running at level 1
Time 9: Process 3 is running at level 2
Time 10: Process 3 is running at level 3
Time 11: Process 4 is running at level 0
Time 12: Process 5 is running at level 0
Time 13: Process 4 is running at level 1
Time 14: Process 5 is running at level 1
Time 15: Process 4 is running at level 2
Process 4 completed
Time 16: Process 5 is running at level 2
Time 17: Process 5 is running at level 3
Time 18: Process 1 is running at level 4
Time 19: Process 3 is running at level 4
Time 20: Process 5 is running at level 4
Time 21: Process 1 is running at level 4
Time 22: Process 3 is running at level 4
Time 23: Process 5 is running at level 4
Time 24: Process 1 is running at level 4
Time 25: Process 3 is running at level 4
Time 26: Process 5 is running at level 4
Time 27: Process 1 is running at level 4
Time 28: Process 3 is running at level 4
Time 29: Process 5 is running at level 4
Process 5 completed
Time 30: Process 1 is running at level 4
Time 31: Process 3 is running at level 4
Time 32: Process 1 is running at level 4
Time 33: Process 3 is running at level 4
Time 34: Process 1 is running at level 4
Time 35: Process 3 is running at level 4
Time 36: Process 1 is running at level 4
Time 37: Process 3 is running at level 4
Process 3 completed
Time 38: Process 1 is running at level 4
Time 39: Process 1 is running at level 4
Time 40: Process 1 is running at level 4
Time 41: Process 1 is running at level 4
Time 42: Process 1 is running at level 4
Time 43: Process 1 is running at level 4
Time 44: Process 1 is running at level 4
Process 1 completed
~~~
可以看到各个任务每执行一个时间片就会进行一次降级，而短任务进入时优先执行（level 0），上述测试中任务2和任务3同时进入等待队列，轮转执行后可见短任务2很快执行完成，而3则继续执行与降级，直到与其他任务进行轮转调度，到后面各个任务都在最低层级轮转调度直到执行完成。

接下来将升级时间缩小，看下效果：

2. BOOST_TIME = 8

~~~
Time 0: Process 1 is running at level 0
Time 1: Process 1 is running at level 1
Time 2: Process 1 is running at level 2
Time 3: Process 1 is running at level 3
Time 4: Process 1 is running at level 4
Time 5: Process 2 is running at level 0
Time 6: Process 3 is running at level 0
Time 7: Process 2 is running at level 1
Process 2 completed
Time 8: Process 3 is running at level 0
Time 9: Process 1 is running at level 0
Time 10: Process 3 is running at level 1
Time 11: Process 4 is running at level 0
Time 12: Process 5 is running at level 0
Time 13: Process 1 is running at level 1
Time 14: Process 4 is running at level 1
Time 15: Process 5 is running at level 1
Time 16: Process 3 is running at level 0
Time 17: Process 1 is running at level 0
Time 18: Process 4 is running at level 0
Process 4 completed
Time 19: Process 5 is running at level 0
Time 20: Process 3 is running at level 1
Time 21: Process 1 is running at level 1
Time 22: Process 5 is running at level 1
Time 23: Process 3 is running at level 2
Time 24: Process 1 is running at level 0
Time 25: Process 5 is running at level 0
Time 26: Process 3 is running at level 0
Time 27: Process 1 is running at level 1
Time 28: Process 5 is running at level 1
Time 29: Process 3 is running at level 1
Time 30: Process 1 is running at level 2
Time 31: Process 5 is running at level 2
Time 32: Process 3 is running at level 0
Time 33: Process 1 is running at level 0
Time 34: Process 5 is running at level 0
Process 5 completed
Time 35: Process 3 is running at level 1
Time 36: Process 1 is running at level 1
Time 37: Process 3 is running at level 2
Time 38: Process 1 is running at level 2
Time 39: Process 3 is running at level 3
Process 3 completed
Time 40: Process 1 is running at level 0
Time 41: Process 1 is running at level 1
Time 42: Process 1 is running at level 2
Time 43: Process 1 is running at level 3
Time 44: Process 1 is running at level 4
Process 1 completed
~~~
可以看到，真正的短任务仍然可以优先执行完成，而长任务也会定期升级从而优先执行，而同优先级内各任务可以轮转执行。