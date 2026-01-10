#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "mini_lib.h"

/* accès aux globals définis dans mini_string.c */
extern char *buffer;
extern int ind;

struct malloc_element {
    void *zone;
    size_t taille;
    bool statut;
    struct malloc_element *suivant;
};
static struct malloc_element *liste = NULL;

static void ecrire_err(const char *s, int len) {
    if (len > 0) write(STDERR_FILENO, s, len);
}

void *mini_calloc(int size_element, int number_element) {
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

    size_t meta = sizeof(struct malloc_element);
    size_t align = sizeof(void*);
    if (meta % align) meta += align - (meta % align);

    size_t total = meta + taille;
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
}

void mini_free(void *ptr) {
    if (!ptr) return;
    struct malloc_element *cur = liste;
    while (cur) {
        if (cur->zone == ptr) {
            cur->statut = false;
            return;
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
