/*
  laby_game.c
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

#include "laby_game.h"
#include "project_defines.h"
#include "labyrinthe.h"
#include "graphics.h"
#include "gameplay.h"
#include <stdio.h>
#include "serialization.h"
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <time.h>



/* Variable globale du labyrinthe */
Labyrinthe lab;
Laby_Cell_str_t* PlayerCell = NULL;
bool Continue = true;
int computerMoves = 0;
char loadedLabyName[26];


int laby_game_menu_init(void){
    return laby_game_menu_handler();
}
int laby_game_menu_handler(void){
    unsigned char selectedIndex = 0;

    while(true) {
        system(CLEAR_CMD);
        printf("\n*********** Labyrinthe *************\n");
        printf("\n\t");
        if(selectedIndex == MENU_CREER_INDEX)
            printf(MENU_CREER_SELECT);
        else
            printf(MENU_CREER);
        printf("\n\t");
        if(selectedIndex == MENU_CHARGER_INDEX)
            printf(MENU_CHARGER_SELECT);
        else
            printf(MENU_CHARGER);
         printf("\n\t");
        if(selectedIndex == MENU_JOUER_INDEX)
            printf(MENU_JOUER_SELECT);
        else
            printf(MENU_JOUER);
         printf("\n\t");
        if(selectedIndex >= MENU_QUITTER_INDEX)
            printf(MENU_QUITTER_SELECT);
        else
            printf(MENU_QUITTER);
        printf("\n\n**********************************\n");

         printf("\n\tPress 'e' to select.\n");


        switch(getchar()) {
            case 'z':
                selectedIndex = (selectedIndex - 1) % MENU_ENTRIES;
                break;

            case 's':
                selectedIndex = (selectedIndex + 1) % MENU_ENTRIES;
                break;

            case 'e':
                switch (selectedIndex) {
                    case MENU_CREER_INDEX:
                        laby_game_init_options();
                        break;
                    case MENU_CHARGER_INDEX:
                        laby_game_loader_init();
                        break;
                    case MENU_JOUER_INDEX:
                        laby_game_init();
                        break;
                    case MENU_QUITTER_INDEX:
                        exit(1);
                }

            default:
                break;


        }

    }

    return SUCCESS;
}



/* Chargement */
int laby_game_loader_init(void) {
    char selectedLevel[33] = {0};
    char selectedLevelWithoutCfg[33] = {0};

    DIR *d;
    FILE *f = NULL;
    struct dirent *dir;

    do {
        int nbLvl = 0;
        d = opendir(".");
        if(!d)
            return ERROR_INVALID_BEHAVIOR;


        printf("\n Niveau existants : ");
        while ((dir = readdir(d)) != NULL){
            if(strstr(dir->d_name,".cfg") != NULL){
                printf("\n%s", dir->d_name);
                nbLvl++;
            }
        }

        closedir(d);

        if(nbLvl == 0)
        {printf("\nAucun niveau n'existe ! :(.\n"); usleep(1000000);return ERROR_INVALID_BEHAVIOR;}

        {
            char input[26] = {0};
            printf("\n\nVotre choix ? > ");
            scanf("%25s",input);
            /* L'utilisateur pourra entrer le nom avec ou sans extension. */
            sprintf(selectedLevel, "./%s.cfg",input);
            sprintf(selectedLevelWithoutCfg, "./%s",input);


            /* On conserve le nom du niveau chargé */
            sprintf(loadedLabyName, "%s",input);
        }
        f = fopen(selectedLevel,"r");
        if(f == NULL){
            f = fopen(selectedLevelWithoutCfg,"r");
            if(f == NULL)
                printf("\nChoix de niveau invalide.\n");
        }

    } while(f == NULL);

 ;

    {
        long fsize = 0;
        char *buffer = NULL;

        fseek(f, 0, SEEK_END);
        fsize = ftell(f);
        rewind(f);

        buffer = (char*)malloc(fsize*sizeof(char));

        fread(buffer, fsize, sizeof(uint8_t), f);

        fclose(f);
        lab = serialize_buffer_to_labyrinthe((BUFFER*)buffer);
    }


    if((computerMoves = labyrinthe_resolve(&lab)) == -1) {
        printf("\nErreur lors de la résolution du labyrinthe : la petite souris semble s'être perdue.\n\
               Veuillez nous en excusez : ceci n'impactera seulement le score.");
    }


    return SUCCESS;

}



/* Gestion du jeu */

