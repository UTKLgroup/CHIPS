##############################################################
###   depth       PMT       n ev / hour    n ev / hour adhoc
#
#      60        radial         20              1000
#      60         down          20


kind_of_detector="module_chips_detector"
date="2014_06_30"
simulation_name="gps__CHIPS__refl_10__at_42__depth_60__2_mirrors"
nevents_per_hour=500
nevents_per_job=500
nevents=10000


name=${date}_${simulation_name}
echo " doing " ${kind_of_detector} ${name} ${nevents_per_hour} ${nevents_per_job} ${nevents} 
	
./do_chips_analysis_gps.sh ${kind_of_detector} ${name} ${nevents_per_hour} ${nevents_per_job} ${nevents} 
	




