include ./local.conf


all:
	make -C libdvr all
	make -C libfvideo all
	make -C libhicap all
	cp lib/* ../vfware/Build/IPC_S2L/Libs/
	make -C ../vfware/Build/IPC_S2L

clean:
	make -C libdvr clean
	make -C libfvideo clean
	make -C libhicap clean