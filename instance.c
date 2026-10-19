#include <stdio.h>
#include "instance.h"

int load_instance(const char *filename, Instance *instance) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror(filename);
        return 0;
    }

    if (fscanf(file, "%99s", instance->name) != 1 ||
        fscanf(file, "%d %d", &instance->num_jobs,
               &instance->num_machines) != 2) {
        fprintf(stderr, "Erreur : en-tete d'instance invalide.\n");
        fclose(file);
        return 0;
    }

    if (instance->num_jobs < 1 || instance->num_jobs > MAX_JOBS ||
        instance->num_machines < 1 ||
        instance->num_machines > MAX_MACHINES) {
        fprintf(stderr, "Erreur : dimensions hors limites (%d jobs, %d machines).\n",
                instance->num_jobs, instance->num_machines);
        fclose(file);
        return 0;
    }

    for (int j = 0; j < instance->num_jobs; ++j) {
        for (int o = 0; o < instance->num_machines; ++o) {
            Operation *op = &instance->operations[j][o];

            if (fscanf(file, "%d %d", &op->machine, &op->duration) != 2) {
                fprintf(stderr, "Erreur de lecture au job %d, operation %d.\n", j, o);
                fclose(file);
                return 0;
            }

            if (op->machine < 0 || op->machine >= instance->num_machines ||
                op->duration <= 0) {
                fprintf(stderr,
                        "Operation invalide au job %d, operation %d "
                        "(machine=%d, duree=%d).\n",
                        j, o, op->machine, op->duration);
                fclose(file);
                return 0;
            }
        }
    }

    fclose(file);
    return 1;
}
