#include <unistd.h>
#include "mini_lib.h"

int main(){
    void *tampon = mini_calloc(sizeof(char),5);
    if(!tampon){
        mini_perror("Erreur d'allocation memoire");
        mini_exit();
    }
    mini_printf("Allocation reussie\n");
    mini_printf("Entrez quelque chose\n");
    mini_scanf((char*)tampon,5);
    mini_printf((char*)tampon);
    mini_printf("\n");

    mini_free(tampon);

    void *tampon2 = mini_calloc(sizeof(char),200);
    if(!tampon2){
        mini_perror("Erreur d'allocation memoire");
        mini_exit();
    }

    MYFILE *f = mini_open("file.txt",'a');
    if(!f){
        mini_perror("Erreur ouverture fichier");
        mini_exit();
    }
    mini_printf("Entrez quelque chose\n");
    mini_scanf((char*)tampon2,30);
    int n = mini_fwrite(tampon2,sizeof(char),29,f);
    if(n < 0){
        mini_perror("Erreur lecture fichier");
        mini_exit();
    }
    void *c = mini_calloc(sizeof(char),1);
    if(!c){
        mini_perror("Erreur d'allocation memoire");
        mini_exit();
    }
    mini_scanf((char*)c,2);
    mini_fputc(f,(*(char*)c));
    mini_printf("Donnees ecrites dans le fichier :\n");
    mini_printf((char*)tampon2);
    //mini_fflush(f);
    mini_free(tampon2);
    mini_exit();
}
