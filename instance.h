#ifndef INSTANCE_H
#define INSTANCE_H

#define MAX_JOBS 100
#define MAX_MACHINES 100
#define MAX_OPERATIONS (MAX_JOBS * MAX_MACHINES)

typedef struct {
    int machine;
    int duration;
} Operation;

typedef struct {
    char name[100];
    int num_jobs;
    int num_machines;
    Operation operations[MAX_JOBS][MAX_MACHINES];
} Instance;

int load_instance(const char *filename, Instance *instance);

#endif
