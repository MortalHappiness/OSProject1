# Convert output to unit time format

import sys

# ========================================

# Calculate unit time
with open("../output/TIME_MEASUREMENT_dmesg.txt") as fin:
    exec_times = list()
    for line in fin:
        start, end = [float(x) for x in line.split()[2:]]
        exec_times.append(end - start)
    unit_time = sum(exec_times) / len(exec_times) / 500

print("Unit time: %.3f nsec" % (unit_time*(10**9)))

pid_to_name = dict()
with open(sys.argv[1][:-9] + "stdout.txt") as fin:
    for line in fin:
        name, pid = line.split()
        pid_to_name[pid] = name

dmesg_log = list()
with open(sys.argv[1]) as fin:
    for line in fin:
        _, pid, start, end = line.split()
        name = pid_to_name[pid]
        start = float(start)
        end = float(end)
        dmesg_log.append((name, start, end))

# sort by start time
dmesg_log.sort(key = lambda x: x[1])
# use the start time of the first process as zero time
zero_time = dmesg_log[0][1]

for log in dmesg_log:
    print("%s %.2f %.2f" % (log[0],
                            (log[1]-zero_time)/unit_time,
                            (log[2]-zero_time)/unit_time
                            ))

