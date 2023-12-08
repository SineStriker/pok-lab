/*
 *                               POK header
 *
 * The following file is a part of the POK project. Any modification should
 * be made according to the POK licence. You CANNOT use this file or a part
 * of a file for your own project.
 *
 * For more information on the POK licence, please see our LICENCE FILE
 *
 * Please follow the coding guidelines described in doc/CODING_GUIDELINES
 *
 *                                      Copyright (c) 2007-2022 POK team
 */

#include <core/semaphore.h>
#include <core/thread.h>
#include <libc/stdio.h>
#include <types.h>

uint8_t val;

void *pinger_job() {
  pok_ret_t ret;
  while (1) {
    printf("P1T1: pok_sem_signal, ret=%d\n", ret);
    pok_thread_sleep(200000);
  }
}

void *pinger_job2() {
  pok_ret_t ret;
  while (1) {
    printf("P1T2: pok_sem_wait, ret=%d\n", ret);
    pok_thread_sleep(200000);
  }
}
