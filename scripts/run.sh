
MAIN_FILE="main.c"
OTHER_FILES="./src/bmp.c ./src/file.c ./src/directory.c ./src/links.c"
EXECUTABLE="./bin/program"

gcc -Wall -o $EXECUTABLE $MAIN_FILE $OTHER_FILES -lm

./"$EXECUTABLE" $1 $2 $3

if test $? -eq 0 
then echo "Succesfully executed"
fi

