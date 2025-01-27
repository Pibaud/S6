########################################
#~ définitions
########################################

# Executables à générer
BIN=bin/client bin/server bin/server2chaines bin/client2chaines

# liste des fichiers sources 
SRCS0=client.c
SRCS1=server.c
SRCS2=client2chaines.c
SRCS3=server2chaines.c

default: $(BIN)

########################################
#~ regles pour l'executable
########################################

obj/%.o: %.c
	gcc -Wall -Iinclude -c $< -o $@

bin/client: $(SRCS0:%.c=obj/%.o)
	gcc -o $@ $+

bin/client2chaines: $(SRCS2:%.c=obj/%.o)
	gcc -o $@ $+

bin/server: $(SRCS1:%.c=obj/%.o)
	gcc -o $@ $+

bin/server2chaines: $(SRCS3:%.c=obj/%.o)
	gcc -o $@ $+

clean:
	rm -f $(BIN) obj/*.o *~
