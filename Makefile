CC = gcc
CFLAGS = -Wall 
#-Wextra -Werror -g

# Sources du répertoire principal
SRC = main.c mini_memory.c mini_string.c mini_io.c
OBJ = $(SRC:.c=.o)

# Nom du programme final
NAME = programme

# Règle par défaut
all: $(NAME)

$(NAME): $(OBJ) 
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) 


%.o: %.c mini_lib.h 
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage des fichiers objets
clean:
	rm -f $(OBJ)

# Suppression complète (objets + exécutable)
fclean: clean
	rm -f $(NAME)

# Reconstruction complète
re: fclean all

# Déclaration des cibles "phoney" pour éviter les conflits avec des fichiers du même nom
.PHONY: all clean fclean re
