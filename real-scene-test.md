## 实际应用场景测试
以下可视化输出部分，thread 1代表prog_life_support，thread 2代表prog_medical_records，thread 3代表prog_patient_monitoring，优先级（紧急度）从高到低。
1. RR调度
~~~
[P1] pok_sem_create return=0, mid=0
[P1] pok_thread_create (1) return=0
[P1] pok_thread_create (2) return=0
[P1] pok_thread_create (3) return=0
thread: 1 elected, time: 51
Start prog life supportthread: 2 elected, time: 100
Start medical recordsthread: 3 elected, time: 150
Start patient monitoringthread: 1 elected, time: 200
thread: 2 elected, time: 250
thread: 3 elected, time: 300
thread: 2 elected, time: 350
thread: 3 elected, time: 400
thread: 2 elected, time: 450
thread: 3 elected, time: 500
thread: 1 elected, time: 550
thread: 2 elected, time: 600
thread: 3 elected, time: 650
thread: 1 elected, time: 700
thread: 2 elected, time: 750
thread: 3 elected, time: 800
thread: 2 elected, time: 850
thread: 3 elected, time: 900
thread: 2 elected, time: 950
thread: 3 elected, time: 1000
thread: 1 elected, time: 1050
thread: 2 elected, time: 1100
thread: 3 elected, time: 1150
thread: 1 elected, time: 1200
thread: 2 elected, time: 1250
thread: 3 elected, time: 1300
thread: 2 elected, time: 1350
thread: 3 elected, time: 1400
thread: 2 elected, time: 1450
thread: 3 elected, time: 1500
thread: 1 elected, time: 1550
thread: 2 elected, time: 1600
thread: 3 elected, time: 1650
thread: 1 elected, time: 1700
thread: 2 elected, time: 1750
thread: 3 elected, time: 1800
thread: 2 elected, time: 1850
thread: 3 elected, time: 1900
thread: 2 elected, time: 1950
thread: 3 elected, time: 2000
thread: 1 elected, time: 2050
thread: 2 elected, time: 2100
thread: 3 elected, time: 2150
thread: 1 elected, time: 2200
thread: 2 elected, time: 2250
thread: 3 elected, time: 2300
thread: 3 elected, time: 2350
thread: 2 elected, time: 2400
thread: 3 elected, time: 2450
thread: 1 elected, time: 2500
thread: 2 elected, time: 2550
thread: 3 elected, time: 2600
thread: 1 elected, time: 2650
thread: 2 elected, time: 2700
thread: 3 elected, time: 2750
thread: 2 elected, time: 2800
thread: 3 elected, time: 2850
thread: 2 elected, time: 2900
thread: 3 elected, time: 2950
thread: 1 elected, time: 3000
thread: 2 elected, time: 3050
thread: 3 elected, time: 3100
thread: 1 elected, time: 3150
thread: 2 elected, time: 3200
thread: 3 elected, time: 3250
thread: 2 elected, time: 3300
thread: 3 elected, time: 3350
thread: 2 elected, time: 3400
thread: 3 elected, time: 3450
thread: 1 elected, time: 3500
thread: 2 elected, time: 3550
thread: 3 elected, time: 3600
thread: 1 elected, time: 3650
thread: 2 elected, time: 3700
thread: 3 elected, time: 3750
thread: 2 elected, time: 3800
thread: 3 elected, time: 3850
thread: 3 elected, time: 3900
thread: 3 elected, time: 3950
thread: 1 elected, time: 4000
~~~
可以看到，在RR调度中，三个任务轮转执行，导致紧急任务(thread 1)很多时候无法及时完成，出现ddl miss，无法很好地满足实时性要求。

