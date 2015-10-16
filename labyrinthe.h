/*
    labyrinthe.h
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
 * @file labyrinthe.h
 *
 * @author Benjamin Debotté
 * @brief Fichier destiné à gérer l'ensemble des opérations nécessaires à la gestion en mémoire du labyrinthe.
 */

#include "matrice.h"
#include "project_defines.h"

#ifndef LABYRINTHE_H
#define LABYRINTHE_H


/* Elements du labyrinthe */
#define LABY_UNINITIALIZED  0x0

#define LABY_EMPTY          0x1
/* Type de sols */
    #define LABY_EMPTY_TYPE_ENTRANCE    0x1000
    #define LABY_EMPTY_TYPE_EXIT        0x2000


#define LABY_WALL           0x2
/* Types de mur */
#define LABY_WALL_TYPE_BASIC    0x0
#define LABY_WALL_TYPE_SOLID    0x1


#define LABY_ITEMS                          0x3
    #define LABY_ITEMS_TYPE_LITTLE_TREASURE         0x1
    #define LABY_ITEMS_TYPE_AVERAGE_TREASURE        0x2
    #define LABY_ITEMS_TYPE_BIG_TREASURE            0x3

#define LABY_HERO           0x4
#define LABY_TRAPS          0x5
#define LABY_ENEMY          0x6
    #define LABY_ENEMY_TYPE_BASIC   0x01
    #define LABY_ENEMY_TYPE_GHOST   0x02

/* Elements de configuration de la génération */
#define LABY_PERCENT_PROBABILITY_OF_ENTITY  5


#define LABY_GEN_ALL_VISITED 0x0F
#define LABY_GEN_VISITED_N  0x08
#define LABY_GEN_VISITED_S  0x04
#define LABY_GEN_VISITED_E  0x02
#define LABY_GEN_VISITED_W  0x01


#define LABY_GEN_REALLOC_SIZE 500
#define LABY_GEN_REALLOC_LIMIT  10 /* Valeur à partir de laquelle on ajoute LABY_GEN_REALLOC_SIZE éléments au tas allouée pour les différentes mécaniques de génération. */


/**
 * @brief Redéfinition du type Matrice en type Labyrinthe.
 *
 */
typedef struct Matrice Labyrinthe;

/**
 * @brief Structure représentant une entité dans le labyrinthe, contenant l'ensemble des données nécessaires à l'identifier.
 *
 */
typedef struct {
    uint8_t type; /**< Type de l'entité (Ennemi, Héros, ..) */
    uint16_t param1; /**< Données associées au type, utilisées pour l'identification plus fine et enrichir le gameplay */
    uint16_t param2; /**< Données associées au type (reservés pour des fins algorithmiques). */
/**
 * @brief Structure représentant une entité dans le labyrinthe, contenant l'ensemble des données nécessaires à l'identifier.
 *
 */
}Laby_Entity_str_t;

/**
 * @brief Structure représentative d'une cellule, pointant vers les cellules adjacentes et disposant d'informations
 * permettant d'identifier la cellule.
 *
 */
struct Laby_Cell_str{
    struct Laby_Cell_str *N, *S, *W, *E; /**< Pointeurs vers les cellules adjacentes */
    uint32_t line, col; /**< Ligne et colonne de la cellule dans la matrice  */
    uint8_t type; /**< Type de la cellule (Mur, sol, ..) */
    uint16_t param1; /**< Données associées au type, utilisées pour l'identification plus fine et enrichir le gameplay */
    uint16_t param2; /**< Données associées au type (reservés pour des fins algorithmiques). */
    Laby_Entity_str_t* entity; /**< Pointeur vers une entité. NULL si aucune entité n'est présente. */
};

/**
 * @brief Redéfinition du type Laby_Cell_str en Laby_Cell_str_t
 *
 */
typedef struct Laby_Cell_str Laby_Cell_str_t;


