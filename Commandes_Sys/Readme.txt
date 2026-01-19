Exo5: Mettre le buffer alloué à '\0' permet de nettoyer le buffer qui pourraient contenir des données sales et ensuite s'assurer en quelque 
sorte qu'une lecture pourrait s'arrếter avec le caractére fin de chaine

Exo6: La fonction free en C est utilisé pour des zones mémoires allouées dynamiquement mais elle ne libére pas vraiment elle marque
plutôt la zone à réutilisable

Exo17: le probléme qui reste à régler est le fait de ne pas avoir le retour à la ligne et donc le contenu s'affiche juste avant le 
prompt du terminal

Exo20 : Si le nombre de caratéres lues est égale à la taille du buffer il n'y aura pas de place pour le caractére fin de chaine

Exo21 : En termes de sécurité le probléme des fonction strlen srcpy est le fait de ne pas vérifier si la destination a une taille
suffisante pour accueillir la source et si le strlen parcourt une chaine non terminé par '\0' il boucle indéfiniment en quelques sorte

Exo34: Si le programme se termine sans que le buffer d'écriture ne soit plein alors les données ne seront jamais écrites dans le fichier 
il faut pour cela flusher tout les fichiers ouverts avant de quitter


Modifications et fonctionnalités ajoutées:
Toutes les questions ont été satisfaites et ainsi voici quelques fonctionnalités de plus 
char* mini_itoa(int n, char* str);
int find_word(const char *str, const char *word);
int mini_strcmp(const char *s1, const char *s2);
void mini_memset(void *s, int c, size_t n);
void mini_strcat(char *dest, const char *src);

Les 4 autres demandées par Vous :
size_t free_memory();
size_t free_buffer();
mini_du 
La fusion des blocs libres mémoires a été faite dans le mini_exit()
