#!/bin/bash

Help()
{
	# Display help
	echo "Builds the specified components of the project."
	echo
	echo "Syntax: scriptTemplate [--t|h]"
	echo "options:"
	echo "t	    Specifiy the target to build: [esp|server|all]"	
	echo "h		Print this Help."
	echo
}

check_idf()
{
	if ! command -v idf.py >/dev/null
	then 
		echo "Can't see idf.py"
		echo "running get_idf"
		get_idf
	fi
}

build_esp()
{
	check_idf
	idf.py build
}

build_server()
{
	echo "build server is not implemented"
}

build_all()
{
	echo "build all is not implemented."
}

# Get the options
while getopts ":ht:" option; do
	case $option in 
		h)	# display Help
			Help
			exit;;
		t)  # Enter a target
			target=$OPTARG;;
		\?) # Invalid option
			echo "Error: Invalid option"
			exit;;
	esac
done

case $target in
	esp) # build for esp
		build_esp
		exit;;
	all) # build for all
		build_all
	    exit;;
	server) # build for server
		build_server
		exit;;
esac

build_all
