OBJS = ./bin/parser.o ./bin/tasks.o ./bin/file.o ./bin/MAD.o

HEADS= ./app_code/definitions.h ./app_code/parser.h ./app_code/tasks.h ./app_code/file.h

MAD: $(OBJS)
	gcc -g -o MAD $(OBJS)

./bin/MAD.o: $(HEADS) ./app_code/MAD.c
	gcc -g -c -o ./bin/MAD.o ./app_code/MAD.c

./bin/parser.o : ./app_code/parser.c ./app_code/parser.h ./app_code/tasks.h
	gcc -g -c -o ./bin/parser.o ./app_code/parser.c

./bin/tasks.o : ./app_code/tasks.c ./app_code/tasks.h
	gcc -g -c -o ./bin/tasks.o ./app_code/tasks.c

./bin/file.o : ./app_code/file.c ./app_code/file.h
	gcc -g -c -o ./bin/file.o ./app_code/file.c

clean:
	rm $(OBJS) MAD
