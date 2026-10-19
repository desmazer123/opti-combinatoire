#ifndef OUTPUT_H
#define OUTPUT_H

#include "schedule.h"

int write_json(const char *filename,
               const Instance *instance,
               const Schedule *schedule);

#endif
