CC = gcc
CFLAGS = -Wall -Wextra -Werror -g
INCLUDE_PATH = ../

# Sources
SRC = mini_export.c
OBJ = $(SRC:.c=.o)

# Nom du programme final
NAME = mini_export
# Ajouter le répertoire principal pour les includes
CFLAGS += -I$(INCLUDE_PATH)

# Règle par défaut
all: $(NAME)

$(NAME): $(OBJ) ../mini_memory.o ../mini_string.o ../mini_io.o
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) ../mini_memory.o ../mini_string.o ../mini_io.o

$(OBJ): $(SRC)
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage des objets
clean:
	rm -f $(OBJ)

# Suppression complète (objets + exécutable)
fclean: clean
	rm -f $(NAME)

# Reconstruction complète
re: fclean all

.PHONY: all clean fclean re
