/*
  labyrinthe.c
 Projet Labyrinthe ENSICAEN 1A
 Written by Benjamin Debotté
 
 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License as
 published by the Free Software Foundation; either version 2, or (at
 your option) any later version.
 
 This program is distributed in the hope that it will be useful, but
 WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 General Public License for more details.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "labyrinthe.h"


void print_info(Laby_Cell_str_t s){
    printf("\nCellule %p (%d,%d) : \nN : %p\nS : %p\nE : %p\nW : %p\n",&s,s.line,s.col,s.N,s.S,s.E,s.W);
    printf("Param1 : %X\n",s.param1);
    printf("Param2 : %X\n",s.param2);
    printf("Type : ");
    switch(s.type) {
        case LABY_EMPTY:
            printf("EMPTY");
            break;
        case LABY_WALL:
            printf("LABY_WALL");
            break;
        case LABY_UNINITIALIZED:
            printf("LABY_UNINITIALIZED");
            break;

        default:
            printf("UNKNOWN");
            break;
    }
    printf("\n\n");
}

int labyrinthe_alloc(int length, int width, Labyrinthe* lab) {
    Labyrinthe laby;
    int line,col;
    Laby_Cell_str_t** data;



    if(lab == NULL)
        return ERROR_PARAM_NULL;
    if(length <= 0 || width <= 0)
        return ERROR_INVALID_PARAM;
    if(length % 2 == 0 || width % 2 == 0)
        return ERROR_INVALID_PARAM;

    /* On alloue la matrice avec les entiers de 32 bits non signés. */
    laby = matrice_alloc(length, width,sizeof(Laby_Cell_str_t));
    if(laby.donnees == NULL)
        return ERROR_INVALID_BEHAVIOR;

    laby.nbcolonnes = width;
    laby.nblignes = length;


    data = ((Laby_Cell_str_t**)laby.donnees);

    /* On initialise le labyrinthe avec des cases non initialisées (pas de paramètres). */
    for(line = 0; line < laby.nblignes; line++)
        for(col = 0; col < laby.nbcolonnes; col++){


            data[line][col].type = LABY_UNINITIALIZED;
            data[line][col].param1 = 0;
            data[line][col].param2 = 0;
            data[line][col].line = line;
            data[line][col].col = col;
            data[line][col].entity = NULL;

            /* On initialise les pointeurs vers les cases environnantes */
            data[line][col].N = (line-1 >= 0 ? &data[line-1][col] : NULL);
            data[line][col].S = (line+1 <= laby.nblignes-1 ? &data[line+1][col] : NULL);
            data[line][col].W = (col-1 >= 0 ? &data[line][col-1] : NULL);
            data[line][col].E = (col+1 <= laby.nbcolonnes-1 ? &data[line][col+1] : NULL);
        }




    *lab = laby;
    return SUCCESS;
}

int labyrinthe_init(char* filename, Labyrinthe* lab) {
    Labyrinthe laby;

    if(filename == NULL || lab == NULL)
        return ERROR_PARAM_NULL;

    laby = matrice_init(filename);

    if(laby.donnees == NULL)
        return ERROR_INVALID_BEHAVIOR;

    *lab = laby;
    return SUCCESS;
}




