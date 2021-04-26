#!/bin/bash
 
corethick=${2}
convthick=2
eKin=50
outFile=conv_${convthick}mm_core_${corethick}mm_E_${eKin}MeV_PrId${1}.root

#set the environment 
source /cvmfs/sft.cern.ch/lcg/releases/LCG_97/Geant4/10.06.p01/x86_64-centos7-gcc8-opt/Geant4-env.sh
source /cvmfs/sft.cern.ch/lcg/releases/LCG_97/Geant4/10.06.p01/x86_64-centos7-gcc8-opt/bin/geant4.sh

# change parameters in macro
sed  "s/eKin/${eKin}/g" test.tmp > test${1}.mac
sed -i  "s/corethick/$corethick/g" test${1}.mac
sed -i  "s/convthick/$convthick/g" test${1}.mac

# run executable with macro and result file 
./e166_asym test${1}.mac $outFile

mv run0_$outFile ../results/run0_$outFile
mv run1_$outFile ../results/run1_$outFile

mv test${1}.mac ../macros/test${1}.mac

