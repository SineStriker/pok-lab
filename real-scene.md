## 实际应用场景设计

我们设想了一个智能医疗监控系统，包含三种实时任务：

1. **prog_life_support**：生命支持进程，负责处理与患者生命体征（如心跳、血压）相关的紧急信号。这个进程的优先级最高，有硬实时要求，绝对不能错过deadline。例如，如果心跳监测仪检测到心跳异常，这个进程必须立即激活相应的紧急响应，比如发送警报、调整心脏起搏器设置等。

2. **prog_medical_records**：医疗记录进程，用于处理患者的电子医疗记录，如病史、检查结果、药物用量等。这个进程的优先级次高，属于软实时要求，可以容忍一定程度的延迟。例如，当医生更新患者的治疗方案时，这个进程需要及时处理并更新数据库，但相比于生命支持进程，其响应时间可以稍长。

3. **prog_patient_monitoring**：患者监控进程，用于在医护人员的监控屏幕上显示患者的实时健康状况和房间环境信息，如温度、湿度。这个进程优先级最低，也属于软实时要求，但处理数据量大，耗时较长。例如，它可能需要从多个传感器收集数据，并将这些信息以图形界面的形式呈现，以便医护人员可以一目了然地掌握患者的整体状况。

在这个智能医疗监控系统中，不同优先级的进程确保了在保障患者紧急生命体征监测的同时，也能有效处理日常的医疗信息和环境监控任务。

我们会针对这个场景进行单分区多线程的调度，以及多分区调度的测试。

### 任务设计

~~~c
tattr.entry = prog_life_support;
tattr.processor_affinity = 0;
tattr.priority = 1;
tattr.period = 500;
tattr.time_capacity = 1;
tattr.deadline = 150;
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
~~~
三种任务的设置分别优先级从高到低（priority越小代表优先级越高），life_support进程每500tick触发一次，每次执行100 tick，ddl是150 tick；medical_records进程每800 tick触发一次，执行300 tick，ddl是600 tick；patient_monitoring每1000 tick触发一次，执行400 tick，ddl 1000 tick，也就是基本上一直在空闲时运行。

分别在上述场景中使用RR，WRR，抢占式EDF，抢占式优先级算法调度，观察4000 tick内的执行情况，见测试报告。