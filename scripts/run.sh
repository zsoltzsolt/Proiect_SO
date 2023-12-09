
MAIN_FILE="main.c"
OTHER_FILES="./src/bmp.c ./src/file.c ./src/directory.c ./src/links.c"
EXECUTABLE="./bin/program"

gcc -Wall -o $EXECUTABLE $MAIN_FILE $OTHER_FILES -lm # We need to link the math library used in grayscale transformation

./"$EXECUTABLE" $1 $2 $3 # This 3 arguments are verified in the main.c file

if test $? -eq 0 # If the program exited with 0, then it was executed succesfully
    then echo "Succesfully executed"
fi

