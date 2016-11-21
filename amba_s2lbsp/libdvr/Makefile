include ../local.conf

LIB_O += Alarm.o\
	Gpio.o\
	Analyse.o\
	Application.o\
	Backup.o\
	BlindDetect.o\
	Comm.o\
	Cmos.o\
	Flash.o\
	Frontboard.o\
	Ide.o\
	iFunCom.o\
	IntAlarm.o\
	IUart.o\
	IUart2.o\
	LossDetect.o\
	Mouse.o\
	Ptz.o\
	System.o\
	SystemUpdate.o\
	net/netArp.o  \
	net/netDns.o  \
	net/netIp.o  \
	net/netMac.o  \
	net/netMii.o  \
	net/netPppoe.o  \
	net/netUpnp.o  \
	net/netIscsi.o  \
	net/netWrieless.o\
	WatchDog.o

all:$(LIB_O)
	$(AR) -rc $(LIB_DVR) $(LIB_O)
	mv $(LIB_DVR) $(LIB_DIR)
%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@
	
clean:
	find -name "*.o" | xargs rm -f
	rm -rf *.a
	rm  $(LIB_DIR)/$(LIB_DVR) -rf;
