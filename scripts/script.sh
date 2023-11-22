
if test $# -ne 1
then
    echo "Invalid number of arguments"
else
    count=0
    while read line
    do
        if echo $line | grep -e "^[A-Z][A-Za-z0-9 ,\.\!\?]*" | 
                grep -e "[\!\?\.]$" | grep -ve ",[ ]*si"
        then
            ((++count))
        fi
    done
    echo "Count is $count"
fi