int labyrinthe_build_walls(Labyrinthe* lab) {
    int line, col,i, totalCells = 0, visitedCells = 0;
    uint16_t ID = 0;
    Laby_Cell_str_t** cells;
    Laby_Cell_str_t cell;
    Laby_Cell_str_t* visitedCellsArray = NULL;
    int cptVisitedCells = 0, currentSizeArray = LABY_GEN_REALLOC_SIZE;

    /* Algorithme de génération :
        0 - On génère la base du labyrinthe
        1 - Générer des îlots, assigner un ID à param1
        2 - Tirer un ID au hasard, se déplacer à cette cellule
        3 - Casser les murs pour générer un labyrinthe parfait
     */


    if(lab == NULL)
        return ERROR_PARAM_NULL;

    cells = (Laby_Cell_str_t**)lab->donnees;
    if(cells == NULL)
        return ERROR_PARAM_NULL;

    /* On monte les murs */
    for(line = 0; line < lab->nblignes; line ++ )
        for(col = 0; col < lab->nbcolonnes; col ++){
            cells[line][col].type = LABY_WALL;
        }

    for(line = 0; line < lab->nblignes; line++)
    {cells[line][0].param1 = LABY_WALL_TYPE_SOLID; cells[line][lab->nbcolonnes-1].param1 = LABY_WALL_TYPE_SOLID;}
    for(col = 0; col < lab->nbcolonnes; col++)
    {cells[0][col].param1 = LABY_WALL_TYPE_SOLID; cells[lab->nblignes-1][col].param1 = LABY_WALL_TYPE_SOLID;}


    totalCells = (lab->nbcolonnes - 2) * (lab->nblignes - 2);

    srand((uint32_t)time(NULL));

    for(line = 1; line < lab->nblignes - 1; line += 2 )
        for(col = 1; col < lab->nbcolonnes - 1; col += 2){
            cells[line][col].type = LABY_EMPTY;
            cells[line][col].param1 = ++ID;
            PRINTDF(("ID Assigné à la case (%d,%d) : %d\n",line,col,cells[line][col].param1));
        }

    /* On choisit un point de départ aléatoire */

    do { /* Recherche d'une case avec un ID pour démarrer */
        line = RANDOM(1, lab->nblignes-1);
        col = RANDOM(1, lab->nbcolonnes-1);
        cell = cells[line][col];
    } while(cell.type != LABY_EMPTY);

    PRINTDF(("Depart de la génération (%d,%d, ID=%d)\n",line,col,cell.param1));

    /* On préalloue pour stocker les cellules parcourues */
    visitedCellsArray = (Laby_Cell_str_t*)malloc(LABY_GEN_REALLOC_SIZE*sizeof(Laby_Cell_str_t));



    while( visitedCells < totalCells - 1 ) {

        /* On adapte l'espace mémoire alloué */
        if(currentSizeArray - cptVisitedCells < LABY_GEN_REALLOC_LIMIT){
            Laby_Cell_str_t* newPtr = NULL;
            newPtr = (Laby_Cell_str_t*)malloc(( currentSizeArray + LABY_GEN_REALLOC_SIZE ) * sizeof(Laby_Cell_str_t));
            if(newPtr == NULL)
                exit(1);

            /* Problème avec realloc sur Mac OS X : on détourne via malloc/memcpy/free */
            memcpy(newPtr, visitedCellsArray, cptVisitedCells*sizeof(Laby_Cell_str_t));
            free(visitedCellsArray);
            visitedCellsArray = newPtr;

            currentSizeArray += LABY_GEN_REALLOC_SIZE;
        }

        if(cell.param2 == LABY_GEN_ALL_VISITED){
            if(cptVisitedCells == 0)
                break;                 /* Tout a été parcouru et toutes les cellules ont été vérifiées : labyrinthe prêt */

            cell = visitedCellsArray[--cptVisitedCells];
            PRINTDF(("\ndepop. (%d) : ",cptVisitedCells));
        }



        for(i = RANDOM(1, 4); i <= 4; i++) {
            Laby_Cell_str_t *cellTarget = NULL, *cellInter = NULL;
            switch (i) {

                case 1:
                    cell.param2 |= LABY_GEN_VISITED_N;
                    cellInter = cell.N;
                    if(cellInter == NULL)
                        continue;
                    cellTarget = cellInter->N;
                    if(cell.param2 & LABY_GEN_VISITED_N) /* Déjà visitée */
                    { PRINTDF(("Cellule Nord déjà visitée.\n")); break;}
                    continue;

                case 2:
                    cell.param2 |= LABY_GEN_VISITED_S;
                    cellInter = cell.S;
                    if(cellInter == NULL)
                        continue;
                    cellTarget = cellInter->S;
                    if(cell.param2 & LABY_GEN_VISITED_S) /* Déjà visitée */
                    { PRINTDF(("Cellule Sud déjà visitée.\n")); break;}
                    continue;

                case 3:
                    cell.param2 |= LABY_GEN_VISITED_E;
                    cellInter = cell.E;
                    if(cellInter == NULL)
                        continue;
                    cellTarget = cellInter->E;
                    if(cell.param2 & LABY_GEN_VISITED_E) /* Déjà visitée */
                    { PRINTDF(("Cellule Est déjà visitée.\n")); break;}
                    continue;
                case 4:
                    cell.param2 |= LABY_GEN_VISITED_W;
                    cellInter = cell.W;
                    if(cellInter == NULL)
                        continue;
                    cellTarget = cellInter->W;
                    if(cell.param2 & LABY_GEN_VISITED_W) /* Déjà visitée */
                    { PRINTDF(("Cellule Ouest déjà visitée.\n")); break;}
                    continue;

            }


            if(cellTarget== NULL)
                break;
            if(cellTarget->param1 == cell.param1)
                break;

            /* Arrivé ici, la case n'est pas reliée à notre case actuelle */
            PRINTDF(("[DEBUG] - Merge entre les cases (%d,%d,ID=%d) et (%d,%d,ID=%d)\n",
                    cell.line,cell.col,cell.param1,
                    cellTarget->line, cellTarget->col, cellTarget->param1));

            /* Case intermédiaire */
            cellInter->type = LABY_EMPTY;
            cellInter->param1 = cell.param1;

            /* Case cible */
            cellTarget->param1 = cell.param1;


            visitedCellsArray[cptVisitedCells] = cell;
            cptVisitedCells++; visitedCells+=3;
            cell = *(cellTarget);

        }
    }

    /* On créé les entrée/sortie */
    cells[1][0].type = LABY_EMPTY;
    cells[1][0].param1 = LABY_EMPTY_TYPE_ENTRANCE;

    cells[lab->nblignes -2][lab->nbcolonnes-1].type = LABY_EMPTY;
    cells[lab->nblignes -2][lab->nbcolonnes-1].param1 = LABY_EMPTY_TYPE_EXIT;


    PRINTDF(("\n\nCréation de chemins alternatifs\n"));
    /* On casse des murs pour créer des passages supplémentaires */
    for(line = 1; line < lab->nblignes - 1; line ++ )
        for(col = 1; col < lab->nbcolonnes - 1; col ++){
            if(cells[line][col].type != LABY_WALL)
                continue;
            /* Sur 10 murs, on décide d'en descendre 1 */
            if(RANDOM(0,10) > 1)
                continue;

            PRINTDF(("Mur descendu en (%d,%d)\n",col,line));
            cells[line][col].type = LABY_EMPTY;
            cells[line][col].param1 = 0;
            cells[line][col].param2 = 0;

        }

    free(visitedCellsArray);
    return SUCCESS;
}

