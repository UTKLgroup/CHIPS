
## script for sending job

## initial parameters
default_nevents=100
default_nevents_per_job=200
default_nevents_per_hour=10000
default_max_njobs=50
default_max_duration=0
default_max_duration_minutes=0
default_max_nevents=0
default_exe_file="run_cosmic.mac"
default_beam_file="beam.mac"
default_cmd_file="cmd.file"
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
commandslist="commandslist"
directory_prefix="/scratch/01230/fnova/french_outputs"
default_directory_name="test"

#define intro
intro(){
echo "************************"
echo "*** STARTING JOBBER ****"
echo "************************"
echo " "
return 1
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

	 if [ $(bc <<< "$user_nevents_per_job < $min_nevents_per_job") -eq 1 ]; then
	     echo "$user_nevents_per_job is less than the minimum ($min_nevents_per_job), which will be used"
	     default_nevents_per_job=$min_nevents_per_job
	 else
             default_nevents_per_job=$user_nevents_per_job
	 fi
     else
         echo $user_nevents_per_job is not a number
     fi
 fi
}


# read n of events per hour
read_n_events_per_hour(){
 default_nevents_per_hour=$default_nevents_per_hour_single_electron
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

# read directory name                                                                                                                                                                                                
read_directory_name(){
 read user_directory_name
 if [ "$user_directory_name" != "" ]
     then
     default_directory_name=$user_directory_name
 fi
}


 #####################################
 ########## main code here ########## 
 ##################################### 

 intro


 if [ "$1" != "" ]; then
     default_directory_name=$1
 else
     echo " suggest a directory name (default = " $default_directory_name ")"
     read_directory_name
 fi
 echo " the directory name will be " $default_directory_name

 default_exe_file=${default_exe_file}.${default_directory_name}
 default_beam_file=${default_beam_file}.${default_directory_name}
 default_cmd_file=${default_cmd_file}.${default_directory_name}

 if [ "$2" != "" ]; then
     default_nevents_per_hour=$2
 else
 echo " how many events per hour? (default = " $default_nevents_per_hour " )"
 read_n_events_per_hour
 fi
 echo " will run on " $default_nevents_per_hour " events per hour "

 min_nevents_per_job=`echo $default_nevents $default_max_njobs  | awk '{print $1 / $2}'`
 min_nevents_per_job=`round_to_int $min_nevents_per_job`
 
 if [ "$3" != "" ]; then
     default_nevents_per_job=$3
 else
     echo " how many events per job? (default= " $default_nevents_per_job ", minimum=" $min_nevents_per_job ")"
     read_n_events_per_job
 fi
 echo " will run on " $default_nevents_per_job " events per job "

 if [ "$4" != "" ]; then
     default_nevents=$4
 else
     echo " how many events overall? (default= " $default_nevents ")"
     read_n_events
 fi
 echo " will run on " $default_nevents " events "

 if [ $(bc <<< "$default_nevents_per_job < $min_nevents_per_job") -eq 1 ]; then
     default_nevents_per_job=$min_nevents_per_job
 fi

 njobs=`echo $default_nevents $default_nevents_per_job  | awk '{print $1 / $2}'`
 njobs=`round_to_int $njobs`

 duration=`echo $default_nevents_per_job $default_nevents_per_hour | awk '{print $1 / $2 }'`
 duration=`round_to_lower_int $duration`
 
 nevents_per_minute=`echo $default_nevents_per_hour | awk '{print $1 / 60. }'`
 
 duration_minutes=`echo $default_nevents_per_job $duration $default_nevents_per_hour $nevents_per_minute | awk '{print ($1 - $2*$3) / $4 }'`
 duration_minutes=`round_to_int $duration_minutes`
 
 
 echo " will run " $njobs " jobs of " $duration "hours " $duration_minutes  " minutes each, " $default_nevents_per_job " events per job, " $default_nevents " events overall "

 if [ "$1" == "" ]; then
 # decide on next step
     ask_continue_with_next_command
 fi


 export WORKING_DIRECTORY=${directory_prefix}/${default_directory_name}
 [ ! -d ${WORKING_DIRECTORY} ] && mkdir ${WORKING_DIRECTORY}
 export OUTPUT_DIR=${WORKING_DIRECTORY}/output
 [ -e $commandslist$default_directory_name ] && rm $commandslist$default_directory_name


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

   echo "export LD_LIBRARY_PATH=${SCRATCH}/output:${WORKING_DIRECTORY}:/usr/lib64:${HOME}/mylibs:$LD_LIBRARY_PATH ; source ${cadfael_install_directory}/Install-Linux-x86_64/etc/cadfael_setup.sh ; do_cadfael_all_setup ; source ${bayeux_install_directory}/Install-Linux-x86_64/etc/bayeux_setup.sh ; do_bayeux_all_setup ; source ${channel_install_directory}/Install-Linux-x86_64/etc/channel_setup.sh ; do_channel_all_setup ; source ${falaise_install_directory}/Install-Linux-x86_64/etc/falaise_setup.sh ; do_falaise_all_setup ; export CRYHOME=${CRY_DIR}; export CRYDATAPATH=${CRY_DIR}/data; cd ${CHIPS_DIR} ; cp *mac ${OUTPUT_DIR}_${FLAG}/; cp ${default_exe_file} ${OUTPUT_DIR}_${FLAG}/; cp ${default_beam_file} ${OUTPUT_DIR}_${FLAG}/; cp ${default_cmd_file} ${OUTPUT_DIR}_${FLAG}/; cd ${OUTPUT_DIR}_${FLAG}; ${G4INSTALL}/bin/Linux-g++/LXe ${default_exe_file} ${RND} | egrep -v hoton | egrep -v G4MaterialPropertyVector | egrep -v etrieve | egrep -v message  > ${OUTPUT_DIR}_${FLAG}/log.txt 2>&1 "  >> $commandslist$default_directory_name



 done

 FLAG=`echo $FLAG | awk '{print $1 +1 }'`
 ntasks=$FLAG
 nnodes=`echo $ntasks $cores_per_node | awk '{print $1 / $2 }'`
 nnodes=`round_to_higher_int $nnodes`
 nnodes=`echo $nnodes $cores_per_node | awk '{print $1 * $2 }'`

 cp $job_launcher_code $job_launcher_code$default_directory_name.tmp

 sed -i "s/.*commandslist.*/setenv CONTROL_FILE   commandslist$default_directory_name/g" $job_launcher_code$default_directory_name.tmp

 if [ -n "$is_lonestar" ]; then
     sed -i "s/.*ncores.*/\#\$ -pe 12way $nnodes    # pe_name,  ncores multiple of 12 /g" $job_launcher_code$default_directory_name.tmp
     sed -i "s/.*Job name.*/\#\$ -N CHIPSJOB       # Job name/g" $job_launcher_code$default_directory_name.tmp
     sed -i "s/.*Name of stdout output.*/\#\$ -o CHIPSJOB.o$JOB_ID      \# Name of stdout output file/g" $job_launcher_code$default_directory_name.tmp
     sed -i "s/.*Run time.*/\#\$ -l h_rt=${duration}:${duration_minutes}:00/g" $job_launcher_code$default_directory_name.tmp
     qsub $job_launcher_code$default_directory_name.tmp
 elif [ -n "$is_stampede" ]; then
     sed -i "s/.*Total number of nodes.*/\#SBATCH -N $nnodes /g" $job_launcher_code$default_directory_name.tmp
     sed -i "s/.*Total number of mpi.*/\#SBATCH -n $ntasks /g" $job_launcher_code$default_directory_name.tmp
     sed -i "s/.*Job name.*/\#SBATCH -J CHIPSJOB/g" $job_launcher_code$default_directory_name.tmp
     sed -i "s/.*Name of stdout output.*/\#SBATCH -o CHIPSJOB.o$JOB_ID /g" $job_launcher_code$default_directory_name.tmp
     sed -i "s/.*Run time.*/\#SBATCH -t ${duration}:${duration_minutes}:00/g" $job_launcher_code$default_directory_name.tmp
     sbatch $job_launcher_code$default_directory_name.tmp
 fi




 echo " all done"



