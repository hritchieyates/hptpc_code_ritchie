C++      = g++
CFLAGS   = -g -w -O3
GETDISOBJ=DIS_validations_reweight_comparison.o
GETDISFLUXOBJ=DIS_validations_reweight_comparison_flux_scaling.o
GETCHECKOBJ=Check_interpolation_fixed_Q2_vs_enu.o
GETPLOTOBJ=plot_x_vs_Q2.o
PROBSINCLUDE = /vols/t2k/users/ea2817/Software/Probs3++/Prob3++.20121225 
GETCLEANOBJ=cuts_2D_plots_macro.o
GETPLOTOBJ=plots_for_harrison_macro.o
GETTRIPOBJ=make_triptychs.o
GETREGOBJ=make_region_plots.o

all: cuts_2D_plots_macro.exe plots_for_harrison_macro.exe make_triptychs.exe make_region_plots.exe

cuts_2D_plots_macro.o: cuts_2D_plots_macro.C
	$(C++) $(CFLAGS) $(shell root-config --cflags) -o cuts_2D_plots_macro.o -c cuts_2D_plots_macro.C

cuts_2D_plots_macro.exe: $(GETCLEANOBJ)
	$(C++) $(CFLAGS) -o cuts_2D_plots_macro.exe cuts_2D_plots_macro.o $(shell root-config --libs)

plots_for_harrison_macro.o: plots_for_harrison_macro.C
	$(C++) $(CFLAGS) $(shell root-config --cflags) -o plots_for_harrison_macro.o -c plots_for_harrison_macro.C

plots_for_harrison_macro.exe: $(GETPLOTOBJ)
	$(C++) $(CFLAGS) -o plots_for_harrison_macro.exe plots_for_harrison_macro.o $(shell root-config --libs)

make_triptychs.o: make_triptychs.C
	        $(C++) $(CFLAGS) $(shell root-config --cflags) -o make_triptychs.o -c make_triptychs.C

make_triptychs.exe: $(GETTRIPOBJ)
	        $(C++) $(CFLAGS) -o make_triptychs.exe make_triptychs.o $(shell root-config --libs)

make_region_plots.o: make_region_plots.C
	        $(C++) $(CFLAGS) $(shell root-config --cflags) -o make_region_plots.o -c make_region_plots.C

make_region_plots.exe: $(GETREGOBJ)
	        $(C++) $(CFLAGS) -o make_region_plots.exe make_region_plots.o $(shell root-config --libs)


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
