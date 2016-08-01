all: release

release:
	cd Release && make -f makefile && cp Snake2d ../Snake2d

debug: 
	cd Debug && make -f makefile && cp Snake2d ../Snake2dDebug
clean: 
	rm -f Snake2d Snake2dDebug && cd Debug && make clean && cd ../Release && make clean