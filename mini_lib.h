#ifndef MINI_LIB_H
#define MINI_LIB_H

#define BUF_SIZE 1024
#define IOBUFFER_SIZE 4096 //2048

typedef struct MYFILE {
    int fd;
    void *buffer_read;
    void *buffer_write;
    int ind_read;
    int ind_write;
} MYFILE;
//Structure pour la gestion des descripteurs de fichiers
typedef struct tab_descripteurs{
    int index_courant;
    MYFILE *tab;
}tab_descripteurs;

/* variables globales pour mini_printf / mini_exit */
extern char *buffer;
extern int ind;
extern tab_descripteurs *descripteurs;

void *mini_calloc(int size_element, int number_element);
void mini_free(void *ptr);
void mini_exit(void);
void mini_printf(char *message);
int mini_scanf(char *buffer, int size_buffer);
int mini_strlen(char *s);
int mini_strcpy(char *s ,char *d, int size_dest);
void mini_perror(char *message);
MYFILE *mini_open(char *file, char mode);
int mini_fread(void *buffer,int size_element ,int number_element,MYFILE *file);
int mini_fwrite(void *buffer,int size_element ,int number_element,MYFILE *file);
int mini_fflush(MYFILE *file);
int mini_fclose(MYFILE *file);
int mini_fgetc(MYFILE *file);
int mini_fputc(MYFILE *file,char c);

char* mini_itoa(int n, char* str);
int find_word(const char *str, const char *word);
int mini_strcmp(const char *s1, const char *s2);
void mini_memset(void *s, int c, size_t n);
void mini_strcat(char *dest, const char *src);

#endif
