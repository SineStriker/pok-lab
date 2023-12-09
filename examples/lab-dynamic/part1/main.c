#include <assert.h>
#include <core/event.h>
#include <core/partition.h>
#include <core/thread.h>
#include <core/time.h>
#include <types.h>

#include <libc/stdio.h>
#include <libc/string.h>

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

static int shell_run(const char *command) {
  if (!strcmp(command, "help")) {
    printf("    help       Show help message\n");
    printf("    task1      Run task1\n");
    printf("    task2      Run task2\n");
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

// Main shell
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

int main() {
  uint32_t tid;
  pok_ret_t ret;
  pok_thread_attr_t tattr;

  // Set attributes
  memset(&tattr, 0, sizeof(tattr));
  tattr.entry = shell_main;
  tattr.period = -1;
  tattr.time_capacity = -1;

  // Create thread
  ret = pok_thread_create(&tid, &tattr);
  (void)ret;

  // Go into normal mode
  pok_partition_set_mode(POK_PARTITION_MODE_NORMAL);
  pok_thread_wait_infinite();

  return 0;
}
