#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include "mini_lib.h"


//Initialisation de la variable globale descripteurs
tab_descripteurs *descripteurs = NULL;
void __mini_lib_constructor(void) __attribute__((constructor));
void __mini_lib_constructor(void) {
    if (descripteurs != NULL) {
        return; 
    }
    void *p = mini_calloc(sizeof(tab_descripteurs), 1);
    descripteurs = (tab_descripteurs*)p;
    
    if (descripteurs == NULL) {
        // Gérer l'erreur si l'allocation échoue
        mini_perror("Erreur d'allocation des descripteurs");
        _exit(1); 
    }
    
    void *p2 = mini_calloc(sizeof(MYFILE), 10);
    descripteurs->tab = (MYFILE*)p2;

    if (descripteurs->tab == NULL) {
        mini_perror("Erreur d'allocation du tableau de fichiers");
        _exit(1);
    }
    
    descripteurs->index_courant = 0;
}
MYFILE *mini_open(char *file, char mode){
    int des;
    if(mode == 'r') des = open(file, O_RDONLY);
    else if(mode == 'w') des = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    else if(mode == 'b') des = open(file, O_RDWR | O_CREAT, 0666);
    else if(mode == 'a') des = open(file, O_WRONLY | O_CREAT | O_APPEND, 0666);
    else return NULL;

    MYFILE *f_global = &descripteurs->tab[descripteurs->index_courant]; 

    // 3. Initialiser cet emplacement DIRECTEMENT
    f_global->fd = des;
    f_global->buffer_read  = NULL;
    f_global->buffer_write = NULL;
    f_global->ind_read  = -1;
    f_global->ind_write = -1;
    
    // 4. Incrémenter l'index pour le prochain appel
    descripteurs->index_courant++;
    
    // 5. Retourner le pointeur vers l'emplacement GLOBAL.
    return f_global;
}

/* version simple et francisée de mini_fread */
int mini_fread(void *tampon, int taille_element, int nb_elements, MYFILE *f)
{
    if (!f || !tampon) return -1;

    int nb_octets = taille_element * nb_elements;

    if (f->buffer_read == NULL) {
        f->buffer_read = mini_calloc(1, IOBUFFER_SIZE);
        f->ind_read = 0;
    }

    if (f->ind_read > 0) {
        int dispo = f->ind_read;
        int a_prendre = (nb_octets < dispo) ? nb_octets : dispo;
        char *src = (char*)f->buffer_read;
        char *dst = (char*)tampon;
        for (int i = 0; i < a_prendre; i++) dst[i] = src[i];
        if (a_prendre < dispo) {
            for (int i = 0; i < dispo - a_prendre; i++) src[i] = src[a_prendre + i];
            f->ind_read = dispo - a_prendre;
        } else {
            f->ind_read = 0;
        }
        return a_prendre / taille_element;
    }

    int lus = (int)read(f->fd, f->buffer_read, IOBUFFER_SIZE);
    if (lus <= 0) return 0;
    f->ind_read = lus;

    int a_copier = (nb_octets < lus) ? nb_octets : lus;
    char *src = (char*)f->buffer_read;
    char *dst = (char*)tampon;
    for (int i = 0; i < a_copier; i++) dst[i] = src[i];

    if (lus > a_copier) {
        for (int i = 0; i < lus - a_copier; i++) src[i] = src[a_copier + i];
        f->ind_read = lus - a_copier;
    } else {
        f->ind_read = 0;
    }

    return a_copier;
}
int mini_fwrite(void *buffer,int size_element ,int number_element,MYFILE *file){
    if (!file || !buffer) return -1;

    int nb_octets = size_element * number_element;

    if (file->buffer_write == NULL) {
        file->buffer_write = mini_calloc(1, IOBUFFER_SIZE);
        file->ind_write = 0;
    }

    char *src = (char*)buffer;
    char *dst = (char*)file->buffer_write;
    int total_written = 0;

    while (total_written < nb_octets) {
        int space_left = IOBUFFER_SIZE - file->ind_write;
        int to_write = (nb_octets - total_written < space_left) ? (nb_octets - total_written) : space_left;

        for (int i = 0; i < to_write; i++) {
            dst[file->ind_write + i] = src[total_written + i];
        }
        file->ind_write += to_write;
        total_written += to_write;

        if (file->ind_write == IOBUFFER_SIZE) {
            int written = (int)write(file->fd, file->buffer_write, IOBUFFER_SIZE);
            if (written < 0) return -1;
            file->ind_write = 0;
        }
    }

    return total_written;
}
int mini_fflush(MYFILE *file){
    if(!file || !file->buffer_write)return -1;
    if (file->ind_write > 0){
        int nb_ecrit = (int)write(file->fd,file->buffer_write,file->ind_write);
        if(nb_ecrit < 0)return -1;
        file->ind_write = 0;
        return nb_ecrit;
    }
    return 0;
}
int mini_fclose(MYFILE *file){
    if(!file)return -1;
    mini_fflush(file);
    int i = 0;
    while(i< descripteurs->index_courant && &descripteurs->tab[i] != file){
        i++;
    }
    if(i == descripteurs->index_courant)return -1;
    //&descripteurs->tab[i] = NULL;
    close(file->fd);
    return 0;

}
int mini_fgetc(MYFILE *file){
    if(!file)return -1;
    char c;
    if(read(file->fd,&c,sizeof(c)) <= 0)return -1;
    return(int)c;
}
int mini_fputc(MYFILE *file,char c){
    if(!file)return -1;
    if(write(file->fd,&c,sizeof(c)) <= 0)return -1;
    return(int)c;
}
