DMTPCPATH = /scratch0/adeistin/code/HPTPC/hptpc-daq/DmtpcCore
#DMTPCLIBS = -L$(DMTPCPATH) $(DMTPCPATH)/lib/libDmtpcCore.so 
DMTPCDAQPATH = /scratch0/eatkin/HPTPC/hptpc-daq/DmtpcDAQ
DMTPCDAQLIBS = -L$(DMTPCDAQPATH)/lib -lDmtpcDAQ
#DMTPC_HOME = /scratch0/adeistin/code/HPTPC/hptpc-daq
DMTPCINCLUDE = -I$(DMTPC_HOME)/DmtpcCore/include -I$(DMTPC_HOME)/DmtpcDAQ/include
DMTPCLIBS =-L$(DMTPC_HOME)/DmtpcCore/lib -l$(DMPTPC_HOME)/DmtpcCore/liDmtpcCore -L$(DMTPC_HOME)/DmtpcDAQ/lib -lDmtpcDAQ
C++ = g++
CFLAGS   = -g -w -lrt -O2
GET_RUN_LITE_OBJ = Make_run_info_lite_table.o
GET_CONFIG = Make_good_runs_config_table.o 
GETSLOWOBJ= Make_slow_control_plots.o
all: Make_good_runs_config_table.exe Make_run_info_lite_table.exe Make_slow_control_plots.exe

Make_run_info_lite_table.o: Make_run_info_lite_table.C
	$(C++) $(CFLAGS) $(shell root-config --cflags) -o $@ -c Make_run_info_lite_table.C

Make_run_info_lite_table.exe: $(GET_RUN_LITE_OBJ)
	$(C++) $(CFLAGS) -o $@ $(GET_RUN_LITE_OBJ) $(shell root-config --libs)

Make_good_runs_config_table.o: Make_good_runs_config_table.C
	$(C++) $(CFLAGS) $(shell root-config --cflags) $(DMTPCINCLUDE) -o $@ -c Make_good_runs_config_table.C

Make_good_runs_config_table.exe: $(GET_CONFIG)
	$(C++) $(CFLAGS) -o $@ $(GET_CONFIG) $(shell root-config --libs) $(DMTPCDAQLIBS)

Make_slow_control_plots.o: Make_slow_control_plots.C
	$(C++) $(CFLAGS) $(shell root-config --cflags) -o $@ -c Make_slow_control_plots.C

Make_slow_control_plots.exe: $(GETSLOWOBJ)
	$(C++) $(CFLAGS) -o $@ ${GETSLOWOBJ} $(shell root-config --libs)

clean:
	rm -f *.o
bootstrap:
	make clean
	make    
 
NAME=file-v1
DATE=$(shell date +%d%m%y)
 
backup:
	mkdir $(NAME)
	cp -r *.C *.h makefile *in *.sh $(NAME)
	tar czf $(NAME).$(DATE).tgz $(NAME)
#        mv *tgz backups/
	rm -rf $(NAME)

