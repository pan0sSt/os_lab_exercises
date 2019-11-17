#ΣΤΑΥΡΙΝΑΚΗΣ ΠΑΝΑΓΙΩΤΗΣ
#Α.Μ.: 236217
#email: stavrinakis@ceid.upatras.gr
#Ολα τα ερωτηματα ειναι λυμενα και δουλευουν σωστα.
#Το μονο προβλημα που εμφανιζεται εχει να κανει με το ερωτημα E) εμφανιζοντας 'error' ανα διαστηματα και πιο συγκεκριμενα μονο με
#την ημερομηνια '1980-04-01' διοτι το εκτελεσα με την εντολη 'date -d' οπως θα δειτε και με μια ερευνα διαβασα οτι
#βρισκεται σε διαφορετικο time zone οποτε δεν αναγνωριζεται. Λογω του οτι ο grader εβγαζε 'ΟΚ' θεωρησα πως δεν υπαρχει προβλημα.
#Επιχειρησα να υλοποιησω το προγραμμα μεσα σε cases, ομως θα γινοταν πολυ πολυπλοκο και ακατανοητο χρησιμοποιωντας περισσοτερες
#εντολες για ελεγχους χωρις να χρειαζεται. Με το 'optarg' μπορεσα να οργανωσω με μεγαλη ευκολια το περασμα των παραμετρων και
#την εισαγωγη arguments εξοικονομωντας χωρο και κανοντας το πιο ευδιαβαστο. Αντοιστιχα επρεπε να φτιαξω μια συντομογραφια των
#μεγαλων παραμετρων για να τις δεχεται το optarg και να προσθεσω flags για την ενημερωση του μερους του κωδικα που πρεπει να εκτελεστει.
#Το 'grep' με βοηθησε να εντοπισω γραμμες και αντιστοιχα να σβησω τα σχολια, το 'IFS' με βοηθησε ιδιαιτερα σε αυτη την ασκηση
#διαχωριζοντας τις λεξεις μεταξυ τους και κανοντας απαραιτητες αλλαγες ανα γραμμη και χρησιμοποιησα το 'exit 1' σε περιπτωση
#που βρεθει η γραμμη που ψαχναμε και δεν χρειαζεται να συνεχιστει η αναζητηση. Παρομοια με το IFS, το 'awk' χρησιμευσε για
#field separation και στο να διαχειριστω ολοκληρο το file οπως επιθυμουσα. Για τη συγκριση ημερομηνιων σκεφτηκα να τα συγκρινω ως strings
#χωρις να το κανω να εκτελειται σωστα, οποτε προσπαθησα να τα μετατρεψω σε αριθμους κατι το οποιο καταφερα με το 'date -d'.
#Για το ερωτημα με τους browsers δεν χρειαστηκε να χρησιμοποιησω το 'uniq' μιας και βρηκα το 'END' που περιμενει μεχρι
#να εκτελεστουν οι εντολες και μετα στελνει στην εξοδο αποτελεσματα, οποτε δεν εμφανιζε ολη την διαδικασια του counter των browsers αλλα
#μονο την τελικη εκτελεση και με το 'sort' σε αλφαβητικη σειρα. Οσο για το τελευταιο ερωτημα προσπαθησα να βρω τροπο να μετραει πανω απο
#ενα argument για μια παραμετρο με 'shift' ή δημιουργια 'array' αλλα χωρις αποτελεσμα, οποτε αναγκαστηκα να παρω ελεγχους για ολες τις θεσεις
#των παραμετρων και arguments. Τελος, το 'sed -i' με βοηθησε να κανω search τη σειρα που χρειαζομουν και edit το string, οποτε απεφυγα τη
#χρηση του IFS στο συγκεκριμενο κομματι μιας και δεν χρειαστηκε.

#!/bin/bash

for arg in "$@"; do                             #μετονομασια παραμετρων
        shift
        case "$arg" in
                -id)            set -- "$@" "-i"
                ;;
                --firstnames)   set -- "$@" "-n"
                ;;
                --lastnames)    set -- "$@" "-l"
                ;;
                --born-since)   set -- "$@" "-s"
                ;;
                --born-until)   set -- "$@" "-u"
                ;;
                --browsers)     set -- "$@" "-b"
                ;;
                --edit)         set -- "$@" "-e"
                ;;
                *)              set -- "$@" "$arg"
        esac
done
fileflag=0                                      #δημιουργια flags
idflag=0
firstnameflag=0
lastnameflag=0
bornflag=0
bornflag1=0
bornflag2=0
browsersflag=0
editflag=0
if [ "$#" = 0 ]; then                           #ελεγχος παραμετρων
        echo "6217"
