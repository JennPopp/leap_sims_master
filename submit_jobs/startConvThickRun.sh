#!/bin/bash
#-------------input parameters--------------------------------------------------
#simulation number
simNo=2
# output Type
outType=bunch #single or bunch

#polarimeter geometry
SetUp=Pol # Pol/Cal/PolCal; Polarimeter, Calorimeter, or both
corethick=300 # thickness of magnet core in mm
convthick=${2} # thickness of converter target in mm

# energy of e-beam
eKin=60 #Energy in MeV
Espread=0.1 #FWHM Energy spread
eneType=Gauss # Gauss for Gaussian energy distribution, Mono for mono-energetic
if [ "$eneType" = "Mono" ]
then
  sigmaE=0
else
  sigmaE= bc <<< "${eKin}*${Espread}"
fi

# beam geometry
NBunch=100000
sourceType=Beam #Beam or Point
if [ "$sourceType" = "Beam" ]
then
  spotSize=0.001 # mm
  div=0.01 # radian
else
  spotSize=0.0 # mm
  div=0.0 # radian
fi

#beam polarization
ePol=1
#-------------------------------------------------------------------------------

#Name of root output file
outFile=${SetUp}_${outType}_NBunch_${NBunch}_E_${eKin}_pm_${sigmaE}MeV_sz_${spotSize}_mm_div_${div}_rad_ePol_${ePol}_conv_${convthick}mm_core_${corethick}mm_PrId${1}.root

#set the environment
source /cvmfs/sft.cern.ch/lcg/views/LCG_98python3/x86_64-centos7-gcc10-opt/setup.sh

# change parameters in macro
sed  "s/eKin/${eKin}/g" test.tmp > test${1}.mac
sed -i  "s/corethick/$corethick/g" test${1}.mac
sed -i  "s/convthick/$convthick/g" test${1}.mac
sed -i  "s/NBunch/$NBunch/g" test${1}.mac
sed -i  "s/sigmaE/$sigmaE/g" test${1}.mac
sed -i  "s/polDeg/$ePol/g" test${1}.mac
sed -i  "s/spotSize/$spotSize/g" test${1}.mac
sed -i  "s/div/$div/g" test${1}.mac

# run executable with macro and result file
./leap_sims -m test${1}.mac -f $outFile -t $outType -v $SetUp

mkdir /nfs/dust/ilc/user/jenpopp/leap_sims/results/simulation_$simNo
mv run0_$outFile /nfs/dust/ilc/user/jenpopp/leap_sims/results/simulation_$simNo/run0_$outFile
mv run1_$outFile /nfs/dust/ilc/user/jenpopp/leap_sims/results/simulation_$simNo/run1_$outFile

mv test${1}.mac /nfs/dust/ilc/user/jenpopp/leap_sims/macros/test${1}.mac
