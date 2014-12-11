C-JSON: main.o JSON.o mylibs.o
	cc -o C-JSON main.o JSON.o mylibs.o
	@make clean

main.o: main.c JSON.h mylibs.h
	cc -c main.c

JSON.o: JSON.c JSON.h mylibs.h
	cc -c JSON.c

mylibs.o: mylibs.c mylibs.h
	cc -c mylibs.c

clean:
	rm -f *.o