/**
 * @brief Éléments nécessaires à une configuration affinée du labyrinthe.
 * longueur et largeur correspondent aux dimensions du labyrinthe, tandis que les autres
 * champs permettent de mieux définir la répartition des différents éléments du labyrinthe.
 * Chaque champs peut contenir une valeur entre 0 et 5 inclus : un calcul aléatoire est réalisé à
 * la génération entre 1 et 8 pour les éléments suivants :
 * RANDOM(1,8) < traps_spawn_lvl -> on pose un piège
 * sinon si RANDOM(1,8) < itemps_spawn_lvl -> on pose un item
 * sinon si RANDOM(1,8) < itemps_spawn_lvl -> on pose une entité ennemie.
 * sinon on annule le placement
 *
 * La répartition des éléments dans le labyrinthe est d'une case sur 100.
 *
 */
typedef struct {
    /* Le type uint8_t était préférable, mais le format "hhu" pour la
     saisie avec scanf n'était pas nécessairement compatible en C89. */
    uint16_t traps_spawn_lvl; /**< Chance sur 5 de faire apparaitre un piège */
    uint16_t enemy_spawn_lvl; /**< Chance sur 5 de faire apparaitre un ennemi */
    uint16_t items_spawn_lvl; /**< Chance sur 5 de faire apparaitre un item */
    uint32_t longueur, largeur; /**< Largeur et longueur du labyrinthe */
} Laby_Config_str_t;

/**
 * @brief Fonction destinée à résoudre le labyrinthe selon un algorithme de parcours tel que :
 *          - À chaque intersection, la cellule est sauvegardée et le labyrinthe essaye d'aller à l'est, au sud, au nord puis à l'ouest.
 *          - À un cul-de-sac, il retourne à la dernière intersection visitée et essaye une autre direction
 *          - Arrivé à la fin, la fonction retourne le nombre de déplacemens nécessaires à la résolution
 *        Pour mémoriser les choix effectués par l'algorithme, le champ param2 de chaque cellule visitée est utilisé.
 *
 * @param lab Labyrinthe à résoudre
 * @return int Nombre de déplacements, -1 en cas d'erreur.
 */
int labyrinthe_resolve(Labyrinthe* lab);

/**
 * @brief Fonction réalisant l'allocation d'un labyrinthe en fonction d'une longueur et d'une largeur. Il est
 * préférable d'utiliser labyrinte_generate.
 *
 * @param length Longueur du labyrinthe, en nombre de cellules
 * @param width Largeur du labyrinthe, en nombre de cellules
 * @param lab Labyrinthe à initialiser
 * @return int SUCCESS en cas de réussite, sinon un code d'erreur.
 */
int labyrinthe_alloc(int length, int width, Labyrinthe* lab);

/**
 * @brief Charge un labyrinthe depuis un fichier
 *
 * @param filename Chemin vers le fichier à charger
 * @param lab Labyrinthe résultant
 * @return int SUCCESS en cas de réussite, sinon un code d'erreur.
 */
int labyrinthe_init(char* filename, Labyrinthe* lab);

/**
 * @brief Initialise et génère un labyrinthe en fonction de la configuration passée en paramètre.
 *
 * @param config Structure contenant les éléments de configuration de la génération.
 * @param lab Labyrinthe à initialiser.
 * @return int SUCCESS en cas de réussite, sinon un code d'erreur.
 */
int labyrinthe_generate(Laby_Config_str_t config ,Labyrinthe* lab);

/**
 * @brief Sous-fonction gérant la distribution des murs au sein du labyrinthe.
 *
 * @param lab Labyrinthe à initialiser
 * @return int SUCCESS en cas de réussite, sinon un code d'erreur.
 */
int labyrinthe_build_walls(Labyrinthe *lab);

/**
 * @brief Sous-fonction gérant la distribution des entités au sein du labyrinthe.
 *
 * @param lab Labyrinthe à initialiser
 * @param config Structure contenant les valeurs de répartitions de la génération.
 * @return int SUCCESS en cas de réussite, sinon un code d'erreur.
 */
int labyrinthe_build_entities(Labyrinthe* lab, Laby_Config_str_t config);

/**
 * @brief Libère la mémoire utilisée par un labyrinthe.
 *
 * @param lab Labyrinthe à désinitialiser.
 */
void labyrinthe_free(Labyrinthe* lab);

/**
 * @brief Fonction permettant d'échanger les propriétés de deux cellules.
 *
 * @param src Cellule source
 * @param dst Cellule de destination
 * @return int SUCCESS en cas de réussite, sinon un code d'erreur.
 */
int laby_swap_cells_properties(Laby_Cell_str_t* src, Laby_Cell_str_t* dst);


#endif





