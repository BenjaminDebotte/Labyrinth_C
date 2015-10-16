/*
  graphics.c
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

#include "graphics.h"
#include <stdlib.h>



void graphics_labyrinthe_draw(Labyrinthe *lab) {
    char *Draw = NULL;
    int i,j;

    if(lab == NULL)
        return;
    if(lab->donnees == NULL)
        return;

    Draw = (char*)malloc((lab->nbcolonnes + 1)*sizeof(char));
    if(Draw == NULL)
        return;
    for(i = 0; i < lab->nblignes; i++){
        for(j = 0; j < lab->nbcolonnes; j++){
            Draw[j] = graphics_labyrinthe_translate(((Laby_Cell_str_t**)lab->donnees)[i][j]);
        }
        Draw[j+1] = '\0';
        printf("\t%s\n",Draw);
    }

    printf("\n\n");
    free(Draw);


}

char graphics_labyrinthe_translate(Laby_Cell_str_t c) {

    switch(c.type) {
            case LABY_EMPTY:
                if(c.entity == NULL)
                    return c.param2 != 0 ? (c.param2 + '0' > 0x39 ? c.param2 - 10 + 'A' : c.param2 + '0') : EMPTY_SYMBOL;
                switch (c.entity->type) {
                    case LABY_ENEMY:
                        if(c.entity->param1 == LABY_ENEMY_TYPE_BASIC)
                            return ENEMY_BASIC_SYMBOL;
                        else if(c.entity->param1 == LABY_ENEMY_TYPE_GHOST)
                            return ENEMY_GHOST_SYMBOL;
                        return UNINITIALIZED_SYMBOL;

                    case LABY_TRAPS:
                        return TRAP_SYMBOL;
                    case LABY_HERO:
                        return HERO_SYMBOL;
                    case LABY_ITEMS:
                        if(c.entity->param1 == LABY_ITEMS_TYPE_LITTLE_TREASURE)
                            return ITEM_LITTLE_SYMBOL;
                        else if (c.entity->param1 == LABY_ITEMS_TYPE_AVERAGE_TREASURE)
                            return ITEM_AVG_SYMBOL;
                        return ITEM_BIG_SYMBOL;
                    default:
                        return UNINITIALIZED_SYMBOL;
                }
            case LABY_WALL:
                if(c.param1 == LABY_WALL_TYPE_SOLID)
                    return WALL_SOLID_SYMBOL;
                if(c.E != NULL && c.N != NULL && c.S != NULL && c.W != NULL){
                    if(c.E->type == LABY_WALL && c.N->type == LABY_WALL && c.S->type == LABY_WALL && c.W->type == LABY_WALL)
                        return WALL_BASIC_NSEW_SYMBOL;
                }
                if(c.E != NULL && c.W != NULL){
                    if(c.E->type == LABY_WALL && c.W->type == LABY_WALL)
                        return WALL_BASIC_EW_SYMBOL;
                }
                if(c.N != NULL && c.S != NULL){
                    if(c.N->type == LABY_WALL && c.S->type == LABY_WALL)
                        return WALL_BASIC_NS_SYMBOL;
                }
                return WALL_BASIC_NSEW_SYMBOL;


            default:
                return UNINITIALIZED_SYMBOL;
    }


}
