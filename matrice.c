/*
 matrice.c
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
#include "matrice.h"

#ifdef WIN32
#include "getline.h"
#endif /* WIN32 */

struct Matrice matrice_alloc(int lignes, int colonnes, size_t size) {
	struct Matrice mat;
    int i;

	mat.nblignes = 0;
	mat.nbcolonnes = 0;
    mat.donnees = NULL;

    if(lignes <= 0 || colonnes <= 0)
		return mat;

    mat.donnees = (void**)malloc(lignes*sizeof(void*));
    if(mat.donnees == NULL)
        return mat;
    for(i = 0; i < lignes; i++) {
        mat.donnees[i] = (void*)malloc((colonnes + 1)*size);
        if(mat.donnees[i] == NULL){
            int j = i;
            for(;j>0; j--)
                free(mat.donnees[j]);
            free(mat.donnees);
            mat.donnees = NULL;
            return mat;
        }
    }

    mat.nblignes = lignes;
    mat.nbcolonnes = colonnes;
    return mat;
}

void matrice_free(struct Matrice m) {
	if(m.donnees == NULL)
		return;

    while(m.nblignes-- > 0) {
		free(m.donnees[m.nblignes]);
        m.donnees[m.nblignes] = NULL;
    }
	free(m.donnees);
}

struct Matrice matrice_init(char* filepath) {
    struct Matrice mat = {NULL,0,0};
	FILE* fd = NULL;

    if(filepath == NULL)
        return mat;
	if((fd = fopen(filepath,"r")) == NULL)
		return mat;

	matrice_load_from_file(&mat,fd);
	return mat;
}

void matrice_print(struct Matrice mat) {
	int i;
	for(i = 0; i < mat.nblignes; i++)
		printf("%s\n",((char**)mat.donnees)[i]);
}

void matrice_save(struct Matrice mat, char* filepath) {
	FILE* fd = NULL;

    if(filepath == NULL)
        return ;
	if((fd = fopen(filepath,"w+")) == NULL) /* Fichier cree si non existant, ecriture seule */
		return ;

	matrice_save_into_file(&mat,fd);
}

bool matrice_save_into_file(struct Matrice* mat, FILE* fd) {
	int i;

	if(fd == NULL || mat == NULL)
		return false;
	if(mat->donnees == NULL)
		return false;

	fprintf(fd,"%d %d\n",mat->nblignes, mat->nbcolonnes);
	for(i = 0; i < mat->nblignes; i++)
		fprintf(fd,"%s\n",mat->donnees[i]);
    return true;
}


bool matrice_load_from_file(struct Matrice* mat, FILE* fd) {
    char* buffer = NULL;
	size_t linecap = 0;
	int col = -1, line = -1, i;

	if(mat == NULL || fd == NULL)
		return false;

	/* On recupere le nombre de colonnes et de lignes pour initialiser la matrice */
	getline(&buffer, &linecap, fd);
	sscanf(buffer,"%d %d",&line,&col);

	mat->donnees = (void**)malloc(line*sizeof(void*));
	if(mat->donnees == NULL)
		return false;
	for(i = 0; i < line; i++){
		mat->donnees[i] = NULL;
		/* Getline s'occupe de l'allocation de memoire automatiquement */
		getline((char**)&mat->donnees[i],&linecap,fd);
		((char**)mat->donnees)[i][col] = '\0';
		linecap = 0;
	}
	mat->nblignes = line;
	mat->nbcolonnes = col;
	return true;
}




