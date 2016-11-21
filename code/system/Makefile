####################################################################################
#   makefile
####################################################################################

include ../MakePacket.local
INCLUDE =  ../include

LIB := Lib_system.a
#BASE_CFLAGS := -DLINUX

SRCS= File.cpp
OBJS = $(addsuffix .o,$(basename $(SRCS)))

all: $(LIB)

$(LIB) :$(OBJS)
	$(AR) -rc $(LIB) $(OBJS)
	$(CP) $(LIB) ../$(TARGET_LIBDIR)
	$(CP) $(OBJS) ../$(RANDLIB_DIR)

%.o : %.cpp
	$(CPP) $(BASE_CFLAGS) -I./$(INCLUDE) -lpthread -lm  -c $< -o $@
	
clean: 
	rm -rf *.o
	rm -rf $(LIB)
	@echo "yyyyyyyyyyyyyyy"
	@echo $(LIB)
	rm -rf ../../$(TARGET_LIBDIR)$(LIB)
	@echo $(TARGET_LIBDIR)$(LIB)
	rm -rf ../../$(RANDLIB_DIR)$(OBJS)
