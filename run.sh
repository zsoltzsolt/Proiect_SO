
MAIN_FILE="main.c"
OTHER_FILES="bmp.c file.c imageStatistics.c"
EXECUTABLE="program"

gcc -Wall -o $EXECUTABLE $MAIN_FILE $OTHER_FILES

./"$EXECUTABLE" $1

if test $? -eq 0 
then echo "Succesfully executed"
fi
