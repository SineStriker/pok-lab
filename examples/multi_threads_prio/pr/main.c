#include <libc/stdio.h>
#include <libc/string.h>
#include <core/thread.h>
#include <core/partition.h>
#include <types.h>

static void task();

int main() {
    uint32_t tid;
    pok_thread_attr_t tattr;
    memset(&tattr, 0, sizeof(pok_thread_attr_t));

    tattr.period = 1000;
    tattr.time_capacity = 100;
    tattr.weight = 3;
    tattr.entry = task;
    tattr.priority = 250;
    pok_thread_create(&tid, &tattr);

    tattr.period = 800;
    tattr.time_capacity = 200;
    tattr.weight = 2;
    tattr.entry = task;
    tattr.priority = 150;
    pok_thread_create(&tid, &tattr);

    tattr.period = 1000;
    tattr.time_capacity = 300;
    tattr.weight = 1;
    tattr.entry = task;
    tattr.priority = 50;
    pok_thread_create(&tid, &tattr);

    pok_partition_set_mode(POK_PARTITION_MODE_NORMAL);
    pok_thread_wait_infinite();
    return 0;
}

static void task() {
    printf("Start Task!\n");
    uint32_t counter = 0;
    const uint32_t max_count = 1000;  // 可以根据需要调整这个值

    while (counter < max_count) {
        // 执行一些工作...
        counter++;
    }

    printf("Task completed!\n");
}
