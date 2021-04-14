# leap_sims

### How to run leap_sims_master  
1.  set the environment:  
  `source /cvmfs/sft.cern.ch/lcg/releases/LCG_97/Geant4/10.06.p01/x86_64-centos7-gcc8-opt/Geant4-env.sh`  
  `source /cvmfs/sft.cern.ch/lcg/releases/LCG_97/Geant4/10.06.p01/x86_64-centos7-gcc8-opt/bin/geant4.sh`  

2.  make a build directory and change to the build directory:  
  `mkdir build`  
  `cd build`  

3. build the application:  
  `/cvmfs/sft.cern.ch/lcg/releases/LCG_97/CMake/3.14.3/x86_64-centos7-gcc8-opt/bin/cmake ../`  
  `make`  
  `./leap_sims  macfile.mac` 
