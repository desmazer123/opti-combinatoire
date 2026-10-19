# Recherche tabou — Job Shop Scheduling

## Compilation (Ubuntu)

```bash
make
```

Ou sans Makefile :

```bash
gcc -O2 -std=c11 -Wall -Wextra -pedantic \
  main.c instance.c schedule.c tabu_search.c output.c \
  -o recherche_tabou
```

## Exécution

Depuis le dossier du projet :

```bash
./recherche_tabou instances/20_10
```

Par défaut, la recherche dure 55 secondes pour rester sous la limite d'une minute.
Vous pouvez choisir une autre durée, jusqu'à 59 secondes :

```bash
./recherche_tabou instances/20_10 30
```

Le programme crée `20_10.json` dans le dossier courant.

## Format d'entrée

Première ligne : nom de l'instance, par exemple `20_10`.
Deuxième ligne : nombre de jobs et nombre de machines.
Puis une ligne par job, avec les paires `machine durée` pour chaque opération.

## Important

La recherche tabou est une heuristique : elle ne garantit pas l'optimum.
Le programme génère une solution initiale aléatoire, teste des échanges dans une
séquence de jobs et conserve le meilleur makespan rencontré. Il construit un
planning faisable en respectant l'ordre des opérations d'un job et l'exclusivité
des machines.
