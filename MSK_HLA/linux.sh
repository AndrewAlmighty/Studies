#!/bin/bash

USAGE="usage: linux.sh [compile] [clean] [execute [Process/Processor/ProcessList]]"

################################
# check command line arguments #
################################
if [ $# = 0 ]
then
	echo $USAGE
	exit;
fi

######################
# test for JAVA_HOME #
######################
JAVA=java
if [ "$JAVA_HOME" = "" ]
then
	echo WARNING Your JAVA_HOME environment variable is not set!
	#exit;
else
        JAVA=$JAVA_HOME/bin/java
fi

#####################
# test for RTI_HOME #
#####################
if [ "$RTI_HOME" = "" ]
then
	echo WARNING Your RTI_HOME environment variable is not set
	exit;
fi

############################################
### (target) clean #########################
############################################
if [ $1 = "clean" ]
then
	echo "deleting example federate jar file and left over logs"
	rm src/msk_project/*.class
	rm src/java-msk.jar
	rm -Rf src/logs
	exit;
fi

############################################
### (target) compile #######################
############################################
if [ $1 = "compile" ]
then
	echo "compiling example federate"
	cd src
	
	javac -cp ./:$RTI_HOME/lib/portico.jar msk_project/*.java
	jar -cf java-msk.jar msk_project/*.class
	exit;	
fi

############################################
### (target) execute #######################
############################################
if [[ $1 = "execute" && $2 = "Processor" ]]
then
	shift;
	cd src
	java -cp ./java-msk.jar:$RTI_HOME/lib/portico.jar msk_project.ProcessorFederate
	exit;
fi

if [[ $1 = "execute" && $2 = "Process" ]]
then
	shift;
	cd src
	java -cp ./java-msk.jar:$RTI_HOME/lib/portico.jar msk_project.ProcessFederate
	exit;
fi

if [[ $1 = "execute" && $2 = "ProcessList" ]]
then
	shift;
	cd src
	java -cp ./java-msk.jar:$RTI_HOME/lib/portico.jar msk_project.ProcessListFederate
	exit;
fi

echo $USAGE

