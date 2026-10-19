#ifndef TABU_SEARCH_H
#define TABU_SEARCH_H

#include "schedule.h"

int tabu_search(const Instance *instance,
                int best_sequence[MAX_OPERATIONS],
                Schedule *best_schedule,
                double time_limit_seconds);

#endif
