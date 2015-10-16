/*
  gameplay.c
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
#include <stdlib.h>
#include "gameplay.h"


uint32_t Score = 0;
uint32_t NbMoves = 0;


int laby_gameplay_calculate_score(uint32_t computerMoves, Player_Score_str_t *player){
    char c;
    if(player == NULL)
        return ERROR_PARAM_NULL;
    
    do {c = getchar();} while (c != '\n' && c != EOF); /* Remplace fflush */
    printf("Entrez votre nom de joueur : ");
    scanf("%20s",player->player);
    player->player[20] = 0;
    
    player->nbMoves = laby_gameplay_get_moves();
    if(player->nbMoves < computerMoves)
        player->score = laby_gameplay_get_score() + (computerMoves - player->nbMoves)*SCORE_FOR_EACH_MOVE_BETTER_THAN_COMP;
    else {/* Attention à ne pas descendre sous 0 : Sinon le joueur ferait le meilleur score de tout les temps. */
        int calculatedMalus;
        
        calculatedMalus = (player->nbMoves - computerMoves)*SCORE_FOR_EACH_MOVE_WORST_THAN_COMP;
        player->score = laby_gameplay_get_score() - ( calculatedMalus > laby_gameplay_get_score() ? laby_gameplay_get_score() :  calculatedMalus);
    }
    return SUCCESS;
}

int laby_gameplay_move_basic_enemy(Laby_Cell_str_t *src, Laby_Cell_str_t* dst) {
    if(src == NULL)
        return ERROR_PARAM_NULL;
    if(src->entity == NULL)
        return ERROR_PARAM_NULL;
    
    if(dst->type == LABY_WALL)
        return ERROR_INVALID_DEST;
    
    /* Trésor ? On campe salement.*/
    if(src->N->entity != NULL && src->N->entity->type == LABY_ITEMS)
        return SUCCESS;
    else if(src->S->entity != NULL && src->S->entity->type == LABY_ITEMS)
        return SUCCESS;
    else if(src->W->entity != NULL && src->W->entity->type == LABY_ITEMS)
        return SUCCESS;
    else if(src->E->entity != NULL && src->E->entity->type == LABY_ITEMS)
        return SUCCESS;
    
    /* Joueur ? CHOPPEZ LE. */
    if(src->N->entity != NULL && src->N->entity->type == LABY_HERO){
        src->N->entity = src->entity;
        src->entity = NULL;
    }
    else if(src->S->entity != NULL && src->S->entity->type == LABY_HERO){
        src->S->entity = src->entity;
        src->entity = NULL;
    }
    else if(src->W->entity != NULL && src->W->entity->type == LABY_HERO){
        src->W->entity = src->entity;
        src->entity = NULL;
    }
    else if(src->E->entity != NULL && src->E->entity->type == LABY_HERO){
        src->E->entity = src->entity;
        src->entity = NULL;
    }
    
    /* Déplacement basique. */
    if(dst->entity == NULL) {
        dst->entity = src->entity;
        src->entity = NULL;
    }
    return SUCCESS;
}
int laby_gameplay_move_ghost(Laby_Cell_str_t *src, Laby_Cell_str_t* dst) {
    if(src == NULL)
        return ERROR_PARAM_NULL;
    if(src->entity == NULL)
        return ERROR_PARAM_NULL;
    
    if(dst->entity == NULL || (dst->entity != NULL && dst->entity->type == LABY_HERO)) {
        dst->entity = src->entity;
        src->entity = NULL;
    }
    return SUCCESS;
}

int laby_gameplay_move_hero(Laby_Cell_str_t *src, Laby_Cell_str_t* dst) {
    if(src == NULL)
        return ERROR_PARAM_NULL;
    if(src->entity == NULL)
        return ERROR_PARAM_NULL;
    
    if(dst->type == LABY_WALL)
        return ERROR_INVALID_DEST;
    
    if(dst->entity != NULL) {
        switch (dst->entity->type) {
            case LABY_TRAPS:
                Score = ((int)Score - 500 < 0 ? 0 : Score - 500);
                free(dst->entity);
                dst->entity = NULL;
                break;
            case LABY_ENEMY:
                return ERROR_HERO_DEAD;
            case LABY_ITEMS:
                Score += 500*dst->entity->param1; /* 500 à 1500pt selon la nature */
                free(dst->entity);
                dst->entity = NULL;
                break;
                
            default:
                break;
        }
        
    }
    dst->entity = src->entity;
    src->entity = NULL;
    NbMoves++;
    return SUCCESS;
}

int laby_gameplay_move(Laby_Cell_str_t *src, Laby_Cell_str_t* dst) {
    int (*ptrFunc)(Laby_Cell_str_t*,Laby_Cell_str_t*) = NULL; /* Pointeur vers la fonction de déplacement */
    
    if(src == NULL || dst == NULL)
        return ERROR_PARAM_NULL;
    if(src->entity == NULL)
        return ERROR_PARAM_NULL;
    
    /* Table des déplacements. Une entité absente est une entité immobile. */
    switch (src->entity->type) {
        case LABY_HERO:
            ptrFunc = laby_gameplay_move_hero;
            break;
        case LABY_ENEMY:
            switch (src->entity->param1) {
                case LABY_ENEMY_TYPE_BASIC:
                    ptrFunc = laby_gameplay_move_basic_enemy;
                    break;
                case LABY_ENEMY_TYPE_GHOST:
                    ptrFunc = laby_gameplay_move_ghost;
                    break;
        }
    }
    
    if(ptrFunc == NULL)
        return ERROR_INVALID_BEHAVIOR;
    return ptrFunc(src,dst);
}

int laby_gameplay_move_right(Laby_Cell_str_t *src){
    return laby_gameplay_move(src,src->E);
}
int laby_gameplay_move_left(Laby_Cell_str_t *src){
    return laby_gameplay_move(src,src->W);

}
int laby_gameplay_move_up(Laby_Cell_str_t *src){
    return laby_gameplay_move(src,src->N);

}
int laby_gameplay_move_down(Laby_Cell_str_t *src){
    return laby_gameplay_move(src,src->S);

}

void laby_gameplay_init(){
    /* Score initial : 5000pt */
    Score = 5000;
    NbMoves = 0;
    
}

uint32_t laby_gameplay_get_moves(void) {
    return NbMoves;
}

uint32_t laby_gameplay_get_score(void) {
    return Score;
}


char laby_gameplay_player_input() {
    char c = INPUT_NONE;

    FFLUSH();
    scanf("%c",&c);
    return c;
}


