/*
  project_defines.h
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
 * @file project_defines.h
 *
 * @author Benjamin Debotté
 * @brief Fichier contenant l'ensemble des définitions utiles au projet.
 */


#ifndef PROJECT_DEFINES_H
#define PROJECT_DEFINES_H

typedef signed char sint8_t;
typedef unsigned char uint8_t;

typedef unsigned short uint16_t;
typedef signed short sint16_t;

typedef int sint32_t;
typedef unsigned int uint32_t;

 /* Remplace fflush */
#define FFLUSH() {char c;do {c = getchar();} while (c != '\n' && c != EOF);}

#ifdef WIN32
#define PAUSE_CMD   "pause"
#else
#define PAUSE_CMD   "read -n1"
#endif

#ifdef WIN32
#define CLEAR_CMD   "cls"
#else
#define CLEAR_CMD   "clear"
#endif

#define RANDOM(MIN,MAX) (((double)rand()/RAND_MAX)*MAX + MIN)


#ifdef PRINT_DEBUG
    #define PRINTDF(X)      printf X
#else
    #define PRINTDF(X)      do{}while(0)
#endif

#define SUCCESS                 0
#define ERROR_PARAM_NULL        1
#define ERROR_INVALID_PARAM     2
#define ERROR_MALLOC            3
#define ERROR_INVALID_BEHAVIOR  4


#endif




