CC=gcc
CCOPT=-Wall
PBC_INCLUDE_PATH=/usr/include/pbc
IOPT=-I $(PBC_INCLUDE_PATH) -I ./include
LDFLAGS= -lgmp -lcifer -lpbc
all:  testpairings_cifer demo_cifer testpairings_pbc demo_pbc libminife.so
onlycifer:  testpairings_cifer demo_cifer libminife.so
testpairings_pbc.o: src/testpairings.c
	$(CC) -o src/testpairings_pbc.o -c $(CCOPT) src/testpairings.c $(IOPT) -DPBC_OR_CIFER=0
testpairings_cifer.o: src/testpairings.c
	$(CC) -o src/testpairings_cifer.o -c $(CCOPT) src/testpairings.c $(IOPT) -DPBC_OR_CIFER=1
nife_pbc.o: src/nife.c
	$(CC) -o src/nife_pbc.o -c $(CCOPT) src/nife.c $(IOPT) -DPBC_OR_CIFER=0
pairings_pbc.o: src/pairings.c
	$(CC) -o src/pairings_pbc.o -c $(CCOPT) src/pairings.c $(IOPT) -DPBC_OR_CIFER=0
orproofs_pbc.o: src/orproofs.c
	$(CC) -o src/orproofs_pbc.o -c $(CCOPT) src/orproofs.c $(IOPT) -DPBC_OR_CIFER=0
demo_pbc.o: src/demo.c
	$(CC) -o src/demo_pbc.o -c $(CCOPT) src/demo.c $(IOPT) -DPBC_OR_CIFER=0
nife_cifer.o: src/nife.c
	$(CC) -o src/nife_cifer.o -c $(CCOPT) src/nife.c $(IOPT) -DPBC_OR_CIFER=1

pairings_cifer.o: src/pairings.c
	$(CC) -o src/pairings_cifer.o -c $(CCOPT) src/pairings.c $(IOPT) -DPBC_OR_CIFER=1
orproofs_cifer.o: src/orproofs.c
	$(CC) -o src/orproofs_cifer.o -c $(CCOPT) src/orproofs.c $(IOPT) -DPBC_OR_CIFER=1
demo_cifer.o: src/demo.c
	$(CC) -o src/demo_cifer.o -c $(CCOPT) src/demo.c $(IOPT) -DPBC_OR_CIFER=1
testpairings_cifer: testpairings_cifer.o pairings_cifer.o 
	$(CC) -o testpairings_cifer src/testpairings_cifer.o src/pairings_cifer.o $(LDFLAGS)
testpairings_pbc: testpairings_pbc.o pairings_pbc.o
	$(CC) -o testpairings_pbc src/testpairings_pbc.o src/pairings_pbc.o $(LDFLAGS)
demo_cifer: nife_cifer.o pairings_cifer.o orproofs_cifer.o demo_cifer.o
	$(CC) -o demo_cifer src/nife_cifer.o src/pairings_cifer.o src/orproofs_cifer.o src/demo_cifer.o $(LDFLAGS)
demo_pbc: nife_pbc.o pairings_pbc.o orproofs_pbc.o demo_pbc.o
	$(CC)  -o demo_pbc src/nife_pbc.o src/pairings_pbc.o src/orproofs_pbc.o src/demo_pbc.o $(LDFLAGS)
libminife.so: src/nife.c src/orproofs.c src/pairings.c
	gcc -shared -o libminife.so -fPIC src/nife.c -fPIC src/orproofs.c src/pairings.c $(CCOPT) $(IOPT) $(LDFLAGS) -DPBC_OR_CIFER=1
clean:
	rm -f demo_cifer demo_pbc testpairings_cifer testpairings_pbc *.o src/*.o
