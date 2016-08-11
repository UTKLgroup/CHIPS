
default_source="gps"
default_kind_of_detector="module_chips_detector"
default_nevents=100
default_nevents_per_job=20
default_nevents_per_hour=20
directory_prefix="/scratch/01230/fnova/french_outputs"
commandslist="commandslist"
machine=`hostname`
is_lonestar=`echo ${machine} | grep "ls" `
is_stampede=`echo ${machine} | grep "stampede" `
if [ -n "$is_lonestar" ]; then
    job_launcher_code="joblauncher_ntuplizer.sge.lonestar"
    cores_per_node=12
elif [ -n "$is_stampede" ]; then
    job_launcher_code="joblauncher_ntuplizer.slurm.stampede"
    cores_per_node=16
fi


#round to nearest integer
round_to_int(){
    local result=`echo "tmp=$1+0.5; tmp /= 1; tmp" | bc`
    echo $result
}


#round to lower integer
round_to_lower_int(){
    local result=`echo "tmp=$1; tmp /= 1; tmp" | bc`
    echo $result
}


#round to higher integer
round_to_higher_int(){
    local result=`echo "tmp=$1+1; tmp /= 1; tmp" | bc`
    echo $result
}


# read kind of detector
read_kind_of_detector(){
 read user_kind_of_detector
 if [ "$user_kind_of_detector" != "" ]
     then
     default_kind_of_detector=$user_kind_of_detector
 fi

}



# read simulation mame
read_simulation_name(){
 read user_simulation_name
 if [ "$user_simulation_name" != "" ]
     then
     suffix=${user_simulation_name}
 fi



}



# read n of events
read_n_events(){
 read user_nevents
 if [ "$user_nevents" != "" ]
     then
     if [ $user_nevents -eq $user_nevents 2> /dev/null ]; then
     # $user_nevents is a number
         default_nevents=$user_nevents
     else
         echo $user_nevents is not a number
     fi
 fi
}

# read n of events per job
read_n_events_per_job(){
 read user_nevents_per_job
 if [ "$user_nevents_per_job" != "" ]
     then
     if [ $user_nevents_per_job -eq $user_nevents_per_job 2> /dev/null ]; then
         # $user_nevents_per_job is a number
	 default_nevents_per_job=$user_nevents_per_job
     else
         echo $user_nevents_per_job is not a number
     fi
 fi
}


# read n of events per hour
read_n_events_per_hour(){
 read user_nevents_per_hour
 if [ "$user_nevents_per_hour" != "" ]
     then
     if [ $user_nevents_per_hour -eq $user_nevents_per_hour 2> /dev/null ]; then
         # $user_nevents_per_job is a number
         default_nevents_per_hour=$user_nevents_per_hour
     else
         echo $user_nevents_per_hour is not a number
     fi
 fi
}



# ask if continue with next command
ask_continue_with_next_command(){
    echo " continue? [Y/n]"
    read a
 
    # exit                                                                                                                                                                                                                                         
    if [ "$a" != "Y" ] && [ "$a" != "y" ] && [ "$a" != "" ] 
	then
	exit
    fi
}



