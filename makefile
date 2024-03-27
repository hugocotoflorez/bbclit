# --------------------------- OPTIONS --------------------------- #
# Compiler options
CC=@gcc -Wall
# Libraries
LIBS =
HEADER_FILES_DIR = .
# Output filename
OUTPUT = main
# Header files
LIB_HEADERS = $(HEADER_FILES_DIR)/bbclit.h
# Source files
SRCS = main.c modules.c keyboard_handler.c boxes.c drawing.c templates.c

# --------------------------------------------------------------- #

# ---------------------------- RULES ---------------------------- #
# includepath
INCLUDES = -I $(HEADER_FILES_DIR)
#ficheros .o: todos los .o con un análogo .c en SRCS
OBJS = $(SRCS:.c=.o)

#regla 1 (dependencia de los .o)
$(OUTPUT): $(OBJS)
	$(CC) -o $(OUTPUT) $(OBJS) $(LIBS)

#regla 2 (genera los .o cuando es necesario, dependencia de los .c y .h)
#$@ es el nombre del fichero que se genera con la regla (.o)
#$< es el nombre del primer prerrequisito (el archivo .c cuyo .o se está generando)
%.o: %.c $(LIB_HEADERS)
	$(CC) -c -o $@ $< $(INCLUDES)

# regla 3 que borra el ejecutable (prerrequisito: clean)
cleanall: clean
	rm -f $(OUTPUT)

#regla 4 que borra los ficheros .o y los de backup (terminan en ~)
clean:
	@rm -f *.o *~

