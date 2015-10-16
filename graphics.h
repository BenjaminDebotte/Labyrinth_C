/*
  graphics.h
 Projet Labyrinthe ENSICAEN 1A
 Written by Benjamin Debotté
 
 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License as
 published by the Free Software Foundation; either version 2, or (at
 your option) any later version.
 
 This program is distributed in the hope that it will be useful, but
 WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 General Public License for more details.*/

/**
 * @file graphics.h
 *
 * @author Benjamin Debotté
 * @brief Fichier destiné à gérer l'ensemble des graphismes du jeu.
 */

#ifndef GRAPHICS_H
#define GRAPHICS_H

#define WALL_BASIC_NS_SYMBOL '|'
#define WALL_BASIC_EW_SYMBOL '-'
#define WALL_BASIC_NSEW_SYMBOL '+'

#define WALL_BASIC_SYMBOL   '+'
#define WALL_SOLID_SYMBOL   '#'
#define HERO_SYMBOL         '@'
#define EMPTY_SYMBOL        ' '

#define TRAP_SYMBOL         't'
#define ENEMY_BASIC_SYMBOL     '!'
#define ENEMY_FAST_SYMBOL      '!'
#define ENEMY_GHOST_SYMBOL      'g'


#define ITEM_LITTLE_SYMBOL      '*'
#define ITEM_AVG_SYMBOL         '$'
#define ITEM_BIG_SYMBOL         '$'

#define UNINITIALIZED_SYMBOL '?'


#include "labyrinthe.h"


/**
 * @brief Dessine le labyrinthe via la table de conversion des symboles graphiques.
 *
 * @param lab Labyrinthe à afficher à l'écran
 */
void graphics_labyrinthe_draw(Labyrinthe *lab);

/**
 * @brief Sous-fonction réalisant la conversion d'une cellule vers un symbole graphique ASCII.
 *
 * @param c Cellule à convertir.
 * @return char Caractère correspondant au contenu de la cellule.
 */
char graphics_labyrinthe_translate(Laby_Cell_str_t c);





#endif

