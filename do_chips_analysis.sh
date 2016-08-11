
variable="nsigmas"
n_events_per_hour=100
n_events_per_job=100

g4runfile_base="run.mac"
g4beamfile_base="beam.mac"
paramfile_base="parameters.txt"

for energy in '100' '200' '300' '400' '500' '600' '700' '800' '900' '1000'; do

    suffix="E${energy}MeV"

    paramfile=${paramfile_base}.${suffix}
    cp ${paramfile_base} ${paramfile}
    sed -i "s/.*E.*/E   ${energy}/g" ${paramfile}

    g4beamfile=${g4beamfile_base}.${suffix}
    cp ${g4beamfile_base} ${g4beamfile}

    g4runfile=${g4runfile_base}.${suffix}
    cp ${g4runfile_base} ${g4runfile}

    n_all_events=`grep n_all_events ${paramfile} | awk '{print $2}'`
    x0=`grep x0 ${paramfile} | awk '{print $2}'`
    y0=`grep y0 ${paramfile} | awk '{print $2}'`
    z0=`grep z0 ${paramfile} | awk '{print $2}'`
    E=`grep E ${paramfile} | awk '{print $2}'`
    
    echo " doing " ${suffix} " n_all_events " ${n_all_events} " x0 " ${x0} " y0 " ${y0} " z0 " ${z0} " E " ${E}
    
    sed -i "s/.*position.*/\/gun\/position ${x0} ${y0} ${z0} m /g" ${g4beamfile}
    sed -i "s/.*energy.*/\/gun\/energy ${E} MeV/g" ${g4beamfile}
    sed -i "s/.*beamOn.*/\/run\/beamOn ${n_all_events} /g" ${g4runfile}
    sed -i "s/.*execute beam.*/\/control\/execute ${g4beamfile} /g" ${g4runfile}
    
    bash jobber.sh ${suffix} ${n_events_per_hour} ${n_events_per_job} ${n_all_events}

done



