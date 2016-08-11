
## clean up jobs

export OUTPUT_DIRECTORY=${SCRATCH}/output
export WORKING_DIRECTORY=${SCRATCH}/french_outputs

scriptlog='CHIPSJOB'
outputname='output_'
rootoutput='chips.root'
rootoutput2='chips_data.root'
temp_filename="tmp.root"
temp_logfile="out"
job_launcher_code="joblauncher.slurm.stampede.tmp"
directory_prefix=${WORKING_DIRECTORY}
default_directory_name=""
txtfile="fhit.txt"

check_directory_name(){
dirlist=`ls $directory_prefix | egrep -v *.so | egrep -v "bin" | egrep -v "sng4.d" `
if [ -n "$dirlist" ]; then
    echo " the possible directory names are:" $dirlist
    default_directory_name=`echo $dirlist | awk '{print $1}'`
    echo " which directory would you like to clean? (default = " $default_directory_name ")"
    read user_directory_name
    if [ "$user_directory_name" != "" ]; then 
	while [ ! -d "$directory_prefix/$user_directory_name" ]; do
	    echo " the directory " $directory_prefix/$user_directory_name " does not exist, please choose again "
	    read user_directory_name
	done
	default_directory_name=$user_directory_name
    fi
else
    echo " there are no directories to be cleaned "
fi
}


# check if ROOT file is good and merge it
merge_root_file(){
    local filefolder=$1
    echo " merging " $filefolder
    local list_of_folders=`ls $filefolder`
    local list_of_good_files=''
    for folder in $list_of_folders; do
	local filename=$filefolder/$folder/$rootoutput
	echo "checking file " $filename

	if [ -e "$filename" ]; then
	    mv $filename $temp_filename
	    root -l test_file.C 1>$temp_logfile 2>/dev/null
	    result_zombie=`grep zombie $temp_logfile`
	    if [ -n "$result_zombie" ]; then
		echo " ... ... file " $filename " is a zombie "
		mv $temp_filename $filename
	    fi
	    
	    result_good=`grep good $temp_logfile`
	    if [ -n "$result_good" ]; then
		echo " ... ... adding " $filename " to list of good files"
		list_of_good_files=`echo $list_of_good_files $filename`
		mv $temp_filename $filename
	    fi
	    
	else
	    echo " ... file " $filename " was not found "
	fi

    done

    echo " merging files " $list_of_good_files
    hadd $default_output_name.root $list_of_good_files
    mv $default_output_name.root $filefolder/

    echo " merging files " ${txtfile}


    local filefolder=$1
    echo " merging " $filefolder
    local list_of_folders=`ls $filefolder`
    local list_of_good_files=''
    for folder in $list_of_folders; do
	local filename=$filefolder/$folder/$rootoutput2
	echo "checking file " $filename

	if [ -e "$filename" ]; then
	    mv $filename $temp_filename
	    root -l test_file.C 1>$temp_logfile 2>/dev/null
	    result_zombie=`grep zombie $temp_logfile`
	    if [ -n "$result_zombie" ]; then
		echo " ... ... file " $filename " is a zombie "
		mv $temp_filename $filename
	    fi
	    
	    result_good=`grep good $temp_logfile`
	    if [ -n "$result_good" ]; then
		echo " ... ... adding " $filename " to list of good files"
		list_of_good_files=`echo $list_of_good_files $filename`
		mv $temp_filename $filename
	    fi
	    
	else
	    echo " ... file " $filename " was not found "
	fi

    done

    echo " merging files " $list_of_good_files
    hadd ${default_output_name}_data.root $list_of_good_files
    mv ${default_output_name}_data.root $filefolder/

    echo " merging files " ${txtfile}

}




if [ "$1" != "" ]; then
    default_directory_name=$1
else
    check_directory_name
fi

if [ -n "$default_directory_name" ]; then
    echo " will clean directory " $default_directory_name
fi


if [ "$1" == "" ]; then

    logslist=`ls $scriptlog*`
    if [ -n "$logslist" ]; then
	echo "delete the following script logs? [Y/n]"
	echo $logslist
	read a
	if [ "$a" == "Y" ] || [ "$a" == "y" ] || [ "$a" == "" ]
	then
	    rm $logslist
	fi
    fi
    
fi

