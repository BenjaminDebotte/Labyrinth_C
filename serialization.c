/*
  serialization.c
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

#include "serialization.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



Player_Score_str_t** score_manage(char* scoreFileName, Player_Score_str_t Scoring_data){
    int i;
    FILE* scoreFile;
    int numbersOfScore = 0;
    Player_Score_str_t **scores = NULL;

    
    if(scoreFileName == NULL)
        return NULL;
    scoreFile = fopen(scoreFileName, "a+");
    if(scoreFile == NULL)
        return NULL;
    
    scores = calloc(10,sizeof(Player_Score_str_t*)); /* calloc mettra à 0 <=> NULL les champs */
    
    rewind(scoreFile);
    fseek(scoreFile, 0, SEEK_END);
    numbersOfScore = (int)ftell(scoreFile) / sizeof(Player_Score_str_t);
    fclose(scoreFile);
    
    if (numbersOfScore < 10) { /* Moins de 10 scores ou fichier inexistant */
        uint8_t currentIndex = 0;
        
        fopen(scoreFileName, "a+");
        fwrite(&Scoring_data, 1, sizeof(Player_Score_str_t), scoreFile);
        rewind(scoreFile);
        
        /* On lit l'ensemble des scores */
        while(1) {
            Player_Score_str_t tmp;
            i = (int)fread(&tmp, 1, sizeof(Player_Score_str_t), scoreFile);
            
            if(i <= 0)
                break;
            
            scores[currentIndex] = (Player_Score_str_t*)malloc(sizeof(Player_Score_str_t));
            memcpy(scores[currentIndex], &tmp, sizeof(Player_Score_str_t));
            
            currentIndex++;
        }
        
    }
    else { /* Ecraser le plus petit */
        uint8_t indexLowest = 0,currentIndex = 0;
        uint32_t lowestScore = 0xFFFFFFFF;
        
        fopen(scoreFileName, "r+");
        rewind(scoreFile);
        
        while(1){
            Player_Score_str_t tmp;
            i = (int)fread(&tmp, 1, sizeof(Player_Score_str_t), scoreFile);
            
            if(i == 0)
                break;
            
            scores[currentIndex] = (Player_Score_str_t*)malloc(sizeof(Player_Score_str_t));
            memcpy(scores[currentIndex], &tmp, sizeof(Player_Score_str_t));
            
            if(lowestScore > tmp.score)
            {indexLowest = currentIndex; lowestScore = tmp.score;}
            currentIndex++;
        }
        fseek(scoreFile, indexLowest*sizeof(Player_Score_str_t), SEEK_SET);
        fwrite(&Scoring_data, 1, sizeof(Player_Score_str_t), scoreFile);
        *scores[indexLowest] = Scoring_data;
        
    }
    fclose(scoreFile);

    
    /* On trie le tableau par ordre décroissant avec un tri par insertion */
    for(i = numbersOfScore; i > 0; i--){
        int d;
        d = i;
        while (scores[d] != NULL && scores[d - 1] != NULL && d > 0) {
            if(scores[d]->score > scores[d-1]->score){
                Player_Score_str_t tmp;
                
                tmp = *scores[d];
                *scores[d] = *(scores[d-1]);
                *(scores[d-1]) = tmp;
            }
            d--;
        }
    }
    
    return scores;
}



BUFFER* serialize_labyrinthe_to_buffer(Labyrinthe* lab, size_t* sizeOfBuffer) {
    BUFFER *buffer = NULL, *pBuffer = NULL;
    Laby_Cell_str_t** cells;
    int i,j;
    
    if(lab == NULL)
        return NULL;
    if(lab->donnees == NULL)
        return NULL;
    
    
    /* MAGIC_BYTE + 2x4 octets + (4*2 + 2*1) octets par cellules */
    if(sizeOfBuffer != NULL)
        *sizeOfBuffer =  1*sizeof(uint8_t) +   2*sizeof(int) + lab->nbcolonnes*lab->nblignes*(4*sizeof(uint16_t) + 2*sizeof(uint8_t));
    buffer = (BUFFER*)malloc(1*sizeof(uint8_t) + 2*sizeof(int) + lab->nbcolonnes*lab->nblignes*(4*sizeof(uint16_t) + 2*sizeof(uint8_t)));
    if(buffer == NULL)
        return NULL;
    
    pBuffer = buffer;
    
    *((uint8_t*)pBuffer) = MAGIC_BYTE;
    pBuffer = ((char*)pBuffer) + sizeof(uint8_t);
    
    memcpy(pBuffer, &(lab->nblignes), sizeof(int));
    pBuffer = ((char*)pBuffer) + sizeof(int);
    memcpy(pBuffer, &(lab->nbcolonnes), sizeof(int));
    pBuffer = ((char*)pBuffer) + sizeof(int);

    cells = (Laby_Cell_str_t**)lab->donnees;
    
    for(i = 0; i < lab->nblignes; i++)
        for(j = 0; j < lab->nbcolonnes; j++)
            pBuffer = serialize_cell(pBuffer, &cells[i][j]);
            
    

    return buffer;
}

