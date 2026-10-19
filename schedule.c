#include <string.h>
#include "schedule.h"

typedef struct {
    int machine;
    int start;
    int end;
    int next;
} ScheduledInterval;

int evaluate_solution(const Instance *instance,
                      const int sequence[MAX_OPERATIONS],
                      Schedule *schedule) {
    const int n = instance->num_jobs;
    const int m = instance->num_machines;
    const int total = n * m;

    int next_operation[MAX_JOBS] = {0};
    int job_ready[MAX_JOBS] = {0};
    int machine_head[MAX_MACHINES];
    ScheduledInterval intervals[MAX_OPERATIONS];

    for (int machine = 0; machine < m; ++machine)
        machine_head[machine] = -1;

    memset(schedule, 0, sizeof(*schedule));

    for (int pos = 0; pos < total; ++pos) {
        int job = sequence[pos];

        if (job < 0 || job >= n || next_operation[job] >= m)
            return 0;

        int op_index = next_operation[job]++;
        const Operation *op = &instance->operations[job][op_index];
        int machine = op->machine;
        int duration = op->duration;
        int start = job_ready[job];

        /*
         * Place l'operation au premier intervalle libre de sa machine,
         * apres la fin de l'operation precedente de son job.
         */
        int changed;
        do {
            changed = 0;
            for (int idx = machine_head[machine]; idx != -1;
                 idx = intervals[idx].next) {
                if (start < intervals[idx].end &&
                    start + duration > intervals[idx].start) {
                    start = intervals[idx].end;
                    changed = 1;
                    break;
                }
            }
        } while (changed);

        int end = start + duration;
        intervals[pos].machine = machine;
        intervals[pos].start = start;
        intervals[pos].end = end;
        intervals[pos].next = machine_head[machine];
        machine_head[machine] = pos;

        schedule->start[job][op_index] = start;
        schedule->end[job][op_index] = end;
        job_ready[job] = end;

        if (end > schedule->makespan)
            schedule->makespan = end;
    }

    for (int job = 0; job < n; ++job) {
        if (next_operation[job] != m)
            return 0;
    }

    return 1;
}