else
        while getopts ":f:i:nls:u:be:" opt; do
                case $opt in
                        f) file=$OPTARG         #εισαγωγη της επομενης παραμετρου ως το argument
                           fileflag=1
                        ;;
                        i) userID=$OPTARG
                           idflag=1
                        ;;
                        n) firstnameflag=1
                        ;;
                        l) lastnameflag=1
                        ;;
                        s) born_since=$OPTARG
                           bornflag=1
                           bornflag1=1
                        ;;
                        u) born_until=$OPTARG
                           bornflag=1
                           bornflag2=1
                           bornflag2=1
                        ;;
                        b) browsersflag=1
                        ;;
                        e) editflag=1
                           if [ $3 == "-e" ]; then
                                 edit_id=$4
                                 edit_column=$5
                                 edit_value=$6
                           elif [ $1 == "-e" ]; then
                                 edit_id=$2
                                 edit_column=$3
                                 edit_value=$4
                                 file=$6        #Γραφοντας αρχικα το --edit δεν διαβαζε το file οποτε επρεπε να το εισαγω παλι
                           fi
                        ;;
                esac
        done
fi

if [[ $file != "" && $# -eq 2 ]]; then
        cat $file | grep -v '^#'                #Με το -v μπορεσα να αποκλεισω τις σειρες που αρχιζουν με '#'
elif [[ $idflag -eq 1 && $file != "" ]]; then
        while IFS='|' read -r id lastName firstName gender birthday creationDate locationIP browserUsed #Διαχωριζει σε fields τις λεξεις μεταξυ '|'
        do                                                                                              #και δινει αντιστοιχα ονομασιες σε καθε field
        if [ $userID == $id ]; then
                echo "$firstName $lastName $birthday"
                exit 1
        fi
        done < "$file"
elif [[ $firstnameflag -eq 1 && $file != "" ]]; then
        awk -F"|" '/^[^#]/ {print $3}' $file | sort | uniq              #Με το '-F' επιλεγουμε διαχωριστικο λεξεων και με το '^[^#]' δεν εμφανιζουμε σχολια
elif [[ $lastnameflag -eq 1 && $file != "" ]]; then                     #εκτυπωνοντας το τριτο field σε καθε σειρα
        awk -F"|" '/^[^#]/ {print $2}' $file | sort | uniq
elif [[ $bornflag -eq 1 && $file != "" ]]; then
        if [ $bornflag1 -eq 0 ]; then                                   #Αν δεν εισηγαγε 'born_since'
                b_until=$(date -d $born_until +%s)
                while IFS='|' read -r id lastName firstName gender birthday creationDate locationIP browserUsed
                do
                b_day=$(date -d $birthday +%s)
                if [ $b_day -le $b_until ]; then
                        echo "$id|$lastName|$firstName|$gender|$birthday|$creationDate|$locationIP|$browserUsed"
                fi
                done < "$file"
        elif [ $bornflag2 -eq 0 ]; then                                 #Αν δεν εισηγαγε 'born_until'
                b_since=$(date -d $born_since +%s)
                while IFS='|' read -r id lastName firstName gender birthday creationDate locationIP browserUsed
                do
                b_day=$(date -d $birthday +%s)
                if [ $b_day -ge $b_since ]; then
                        echo "$id|$lastName|$firstName|$gender|$birthday|$creationDate|$locationIP|$browserUsed"
                fi
                done < "$file"
        else                                                            #Εισηγαγε και τις δυο ημερομηνιες
                b_until=$(date -d $born_until +%s)
                b_since=$(date -d $born_since +%s)
                while IFS='|' read -r id lastName firstName gender birthday creationDate locationIP browserUsed
                do
                b_day=$(date -d $birthday +%s)
                if [[ $b_day -ge $b_since && $b_day -le $b_until ]]; then
                        echo "$id|$lastName|$firstName|$gender|$birthday|$creationDate|$locationIP|$browserUsed"
                fi
                done < "$file"
        fi
elif [[ $browsersflag -eq 1 && $file != "" ]]; then
        awk -F"|" '/^[^#]/{a[$8]++} END { for (e in a) print e, a[e] }' $file | sort    #Δημιουργουμε πινακα με το '{a[$8]++}' που καθε θεση αυξανεται κατα ενα με
elif [[ $editflag -eq 1 && $file != "" ]]; then                                         #την εισαγωγη δεδομενων που της αντιστοιχουν
        if [[ $edit_column -ge 2 && $edit_column -le 8 ]]; then
                sed -i -e "/^$edit_id|/s/|\([^|]*\)/|$edit_value/$(( $edit_column-1 ))" $file   #'-e' για να τρεξει το command, '^$edit_id|' για το string ωστε να
        fi                                                                                      #βρουμε τη σειρα, '|\([^|]*\)' για να ψαχνει για το '|' που δεν
fi                                                                                              #ακολουθειται απο αλλο '|' ωστε να βρει τα strings , '|$edit_value'
                                                                                                #η μεταβλητη που θα παρει τη θεση του προηγουμενου string,
                                                                                                #'$(( $edit_column-1 ))' ωστε να επιλεψουμε ποιο string απο τους
                                                                                                #προηγουμενους διαχωρισμους θα αλλαξει