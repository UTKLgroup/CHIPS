
## script for sending job

## initial parameters
default_nevents_per_job=200
default_nevents_per_hour=1000000
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
directory_prefix="/scratch/01230/fnova/output"
directory_prefix_sed="\/scratch\/01230\/fnova\/output"
fhit_file="fhit.txt"
coordinates_file="coordinates.root"
temp_filename="tmp.root"
temp_logfile="out"
default_directory_name="test"
input_file_name="test.root"
exe_code="Exe_count_file.C"
exe_code_tmp="Exe.C"
plot_file="plot_coordinates.C"

#define intro
intro(){
echo "************************"
echo "*** STARTING JOBBER ****"
echo "************************"
echo " "
return 1
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
  cd ${directory_prefix}
  ls -ltr
  cd -
  echo " which directory to plot? "
  read user_directory_name
  while [ ! -d "${directory_prefix}/${user_directory_name}" ] || [ ! -f "${directory_prefix}/${user_directory_name}/${fhit_file}" ]; do
      echo " the directory " ${directory_prefix}/${user_directory_name} " or the file " ${directory_prefix}/${user_directory_name}/${fhit_file} " does not exist, choose again "
      read user_directory_name
  done
  default_directory_name=${user_directory_name}
  input_file_name="${default_directory_name}_data.root"
}


 #####################################
 ########## main code here ########## 
 ##################################### 

 intro

 read_directory_name
 echo " I will plot the file " ${input_file_name}

 echo ${directory_prefix}/${user_directory_name}/${input_file_name} > tmp
 sed -i 's/\//\\\//g' tmp
 fixed_input_file=`cat tmp`
 rm tmp
 cp $exe_code $exe_code_tmp
 sed -i "s/QQQ/${fixed_input_file}/g" $exe_code_tmp
 temp_logfile="out"
 root -l $exe_code_tmp 1>$temp_logfile 2>/dev/null
 result_zombie=`grep zombie $temp_logfile`
 if [ -n "$result_zombie" ]; then
     echo " file is a zombie, cannot root it "
     exit
 fi
 all_events=`grep events $temp_logfile | awk -F "events" '{print $2}' `
 if [ -z "$all_events" ]; then
     echo " problem: no n of events assigned "
     exit 0
 fi
 echo " there are " $all_events " events in the file "
 
 default_nevents=${all_events}

 echo " how many events per hour? (default = " $default_nevents_per_hour " )"
 read_n_events_per_hour
 echo " will run on " $default_nevents_per_hour " events per hour "

 duration=`echo $default_nevents $default_nevents_per_hour | awk '{print $1 / $2 }'`
 duration=`round_to_lower_int $duration`
 
 nevents_per_minute=`echo $default_nevents_per_hour | awk '{print $1 / 60. }'`
 
 duration_minutes=`echo $default_nevents $duration $default_nevents_per_hour $nevents_per_minute | awk '{print ($1 - $2*$3) / $4 }'`
 duration_minutes=`round_to_int $duration_minutes`
 
 
 echo " will run 1 job of " $duration "hours " $duration_minutes  " minutes each, " $default_nevents " lines overall "

 if [ "$1" == "" ]; then
 # decide on next step
     ask_continue_with_next_command
 fi


 export WORKING_DIRECTORY=${directory_prefix}/${default_directory_name}
 [ ! -d ${WORKING_DIRECTORY} ] && mkdir ${WORKING_DIRECTORY}
 export OUTPUT_DIR=${WORKING_DIRECTORY}/output
 [ -e $commandslist$default_directory_name ] && rm $commandslist$default_directory_name
 
 sed -i "s/.*std::string direc.*/std::string direc=\"${default_directory_name}\";/g" ${plot_file}
 cp ${plot_file} ${directory_prefix}/${default_directory_name}


 echo "export LD_LIBRARY_PATH=${SCRATCH}/output:${WORKING_DIRECTORY}:/usr/lib64:${HOME}/mylibs:$LD_LIBRARY_PATH ; source ${cadfael_install_directory}/Install-Linux-x86_64/etc/cadfael_setup.sh ; do_cadfael_all_setup ; source ${bayeux_install_directory}/Install-Linux-x86_64/etc/bayeux_setup.sh ; do_bayeux_all_setup ; source ${channel_install_directory}/Install-Linux-x86_64/etc/channel_setup.sh ; do_channel_all_setup ; source ${falaise_install_directory}/Install-Linux-x86_64/etc/falaise_setup.sh ; do_falaise_all_setup ; export CRYHOME=${CRY_DIR}; export CRYDATAPATH=${CRY_DIR}/data; mkdir ${directory_prefix}/${default_directory_name}/figure; cd ${directory_prefix}/${default_directory_name}; root ${plot_file} "  >> $commandslist$default_directory_name

 ntasks=1
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



