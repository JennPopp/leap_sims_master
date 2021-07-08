outType=single
corethick=75
convthick=1.75
eKin=60
NBunch=100000
outFile=NBunch_${NBunch}_conv_${convthick}mm_core_${corethick}mm_E_${eKin}MeV.root

#set the environment
#source /cvmfs/sft.cern.ch/lcg/releases/LCG_97/Geant4/10.06.p01/x86_64-centos7-gcc8-opt/Geant4-env.sh
#source /cvmfs/sft.cern.ch/lcg/releases/LCG_97/Geant4/10.06.p01/x86_64-centos7-gcc8-opt/bin/geant4.sh
source /cvmfs/sft.cern.ch/lcg/views/LCG_98python3/x86_64-centos7-gcc10-opt/setup.sh

# change parameters in macro
sed  "s/eKin/${eKin}/g" singleout.tmp > singleout.mac
sed -i  "s/corethick/$corethick/g" singleout.mac
sed -i  "s/convthick/$convthick/g" singleout.mac
sed -i  "s/NBunch/$NBunch/g" singleout.mac

# run executable with macro and result file
./leap_sims -m singleout.mac -f $outFile -t $outType # >testOut.txt

mv run0_$outFile ../results_single/run0_$outFile
mv run1_$outFile ../results_single/run1_$outFile

#rm singleout.mac
