

kind_of_detector="module_chips_detector"
date="2014_04_24"
simulation_name="gun__CHIPS__refl_0_10_not_looking_down__yes_bottom"
nevents_per_hour=60
nevents_per_job=25
nevents=100
gun_position_x0=2.
gun_position_z0_center=-19.9613
gun_direction_x0=-1.
gun_direction_y0=0
gun_direction_z0=0
energy=1500


for the_gun_position_y0 in '-1.5' '-1' '-0.5' '0' '0.5' '1' '1.5' ; do
    gun_position_y0=${the_gun_position_y0}
    for the_gun_position_z0 in '-1.5' '-1' '-0.5' '0' '0.5' '1' '1.5' ; do
	gun_position_z0=`echo ${gun_position_z0_center} ${the_gun_position_z0} | awk '{SUM=$1 + $2; print SUM}' `
	
	name=${date}_posx_${gun_position_x0}_posy_${gun_position_y0}_posz_${gun_position_z0}_${simulation_name}
	echo " doing " ${kind_of_detector} ${name} ${nevents_per_hour} ${nevents_per_job} ${nevents} ${gun_position_x0} ${gun_position_y0} ${gun_position_z0} ${gun_direction_x0} ${gun_direction_y0} ${gun_direction_z0} ${energy}
	
	./do_chips_analysis_gun.sh ${kind_of_detector} ${name} ${nevents_per_hour} ${nevents_per_job} ${nevents} ${gun_position_x0} ${gun_position_y0} ${gun_position_z0} ${gun_direction_x0} ${gun_direction_y0} ${gun_direction_z0} ${energy}
	
    done
done




