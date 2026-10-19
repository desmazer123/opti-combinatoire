#ifndef SCHEDULE_H
#define SCHEDULE_H

#include "instance.h"

typedef struct {
    int makespan;
    int start[MAX_JOBS][MAX_MACHINES];
    int end[MAX_JOBS][MAX_MACHINES];
} Schedule;

/*
 * La sequence contient num_jobs * num_machines identifiants de jobs.
 * Chaque job doit apparaitre exactement num_machines fois.
 * La k-ieme apparition d'un job correspond a sa k-ieme operation.
 */
int evaluate_solution(const Instance *instance,
                      const int sequence[MAX_OPERATIONS],
                      Schedule *schedule);

#endif
