include ../MakePacket.local
#USRLIB_DIR = ../UsrLib/ 
OBJS_o=*.o

ifeq ($(TOOLS), amba_a5s)
	Need_Lib += Lib_Timer_a5s.a

endif

ifeq ($(TOOLS), amba_s2l)
	Need_Lib += Lib_Timer_s2l.a
endif

ifeq ($(TOOLS), amba_s2l_qiu)
	Need_Lib += Lib_Timer_s2l_qiu.a
endif

ifeq ($(TOOLS), hisi)
	Need_Lib += Lib_Timer_hisi.a
endif	

ifeq ($(TOOLS), x86)
	Need_Lib += Lib_Timer.a
endif	

all:
#	$(AR) -x ../ExModules/Timer/$(Need_Lib)
#	mv *.o ../$(RANDLIB_DIR)
	$(AR) -cru $(USRLIB_TARGET) ../$(RANDLIB_DIR)$(OBJS_o)
#	ranlib $(USRLIB_TARGET)
	$(CP) $(USRLIB_TARGET) ../$(USRLIB_DIR)
	$(CP) $(USRLIB_TARGET) ../$(DEMO_DIR)
	rm $(USRLIB_TARGET)

#	$(CC) -shared -fPIC  ../$(RANDLIB_DIR)$(OBJS_o)  -o $(USRSO_TARGET)
#	$(CP) $(USRSO_TARGET) ../$(USRLIB_DIR)
#	$(CP) $(USRSO_TARGET) ../$(DEMO_DIR)
#	rm $(USRSO_TARGET)
clean:
	-rm ../$(RANDLIB_DIR)$(OBJS_o)
	-rm ../$(USRLIB_DIR)$(USRLIB_TARGET)
	-rm ../$(DEMO_DIR)$(USRLIB_TARGET)
#	-rm ../$(USRLIB_DIR)$(USRSO_TARGET)
#	-rm ../$(DEMO_DIR)$(USRSO_TARGET)



