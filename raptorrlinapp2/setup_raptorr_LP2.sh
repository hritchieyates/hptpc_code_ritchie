#Initial setup (needed at every login)
#source /cvmfs/dune.opensciencegrid.org/products/dune/setup_dune.sh
#setup gcc v6_4_0
#setup cmake v3_12_2
#source /scratch0/dbrailsf/software/root/6.14.00/install_linapp2/bin/thisroot.sh
#export LIBCONFIGSYS=/scratch0/dbrailsf/software/libconfig/libconfig/install_linapp2
#export DMTPCSYS=/scratch0/dbrailsf/software/dmtpc/root6_linapp2

#to install (basically the usual instructions)
#git clone git@github.com:HPTPC/raptorr.git
#cd raptorr
#source setup_raptorr.sh
#cd build
#cmake ../
#make install

#to setup an already compiled version of RAPtorr (compiled in linapp1/2)
source /cvmfs/dune.opensciencegrid.org/products/dune/setup_dune.sh
setup gcc v6_4_0
setup cmake v3_12_2
source /scratch0/dbrailsf/software/root/6.14.00/install_linapp2/bin/thisroot.sh
export LIBCONFIGSYS=/scratch0/dbrailsf/software/libconfig/libconfig/install_linapp2
export DMTPCSYS=/scratch0/dbrailsf/software/dmtpc/root6_linapp2
#source setup_raptorr.sh
