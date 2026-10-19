#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "instance.h"
#include "schedule.h"
#include "tabu_search.h"
#include "output.h"

int main(int argc, char *argv[]) {
    if (argc < 2 || argc > 3) {
        fprintf(stderr, "Usage : %s <fichier_instance> [temps_en_secondes]\n",
                argv[0]);
        fprintf(stderr, "Exemple : %s instances/20_10 55\n", argv[0]);
        return EXIT_FAILURE;
    }

    double time_limit = 55.0;
    if (argc == 3) {
        char *end = NULL;
        time_limit = strtod(argv[2], &end);
        if (end == argv[2] || *end != '\0' ||
            time_limit <= 0.0 || time_limit > 59.0) {
            fprintf(stderr, "Le temps doit etre entre 0 et 59 secondes.\n");
            return EXIT_FAILURE;
        }
    }

    Instance instance;
    if (!load_instance(argv[1], &instance))
        return EXIT_FAILURE;

    printf("Instance : %s (%d jobs, %d machines)\n",
           instance.name, instance.num_jobs, instance.num_machines);
    printf("Recherche tabou pendant %.1f secondes...\n", time_limit);

    srand((unsigned int)time(NULL));

    int best_sequence[MAX_OPERATIONS];
    Schedule best_schedule;

    if (!tabu_search(&instance, best_sequence, &best_schedule, time_limit))
        return EXIT_FAILURE;

    char output_filename[160];
    if (snprintf(output_filename, sizeof(output_filename), "%s.json",
                 instance.name) >= (int)sizeof(output_filename)) {
        fprintf(stderr, "Nom de fichier de sortie trop long.\n");
        return EXIT_FAILURE;
    }

    if (!write_json(output_filename, &instance, &best_schedule))
        return EXIT_FAILURE;

    printf("Meilleur makespan : %d\n", best_schedule.makespan);
    printf("Fichier cree : %s\n", output_filename);

    return EXIT_SUCCESS;
}