void laby_game_init_options() {
    Laby_Config_str_t c;
    char inputNiveau[25] = {0};
    char niveau[33] = {0};

    system(CLEAR_CMD);
    do {
        FFLUSH();
        printf("Entrez la longueur du labyrinthe : ");
        scanf("%d", &c.longueur);
    } while(c.longueur < 5 || c.longueur % 2 == 0);

    do {
        FFLUSH();
        printf("\nEntrez la largeur du labyrinthe : ");
        scanf("%d",&c.largeur);
    } while(c.largeur < 5 || c.largeur % 2 == 0);

    do {
        FFLUSH();
        printf("\nEntre le taux de trésor dans le niveau [0,5] : ");
        scanf("%hu",&c.items_spawn_lvl);
    } while(c.items_spawn_lvl > 5 || c.items_spawn_lvl < 0);

    do {
        FFLUSH();
        printf("\nEntre le taux d'ennemis dans le niveau [0,5] : ");
        scanf("%hu",&c.enemy_spawn_lvl);
    } while(c.enemy_spawn_lvl > 5 || c.enemy_spawn_lvl < 0);

    do {
        FFLUSH();
        printf("\nEntre le taux de pièges dans le niveau [0,5] : ");
        scanf("%hu",&c.traps_spawn_lvl);
    } while(c.traps_spawn_lvl > 5 || c.traps_spawn_lvl < 0);

    printf("\nGénération en cours ...");
    labyrinthe_generate(c, &lab);

    printf("\nEntrez un nom pour le niveau (25 caractères max) : ");
    scanf("%24s",inputNiveau);
    strcpy(loadedLabyName, inputNiveau); /* On conserve le nom créé pour le prochain scoring s'il joue directement */

    sprintf(niveau,"./%s.cfg",inputNiveau);

    {
        FILE* file;
        BUFFER* buff;
        size_t sizeBuff;

        buff = serialize_labyrinthe_to_buffer(&lab,&sizeBuff);

        file = fopen(niveau, "w");
        fwrite(buff, sizeBuff, sizeof(uint8_t), file);
        fclose(file);

        lab = serialize_buffer_to_labyrinthe(buff);
    }
    if((computerMoves = labyrinthe_resolve(&lab)) == -1) {
        printf("\nErreur lors de la résolution du labyrinthe : la petite souris semble s'être perdue.\n\
               Veuillez nous en excusez : ceci n'impactera seulement le score.");
    }
}

int laby_game_init() {
    Laby_Entity_str_t hero;

    if(lab.donnees == NULL)
    {printf("\nVeuillez charger un niveau au préalable !\n");usleep(3000000);return ERROR_INVALID_BEHAVIOR;}

    hero.type = LABY_HERO;
    PlayerCell = &(((Laby_Cell_str_t**)lab.donnees))[1][0];
    PlayerCell->entity = &hero;

    laby_gameplay_init(); /* Initialisation des mécaniques de jeu */
    laby_game_cycle(); /* Lancement de la "boucle d'événement" */

    /* À ce niveau, la partie est terminée. */
    labyrinthe_free(&lab);

    return SUCCESS;
}
int laby_game_player_move(){
    int ret = SUCCESS;
    switch(laby_gameplay_player_input()) {
        case INPUT_UP:
            ret = laby_gameplay_move_up(PlayerCell);

            if(ret == SUCCESS)
                PlayerCell = PlayerCell->N;
            break;
        case INPUT_DOWN:
            ret = laby_gameplay_move_down(PlayerCell);

            if(ret == SUCCESS)
                PlayerCell = PlayerCell->S;
            break;
        case INPUT_LEFT:
            ret = laby_gameplay_move_left(PlayerCell);

            if(ret == SUCCESS)
                PlayerCell = PlayerCell->W;
            break;
        case INPUT_RIGHT:
            ret = laby_gameplay_move_right(PlayerCell);

            if(ret == SUCCESS)
                PlayerCell = PlayerCell->E;
            break;
        case INPUT_QUIT:
            Continue = false;
            break;
        case INPUT_SELECT:
            break;


    }

    return ret;
}

int laby_game_enemies_move() {
    int i,j;
    Laby_Cell_str_t** cells;



    cells = (Laby_Cell_str_t**)lab.donnees;

    /* On réinitialise les indicateurs de mouvements : */
    for(i = 0; i < lab.nblignes; i++)
        for(j = 0; j <lab.nbcolonnes; j++){
            if(cells[i][j].entity == NULL)
                continue;
            if(cells[i][j].entity->type != LABY_ENEMY)
                continue;

            cells[i][j].entity->param2 = (cells[i][j].entity->param2 & !ENTITY_HAS_MOVED_FLAG);
        }


    for(i = 0; i < lab.nblignes; i++)
        for(j = 0; j <lab.nbcolonnes; j++){
            int random;
            if(cells[i][j].entity == NULL)
                continue;
            if(cells[i][j].entity->type != LABY_ENEMY)
                continue;

            /* Comme on parcourt les cases de manières descendantes, cette protection permet d'éviter de déplacer 2 fois la même entité. */
            if(cells[i][j].entity->param2 & ENTITY_HAS_MOVED_FLAG){
                continue;
            }
            cells[i][j].entity->param2 |= ENTITY_HAS_MOVED_FLAG;

            random = RANDOM(1,4);
            switch(random){
                case 1:
                    laby_gameplay_move_right(&cells[i][j]);
                    break;
                case 2:
                    laby_gameplay_move_up(&cells[i][j]);
                    break;
                case 3:
                    laby_gameplay_move_left(&cells[i][j]);
                    break;
                case 4:
                    laby_gameplay_move_down(&cells[i][j]);
                    break;


            }

        }



    return SUCCESS;
}

