

from bcc import BPF
import sys
import time
import os

# Load the eBPF program from the C source code
bpf_source = """
#include <uapi/linux/bpf.h>
#include <linux/ptrace.h>
#include <linux/sched.h>
#include <linux/bpf.h>

struct syscall_event {
    u32 pid;
    u32 syscall_id;
};

BPF_HASH(syscalls_map, struct syscall_event, u64);

TRACEPOINT_PROBE(syscalls, sys_enter_read) {
    struct syscall_event event = {};
    event.pid = bpf_get_current_pid_tgid() >> 32;
    event.syscall_id = 0;  // read syscall ID
    u64 ts = bpf_ktime_get_ns();
    syscalls_map.update(&event, &ts);
    return 0;
}

TRACEPOINT_PROBE(syscalls, sys_enter_write) {
    struct syscall_event event = {};
    event.pid = bpf_get_current_pid_tgid() >> 32;
    event.syscall_id = 1;  // write syscall ID
    u64 ts = bpf_ktime_get_ns();
    syscalls_map.update(&event, &ts);
    return 0;
}

TRACEPOINT_PROBE(syscalls, sys_enter_open) {
    struct syscall_event event = {};
    event.pid = bpf_get_current_pid_tgid() >> 32;
    event.syscall_id = 2;  // open syscall ID
    u64 ts = bpf_ktime_get_ns();
    syscalls_map.update(&event, &ts);
    return 0;
}

TRACEPOINT_PROBE(syscalls, sys_enter_close) {
    struct syscall_event event = {};
    event.pid = bpf_get_current_pid_tgid() >> 32;
    event.syscall_id = 3;  // close syscall ID
    u64 ts = bpf_ktime_get_ns();
    syscalls_map.update(&event, &ts);
    return 0;
}

TRACEPOINT_PROBE(syscalls, sys_enter_execve) {
    struct syscall_event event = {};
    event.pid = bpf_get_current_pid_tgid() >> 32;
    event.syscall_id = 59;  // execve syscall ID
    u64 ts = bpf_ktime_get_ns();
    syscalls_map.update(&event, &ts);
    return 0;
}

TRACEPOINT_PROBE(syscalls, sys_enter_nanosleep) {
    struct syscall_event event = {};
    event.pid = bpf_get_current_pid_tgid() >> 32;
    event.syscall_id = 35;  // nanosleep syscall ID
    u64 ts = bpf_ktime_get_ns();
    syscalls_map.update(&event, &ts);
    return 0;
}
"""

# Create BPF object and load the program
b = BPF(text=bpf_source)

# Dictionary to map syscall IDs to names
syscall_names = {
    0: "read",
    1: "write",
    2: "open",
    3: "close",
    35: "nanosleep",
    59: "execve"
}

# Specify the process name to monitor
process_name = sys.argv[1] if len(sys.argv) > 1 else "abdur"

def get_pid_by_name(name):
    for line in os.popen("pgrep -f " + name).read().strip().split("\n"):
        return int(line)
    return None

# Print headers
print(f"Tracing system calls for process: {process_name}... Ctrl-C to end.")
print(f"{'PID':<8}{'P_NAME':<12}{'SYSCALL':<12}{'TIMESTAMP'}")

target_pid = get_pid_by_name(process_name)
if target_pid is None:
    print(f"Process '{process_name}' not found. Please start the process and try again.")
    sys.exit(1)

print(f"Monitoring PID: {target_pid}")

def print_syscalls():
    while True:
        time.sleep(1)
        try:
            # Dictionary to store the latest timestamp for each syscall
            latest_syscalls = {}
            
            # Get the syscalls
            for k, v in b["syscalls_map"].items():
                if k.pid == target_pid:
                    syscall_id = k.syscall_id
                    # Only update if this is a more recent call of this syscall type
                    if syscall_id not in latest_syscalls or v.value > latest_syscalls[syscall_id][1]:
                        latest_syscalls[syscall_id] = (k.pid, v.value)
            
            # Print the syscalls
            for syscall_id, (pid, timestamp) in latest_syscalls.items():
                syscall_name = syscall_names.get(syscall_id, f"Unknown({syscall_id})")
                human_time = time.strftime('%H:%M:%S')
                print(f"{pid:<8}{process_name:<12}{syscall_name:<12}{human_time}")
                
        except KeyboardInterrupt:
            print("\nDetaching...")
            return

try:
    print_syscalls()
except KeyboardInterrupt:
    print("Detaching...")



