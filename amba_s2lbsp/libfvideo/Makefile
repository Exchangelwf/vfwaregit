include ../local.conf

LIB_O += Capture.o\
	Decoder.o\
	ExCapture.o\
	Garphics.o\
	Play.o\
	SlaveUart.o\
	Split.o\
	CaptureList.o\
	Spot.o\
	initial.o\
	Color2Grey.o\
	ColorAdjustment.o\
	PrivacyMask.o\
	OsdInsert.o\
	DrawOsd.o
	

all:$(LIB_O)
	$(AR) -rc $(LIB_FVIDEO) $(LIB_O)
	mv $(LIB_FVIDEO) $(LIB_DIR)
%.o: %.c
	$(CC) $(FLAGS) -c $<
	
clean:
	rm -rf *.o
	rm -rf *.a
	rm  $(LIB_DIR)/$(LIB_FVIDEO) -rf;
