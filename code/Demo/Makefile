####################################################################################
#   makefile
####################################################################################
include ../MakePacket.local
#########################
LOCAL_TARGET = server.exe
INCLUDE_DIR = ../include

Need_Lib += ../$(USRLIB_DIR)$(USRLIB_TARGET)
#########################
all: $(LOCAL_TARGET)
CCFLAGS= -D_DETACHED_THREAD_\
	-D__DEBUG__	
SRCS= main.cpp
OBJS = $(addsuffix .o,$(basename $(SRCS)))

$(LOCAL_TARGET): $(OBJS) 
	
	$(CPP) $(WAR) $(BASE_CFLAGS) \
	$(OBJS) \
	$(Need_Lib) \
	 -lnsl -lresolv  -ldl -lm -lpthread -lrt -fPIC -o $@

	$(STRIP) $(LOCAL_TARGET)
	rm -f *.o
	cp $(LOCAL_TARGET) ../


%.o:%.cpp
	$(CPP) $(WAR) $(CCFLAGS)   -I$(INCLUDE_DIR) -I../include  $(MAKETARGET) -ldl  -c $<
	
clean: 
	-rm -f *.o $(LOCAL_TARGET)
	-rm ../$(LOCAL_TARGET) 
