#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include "mini_lib.h"

char *buffer = NULL;
int ind = -1;

int mini_strlen(char *s){
    if (!s) return 0;
    int i = 0;
    while (s[i] != '\0') i++;
    return i;
}

int mini_strcpy(char *s ,char *d, int size_dest){
    if (!s || !d || size_dest <= 0) return -1;
    int i = 0;
    while (i < size_dest - 1) {
        d[i] = s[i];
        if (s[i] == '\0') return i;
        i++;
    }
    d[i] = '\0';
    return i;
}

void mini_printf(char *message){
    if (message == NULL) return;
    if (buffer == NULL) {
        buffer = mini_calloc(sizeof(char), BUF_SIZE);
        if (!buffer) return;
        ind = -1;
    }
    int len = mini_strlen(message);
    int i = 0;
    while (i < len && message[i] != '\n') {
        ind++;
        if (ind >= BUF_SIZE) {
            write(STDOUT_FILENO, buffer, ind);
            for (int k = 0; k < ind; ++k) buffer[k] = 0;
            ind = -1;
        }
        buffer[ind ] = message[i];
        i++;
    }
    if (i < len && message[i] == '\n') {
        write(STDOUT_FILENO, buffer, ind + 1);
        write(STDOUT_FILENO, "\n", 1);
        for (int k = 0; k <= ind; ++k) buffer[k] = 0;
        ind = -1;
    }
}

int mini_scanf(char *buf, int size_buffer){
    if (!buf || size_buffer <= 0) return -1;
    int to_read = size_buffer - 1;
    int nbytes = (int)read(STDIN_FILENO, buf, to_read);
    if (nbytes < 0) return -1;
    if (nbytes >= 0) buf[nbytes] = '\0';
    if (nbytes == to_read) {
        char dump;
        while (read(STDIN_FILENO, &dump, 1) == 1 && dump != '\n');
    }
    return nbytes;
}

/* mini_perror sans perror/fprintf */
static int itoa_unsigned(int val, char *out, int out_size) {
    if (out_size <= 0) return 0;
    if (val == 0) {
        if (out_size > 1) { out[0] = '0'; out[1] = '\0'; return 1; }
        return 0;
    }
    int tmp = val;
    char rev[32];
    int r = 0;
    while (tmp > 0 && r < (int)sizeof(rev)) {
        rev[r++] = '0' + (tmp % 10);
        tmp /= 10;
    }
    if (r + 1 > out_size) return 0;
    for (int j = 0; j < r; ++j) out[j] = rev[r - 1 - j];
    out[r] = '\0';
    return r;
}

void mini_perror(char *message){
   if (message) {
       write(STDERR_FILENO, "Erreur ", 7);
       write(STDERR_FILENO, message, mini_strlen(message));
       write(STDERR_FILENO, " : ", 3);
   }
   char num[32];
   int n = itoa_unsigned(errno, num, sizeof(num));
   if (n > 0) write(STDERR_FILENO, num, n);
   write(STDERR_FILENO, "\n", 1);
}
char* mini_itoa(int n, char* str) {
    int i = 0;
    int is_negative = 0;

    // Gestion du zéro
    if (n == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }

    // Gestion des nombres négatifs
    if (n < 0) {
        is_negative = 1;
        n = -n;
    }

    // Extraction des chiffres (ils arrivent à l'envers)
    while (n != 0) {
        str[i++] = (n % 10) + '0';
        n = n / 10;
    }

    if (is_negative) {
        str[i++] = '-';
    }

    str[i] = '\0';

    // Inverser la chaîne pour l'avoir dans le bon sens
    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }

    return str;
}
int find_word(const char *str, const char *word) {
    if (word[0] == '\0') return 0;
    for (int i = 0; str[i] != '\0'; i++) {
        int j = 0;
        while (str[i + j] == word[j] && word[j] != '\0') {
            j++;
        }
        if (word[j] == '\0') return 1;
    }
    return 0;
}
int mini_strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(unsigned char *)s1 - *(unsigned char *)s2;
}
void mini_memset(void *s, int c, size_t n) {
    unsigned char *p = s;
    while (n--) {
        *p++ = (unsigned char)c;
    }
}
void mini_strcat(char *dest, const char *src) {
    char *ptr = dest + mini_strlen(dest);
    while (*src) {
        *ptr++ = *src++;
    }
    *ptr = '\0';
}