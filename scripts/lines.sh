
# Se verifica daca a primit exact un argument
if test $# -ne 1
    then
        echo "Invalid number of arguments"
    else 
        cat $1
fi
exit 8