2. WRR调度
~~~
[P1] pok_sem_create return=0, mid=0
[P1] pok_thread_create (1) return=0
[P1] pok_thread_create (2) return=0
[P1] pok_thread_create (3) return=0
thread: 1 elected, time: 51
Start prog life supportthread: 2 elected, time: 100
Start medical recordsthread: 2 elected, time: 150
thread: 2 elected, time: 200
thread: 2 elected, time: 250
thread: 2 elected, time: 300
thread: 3 elected, time: 350
Start patient monitoringthread: 3 elected, time: 400
thread: 3 elected, time: 450
thread: 3 elected, time: 500
thread: 3 elected, time: 550
thread: 3 elected, time: 600
thread: 3 elected, time: 650
thread: 3 elected, time: 700
thread: 3 elected, time: 750
thread: 1 elected, time: 800
thread: 2 elected, time: 850
thread: 3 elected, time: 900
thread: 1 elected, time: 950
thread: 2 elected, time: 1000
thread: 2 elected, time: 1050
thread: 2 elected, time: 1100
thread: 2 elected, time: 1150
thread: 2 elected, time: 1200
thread: 3 elected, time: 1250
thread: 3 elected, time: 1300
thread: 3 elected, time: 1350
thread: 3 elected, time: 1400
thread: 3 elected, time: 1450
thread: 3 elected, time: 1500
thread: 3 elected, time: 1550
thread: 3 elected, time: 1600
thread: 3 elected, time: 1650
thread: 1 elected, time: 1700
thread: 2 elected, time: 1750
thread: 3 elected, time: 1800
thread: 1 elected, time: 1850
thread: 2 elected, time: 1900
thread: 2 elected, time: 1950
thread: 2 elected, time: 2000
thread: 2 elected, time: 2050
thread: 2 elected, time: 2100
thread: 3 elected, time: 2150
thread: 3 elected, time: 2200
thread: 3 elected, time: 2250
thread: 3 elected, time: 2300
thread: 3 elected, time: 2350
thread: 3 elected, time: 2400
thread: 3 elected, time: 2450
thread: 3 elected, time: 2500
thread: 3 elected, time: 2550
thread: 1 elected, time: 2600
thread: 2 elected, time: 2650
thread: 3 elected, time: 2700
thread: 1 elected, time: 2750
thread: 2 elected, time: 2800
thread: 2 elected, time: 2850
thread: 2 elected, time: 2900
thread: 2 elected, time: 2950
thread: 2 elected, time: 3000
thread: 3 elected, time: 3050
thread: 3 elected, time: 3100
thread: 3 elected, time: 3150
thread: 3 elected, time: 3200
thread: 3 elected, time: 3250
thread: 3 elected, time: 3300
thread: 3 elected, time: 3350
thread: 3 elected, time: 3400
thread: 3 elected, time: 3450
thread: 1 elected, time: 3500
thread: 2 elected, time: 3550
thread: 3 elected, time: 3600
thread: 1 elected, time: 3650
thread: 2 elected, time: 3700
thread: 2 elected, time: 3750
thread: 2 elected, time: 3800
thread: 2 elected, time: 3850
thread: 2 elected, time: 3900
thread: 1 elected, time: 3950
thread: 2 elected, time: 4000
~~~
可以发现，WRR即使添加给执行时间片添加执行权重，也无法很好地满足实时性需求，因为本身还是基于轮转策略，会出现ddl miss情况。

3. 抢占式EDF调度
~~~
[P1] pok_sem_create return=0, mid=0
[P1] pok_thread_create (1) return=0
[P1] pok_thread_create (2) return=0
[P1] pok_thread_create (3) return=0
thread: 1 elected, time: 51
thread: 1 elected, time: 100
thread: 2 elected, time: 150
thread: 2 elected, time: 200
thread: 2 elected, time: 250
thread: 2 elected, time: 300
thread: 2 elected, time: 350
thread: 2 elected, time: 400
thread: 3 elected, time: 450
thread: 3 elected, time: 500
thread: 3 elected, time: 550
thread: 3 elected, time: 600
thread: 3 elected, time: 650
thread: 3 elected, time: 700
thread: 3 elected, time: 750
thread: 3 elected, time: 800
thread: 1 elected, time: 850
thread: 1 elected, time: 900
thread: 2 elected, time: 950
thread: 1 elected, time: 1000
thread: 1 elected, time: 1050
thread: 2 elected, time: 1100
thread: 2 elected, time: 1150
thread: 2 elected, time: 1200
thread: 2 elected, time: 1250
thread: 2 elected, time: 1300
thread: 3 elected, time: 1350
thread: 3 elected, time: 1400
thread: 3 elected, time: 1450
thread: 1 elected, time: 1500
thread: 1 elected, time: 1550
thread: 3 elected, time: 1600
thread: 3 elected, time: 1650
thread: 3 elected, time: 1700
thread: 3 elected, time: 1750
thread: 3 elected, time: 1800
thread: 2 elected, time: 1850
thread: 2 elected, time: 1900
thread: 2 elected, time: 1950
thread: 1 elected, time: 2000
thread: 1 elected, time: 2050
thread: 2 elected, time: 2100
thread: 2 elected, time: 2150
thread: 2 elected, time: 2200
thread: 3 elected, time: 2250
thread: 3 elected, time: 2300
thread: 3 elected, time: 2350
thread: 3 elected, time: 2400
thread: 3 elected, time: 2450
thread: 1 elected, time: 2500
thread: 1 elected, time: 2550
thread: 2 elected, time: 2600
thread: 2 elected, time: 2650
thread: 2 elected, time: 2700
thread: 2 elected, time: 2750
thread: 2 elected, time: 2800
thread: 2 elected, time: 2850
thread: 3 elected, time: 2900
thread: 3 elected, time: 2950
thread: 3 elected, time: 3000
thread: 1 elected, time: 3050
thread: 1 elected, time: 3100
thread: 3 elected, time: 3150
thread: 2 elected, time: 3200
thread: 2 elected, time: 3250
thread: 2 elected, time: 3300
thread: 2 elected, time: 3350
thread: 2 elected, time: 3400
thread: 2 elected, time: 3450
thread: 1 elected, time: 3500
thread: 1 elected, time: 3550
thread: 3 elected, time: 3600
thread: 3 elected, time: 3650
thread: 3 elected, time: 3700
thread: 3 elected, time: 3750
thread: 3 elected, time: 3800
thread: 3 elected, time: 3850
thread: 3 elected, time: 3900
thread: IDLE, time: 3950
thread: 1 elected, time: 4000
~~~
可以看到，使用抢占式EDF调度的情况下，除了最开始一个初始化period，后续的紧急任务可以及时完成，次优先任务也可以比较好的满足ddl。