if [ -n "$default_directory_name" ]; then
    
    if [ "$1" == "" ]; then
	
	rootslist=`ls $directory_prefix/$default_directory_name/*root`
	if [ -n "$rootslist" ]; then
            echo "delete the following roots? [Y/n]"
            echo $rootslist
            read a
            if [ "$a" == "Y" ] || [ "$a" == "y" ] || [ "$a" == "" ]
            then
		rm $rootslist
            fi
	fi
    fi


    outputslist=`ls ${WORKING_DIRECTORY}/$default_directory_name/output_*/*root | awk -F "$default_directory_name/" '{print $2}' | awk -F "/" '{print $1}'`
    if [ -n "$outputslist" ]; then

	if [ "$1" != "" ]; then
	    a="Y"
	else
	    echo "merge the following outputs? [Y/n]"
	    echo $outputslist	
	    read a
	fi

	if [ "$a" == "Y" ] || [ "$a" == "y" ] || [ "$a" == "" ]
	then
	    default_output_name=$default_directory_name

            if [ "$2" != "" ]; then
		outputname=$2
	    else
		echo "here is a list of used directories"
		ls -ltrd ${OUTPUT_DIRECTORY}/* | awk -F "${OUTPUT_DIRECTORY}/" '{print $2}'
		echo "suggest a global name for these runs (default=" $default_output_name ", directory name is " $default_directory_name " )"
		read outputname
	    fi

	    if [ "$outputname" != "" ]
	    then
		default_output_name=$outputname
		while [ -d "$default_output_name" ]; do
		    echo "directory already exist, please give another name"
		    read outputname
		    default_output_name=$outputname
		done
		
	    fi
	    full_output_name=${OUTPUT_DIRECTORY}/${default_output_name}
	    mkdir $full_output_name
	    for folder in $outputslist; do
		echo " moving " $folder " to " $full_output_name
		mv ${WORKING_DIRECTORY}/$default_directory_name/$folder $full_output_name/
	    done
	    merge_root_file $full_output_name
	    cat ${full_output_name}/output_*/${txtfile} >> ${full_output_name}/${txtfile}
	else
            if [ "$1" == "" ]; then
		echo "delete the outputs? [Y/n]"
		read a
	    fi
	    if [ "$a" == "Y" ] || [ "$a" == "y" ] || [ "$a" == "" ]
	    then
		rm -rf $outputslist
	    fi
	fi
    fi

    outputslist=`ls ${WORKING_DIRECTORY}/$default_directory_name/output_*/${txtfile} | awk -F "$default_directory_name/" '{print $2}' | awk -F "/" '{print $1}'`
    if [ -n "$outputslist" ]; then

	if [ "$1" != "" ]; then
	    a="Y"
	else
	    echo "merge the following txt files? [Y/n]"
	    echo $outputslist	
	    read a
	fi

	if [ "$a" == "Y" ] || [ "$a" == "y" ] || [ "$a" == "" ]
	then
	    default_output_name=$default_directory_name

            if [ "$2" != "" ]; then
		outputname=$2
	    else
		echo "here is a list of used directories"
		ls -ltrd ${OUTPUT_DIRECTORY}/* | awk -F "${OUTPUT_DIRECTORY}/" '{print $2}'
		echo "suggest a global name for these runs (default=" $default_output_name ", directory name is " $default_directory_name " )"
		read outputname
	    fi

	    if [ "$outputname" != "" ]
	    then
		default_output_name=$outputname
		while [ -d "$default_output_name" ]; do
		    echo "directory already exist, please give another name"
		    read outputname
		    default_output_name=$outputname
		done
		
	    fi
	    full_output_name=${OUTPUT_DIRECTORY}/${default_output_name}
	    mkdir $full_output_name
	    for folder in $outputslist; do
		echo " moving " $folder " to " $full_output_name
		mv ${WORKING_DIRECTORY}/$default_directory_name/$folder $full_output_name/
	    done
	    cat ${full_output_name}/output_*/${txtfile} >> ${full_output_name}/${txtfile}
	else
            if [ "$1" == "" ]; then
		echo "delete the outputs? [Y/n]"
		read a
	    fi
	    if [ "$a" == "Y" ] || [ "$a" == "y" ] || [ "$a" == "" ]
	    then
		rm -rf $outputslist
	    fi
	fi
    fi

    outputslistdelete=`ls $directory_prefix/$default_directory_name/output_* | grep run | awk -F ":" '{print $1}'`
    if [ -z "$outputslistdelete" ]; then
        outputslistdelete=`ls -d $directory_prefix/$default_directory_name/output_*`
    fi
    if [ -n "$outputslistdelete" ]; then

        if [ "$1" == "" ]; then
	    echo "delete these outputs? [Y/n]"
	    echo $outputslistdelete
	    read a
	fi
	if [ "$a" == "Y" ] || [ "$a" == "y" ] || [ "$a" == "" ]
	then
            rm -rf $outputslistdelete
	fi
    fi
    
    if [ "$1" == "" ]; then
	echo " delete the directory " $directory_prefix/$default_directory_name "? [Y/n]"
	read a
    fi
    if [ "$a" == "Y" ] || [ "$a" == "y" ] || [ "$a" == "" ]
    then
	rm -rf  $directory_prefix/$default_directory_name
    fi
fi

[ -e $job_launcher_code ] && rm $job_launcher_code

echo "all done"


