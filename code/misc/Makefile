####################################################################################
#   makefile
####################################################################################

include ../MakePacket.local

INCLUDE =  ../include

LIB := Lib_misc.a
#BASE_CFLAGS := -DLINUX

SRCS= \
	mutex.c\
	sem.c\
	tsk.c\
	sys.c
OBJS = $(addsuffix .o,$(basename $(SRCS)))

all: $(LIB)

$(LIB) :$(OBJS)
	$(AR) -rc $(LIB) $(OBJS)
	$(CP) $(LIB) ../$(TARGET_LIBDIR)
	$(CP) $(OBJS) ../$(RANDLIB_DIR)

%.o : %.c
	$(CPP)  $(BASE_CFLAGS) -I./$(INCLUDE) -lpthread -lm  -c $< -o $@
	
clean: 
	rm -rf *.o
	rm -rf $(LIB)
	rm -rf ../../$(TARGET_LIBDIR)$(LIB)
	rm -rf ../../$(RANDLIB_DIR)$(OBJS)