#define intro
intro(){
echo "************************"
echo "*** STARTING CHIPS JOBBER ****"
echo "************************"
echo " "
return 1
}



 #####################################
 ########## main code here ########## 
 ##################################### 

 intro

 if [ "$1" != "" ]; then
     default_kind_of_detector=$1
 else
     echo " suggest a kind of detector (full_chips_detector, module_chips_detector; default = " $default_kind_of_detector ")"
     read_kind_of_detector
 fi
 echo " the kind of detector will be " $default_kind_of_detector

 if [ "${default_kind_of_detector}" == "full_chips_detector" ]; then
     g4runfile_base="run_gun__full_chips_detector.mac"
     g4beamfile_base="beam__full_chips_detector.mac"
 elif [ "${default_kind_of_detector}" == "module_chips_detector" ]; then
     g4runfile_base="run_gun__module_chips_detector.mac"
     g4beamfile_base="beam__module_chips_detector.mac"
 else
     echo " problem: unknown kind of detector " ${default_kind_of_detector}
     exit
 fi

 suffix=${default_source}"__"${default_kind_of_detector}
 if [ "$2" != "" ]; then
     suffix=$2
 else
     echo " suggest simulation name (default = " ${suffix} ")"
     read_simulation_name
 fi
 echo " the simulation name will be " ${suffix}


 if [ "$3" != "" ]; then
     default_nevents_per_hour=$3
 else
     echo " how many events per hour? (default = " $default_nevents_per_hour ")"
     read_n_events_per_hour
 fi
 echo " will run on " $default_nevents_per_hour " events per hour "


 if [ "$4" != "" ]; then
     default_nevents_per_job=$4
 else
     echo " how many events per job? (default= " $default_nevents_per_job ")"
     read_n_events_per_job
 fi
 echo " will run on " $default_nevents_per_job " events per job "


 if [ "$5" != "" ]; then
     default_nevents=$5
 else
     echo " how many events overall? (default= " $default_nevents ")"
     read_n_events
 fi
 echo " will run on " $default_nevents " events "


 njobs=`echo $default_nevents $default_nevents_per_job  | awk '{print $1 / $2}'`
 njobs=`round_to_int $njobs`


 duration=`echo $default_nevents_per_job $default_nevents_per_hour | awk '{print $1 / $2 }'`
 duration=`round_to_lower_int $duration`
 

 nevents_per_minute=`echo $default_nevents_per_hour | awk '{print $1 / 60. }'`
 
 duration_minutes=`echo $default_nevents_per_job $duration $default_nevents_per_hour $nevents_per_minute | awk '{print ($1 - $2*$3) / $4 }'`
 duration_minutes=`round_to_int $duration_minutes`
 
 
 echo " will run " $njobs " jobs of " $duration "hours " $duration_minutes  " minutes each, " $default_nevents_per_job " events per job, " $default_nevents " events overall "


 if [ "$1" == "" ]; then
     ask_continue_with_next_command
 fi

 g4runfile=${g4runfile_base}.${suffix}
 cp ${g4runfile_base} ${g4runfile}
    
 g4beamfile=${g4beamfile_base}.${suffix}
 cp ${g4beamfile_base} ${g4beamfile}
    
 sed -i "s/.*beamOn.*/\/run\/beamOn ${default_nevents_per_job} /g" ${g4runfile}
 sed -i "s/.*execute beam.*/\/control\/execute ${g4beamfile} /g" ${g4runfile}
    

 export WORKING_DIRECTORY=${directory_prefix}/${suffix}
 [ ! -d ${WORKING_DIRECTORY} ] && mkdir ${WORKING_DIRECTORY}
 export OUTPUT_DIR=${WORKING_DIRECTORY}/output
 [ -e $commandslist$suffix ] && rm $commandslist$suffix


 for (( c=1; c<=$njobs; c++ ))
   do
   FLAG="$c"
   RND="$(date +%6N)"
   if [ -d "${OUTPUT_DIR}_${FLAG}" ]; then
       FLAG="$(date +%y%m%d%s)"
       echo " folder $c already exists, I will call it ${FLAG}"
   fi
   echo " preparing job $FLAG"
   mkdir ${OUTPUT_DIR}_${FLAG}

   cd ${CHIPS_DIR} 
   cp defaults.mac  ${OUTPUT_DIR}_${FLAG}/ 
   cp detector_chips.mac ${OUTPUT_DIR}_${FLAG}/ 
   cp detector_chips_module.mac ${OUTPUT_DIR}_${FLAG}/ 
   cp ${g4runfile} ${OUTPUT_DIR}_${FLAG}/ 
   cp ${g4beamfile} ${OUTPUT_DIR}_${FLAG}/ 

   echo "export LD_LIBRARY_PATH=${SCRATCH}/output:${WORKING_DIRECTORY}:/usr/lib64:${HOME}/mylibs:$LD_LIBRARY_PATH ; source ${cadfael_install_directory}/Install-Linux-x86_64/etc/cadfael_setup.sh ; do_cadfael_all_setup ; source ${bayeux_install_directory}/Install-Linux-x86_64/etc/bayeux_setup.sh ; do_bayeux_all_setup ; source ${channel_install_directory}/Install-Linux-x86_64/etc/channel_setup.sh ; do_channel_all_setup ; source ${falaise_install_directory}/Install-Linux-x86_64/etc/falaise_setup.sh ; do_falaise_all_setup ; export CRYHOME=${CRY_DIR}; export CRYDATAPATH=${CRY_DIR}/data; cd ${OUTPUT_DIR}_${FLAG}; ${G4INSTALL}/bin/Linux-g++/LXe ${g4runfile} ${RND} | egrep -v hoton | egrep -v G4MaterialPropertyVector | egrep -v etrieve | egrep -v message  > ${OUTPUT_DIR}_${FLAG}/log.txt 2>&1 "  >> $commandslist$suffix

 done


 FLAG=`echo $FLAG | awk '{print $1 +1 }'`
 ntasks=$FLAG
 nnodes=`echo $ntasks $cores_per_node | awk '{print $1 / $2 }'`
 nnodes=`round_to_higher_int $nnodes`
 nnodes=`echo $nnodes $cores_per_node | awk '{print $1 * $2 }'`

 cp $job_launcher_code $job_launcher_code$suffix.tmp

 sed -i "s/.*commandslist.*/setenv CONTROL_FILE   commandslist$suffix/g" $job_launcher_code$suffix.tmp

 if [ -n "$is_lonestar" ]; then
     sed -i "s/.*ncores.*/\#\$ -pe 12way $nnodes    # pe_name,  ncores multiple of 12 /g" $job_launcher_code$suffix.tmp
     sed -i "s/.*Job name.*/\#\$ -N CHIPSJOB       # Job name/g" $job_launcher_code$suffix.tmp
     sed -i "s/.*Name of stdout output.*/\#\$ -o CHIPSJOB.o$JOB_ID      \# Name of stdout output file/g" $job_launcher_code$suffix.tmp
     sed -i "s/.*Run time.*/\#\$ -l h_rt=${duration}:${duration_minutes}:00/g" $job_launcher_code$suffix.tmp
     qsub $job_launcher_code$suffix.tmp
 elif [ -n "$is_stampede" ]; then
     sed -i "s/.*Total number of nodes.*/\#SBATCH -N $nnodes /g" $job_launcher_code$suffix.tmp
     sed -i "s/.*Total number of mpi.*/\#SBATCH -n $ntasks /g" $job_launcher_code$suffix.tmp
     sed -i "s/.*Job name.*/\#SBATCH -J CHIPSJOB/g" $job_launcher_code$suffix.tmp
     sed -i "s/.*Name of stdout output.*/\#SBATCH -o CHIPSJOB.o$JOB_ID /g" $job_launcher_code$suffix.tmp
     sed -i "s/.*Run time.*/\#SBATCH -t ${duration}:${duration_minutes}:00/g" $job_launcher_code$suffix.tmp
     sbatch $job_launcher_code$suffix.tmp
 fi




 echo " all done"




