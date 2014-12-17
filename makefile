C-JSON: main.o JSON.o mylibs.o
	cc -o C-JSON main.o JSON.o stringhandler.o
	@make clean

main.o: main.c JSON.h stringhandler.h
	cc -c main.c

JSON.o: JSON.c JSON.h stringhandler.h
	cc -c JSON.c

mylibs.o: stringhandler.c stringhandler.h
	cc -c stringhandler.c

clean:
	rm -f *.o
