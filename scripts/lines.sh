
# We need 2 arguments: file name and number of lines
if test $# -ne 2
    then
        echo "Invalid number of arguments"
    else 
        cat $1
fi
exit $2