BUFFER* serialize_cell(BUFFER* dst, const Laby_Cell_str_t *cell) {
    BUFFER* pBuffer = NULL;

    if(dst == NULL || cell == NULL)
        return NULL;
    
    pBuffer = dst;

    memcpy(pBuffer, &cell->type, sizeof(uint8_t));
    pBuffer = ((char*)pBuffer) + sizeof(uint8_t);
    
    memcpy(pBuffer, &cell->param1, sizeof(uint16_t));
    pBuffer = ((char*)pBuffer) + sizeof(uint16_t);
    
    memcpy(pBuffer, &cell->param2, sizeof(uint16_t));
    pBuffer = ((char*)pBuffer) + sizeof(uint16_t);
    
    if(cell->entity != NULL) {
        /* Une entité est présente, on sauvegarde */
        memcpy(pBuffer, &cell->entity->type, sizeof(uint8_t));
        pBuffer = ((char*)pBuffer) + sizeof(uint8_t);
        
        memcpy(pBuffer, &cell->entity->param1, sizeof(uint16_t));
        pBuffer = ((char*)pBuffer) + sizeof(uint16_t);
        
        memcpy(pBuffer, &cell->entity->param2, sizeof(uint16_t));
        pBuffer = ((char*)pBuffer) + sizeof(uint16_t);
    }
    else {
        /* Sinon, on assigne 5 octets à 0x00  <=> 1 octet à 0x00 + 2 octets à 0x0000) */
        memset(pBuffer, 0x00, sizeof(uint8_t) + 2*sizeof(uint16_t));
        pBuffer = ((char*)pBuffer) + sizeof(uint8_t) + 2*sizeof(uint16_t);
    }

    return pBuffer;
}


Labyrinthe serialize_buffer_to_labyrinthe(BUFFER* buff) {
    Labyrinthe laby;
    int i,j;
    BUFFER* pBuffer = NULL;
    Laby_Cell_str_t** cells = NULL;
    
    laby.nblignes = 0;
    laby.nbcolonnes = 0;
    laby.donnees = NULL;
    
    if(buff == NULL)
        return laby;
    
    pBuffer = buff;
    
    if(*((uint8_t*)pBuffer) != MAGIC_BYTE){
        PRINTDF(("Le labyrinthe semble être invalide. Oops.\n"));
        return laby;
    }
    pBuffer = ((char*)pBuffer) + sizeof(uint8_t);

    
    if(labyrinthe_alloc(((int*)pBuffer)[0],((int*)pBuffer)[1],&laby) != SUCCESS){
        laby.nbcolonnes = 0;
        laby.nblignes = 0;
        laby.donnees = NULL;
        return laby;
    }
    
    cells = (Laby_Cell_str_t**)laby.donnees;
    
    if(cells == NULL){
        laby.nbcolonnes = 0;
        laby.nblignes = 0;
        laby.donnees = NULL;
        return laby;
    }
    
    pBuffer = ((char*)pBuffer) + 2*sizeof(int); /* On passe les deux premiers entiers déjà lu */
    
    for(i = 0; i < laby.nblignes; i++)
        for(j = 0; j < laby.nbcolonnes; j++){
            cells[i][j].type = *(uint8_t*)pBuffer;
            pBuffer = ((char*)pBuffer) + sizeof(uint8_t);
            
            cells[i][j].param1 = *(uint16_t*)pBuffer;
            pBuffer = ((char*)pBuffer) + sizeof(uint16_t);
            
            cells[i][j].param2 = *(uint16_t*)pBuffer;
            pBuffer = ((char*)pBuffer) + sizeof(uint16_t);
            
            
            if(*(uint8_t*)pBuffer != 0x00) {
                cells[i][j].entity = (Laby_Entity_str_t*)malloc(sizeof(Laby_Entity_str_t));
                
                cells[i][j].entity->type = *(uint8_t*)pBuffer;
                pBuffer = ((char*)pBuffer) + sizeof(uint8_t);
                
                cells[i][j].entity->param1 = *(uint16_t*)pBuffer;
                pBuffer = ((char*)pBuffer) + sizeof(uint16_t);
                
                cells[i][j].entity->param2 = *(uint16_t*)pBuffer;
                pBuffer = ((char*)pBuffer) + sizeof(uint16_t);
                
            }
            else {
                cells[i][j].entity = NULL;
                pBuffer = ((char*)pBuffer) + sizeof(uint8_t) + 2*sizeof(uint16_t);
            }
        }
    
    return laby;
}


