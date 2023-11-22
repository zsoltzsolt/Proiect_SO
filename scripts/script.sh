
if test $# -ne 1
then
    echo "Invalid number of arguments"
else
    count=0
    while read line
    do
        echo $line | grep -e "^[A-Z][A-Za-z0-9 ,\.\!\?]*" | 
        grep -e "[\!\?\.]$" | grep -ve ",[ ]*si"
    done
fi