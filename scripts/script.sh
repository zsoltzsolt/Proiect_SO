#!/bin/bash

# Se verifica daca a primit exact un argument
if test $# -ne 1
then
    echo "Invalid number of arguments"
else
    # Contor pentru numarul de propozitii corecte
    count=0
    # Se citesc linii de la intrarea standard pana la intalnirea EOF
    while read line
    do
        if echo "$line" | grep -e "^[A-Z][A-Za-z0-9 ,\.\!\?]*" | # Incepe cu litera mare, contine doar litere mari, mici, cifre, spatii, virgula, punct semnul exclamarii si semnul intrebarii
        grep -e "[\!\?\.]$" |  # Se termina cu semnul exclamarii, intrebarii sau punct
        grep -ve ",[ ]*si" |  # Nu contine virgula inainte de si
        grep -qe "$1" # Contine caracterul dat ca argument, folosim q pentru a nu afisa liniile corecte la iesirea standard
        then
            ((++count))
        fi
    done 
fi

echo $count # Se afiseaza numarul de propozitii corecte la iesirea standard