int labyrinthe_build_entities(Labyrinthe* lab, Laby_Config_str_t c) {
    Laby_Cell_str_t **cells;
    int i, j;

    if(lab == NULL)
        return ERROR_PARAM_NULL;

    cells = (Laby_Cell_str_t**)lab->donnees;
    if(cells == NULL)
        return ERROR_PARAM_NULL;

    srand((uint32_t)time(NULL));



    for(i = 1; i < lab->nblignes - 1; i++)
        for(j = 1; j < lab->nbcolonnes - 1; j++){
            int random;

            if(cells[i][j].type != LABY_EMPTY)
                continue;
            if(cells[i][j].entity != NULL)
                continue;

            /* Un cul de sac a une chance sur 4 de contenir un Fast, sinon un moyen/gros trésor. */
            if( (cells[i][j].N->type == LABY_WALL) + (cells[i][j].E->type == LABY_WALL) + (cells[i][j].S->type == LABY_WALL) + (cells[i][j].W->type == LABY_WALL) == 3)
            {
                cells[i][j].entity = malloc(sizeof(Laby_Entity_str_t));
                cells[i][j].entity->type = (RANDOM(3, 7) >= 6 ? LABY_ENEMY : LABY_ITEMS);
                if(cells[i][j].entity->type == LABY_ENEMY)
                    cells[i][j].entity->param1 = ( RANDOM(3, 6) <= 4 ? LABY_ENEMY_TYPE_GHOST : LABY_ENEMY_TYPE_BASIC);
                else
                    cells[i][j].entity->param1 = ( RANDOM(3, 6) <= 4 ? LABY_ITEMS_TYPE_BIG_TREASURE : LABY_ITEMS_TYPE_AVERAGE_TREASURE);
            }

            /* Système de répartition des objets :
                LABY_PERCENT_PROBABILITY_OF_ENTITY % de chance qu'une entité apparaisse
                On tient ensuite compte des choix de l'utilisateur concernant la répartition
                des différentes possibilités
             */
            random = RANDOM(1,100);
            if(random > LABY_PERCENT_PROBABILITY_OF_ENTITY)
                continue;

            cells[i][j].entity = (Laby_Entity_str_t*)malloc(sizeof(Laby_Entity_str_t));


            PRINTDF(("Génération d'une entité : "));

            if(RANDOM(1,8) <= c.items_spawn_lvl){
                cells[i][j].entity->type = LABY_ITEMS;
                cells[i][j].entity->param1 = LABY_ITEMS_TYPE_LITTLE_TREASURE;
                PRINTDF(("Placement d'un item en (%d,%d)\n",i,j));
            }
            else if(RANDOM(1,8) <= c.traps_spawn_lvl) {
                cells[i][j].entity->type = LABY_TRAPS;
                PRINTDF(("Placement d'un piège en (%d,%d)\n",i,j));
            }
            else if(RANDOM(1,8) <= c.enemy_spawn_lvl) {
                cells[i][j].entity->type = LABY_ENEMY;
                if(RANDOM(1,4) == 3){
                    cells[i][j].entity->param1 = LABY_ENEMY_TYPE_GHOST;
                    PRINTDF(("Placement d'un fantôme en (%d,%d)\n",i,j));
                }
                else {
                    cells[i][j].entity->param1 = LABY_ENEMY_TYPE_BASIC;
                    PRINTDF(("Placement d'un monstre en (%d,%d)\n",i,j));
                }
            }
            else {
                free(cells[i][j].entity);
                cells[i][j].entity = NULL;
            }

        }

    return SUCCESS;
}

