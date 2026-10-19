#include <stdio.h>
#include "output.h"

int write_json(const char *filename,
               const Instance *instance,
               const Schedule *schedule) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror(filename);
        return 0;
    }

    fprintf(file, "{\n  \"makespan\": %d,\n  \"jobs_schedule\": {\n",
            schedule->makespan);

    for (int job = 0; job < instance->num_jobs; ++job) {
        fprintf(file, "    \"Job_%d\": [\n", job);

        for (int op = 0; op < instance->num_machines; ++op) {
            const Operation *operation = &instance->operations[job][op];
            fprintf(file,
                    "      {\"machine\": %d, \"start\": %d, \"end\": %d}%s\n",
                    operation->machine,
                    schedule->start[job][op],
                    schedule->end[job][op],
                    (op + 1 < instance->num_machines) ? "," : "");
        }

        fprintf(file, "    ]%s\n",
                (job + 1 < instance->num_jobs) ? "," : "");
    }

    fprintf(file, "  }\n}\n");
    fclose(file);
    return 1;
}
