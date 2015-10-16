/*
  laby_game.h
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
 * @file laby_game.h
 *
 * @author Benjamin Debotté
 * @brief Fichier destiné à rassembler l'ensemble des mécaniques pour obtenir
 * le produit final : le jeu Labyrinthe.
 */

#ifndef LABY_GAME_H
#define LABY_GAME_H

#include <stdbool.h>

#define MENU_ENTRIES 4
#define MENU_CREER  "Creer"
#define MENU_CHARGER  "Charger"
#define MENU_JOUER  "Jouer"
#define MENU_QUITTER  "Quitter"

#define MENU_CREER_INDEX  0
#define MENU_CHARGER_INDEX  1
#define MENU_JOUER_INDEX  2
#define MENU_QUITTER_INDEX  3

#define MENU_CREER_SELECT  "> Creer <"
#define MENU_CHARGER_SELECT  "> Charger <"
#define MENU_JOUER_SELECT  "> Jouer <"
#define MENU_QUITTER_SELECT  "> Quitter <"


#define ENTITY_HAS_MOVED_FLAG   0x8000 /* Indicateur de mouvement */


/* Gestion du menu */
/**
 * @brief Initialise le menu principal
 *
 * @return int SUCCESS en cas de réussite, sinon un code d'erreur.
 */
int laby_game_menu_init(void);
/**
 * @brief Gère le menu principal, et la navigation en son sein.
 *
 * @return int SUCCESS en cas de réussite, sinon un code d'erreur.
 */
int laby_game_menu_handler(void);

/* Chargement */
/**
 * @brief Gère le menu de chargement des niveaux.
 *
 * @return int SUCCESS en cas de réussite, sinon un code d'erreur.
 */
int laby_game_loader_init(void);


/* Créer */
/**
 * @brief Gère la saisie des options nécessaires à la génération d'un niveau.
 *
 */
void laby_game_init_options(void);


/* Assemblage des fonctionnalités graphiques / gameplay du jeu */
/**
 * @brief Initialise la partie "jeu" : mise en place du labyrinthe et des graphismes, association avec les mécaniques de gameplay.
 *
 * @return int SUCCESS en cas de réussite, sinon un code d'erreur.
 */
int laby_game_init(void);
/**
 * @brief Fonction appellée à la fin d'une partie.
 *
 */
void laby_game_end();

/**
 * @brief Fonction réalisant l'attente puis la gestion du déplacement du joueur.
 *
 * @return int SUCCESS en cas de réussite, sinon un code d'erreur.
 */
int laby_game_player_move(void);
/**
 * @brief Fonction gérant les tours dans le jeu du labyrinthe. Elle s'occupe de la boucle d'événement du jeu.
 *
 * @return int SUCCESS en cas de réussite, sinon un code d'erreur.
 */
int laby_game_cycle(void);
/**
 * @brief Fonction vérifiant si le joueur a atteint la fin du labyrinthe, ou autre condition de victoire.
 *
 * @return bool Vrai si le joueur à fini le jeu, sinon faux.
 */
bool laby_game_check_victory(void);
/**
 * @brief Vérifie que le joueur ne soit pas mort.
 *
 * @return bool Vrai si le joueur est mort.
 */
bool laby_game_check_if_player_dead(void);







#endif
