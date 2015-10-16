/*
  main.c
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
 * @mainpage Documentation du projet Labyrinthe
 * Ce projet a pour objectif de produire un jeu, rassemblant l’ensemble des connaissances acquises en cours d’Initiation à la Programmation ainsi qu’en cours d’Outils au Développement Logiciel.
 * Le travail à réaliser est découpé en quatre étapes, ajoutant à chaque fois des mécaniques de jeu plus complexes devant utiliser des outils du langage C elles aussi plus complexes.
 *
 * La première étape consiste à générer un labyrinthe parfait grâce à un algorithme de fusion par identifiant puis de pouvoir y incorporer un personnage ayant la capacité de s’y mouvoir via une entrée de l’utilisateur.
 * La seconde permet de mettre en place une interface ergomique pour que l’utilisateur puisse créer, charger, jouer et quitter à son gré. Cette étape rend nécéssaire la mise en place de sérialisation pour pouvoir enregistrer puis recharger un labyrinthe.
 * La troisième étape pose de nouvelles mécaniques liées au scoring, avec la sauvegarde des meilleurs scores réalisés par le joueur.
 *  La dernière étape ajoute la notion d’ennemis ainsi que la gestion de leur déplacement.
 *
 */


#include <stdio.h>
#include "laby_game.h"
#include "project_defines.h"

int main(int argc, const char * argv[]) {
    laby_game_menu_init();
    return SUCCESS;
}