int laby_game_player_check_death() {

    return SUCCESS;
}

void laby_game_end() {
    labyrinthe_free(&lab);
    exit(1);
}

void laby_game_graphics_draw() {
    system(CLEAR_CMD);
    printf("\n\t\t SCORE : %d",laby_gameplay_get_score());
    printf("\tMoves : %d",laby_gameplay_get_moves());

    printf("\tComputer Best : %d",computerMoves);

    printf("\n\n");
    graphics_labyrinthe_draw(&lab);
    printf("UP : '%c'\tDOWN : '%c'\nLEFT : '%c'\tRIGHT : '%c'\nEXIT : '%c'\n",INPUT_UP,INPUT_DOWN,INPUT_LEFT,INPUT_RIGHT,INPUT_QUIT);
}

bool laby_game_check_if_player_dead(void) {
    int i, j;
    Laby_Cell_str_t** cells;

    if(lab.donnees == NULL)
        return false;
    cells = (Laby_Cell_str_t**)lab.donnees;

    for(i = 0; i < lab.nblignes; i++)
        for(j = 0; j < lab.nbcolonnes; j++)
            if(cells[i][j].entity != NULL)
                if(cells[i][j].entity->type == LABY_HERO){
                    PRINTDF(("Héros vivant en (%d,%d)\n",i,j));
                    return false;
                }

    return true;
}

void laby_game_print_score(Player_Score_str_t Scoring_data) {
    uint32_t InitialScore = 0;

    system(CLEAR_CMD);
    InitialScore = laby_gameplay_get_score();
    printf("\n\n\t\tScore initial : ");
    usleep(300000);
    printf("%d pt.",InitialScore);
    usleep(400000);

    printf("\n\t\tScore de résolution : ");
    usleep(300000);
    printf("%d pt.",Scoring_data.score - InitialScore);
    printf("\n\n\t\tScore final : %d pt.",Scoring_data.score);
    printf("\n\n\n");
    usleep(1000000);

}



void laby_game_scoring_handling(void) {
    int i;
    char scoreFileName[35] = {0};
    Player_Score_str_t Scoring_data, **scores = NULL;
    /* Affichage du score final */
    laby_gameplay_calculate_score(computerMoves, &Scoring_data);
    laby_game_print_score(Scoring_data);

    /* Chargement des scores : s'il le fichier n'existe pas, on le crée et on le rempli.
     Sinon, on le charge et on regarde les scores.
     */
    sprintf(scoreFileName, "./%s.score",loadedLabyName);
    scores = score_manage(scoreFileName,Scoring_data);
    if(scores == NULL)
        return;
    
    /* On affiche ensuite les scores chargés  */
    {
        Player_Score_str_t* s;

        i = 0;
        while(1) {
            s = *(scores + i);
            if(s == NULL || i > 9)
                break;
            printf("%d - Joueur '%s' : %d (%d moves)\n",i+1,s->player,s->score,s->nbMoves);
            usleep(200000);
            free(scores[i]);
            i++;
        }
    }
    free(scores);
    
    FFLUSH();

    printf("\nAppuyez sur une touche pour continuer..\n");
    system(PAUSE_CMD);
}

int laby_game_cycle(){
    bool isNotDead = true;
    bool Win = false;
    Continue = true;

    /* Dans la boucle, les evenements à traiter sont :
        - Dessiner le labyrinthe
        - Verifier que le joueur n'a pas gagné
        - Laisser le choix au joueur de se déplacer
        - Vérifier que le joueur n'est pas mort
        - Déplacer les monstres
        - Vérifier qu'un monstre n'a pas tué le joueur
     */

    laby_game_graphics_draw();
    while(isNotDead && Continue) {
        if(laby_game_check_victory()) {Continue = false; Win = true;}

        /* On vérifie que le joueur ne soit pas mort suite à un déplacement malencontreux */
        isNotDead = (laby_game_player_move() != ERROR_HERO_DEAD);

        laby_game_enemies_move();

        /* On vérifie que le joueur ne soit pas mort suite à un déplacement ennemi */
        isNotDead = !laby_game_check_if_player_dead() && isNotDead;
        laby_game_graphics_draw();

    }

    if(!isNotDead) { /* Sortie par mort */
        printf("\nPerdu ! :(\n");
        usleep(3000000);
    }
    if(Win) { /* Victoire */
        printf("\nBRAVO !\n");
        usleep(2000000);
        laby_game_scoring_handling();
    }


    return SUCCESS;
}
bool laby_game_check_victory(){
    return (PlayerCell->param1 == LABY_EMPTY_TYPE_EXIT);

}

