/*
 matrice.h
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
 * @brief Fichier destiné à gérer l'ensemble des opérations sur une matrice de données.
 */

#ifndef MATRICE_H
#define MATRICE_H
#include <stdbool.h>
#include <stdio.h>
#include "project_defines.h"

/**
 * @brief Structure représentative d'une Matrice de données, avec son compte de lignes et de colonnes.
 * Les données sont contenues sont la formes d'un tableau d'octets en deux dimensions.
 *
 */
struct Matrice {
    void** donnees; /**< Données sous une forme quelconque. */
    int nblignes; /**< Nombre de lignes du tableau */
    int nbcolonnes; /**< Nombre de colonnes du tableau */
};


/**
 * @brief Allocation d'une matrice vide en précisant le nombre de lignes et de colonnes.
 *
 * @param int Nombre de lignes à initialiser
 * @param int Nombre de colonnes à initialiser
 * @param size_t Taille d'un élément
 * @return Matrice Structure initialisée avec les informations rentrées en paramètres.
 * Les données sont pré-allouées mais aucun contenu n'est présent.
 */
struct Matrice matrice_alloc(int, int, size_t);
/**
 * @brief Initialise une structure Matrice à partir des informations contenues dans un fichier.
 *
 * @param filename Chemin vers le fichier contenant les données à charger
 * @return Matrice Structure contenant les données contenues par le fichier.
 */
struct Matrice matrice_init(char* filename);
/**
 * @brief Affiche le contenu de la matrice dans la sortie standard.
 *
 * @param Matrice Structure contenant des données à afficher.
 */
void matrice_print(struct Matrice);
/**
 * @brief Transpose le contenu d'une structure Matrice dans un fichier 'file', pour le recharger ultérieurement.
 * Si le fichier existe, il sera ecrasé. Sinon, il sera créé.
 *
 * @param Matrice Matrice à sauvegarder
 * @param file Chemin vers le fichier conteneur.
 */
void matrice_save(struct Matrice, char* file);
/**
 * @brief Fonction privée destinée à écrire le contenu d'une structure Matrice dans un pointeur FILE*.
 * Il est recommandé d'utiliser matrice_save au lieu de celle-ci.
 *
 * @param m Structure à enregistrer
 * @param fd Pointeur vers un fichier de destination
 * @return bool True si l'opération s'est bien déroulée, false sinon.
 */
bool matrice_save_into_file(struct Matrice* m, FILE* fd);
/**
 * @brief
 *
 * @param
 * @param
 * @return bool
 */
bool matrice_load_from_file(struct Matrice*, FILE*);


/**
 * @brief
 *
 * @param Matrice
 */
void matrice_free(struct Matrice);


#endif /* MATRICE_H */
