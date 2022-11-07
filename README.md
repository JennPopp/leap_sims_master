# leap_sims

### How to run leap_sims_detector_test
1.  set the environment:  
  `source /cvmfs/sft.cern.ch/lcg/views/LCG_102b/x86_64-centos7-gcc11-opt/setup.sh`  

2.  make a build directory and change to the build directory:  
  `mkdir build`  
  `cd build`  

3. build the application:  
  `cmake ../`  
  `make`  
4. run it in batch mode with macro
  `./leap_sims  -m macfile.mac -f outFileName -t outType -v version`

  `-m` specifies the name of the macrofile and starts the run in batch mode
  `-f` specifies the name of the ttree saved
  `-t` specifies the what is saved to the ttree. Use `bunch` to save the photon energy sum, generated by one electron bunch. Use `single` to save information about every photon passing the detector volume.   
  `-v` specifies the version of the code that is used. `PolCal` simulates polarimeter and calorimeter. `Pol` simulates only the polarimeter. `Cal` simulates only the calorimeter.

  **Important**: If outType `single` is used, it is not recommended to simulate more than one electron bunch!

  ### How to submit loads of jobs to the NAF
  1. cd into the submit_jobs directory
  2. There are three bash skripts (files with `.sh` ending) to start runs varying the energy of the incident electrons, the core thickness, or the thickness of the converter target. Edit them to set the parameters you want for the run.
  3. to submit jobs for a run where the energy varies:
  `condor_submit EkinRun.sub`   
  4. watch the progress
  `watch condor_q`
  5. to stop watching simply press `ctrl` `c`

  ### How to run Visualization
  1. cd into the build directory
  2. run the user interaction mode without macro and name the version you want to see, use
     `Pol`,`Cal` or `PolCal`
  `./leap_sims -v version`   
  2. use the OGL (default). To visualize a certain plane use
     `/vis/viewer/set/sectionPlane [on|off] <3 vector of point> [unit of point] <3 vector of plane normal>`
     e.g., for a y-z plane at x = 1 cm:
     `/vis/viewer/set/sectionPlane on 1 0 0 cm 1 0 0`
  3. for HepRep type in `/vis/open HepRepFile` and `/vis/viewer/flush` to save a         HepRepFile of the geometry
  4. copy the HepRepFiles to the HepRep directory
  5. change to HepRep directory and type `java -jar HepRApp.jar` to open the HepRep Browser
  6. select the HepRapFile you want to see

  ### Some useful detector commands
  1. To set the number of calorimeter crystals use `/leap/det/Setnumberofcrystals`.
     choose either `one` for one centered crystals of `nine` for a matrix of nine crystals. The default is `one`!