int labyrinthe_generate(Laby_Config_str_t config ,Labyrinthe* lab) {
    labyrinthe_alloc(config.longueur, config.largeur, lab);

    labyrinthe_build_walls(lab);
    labyrinthe_build_entities(lab,config);

    return SUCCESS;
}



void labyrinthe_free(Labyrinthe* lab) {
    int i,j;

    if(lab == NULL)
        return;
    if(lab->donnees == NULL)
        return;

    for(i = 1; i < lab->nblignes -1; i++)
        for (j = 1; j < lab->nbcolonnes - 1; j++)
            if(((Laby_Cell_str_t**)lab->donnees)[i][j].entity != NULL)
                if(((Laby_Cell_str_t**)lab->donnees)[i][j].entity->type != LABY_HERO){
                    free(((Laby_Cell_str_t**)lab->donnees)[i][j].entity);
                    ((Laby_Cell_str_t**)lab->donnees)[i][j].entity = NULL;
                }



    matrice_free(*lab);
    lab->donnees = NULL;
    lab->nbcolonnes = 0;
    lab->nblignes = 0;
}


int labyrinthe_resolve(Labyrinthe* lab) {
    int i,j,cptIntersectionCells = 0, currentIntersectionCellsSize = LABY_GEN_REALLOC_SIZE;
    int numberOfCellsToResolution = 1; /* On passe la case de départ */
    Laby_Cell_str_t** cells = NULL, **intersectionCells = NULL, *cell = NULL;

    /* On part de la cellule de démarrage + 1 en (1,1). On parcourt le labyrinthe, en gardant en mémoire chaque intersection. Si on tombe sur un cul de sac, on repart depuis la dernière intersection en changeant de direction jusqu'à arriver à la fin.

        Cet algorithme permet de poser les bases du scoring : l'algorithme n'est pas parfait et effectue de nombreux
        retour en arrière. Un "bon score" correspondra donc au nombre de cases parcouru par celui-ci.
        On ne compte pas le nombre de cases nécessaire à un retour en arrière
        que l'algorithme effectue directement, les joueurs les plus hardcore y trouveront leur compte via un scoring
        plus exigent.
     */

    if(lab == NULL)
        return ERROR_PARAM_NULL;
    if(lab->donnees == NULL)
        return ERROR_PARAM_NULL;

    cells = (Laby_Cell_str_t**)lab->donnees;
    intersectionCells = (Laby_Cell_str_t**)malloc(LABY_GEN_REALLOC_SIZE*sizeof(Laby_Cell_str_t*));
    cell = &cells[1][1]; /* Cellule de départ */

    /* On réinitialise param2, qui sera utilisé pour mémoriser quelle direction a déjà été prise */
    for(i = 0; i < lab->nblignes; i++)
        for(j = 0; j < lab->nbcolonnes; j++)
            if(cells[i][j].type == LABY_EMPTY)
                cells[i][j].param2 = 0;


    while(1) {
        int nbChemins;
        if(cptIntersectionCells + LABY_GEN_REALLOC_LIMIT >= currentIntersectionCellsSize){
            Laby_Cell_str_t** newPtr = NULL;
            newPtr = (Laby_Cell_str_t**)malloc(( currentIntersectionCellsSize + LABY_GEN_REALLOC_SIZE ) * sizeof(Laby_Cell_str_t*));
            if(newPtr == NULL)
                exit(1);

            /* Problème avec realloc sur Mac OS X : on détourne via malloc/memcpy/free */
            memcpy(newPtr, intersectionCells, cptIntersectionCells*sizeof(Laby_Cell_str_t));
            free(intersectionCells);
            intersectionCells = newPtr;

            currentIntersectionCellsSize += LABY_GEN_REALLOC_SIZE;
        }

        /* Dans un chemin basique, 3 cas sont dissociés :
            - 1 seul chemin : cul de sac, on retourne à la dernière intersection
            - 2 ouvertures : c'est un chemin, on continue.
            - 3 et + : intersection, on mémorise et on essaye tous les chemins. */
        nbChemins = 0;
        nbChemins += cell->E != NULL ? cell->E->type == LABY_EMPTY : 0;
        nbChemins += cell->S != NULL ? cell->S->type == LABY_EMPTY : 0;
        nbChemins += cell->W != NULL ? cell->W->type == LABY_EMPTY : 0;
        nbChemins += cell->N != NULL ? cell->N->type == LABY_EMPTY : 0;

        if(nbChemins > 2){

            /* On suit un ordre défini de parcours : E,S,N,W . E et S sont les plus probables à tester en premiers pour avancer vers le "bas-droite" du labyrinthe.
               Plutôt que de manipuler de nombreux if imbriqués, ou perdre des itérations avec des if/else if, j'utilise des goto une fois une case exploitable trouvée. */

            if(cell->param2 == LABY_GEN_ALL_VISITED){
                if(cptIntersectionCells > 0){
                    cell = intersectionCells[--cptIntersectionCells];
                }
                else {
                    free(intersectionCells);
                    return -1;
                }

            }
            if(!(cell->param2 & LABY_GEN_VISITED_E)){
                intersectionCells[cptIntersectionCells++] = cell;
                cell->param2 |= LABY_GEN_VISITED_E;
                if(cell->E != NULL)
                    if(cell->E->type == LABY_EMPTY){
                        cell = cell->E;
                        cell->param2 |= LABY_GEN_VISITED_W;
                        goto advance;
                    }
            }
            if(!(cell->param2 & LABY_GEN_VISITED_S)){
                intersectionCells[cptIntersectionCells++] = cell;
                cell->param2 |= LABY_GEN_VISITED_S;
                if(cell->S != NULL)
                    if(cell->S->type == LABY_EMPTY){
                        cell = cell->S;
                        cell->param2 |= LABY_GEN_VISITED_N;
                        goto advance;
                    }
            }
            if(!(cell->param2 & LABY_GEN_VISITED_N)){
                intersectionCells[cptIntersectionCells++] = cell;
                cell->param2 |= LABY_GEN_VISITED_N;
                if(cell->N != NULL)
                    if(cell->N->type == LABY_EMPTY){
                        cell = cell->N;
                        cell->param2 |= LABY_GEN_VISITED_S;
                        goto advance;
                    }
            }
            if(!(cell->param2 & LABY_GEN_VISITED_W)){
                intersectionCells[cptIntersectionCells++] = cell;
                cell->param2 |= LABY_GEN_VISITED_W;
                if(cell->W != NULL)
                    if(cell->W->type == LABY_EMPTY){
                        cell = cell->W;
                        cell->param2 |= LABY_GEN_VISITED_E;
                        goto advance;
                    }
            }

        }
        else if(nbChemins == 1) { /* Cul de sac */
            if(cptIntersectionCells > 0){
                cell->param2 = LABY_GEN_ALL_VISITED;
                cell = intersectionCells[--cptIntersectionCells];
            }
            else {
                free(intersectionCells);
                return -1;
            }
        }
        else { /* Chemin standard */
            if(cell->E != NULL && ((cell->param2 & LABY_GEN_VISITED_E) == 0)){
                    cell->param2 |= LABY_GEN_VISITED_E;
                    if(cell->E->type == LABY_EMPTY) {
                        cell = cell->E;
                        cell->param2 |= LABY_GEN_VISITED_W;
                        goto advance;
                    }
            }
            if(cell->S != NULL && ((cell->param2 & LABY_GEN_VISITED_S) == 0)){
                cell->param2 |= LABY_GEN_VISITED_S;
                if(cell->S->type == LABY_EMPTY) {
                    cell = cell->S;
                    cell->param2 |= LABY_GEN_VISITED_N;
                    goto advance;
                }
            }
            if(cell->N != NULL && ((cell->param2 & LABY_GEN_VISITED_N) == 0)){
                cell->param2 |= LABY_GEN_VISITED_N;
                if(cell->N->type == LABY_EMPTY) {
                    cell = cell->N;
                    cell->param2 |= LABY_GEN_VISITED_S;
                    goto advance;

                }
            }
            if(cell->W != NULL && ((cell->param2 & LABY_GEN_VISITED_W) == 0)){
                cell->param2 |= LABY_GEN_VISITED_W;
                if(cell->W->type == LABY_EMPTY) {
                    cell = cell->W;
                    cell->param2 |= LABY_GEN_VISITED_E;
                    goto advance;

                }
            }
        }

    /* Une cellule de destination a été trouvée */
    advance:
        numberOfCellsToResolution++;
        if(cell->param1 == LABY_EMPTY_TYPE_EXIT){
            break;
        }

    }

    /* On réinitialise param2 à nouveau */
    for(i = 0; i < lab->nblignes; i++)
        for(j = 0; j < lab->nbcolonnes; j++)
            if(cells[i][j].type == LABY_EMPTY)
                cells[i][j].param2 = 0;

    free(intersectionCells);
    return numberOfCellsToResolution;
}


int laby_swap_cells_properties(Laby_Cell_str_t* src, Laby_Cell_str_t* dst){
    Laby_Cell_str_t tmp;

    if(src == NULL || dst == NULL)
        return ERROR_PARAM_NULL;

    tmp = *src;
    src->param1 = dst->param1;
    src->param2 = dst->param2;
    src->type = dst->type;

    dst->param1 = tmp.param1;
    dst->param2 = tmp.param2;
    dst->type = tmp.type;

    return SUCCESS;
}


