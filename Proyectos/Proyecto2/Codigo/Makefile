all: libs code 

libs:
		(cd lib && make)

code:
		(cd src && make)

clean:
	cd bin && rm -f Ants
	cd lib && rm **/*.a && rm **/*.o