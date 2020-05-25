COMPILEFLAGS=-lm -lpthread -g
CCOMP=cc
EXEC=assignment6
all : $(EXEC) random.o
	$(CCOMP) -o assignment6 assignment6.o random.o $(COMPILEFLAGS)

assignment6.o : assignment6.c random.h
	$(CCOMP) -c assignment6.c $(COMPILEFLAGS)
random.o : random.c random.h
	$(CCOMP) -c random.c $(COMPILEFLAGS)
run:
	(EXEC)
	./$(EXEC)
clean:
	rm -f assignment6.o random.o
