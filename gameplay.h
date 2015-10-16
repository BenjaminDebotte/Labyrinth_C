/*
  gameplay.h
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

/**
 * @file gameplay.h
 *
 * @author Benjamin Debotté
 * @brief Fichier destiné à gérer l'ensemble des mécaniques de gameplay.
 */

#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include "labyrinthe.h"

#define INPUT_NONE '\0'
#define INPUT_UP  'z'
#define INPUT_DOWN 's'
#define INPUT_LEFT 'q'
#define INPUT_RIGHT 'd'
#define INPUT_SELECT 'e'
#define INPUT_QUIT 'x'


#define ERROR_INVALID_SOURCE            0x1000
#define ERROR_INVALID_DEST              0x1001
#define ERROR_HERO_DEAD                0x1002
#define ERROR_TRAP_DEAD                 0x1003



#define SCORE_FOR_EACH_MOVE_BETTER_THAN_COMP    100
#define SCORE_FOR_EACH_MOVE_WORST_THAN_COMP     20 /* Ce score sera soustrait au score total*/


/**
 * @brief Structure représentant un résultat de score d'un joueur.
 *
 */
typedef struct {
    char player[21]; /**< Nom du joueur */
    uint32_t score; /**< Score */
    uint32_t nbMoves; /**< Nombre de mouvements nécessaires à la complétion du labyrinthe. */

} Player_Score_str_t;




/**
 * @brief Fonction gérant le mouvement d'une entité présente dans src vers une cellule dst.
 *
 * @param src Cellule source contenant l'entité à déplacer
 * @param dst Cellule de destination
 * @return int SUCCESS si le déplacement s'est effectué correctement, sinon un code d'erreur (ex : ERROR_INVALID_XXXX).
 */
int laby_gameplay_move(Laby_Cell_str_t *src, Laby_Cell_str_t* dst);

/**
 * @brief Fonction permettant de déplacer l'entité présente dans src vers la droite.
 *
 * @param src Cellule contenant l'entité à déplacer
 * @return int SUCCESS si le déplacement s'est effectué correctement, sinon un code d'erreur (ex : ERROR_INVALID_XXXX).
 */
int laby_gameplay_move_right(Laby_Cell_str_t *src);
/**
 * @brief Fonction permettant de déplacer l'entité présente dans src vers la gauche.
 *
 * @param src Cellule contenant l'entité à déplacer
 * @return int SUCCESS si le déplacement s'est effectué correctement, sinon un code d'erreur (ex : ERROR_INVALID_XXXX).
 */
int laby_gameplay_move_left(Laby_Cell_str_t *src);
/**
 * @brief Fonction permettant de déplacer l'entité présente dans src vers le haut.
 *
 * @param src Cellule contenant l'entité à déplacer
 * @return int SUCCESS si le déplacement s'est effectué correctement, sinon un code d'erreur (ex : ERROR_INVALID_XXXX).
 */
int laby_gameplay_move_up(Laby_Cell_str_t *src);
/**
 * @brief Fonction permettant de déplacer l'entité présente dans src vers le bas.
 *
 * @param src Cellule contenant l'entité à déplacer
 * @return int SUCCESS si le déplacement s'est effectué correctement, sinon un code d'erreur (ex : ERROR_INVALID_XXXX).
 */
int laby_gameplay_move_down(Laby_Cell_str_t *src);



/**
 * @brief Sous-fonction destinée à gérer le déplacement d'un fantôme.
 * Un fantôme a la particularité de pouvoir traverser les murs.
 *
 * @param src Cellule source
 * @param dst Cellule destination
 * @return int SUCCESS si le déplacement s'est effectué correctement, sinon un code d'erreur (ex : ERROR_INVALID_XXXX).
 */
int laby_gameplay_move_ghost(Laby_Cell_str_t *src, Laby_Cell_str_t* dst);

/**
 * @brief Sous-fonction destinée à gérer le déplacement d'un fantôme.
 * Un ennemi a la particularité de vouloir attraper le héros à portée.
 *
 * @param src Cellule source
 * @param dst Cellule destination
 * @return int SUCCESS si le déplacement s'est effectué correctement, sinon un code d'erreur (ex : ERROR_INVALID_XXXX).
 */
int laby_gameplay_move_basic_enemy(Laby_Cell_str_t *src, Laby_Cell_str_t* dst);

/**
 * @brief Sous-fonction destinée à gérer le déplacement d'un fantôme.
 * Le héros à la particularité de pouvoir ramasser certains objets.
 *
 * @param src Cellule source
 * @param dst Cellule destination
 * @return int SUCCESS si le déplacement s'est effectué correctement, sinon un code d'erreur (ex : ERROR_INVALID_XXXX).
 */
int laby_gameplay_move_hero(Laby_Cell_str_t *src, Laby_Cell_str_t* dst);

/**
 * @brief Fonction réalisant le calcul du score final en fonction de son déplacement.
 * Le joueur gagne 100 points par déplacement au delà de ce qu'avait fait l'ordinateur ou perd
 * 20 points pour chaque déplacement en dessous.
 *
 * @param computerMoves Score de référence, en l'occurence celui de l'ordinateur.
 * @param player Structure score qui sera remplie par la fonction
 * @return int SUCCESS si le calcul s'est bien effectué, sinon un code d'erreur.
 */
int laby_gameplay_calculate_score(uint32_t computerMoves, Player_Score_str_t *player);

/**
 * @brief Retourne le score actuel du joueur
 *
 * @return uint32_t Score
 */
uint32_t laby_gameplay_get_score(void);
/**
 * @brief Retourne le nombre de mouvements du joueur
 *
 * @return uint32_t Nombre de mouvements.
 */
uint32_t laby_gameplay_get_moves(void);

/**
 * @brief Fonction destinée à attendre et recueillir l'entrée de l'utilisateur.
 *
 * @return char Caractère entré par l'utilisateur.
 */
char laby_gameplay_player_input();
/**
 * @brief Initialise ou réinitialise les mécaniques de gameplay.
 *
 */
void laby_gameplay_init();


#endif

