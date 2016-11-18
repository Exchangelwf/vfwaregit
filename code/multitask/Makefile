####################################################################################
#   makefile
####################################################################################

include ../MakePacket.local
INCLUDE =  ../include

LIB := Lib_MultiTask.a
#BASE_CFLAGS := -DLINUX

SRCS= \
	Thread.cpp\
	Timer.cpp
OBJS = $(addsuffix .o,$(basename $(SRCS)))

all:print  $(LIB)

print:
	@echo "the tool is "
	@echo $(MVTOOL_PREFIX)
	@echo $(CPP)
$(LIB) :$(OBJS)
	$(AR) -rc $(LIB) $(OBJS)
	$(CP) $(LIB) ../$(TARGET_LIBDIR)
	$(CP) $(OBJS) ../$(RANDLIB_DIR)

%.o : %.cpp
	
	$(CPP) $(BASE_CFLAGS) -I./$(INCLUDE) -lpthread -lm  -c $< -o $@
	
clean: 
	rm -rf *.o
	rm -rf $(LIB)
	rm -rf ../../$(TARGET_LIBDIR)$(LIB)
	rm -rf ../../$(RANDLIB_DIR)$(OBJS)
