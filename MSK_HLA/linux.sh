#!/bin/bash

USAGE="usage: linux.sh [compile] [clean] [execute [process/processor/process_list]]"

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
	rm src/*.class
	rm artifacts/java-msk.jar
	rm -Rf logs/logs
	exit;
fi

############################################
### (target) compile #######################
############################################
if [ $1 = "compile" ]
then
	echo "compiling example federate"
	cd src
	javac -cp ./:$RTI_HOME/lib/portico.jar *.java
	jar -cf ../artifacts/java-msk.jar *.class
	cd ../
	exit;	
fi

############################################
### (target) execute #######################
############################################
if [[ $1 = "execute" && $2 = "processor" ]]
then
	shift;
	java -cp ./java-msk.jar:$RTI_HOME/lib/portico.jar artifacts/msk_project.processor $*
	exit;
fi

if [[ $1 = "execute" && $2 = "process" ]]
then
	shift;
	java -cp ./java-msk.jar:$RTI_HOME/lib/portico.jar artifacts/msk_project.process $*
	exit;
fi

if [[ $1 = "execute" && $2 = "process_list" ]]
then
	shift;
	java -cp ./java-msk.jar:$RTI_HOME/lib/portico.jar artifacts/msk_project.process_list $*
	exit;
fi

echo $USAGE

