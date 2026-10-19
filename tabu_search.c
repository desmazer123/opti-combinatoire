#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "tabu_search.h"

#define CANDIDATES_PER_ITERATION 80
#define TABU_TENURE_MIN 7
#define TABU_TENURE_MAX 15

static double elapsed_seconds(const struct timespec *start) {
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    return (double)(now.tv_sec - start->tv_sec) +
           (double)(now.tv_nsec - start->tv_nsec) / 1e9;
}

static void copy_sequence(int *dest, const int *src, int length) {
    memcpy(dest, src, (size_t)length * sizeof(int));
}

static void make_initial_solution(const Instance *instance, int *sequence) {
    int length = instance->num_jobs * instance->num_machines;
    int p = 0;

    for (int job = 0; job < instance->num_jobs; ++job) {
        for (int k = 0; k < instance->num_machines; ++k)
            sequence[p++] = job;
    }

    /* Fisher-Yates shuffle. */
    for (int i = length - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        int tmp = sequence[i];
        sequence[i] = sequence[j];
        sequence[j] = tmp;
    }
}

int tabu_search(const Instance *instance,
                int best_sequence[MAX_OPERATIONS],
                Schedule *best_schedule,
                double time_limit_seconds) {
    const int length = instance->num_jobs * instance->num_machines;
    int current[MAX_OPERATIONS];
    int candidate[MAX_OPERATIONS];
    int best_any_sequence[MAX_OPERATIONS];
    int best_allowed_sequence[MAX_OPERATIONS];
    Schedule current_schedule;
    Schedule candidate_schedule;
    Schedule best_any_schedule;
    Schedule best_allowed_schedule;
    int iteration = 0;

    /*
     * Tabu memory is sized to the current instance, not to MAX_OPERATIONS^2.
     * An entry (i,j) records when swapping these two positions becomes allowed.
     */
    int *tabu_until = calloc((size_t)length * (size_t)length, sizeof(int));
    if (tabu_until == NULL) {
        fprintf(stderr, "Erreur : allocation de la memoire tabou impossible.\n");
        return 0;
    }

    make_initial_solution(instance, current);

    if (!evaluate_solution(instance, current, &current_schedule)) {
        fprintf(stderr, "Erreur : impossible d'evaluer la solution initiale.\n");
        free(tabu_until);
        return 0;
    }

    copy_sequence(best_sequence, current, length);
    *best_schedule = current_schedule;

    struct timespec start_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);

    while (elapsed_seconds(&start_time) < time_limit_seconds) {
        int found_allowed = 0;
        int found_any = 0;
        int best_allowed_cost = 0;
        int best_any_cost = 0;
        int allowed_i = -1, allowed_j = -1;
        int any_i = -1, any_j = -1;

        for (int c = 0; c < CANDIDATES_PER_ITERATION; ++c) {
            if (elapsed_seconds(&start_time) >= time_limit_seconds)
                break;

            int i = rand() % length;
            int j = rand() % length;
            if (i == j || current[i] == current[j])
                continue;
            if (i > j) {
                int tmp = i;
                i = j;
                j = tmp;
            }

            copy_sequence(candidate, current, length);
            int tmp = candidate[i];
            candidate[i] = candidate[j];
            candidate[j] = tmp;

            if (!evaluate_solution(instance, candidate, &candidate_schedule))
                continue;

            int cost = candidate_schedule.makespan;
            int is_tabu = tabu_until[(size_t)i * length + j] > iteration;
            int aspiration = cost < best_schedule->makespan;

            if (!found_any || cost < best_any_cost) {
                found_any = 1;
                best_any_cost = cost;
                copy_sequence(best_any_sequence, candidate, length);
                best_any_schedule = candidate_schedule;
                any_i = i;
                any_j = j;
            }

            if ((!is_tabu || aspiration) &&
                (!found_allowed || cost < best_allowed_cost)) {
                found_allowed = 1;
                best_allowed_cost = cost;
                copy_sequence(best_allowed_sequence, candidate, length);
                best_allowed_schedule = candidate_schedule;
                allowed_i = i;
                allowed_j = j;
            }
        }

        if (!found_any)
            continue;

        /*
         * Prefer the best admissible neighbour. If all sampled moves are tabu,
         * use the best sampled neighbour to avoid getting stuck.
         */
        if (found_allowed) {
            copy_sequence(current, best_allowed_sequence, length);
            current_schedule = best_allowed_schedule;
            tabu_until[(size_t)allowed_i * length + allowed_j] =
                iteration + TABU_TENURE_MIN +
                rand() % (TABU_TENURE_MAX - TABU_TENURE_MIN + 1);
        } else {
            copy_sequence(current, best_any_sequence, length);
            current_schedule = best_any_schedule;
            tabu_until[(size_t)any_i * length + any_j] =
                iteration + TABU_TENURE_MIN +
                rand() % (TABU_TENURE_MAX - TABU_TENURE_MIN + 1);
        }

        if (current_schedule.makespan < best_schedule->makespan) {
            copy_sequence(best_sequence, current, length);
            *best_schedule = current_schedule;
        }

        ++iteration;
    }

    free(tabu_until);
    fprintf(stderr, "Iterations realisees : %d\n", iteration);
    return 1;
}
