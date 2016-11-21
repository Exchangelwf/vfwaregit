include ../local.conf

LIB_O += Video.o\
	Audio.o\
	CSemaphore.o\
	Memory.o\
	Mutex.o\
	Thread.o\
	sem.o\
	Motion.o\
	MotionDetect.o\
	audio_encode.o

all:$(LIB_O)
	$(AR) -rc $(LIB_HICAP) $(LIB_O)
	mv $(LIB_HICAP) $(LIB_DIR)
%.o: %.c
	$(CC) $(FLAGS) -c $<
	
clean:
	rm -rf *.o
	rm -rf *.a
	rm  $(LIB_DIR)/$(LIB_HICAP) -rf;
	