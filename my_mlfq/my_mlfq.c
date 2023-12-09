#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_PRIORITY_LEVELS 5
#define TIME_SLICE 1
#define BOOST_TIME 8

// 进程结构
typedef struct {
    int pid;
    int priority;
    int burst_time;
    int start_time;
    bool is_completed;
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

// 创建队列
Queue* createQueue() {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    q->front = q->rear = NULL;
    q->size = 0;
    return q;
}

// 入队列
void enqueue(Queue* q, Process process) {
    Node* temp = (Node*)malloc(sizeof(Node));
    temp->process = process;
    temp->next = NULL;

    if (q->rear == NULL) {
        q->front = q->rear = temp;
    } else {
        q->rear->next = temp;
        q->rear = temp;
    }
    q->size++;
}

// 出队列
Process dequeue(Queue* q) {
    if (q->front == NULL) {
        return (Process){0, 0, 0, 0, true};
    }

    Node* temp = q->front;
    Process process = temp->process;
    q->front = q->front->next;

    if (q->front == NULL) {
        q->rear = NULL;
    }

    free(temp);
    q->size--;
    return process;
}

// 判断队列是否为空
bool isEmpty(Queue* q) {
    return q->front == NULL;
}

// 提高优先级
void boostPriority(Queue* queues[]) {
    Queue* tempQueue = createQueue();

    // 将所有进程移动到临时队列
    for (int i = 1; i < MAX_PRIORITY_LEVELS; i++) {
        while (!isEmpty(queues[i])) {
            enqueue(tempQueue, dequeue(queues[i]));
        }
    }

    // 将所有进程重置优先级并移回原队列
    while (!isEmpty(tempQueue)) {
        Process process = dequeue(tempQueue);
        process.priority = 0;
        enqueue(queues[0], process);
    }

    free(tempQueue);
}

// 运行进程
void runProcess(Queue* queues[], Process* process, int quantum, int total_time) {
    printf("Time %d: Process %d is running at level %d\n", total_time, process->pid, process->priority);
    process->burst_time -= quantum;
    if (process->burst_time <= 0) {
        process->is_completed = true;
        printf("Process %d completed\n", process->pid);
    }
}

// 初始化优先级队列
void initQueues(Queue* queues[]) {
    for (int i = 0; i < MAX_PRIORITY_LEVELS; i++) {
        queues[i] = createQueue();
    }
}

// 初始化模拟进程
void initProcesses(Queue* queues[]) {
    FILE* configFile = fopen("./config", "r");
    if (configFile == NULL) {
        perror("Error opening config file");
        exit(1);
    }

    int pid, burst_time, start_time;
    while (fscanf(configFile, "%d %d %d", &pid, &burst_time, &start_time) != EOF) {
        Process process = {pid, 0, burst_time, start_time, false};
        enqueue(queues[0], process);
    }

    fclose(configFile);
}

// 释放队列资源
void freeQueues(Queue* queues[]) {
    for (int i = 0; i < MAX_PRIORITY_LEVELS; i++) {
        free(queues[i]);
    }
}

// 调度函数
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

int main() {
    Queue* queues[MAX_PRIORITY_LEVELS];
    initQueues(queues);
    initProcesses(queues); 
    sched(queues);
    freeQueues(queues);

    return 0;
}
