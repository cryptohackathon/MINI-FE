CC=gcc
CCOPT=-Wall
PBC_INCLUDE_PATH=/usr/include/pbc
IOPT=-I $(PBC_INCLUDE_PATH) -I ./include
LDFLAGS=-lgmp -lcifer -lpbc
DFLAGS=-DRANGE_OF_GRADING=4
all:  testpairings_cifer demo_cifer testpairings_pbc demo_pbc libminife.so compute_generator generate_keys generate_keys_from_pin  submitgrade evaltally
onlycifer:  testpairings_cifer demo_cifer libminife.so
testpairings_pbc.o: src/testpairings.c
	$(CC) -o src/testpairings_pbc.o -c $(CCOPT) src/testpairings.c $(IOPT) -DPBC_OR_CIFER=0 $(DFLAGS)
testpairings_cifer.o: src/testpairings.c
	$(CC) -o src/testpairings_cifer.o -c $(CCOPT) src/testpairings.c $(IOPT) -DPBC_OR_CIFER=1 $(DFLAGS)
nife_pbc.o: src/nife.c
	$(CC) -o src/nife_pbc.o -c $(CCOPT) src/nife.c $(IOPT) -DPBC_OR_CIFER=0 $(DFLAGS)
pairings_pbc.o: src/pairings.c
	$(CC) -o src/pairings_pbc.o -c $(CCOPT) src/pairings.c $(IOPT) -DPBC_OR_CIFER=0 $(DFLAGS)
orproofs_pbc.o: src/orproofs.c
	$(CC) -o src/orproofs_pbc.o -c $(CCOPT) src/orproofs.c $(IOPT) -DPBC_OR_CIFER=0 $(DFLAGS)
demo_pbc.o: src/demo.c
	$(CC) -o src/demo_pbc.o -c $(CCOPT) src/demo.c $(IOPT) -DPBC_OR_CIFER=0 $(DFLAGS)
nife_cifer.o: src/nife.c
	$(CC) -o src/nife_cifer.o -c $(CCOPT) src/nife.c $(IOPT) -DPBC_OR_CIFER=1 $(DFLAGS)

pairings_cifer.o: src/pairings.c
	$(CC) -o src/pairings_cifer.o -c $(CCOPT) src/pairings.c $(IOPT) -DPBC_OR_CIFER=1 $(DFLAGS)
orproofs_cifer.o: src/orproofs.c
	$(CC) -o src/orproofs_cifer.o -c $(CCOPT) src/orproofs.c $(IOPT) -DPBC_OR_CIFER=1 $(DFLAGS)
demo_cifer.o: src/demo.c
	$(CC) -o src/demo_cifer.o -c $(CCOPT) src/demo.c $(IOPT) -DPBC_OR_CIFER=1 $(DFLAGS)
testpairings_cifer: testpairings_cifer.o pairings_cifer.o 
	$(CC) -o testpairings_cifer src/testpairings_cifer.o src/pairings_cifer.o $(LDFLAGS) $(DFLAGS)
testpairings_pbc: testpairings_pbc.o pairings_pbc.o
	$(CC) -o testpairings_pbc src/testpairings_pbc.o src/pairings_pbc.o $(LDFLAGS) $(DFLAGS)
demo_cifer: nife_cifer.o pairings_cifer.o orproofs_cifer.o demo_cifer.o
	$(CC) -o demo_cifer src/nife_cifer.o src/pairings_cifer.o src/orproofs_cifer.o src/demo_cifer.o $(LDFLAGS) $(DFLAGS)
demo_pbc: nife_pbc.o pairings_pbc.o orproofs_pbc.o demo_pbc.o
	$(CC)  -o demo_pbc src/nife_pbc.o src/pairings_pbc.o src/orproofs_pbc.o src/demo_pbc.o $(LDFLAGS) $(DFLAGS)
libminife.so: src/nife.c src/orproofs.c src/pairings.c
	gcc -shared -o libminife.so -fPIC src/nife.c -fPIC src/orproofs.c src/pairings.c $(CCOPT) $(IOPT) $(LDFLAGS) -DPBC_OR_CIFER=1 $(DFLAGS)
compute_generator.o: src/compute_generator.c
	$(CC) -o src/compute_generator.o -c $(CCOPT) src/compute_generator.c $(IOPT) -DPBC_OR_CIFER=1 $(DFLAGS)
generate_keys.o: src/generate_keys.c
	$(CC) -o src/generate_keys.o -c $(CCOPT) src/generate_keys.c $(IOPT) -DPBC_OR_CIFER=1 $(DFLAGS)
generate_keys_from_pin.o: src/generate_keys_from_pin.c
	$(CC) -o src/generate_keys_from_pin.o -c $(CCOPT) src/generate_keys_from_pin.c $(IOPT) -DPBC_OR_CIFER=1 $(DFLAGS)
submitgrade.o: src/submitgrade.c
	$(CC) -o src/submitgrade.o -c $(CCOPT) src/submitgrade.c $(IOPT) -DPBC_OR_CIFER=1 $(DFLAGS)
evaltally.o: src/evaltally.c
	$(CC) -o src/evaltally.o -c $(CCOPT) src/evaltally.c $(IOPT) -DPBC_OR_CIFER=1 $(DFLAGS)
compute_generator: nife_cifer.o pairings_cifer.o orproofs_cifer.o compute_generator.o
	$(CC) -o compute_generator src/compute_generator.o src/nife_cifer.o src/orproofs_cifer.o src/pairings_cifer.o  $(LDFLAGS) $(DFLAGS)
generate_keys: nife_cifer.o pairings_cifer.o  generate_keys.o
	$(CC) -o generate_keys src/generate_keys.o src/pairings_cifer.o src/orproofs_cifer.o src/nife_cifer.o $(LDFLAGS) $(DFLAGS)
generate_keys_from_pin: nife_cifer.o pairings_cifer.o generate_keys_from_pin.o
	$(CC) -o generate_keys_from_pin src/generate_keys_from_pin.o src/pairings_cifer.o src/orproofs_cifer.o src/nife_cifer.o  $(LDFLAGS) $(DFLAGS)
submitgrade: nife_cifer.o pairings_cifer.o orproofs_cifer.o submitgrade.o
	$(CC) -o submitgrade src/submitgrade.o src/nife_cifer.o src/pairings_cifer.o src/orproofs_cifer.o  $(LDFLAGS) $(DFLAGS)
evaltally: nife_cifer.o pairings_cifer.o orproofs_cifer.o evaltally.o
	$(CC) -o evaltally src/evaltally.o src/pairings_cifer.o src/nife_cifer.o src/orproofs_cifer.o  $(LDFLAGS) $(DFLAGS)
clean:
	rm -f demo_cifer demo_pbc testpairings_cifer testpairings_pbc compute_generator generate_keys generate_keys_from_pin submitgrade evaltally *.o src/*.o
