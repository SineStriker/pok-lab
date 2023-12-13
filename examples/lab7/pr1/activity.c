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
#include <core/time.h>


extern uint8_t sid;
uint8_t val;

void *pinger_job() {
  pok_ret_t ret;
  while (1) { 
    printf("job vedio 1 work\n");
  }
}

void *pinger_job2() {
  pok_ret_t ret;
  while (1) {
    printf("job vedio 2 work\n");
  }
}

void *pinger_job3() {
  pok_ret_t ret;
  while (1) {
    printf("job vedio 3 work\n");
  }
}
