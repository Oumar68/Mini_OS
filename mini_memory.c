#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "mini_lib.h"

#define ALIGNMENT 16
#define ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1))

/* accès aux globals définis dans mini_string.c */
extern char *buffer;
extern int ind;

struct malloc_element {
    int magic ; // pour debuguer les core dumps
    void *zone;
    size_t taille;
    bool statut;
    struct malloc_element *suivant;
};
static struct malloc_element *liste = NULL;

/*static void ecrire_err(const char *s, int len) {
    if (len > 0) write(STDERR_FILENO, s, len);
}*/

/*void *mini_calloc(int size_element, int number_element) {
    if (size_element <= 0 || number_element <= 0) return NULL;
    size_t taille = (size_t)size_element * (size_t)number_element;
    
    struct malloc_element *crt = liste;
    while (crt) {
        if (!crt->statut && crt->taille >= taille) {
            crt->statut = true;
            char *p = (char*)crt->zone;
            for (size_t i = 0; i < taille; ++i) p[i] = 0;
            return crt->zone;
        }
        crt = crt->suivant;
    }

    //size_t meta = sizeof(struct malloc_element);
    //size_t align = sizeof(void*);
    //if (meta % align) meta += align - (meta % align);
    // Aligner la taille de la structure ET la taille demandée
    size_t meta = ALIGN(sizeof(struct malloc_element));
    size_t total = meta + ALIGN(taille);
    void *bloc = sbrk((intptr_t)total);
    if (bloc == (void*)-1) {
        const char msg[] = "mini_calloc: sbrk failed\n";
        ecrire_err(msg, sizeof(msg)-1);
        return NULL;
    }

    struct malloc_element *elem = (struct malloc_element*)bloc;
    void *zone = (char*)bloc + meta;

    char *pzone = (char*)zone;
    for (size_t i = 0; i < taille; ++i) pzone[i] = 0;

    elem->zone = zone;
    elem->taille = taille;
    elem->statut = true;
    elem->suivant = liste;
    liste = elem;

    return zone;
}*/
void *mini_calloc(int size_element, int number_element) {
    if (size_element <= 0 || number_element <= 0) return NULL;
    size_t taille = (size_t)size_element * (size_t)number_element;
    
    // 1. Recherche dans la liste existante
    struct malloc_element *crt = liste;
    while (crt) {
        // Sécurité : si le magic n'est pas bon, la liste est corrompue
        if (crt->magic != 0x41414141) {
            write(2, "FATAL: Liste malloc corrompue\n", 30);
            return NULL; 
        }
        if (!crt->statut && crt->taille >= taille) {
            crt->statut = true;
            char *p = (char*)crt->zone;
            for (size_t i = 0; i < taille; ++i) p[i] = 0;
            return crt->zone;
        }
        crt = crt->suivant;
    }

    // 2. Allocation d'un nouveau bloc
    size_t meta = ALIGN(sizeof(struct malloc_element));
    size_t total = meta + ALIGN(taille);
    
    void *bloc = sbrk((intptr_t)total);
    if (bloc == (void*)-1) return NULL;

    struct malloc_element *elem = (struct malloc_element*)bloc;
    
    // On s'assure que zone est bien décalée de 'meta' octets réels
    void *zone = (void*)((char*)bloc + meta);

    // Initialisation à zéro
    char *pzone = (char*)zone;
    for (size_t i = 0; i < taille; ++i) pzone[i] = 0;

    elem->magic = 0x41414141; // Signature de sécurité
    elem->zone = zone;
    elem->taille = taille;
    elem->statut = true;
    elem->suivant = liste;
    liste = elem;

    return zone;
}


void mini_free(void *ptr) {
    if (!ptr) return;

    struct malloc_element *cur = liste;

    // 1. On cherche le bloc et on le marque comme libre
    while (cur) {
        if (cur->zone == ptr) {
            cur->statut = false; 
            break; 
        }
        cur = cur->suivant;
    }
    cur = liste;
    while (cur && cur->suivant) {
        if (cur->statut == false && cur->suivant->statut == false) {
            // FUSION DES BLOCS SUCCESSIFS LIBRES
            struct malloc_element *a_supprimer = cur->suivant;
            
            // La nouvelle taille = taille actuelle + taille metadata + taille du bloc suivant
            cur->taille += sizeof(struct malloc_element) + a_supprimer->taille;
            
            // On saute l'élément fusionné en pointant vers le suivant du suivant 
            cur->suivant = a_supprimer->suivant;
            
            //car le nouveau "suivant" est peut-être libre aussi !
            continue; 
        }
        cur = cur->suivant;
    }
}

void mini_exit(void) {
    if (buffer != NULL && ind >= 0) {
        write(STDOUT_FILENO, buffer, ind + 1);
        write(STDOUT_FILENO, "\n", 1);
    }
    if(descripteurs->index_courant > 0){
        for(int i = 0 ; i < descripteurs->index_courant ; i++){
            mini_fflush(&descripteurs->tab[i]);
        }
        descripteurs->index_courant = 0;
    }
    _exit(0);
}

size_t free_memory(){
    struct malloc_element *cur = liste;
    size_t taille_libre = 0;
    while(cur){
        if(!cur->statut){
            taille_libre += cur->taille;
        }
     cur = cur->suivant;
    }
    return taille_libre;
}