4. 抢占式优先级调度
~~~
[P1] pok_sem_create return=0, mid=0
[P1] pok_thread_create (1) return=0
[P1] pok_thread_create (2) return=0
[P1] pok_thread_create (3) return=0
thread: 1 elected, time: 51
thread: 1 elected, time: 100
thread: 2 elected, time: 150
thread: 2 elected, time: 200
thread: 2 elected, time: 250
thread: 2 elected, time: 300
thread: 2 elected, time: 350
thread: 2 elected, time: 400
thread: 3 elected, time: 450
thread: 1 elected, time: 500
thread: 1 elected, time: 550
thread: 3 elected, time: 600
thread: 3 elected, time: 650
thread: 3 elected, time: 700
thread: 3 elected, time: 750
thread: 2 elected, time: 800
thread: 2 elected, time: 850
thread: 2 elected, time: 900
thread: 2 elected, time: 950
thread: 1 elected, time: 1000
thread: 1 elected, time: 1050
thread: 2 elected, time: 1100
thread: 2 elected, time: 1150
thread: 3 elected, time: 1200
thread: 3 elected, time: 1250
thread: 3 elected, time: 1300
thread: IDLE, time: 1350
thread: 3 elected, time: 1400
thread: 3 elected, time: 1450
thread: 1 elected, time: 1500
thread: 1 elected, time: 1550
thread: 2 elected, time: 1600
thread: 2 elected, time: 1650
thread: 2 elected, time: 1700
thread: 2 elected, time: 1750
thread: 2 elected, time: 1800
thread: 2 elected, time: 1850
thread: 3 elected, time: 1900
thread: 3 elected, time: 1950
thread: 1 elected, time: 2000
thread: 1 elected, time: 2050
thread: 3 elected, time: 2100
thread: 3 elected, time: 2150
thread: 3 elected, time: 2200
thread: 3 elected, time: 2250
thread: IDLE, time: 2300
thread: 3 elected, time: 2350
thread: 2 elected, time: 2400
thread: 2 elected, time: 2450
thread: 1 elected, time: 2500
thread: 1 elected, time: 2550
thread: 2 elected, time: 2600
thread: 2 elected, time: 2650
thread: 2 elected, time: 2700
thread: 2 elected, time: 2750
thread: 3 elected, time: 2800
thread: 3 elected, time: 2850
thread: 3 elected, time: 2900
thread: 3 elected, time: 2950
thread: 1 elected, time: 3000
thread: 1 elected, time: 3050
thread: 3 elected, time: 3100
thread: 3 elected, time: 3150
thread: 2 elected, time: 3200
thread: 2 elected, time: 3250
thread: 2 elected, time: 3300
thread: 2 elected, time: 3350
thread: 2 elected, time: 3400
thread: 2 elected, time: 3450
thread: 1 elected, time: 3500
thread: 1 elected, time: 3550
thread: 3 elected, time: 3600
thread: IDLE, time: 3650
thread: 3 elected, time: 3700
thread: 3 elected, time: 3750
thread: 3 elected, time: 3800
thread: 3 elected, time: 3850
thread: 3 elected, time: 3900
thread: 3 elected, time: 3950
thread: 1 elected, time: 4000
~~~
可以看到，由于紧急任务有着最高的优先级，所以总是能在开始后优先执行满足ddl要求，而次紧急任务也能得到相对优先的执行，总体在这种实际场景下有较好的表现。