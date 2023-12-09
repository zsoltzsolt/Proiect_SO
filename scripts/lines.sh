
# We need 2 arguments: file name and number of lines
if test $# -ne 2 
then
    echo "Invalid number of arguments"
else 
    if test ! -f $1 # We verify if the first argument is a file
    then
        echo "File does not exist"
    else 
        if echo $2 | grep -q "^[0-9]*$" # Verify if the number of lines is a valid number
        then
            cat "$1" # We print the file content
            exit "$2" # We exit with the number of lines
        else
            echo "Invalid number of lines"
        fi
    fi
fi
