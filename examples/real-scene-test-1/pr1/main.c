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

#include "activity.h"
#include <core/partition.h>
#include <core/semaphore.h>
#include <core/thread.h>
#include <libc/stdio.h>
#include <core/time.h>
#include <types.h>

uint8_t sid;

int main() {
  uint32_t tid;
  pok_ret_t ret;
  pok_thread_attr_t tattr;

  ret = pok_sem_create(&sid, 0, 50, POK_QUEUEING_DISCIPLINE_DEFAULT);
  printf("[P1] pok_sem_create return=%d, mid=%d\n", ret, sid);

  tattr.processor_affinity = 0;
  tattr.entry = prog_life_support;
  tattr.priority = 1;
  tattr.period = 500;
  tattr.time_capacity = 1;
  tattr.deadline = 100;
  ret = pok_thread_create(&tid, &tattr);
  printf("[P1] pok_thread_create (1) return=%d\n", ret);

  tattr.priority = 30;
  tattr.period = 800;
  tattr.time_capacity = 5;
  tattr.deadline = 600;
  tattr.entry = prog_medical_records;
  ret = pok_thread_create(&tid, &tattr);
  printf("[P1] pok_thread_create (2) return=%d\n", ret);
  
  tattr.priority = 40;
  tattr.period = 1000;
  tattr.time_capacity = 7;
  tattr.deadline = 1000;
  tattr.entry = prog_patient_monitoring;
  ret = pok_thread_create(&tid, &tattr);
  printf("[P1] pok_thread_create (3) return=%d\n", ret);

  pok_partition_set_mode(POK_PARTITION_MODE_NORMAL);
  pok_thread_wait_infinite();

  return (0);
}
