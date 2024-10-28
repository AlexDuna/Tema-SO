#!/bin/sh

while true
do 
	echo "1. Iesire"
	echo "2. Afisare: ID-ul procesului initiat la executarea scriptului, tipul sistemului de operare, numarul de secunde de cand s-a executat scriptul si calea curenta"
	echo "3. Afisarea numarului saptamanii din an, si a zilei saptamanii in care va fi Craciunul in acest an "
	echo "4. Afisare primelor 2 shell-uri instalate in sistem"
	echo "5. Afisarea pentru toti utilizatorii din sistem a numelui de login, a user ID-ului, a numelui complet si a directorului home"
	echo "6. Afisare ultimele linii incepand cu linia 9 din fisierul /etc/protocols folosind comanda tail"
	echo "7. Afisare tip procesor si marimea cacher-ului "
	echo "8. Scrieți comanda ls care afișează toate numele de directoare din directorul /proc care nu reprezintă imaginea unui proces pe disc (numele directorului care reprezintă imaginea unui proces pe disc este format doar din cifre)"
	echo "9. Sa se afiseze toate fisierele de log. Sa se creeze o functie care primeste ca si parametrii cititi de la tastatura un nume de fisier de log (ori curent ori arhivat) si un text, si care va cauta si afisa liniile care contin textul primit ca si parametru"
	
	echo "Optiune: "
	read optiune
	
	case $optiune in
	1)
		echo "Iesire program!"
		exit 0
		;;
	2)
		echo "#############################################################"
		echo "PID-ul acestui script este $$" #$$ ne da id-ul procesului initiat la exec. scriptului
		echo "Sistem de operare: $(uname -s)" #uname -s ne da sistemul de operare
		echo "Numarul de secunde de cand s-a executat scriptul: $SECONDS" #nr de secunde de cand s-a executat scriptul
		echo "Calea curenta: $(pwd)" #calea curenta
		echo "#############################################################"	
		;;
	3)
		echo "#############################################################"
		echo "Numarul saptamanii din an: $(date +'%W')" 
		Craciun=$(date -d "25 Dec" +'%A')  #-d ca sa folosim o anumita zi si %A pt ziua din saptamana
		echo "Numarul zilei cu Craciunul, (1-Luni, 7-Duminica): $Craciun, din saptamana: $(date -d "25 Dec" +'%U')"  #%U, numarul saptamanii din an
		echo "#############################################################"
		;;
	4)
		echo "#############################################################"
		echo "Primele doua shell-uri instalate in sistem: $(cat /etc/shells | head -n 2)"	#folosim head ca sa numaram shellurile de la inceputul directorului etc, si cat ca sa le afisam
		echo "#############################################################"
		;;
	5)
		echo "#############################################################"
		echo "Afisare pentru toti utilizatorii: Numele de login, user ID, NUme complet, si a directorului Home"
		getent passwd | awk -F: '{printf "Nume de conectare: %-10s User ID: %-6s Nume complet: %-20s Directorul Home: %s\n", $1, $3, $5, $6}' #getent ca sa aflam informatii de baza despre utilizatori, parole, retele etc din directorul passwd
		#awk folosim ca sa cautam text intr-un fisier, cautam un sablon in functie de niste reguli pe care noi le dam 
		#am pus -F ca sa adaug separator personalizat
		echo "#############################################################"
		;;
	6)
		echo "#############################################################"
		echo "Ultimele linii incepand cu linia 9 din /etc/protocols:"
		cat /etc/protocols | tail -n 9 #cat pentru afisare si tail ca sa ne ducem la finalul fisierului
		echo "#############################################################"
		;;
	7) 
		echo "#############################################################"
		echo "Tipul procesorului si marimea Cache-ului: "
		cat /proc/cpuinfo | grep -E 'model name|cache size' #folosim filtrul grep, -E ca sa putem face cautari extinse
		echo "#############################################################"
		;;
	8)
		echo "#############################################################"
		echo "Afisarea directoriilor din /proc care nu reprezinta imaginea unui proces pe disc: "  #adica numele sa nu fie format din cifre
		ls -d /proc/*/ | grep -vE '/proc/[0-9]+/' #folosim ls -d /proc/*/ ca sa afisam doar numele a tuturor fisierelor din proc, iar punand filtrul grep -vE ni se afiseaza toate fisierele a caror nume nu se potrivesc cu modelul pus de mine [0-9]+
		echo "#############################################################"
		;;
	9)
		echo "#############################################################"
		echo "Afisarea fisierelor de tip log: "
		ls /var/log/  #afisam cu ls
		echo "Functia de cautare: "
		cautare() {
    		fisier="$1"    #parametrul 1 va fi numele fisierului in care cautam
    		text="$2"      #parametrul 2 va si textul pe care il cautam in fisier
	
		    if [[ $fisier == *".gz" ]]; then  #verificam daca fisierul are extensia .gz
		        gunzip -c "$fisier" | grep "$text"  #daca fisierul este un Gzip, atunci il dezarhivam si cautam folosind filtrul grep textul dorit
		    else
		        grep "$text" "$fisier"	#altfel, daca nu este o arhiva, atunci cautam direct textul in fisier
		    fi
		}


		echo "Introduceți numele fișierului de log:"
		read un_fisier_log 	#citim un nume de fisier

		echo "Introduceți textul de căutat:"
		read textul	#citim un text pe care dorim sa-l cautam

		cautare "$un_fisier_log" "$textul"  #apelam functia cautare
		echo "#############################################################"
		;;
	*)
		echo "Nu ai introdus o optiune valida!"
		;;
		
	esac 
done
;;
