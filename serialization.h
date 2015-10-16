/*
  serialization.h
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
 * @file serialization.h
 *
 * @author Benjamin Debotté
 * @brief Fichier destiné à gérer l'ensemble des opérations d'entrées/sorties sur
 * le système de fichier.
 */

#ifndef SERIALIZATION_H
#define SERIALIZATION_H

#include "labyrinthe.h"
#include "gameplay.h"




#define MAGIC_BYTE  0xBE

/**
 * @brief Redéfinition du type de donnée "void" en BUFFER, plus explicite.
 *
 */
typedef void BUFFER;


/*  1 - Longueur * Largeur
    2 - labyrinthe_alloc(longueur, largeur) 
    3 - Remplir chaque case avec type, param1, param2 et entity si existant. 
    La structure contient des pointeurs, il faut lire les données : on ne peut stocker la structure directement.
 */


/* Global : <(1)Magic_byte><(4)lignes><(4)colonnes><(10)case(0,0)><(10)case(0,1)>...<(10)case(longueur-1,largeur-1)> */
/* Cellule avec entité : <(1)type><(2)param1><(2)param2><(1)type><(2)param1><(2)param2> */
/* Cellule sans entité : <(1)type><(2)param1><(2)param2><0x00><0x0000><0x0000> */

/**
 * @brief Fonction réalisant la conversion d'un labyrinthe vers un buffer de donnée prêt à enregistrer dans un fichier.
 *
 * @param l Labyrinthe à enregistrer
 * @param s Taille du buffer résultant, retournée dans la variable s
 * @return BUFFER Pointeur vers le buffer contenant les données, NULL en cas d'erreur.
 */
BUFFER* serialize_labyrinthe_to_buffer(Labyrinthe* l, size_t* s);
/**
 * @brief Sous-fonction réalisant la sérialisation d'une cellule vers un BUFFER. Par convéniance, la fonction
 * prend en entrée un BUFFER* initial et retourne un autre pointeur vers ce même buffer, pointant après la dernière donnée ajoutée.
 * 10 octets seront stockés :   Cellule avec entité : <(1)type><(2)param1><(2)param2><(1)type><(2)param1><(2)param2>
 *                              Cellule sans entité : <(1)type><(2)param1><(2)param2><0x00><0x0000><0x0000>
 *
 * @param dst BUFFER dans lequel stocker les données de la cellule cell
 * @param cell Cellule dont les données
 * @return BUFFER Pointeur correspondant à dst décallé de 10 octets.
 */
BUFFER* serialize_cell(BUFFER* dst, const Laby_Cell_str_t *cell);
/**
 * @brief Permet de reconstruire un labyrinthe à partir d'un BUFFER de données.
 *
 * @param Pointeurs vers les données du labyrinthe
 * @return Labyrinthe Labyrinthe reconsitué.
 */
Labyrinthe serialize_buffer_to_labyrinthe(BUFFER*);


/* Gestion des scores (I/O dans fichier) */
/**
 * @brief Fonction permettant de gérer l'enregistrement d'un nouveau score dans un fichier.
 * La limite imposée est de 10 scores par fichier. Le score le plus faible sera toujours remplacé
 * par un score plus élevé. La fonction retournera ensuite l'ensemble des scores, triés par ordre décroissants.
 *
 * @param scoreFileName Nom du fichier de score avec extension
 * @param Scoring_data Nouveau score à enregistrer
 * @return Player_Score_str_t Scores chargés par la fonction, triés par ordre décroissants.
 */
Player_Score_str_t** score_manage(char* scoreFileName, Player_Score_str_t Scoring_data);

#endif
