# 动态创建线程

## 代码理解

用户态调用`pok_thread_create`函数后，`libc`执行`POK_SYSCALL_THREAD_CREATE`系统调用，内核在`thread.c`中的`pok_partition_thread_create`中处理此请求。

> We can create a thread only if the partition is in INIT mode

由注释可知，当且仅当在 INIT 模式下才允许创建线程。为了在 NORMAL 模式动态创建线程，在`pok_thread_attr_t`中添加一个成员`dynamic`，修改代码使其允许该属性被设定时，作为可创建的特例。

```c++
if ((pok_partitions[partition_id].mode != POK_PARTITION_MODE_INIT_COLD)
    && (pok_partitions[partition_id].mode != POK_PARTITION_MODE_INIT_WARM) && (!attr->dynamic)) {
    return POK_ERRNO_MODE;
}
```

为了在用户模式能够创建线程，我们允许用户态使用标准输入读取字符串，根据字符串的值创建不同类型的任务。

利用`libc`提供的`getChar`与`printf`函数，我们可以实现一个简单的 shell 程序，从标准输入读取需要执行的任务，并提供一个 help 命令。为了制作 shell 的方便，我们为`libc`补充一个`putc`函数。

将`events`示例直接复制为`lab-dynamic`，清除不必要的文件并适当修改 Makefile。

在`part1/development.h`中添加`POK_CONFIG_NEEDS_FUNC_GETCHAR`以启用`getc`函数，设置`POK_CONFIG_NB_THREADS`、`POK_CONFIG_PARTITIONS_SIZE`与`POK_CONFIG_PARTITIONS_NTHREADS`为较大的值。

## 代码实现

### shell
实现一个从终端读取一行输入并填入缓冲区的`readline`函数，支持回退。

```c++
#define BUFLEN 2048

static void shell_backspace() {
  putc('\b');
  putc(' ');
  putc('\b');
}

// read a command from stdin leading by `prompt`
// put the commond in `buf` and return `buf`
// What you typed should be displayed on the screen
void shell_readline(const char *prompt, char *buf) {
  int i = 0, j = 0;
  signed char c = 0;
  char complement[BUFLEN];

  if (prompt != NULL) {
    printf("%s", prompt);
  }

  while (1) {
    c = getc();

    if (i == 0) {
      buf[0] = '\0';
    }
    if (j == 0) {
      complement[0] = '\0';
    }

    if (c == '\t') {
      // Ignore
    } else {
      // Sync with screen
      if (j > 0) {
        strcpy(buf, complement);
        i = j;
        complement[0] = '\0';
        j = 0;
      }

      if (c == 127 || c == '\b') {
        if (i > 0) {
          i--;
          buf[i] = '\0';
          shell_backspace();
        }
      } else if (c == '\n' || c == 13) {
        buf[i] = '\0';
        i++;
        putc('\n');
        break;
      } else if (c >= 32 && c <= 126) {
        buf[i] = c;
        buf[i + 1] = '\0';
        i++;
        putc(c);
      }
    }
  }
}
```

### 主循环

```c++
static void shell_main() {
  char buf[BUFLEN];
  int ret = 0;
  while (1) {
    shell_readline("> ", buf);
    if (buf[0] == 0)
      continue;

    if ((ret = shell_run(buf)) < 0) {
      printf("Cannot run %s, ERROR %d\n", buf, ret);
    }
    printf("\n");
  }
}
```

### 命令执行

我们定义了 2 个任务，分别通过`task1`与`task2`触发
```c++
static int shell_run(const char *command) {
  if (!strcmp(command, "help")) {
    printf("    help       Show help message\n");
    printf("    task1      Run task1\n");
    printf("    task2      Run task2n");
  } else if (!strcmp(command, "task1")) {

    uint32_t tid;
    pok_thread_attr_t tattr;
    memset(&tattr, 0, sizeof(tattr));

    tattr.entry = task1;
    tattr.period = 1000;
    tattr.time_capacity = 100;
    tattr.dynamic = 1;

    pok_ret_t ret = pok_thread_create(&tid, &tattr);
    printf("[P1] pok_thread_create (1) return=%d\n", ret);

  } else if (!strcmp(command, "task2")) {

    uint32_t tid;
    pok_thread_attr_t tattr;
    memset(&tattr, 0, sizeof(tattr));

    tattr.entry = task2;
    tattr.period = 800;
    tattr.time_capacity = 200;
    tattr.dynamic = 1;

    pok_ret_t ret = pok_thread_create(&tid, &tattr);
    printf("[P1] pok_thread_create (1) return=%d\n", ret);

  } else {
    printf("Command \"%s\" not found.", command);
  }
  return 0;
}
```

### 任务

任务分别是每隔 500ms 与 1000ms 输出一次，一共输出 5 次。
```c++
static void task1() {
  for (int i = 0; i < 5; ++i) {
    pok_thread_sleep(500);
    printf("task1: %d\n", i);
  }
}

static void task2() {
  for (int i = 0; i < 5; ++i) {
    pok_thread_sleep(1000);
    printf("task2: %d\n", i);
  }
}
```

## 测试

先后使用标准输入启动`task1`与`task2`，可以观察到交替打印消息。

```sh
> task1
[P1] pok_thread_create (1) return=0

> task2
[P1] pok_thread_create (1) return=0

> 
task1: 0
task1: 1
task1: 2
task2: 0
task1: 3
task1: 4
task2: 1
task2: 0
task2: 2
task2: 1
task2: 3
task2: 2
...
```