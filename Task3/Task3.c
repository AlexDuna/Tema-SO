/*Se consideră un program C ce conţine două procese (părinte + fiu). 
1. Procesul părinte va trimite, printr-un pipe, timp de 7s caracterul “e” către procesul fiu, în plus la fiecare secundă o să trimită semnalul SIGUSR1. 
2. Procesul fiu va citi caracterele din pipe şi va realiza o statistică ce va conţine numărul total de caractere, precum şi numărul de caractere primite după fiecare semnal SIGUSR1. 
3. La final statistica va fi trimisă printr-un pipe procesului părinte ce o va afişa.
 
*/
 
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdlib.h>
 
int nrcaractere = 0;
int nrcaractere_primite = 0;
 
 
//functia handler pentru tratarea semnalului SIGUSR1
//cand semnalul SIGUSR1 este primit atunci se apeleaza aceasta functie handler
void handler(int semnal)
{
    printf("Semnal primit!\n");
    nrcaractere_primite = 0;
}
  
int main()
{ 
    int pipe_caractere[2]; // Pipe pentru transmiterea caracterului, cu 2 elemente deoarece un element este o cale de citire si unul este o cale de scriere
    int pipe_statistica[2]; // Pipe pentru transmiterea statisticii
 
    if (pipe(pipe_caractere) == -1 || pipe(pipe_statistica) == -1)	//daca vreun apel este egal cu -1 atunci inseamna ca avem eroare, daca se returneaza 0 atunci rezulta succes
    {
        perror("Eroare la crearea canalului (pipe).");
        exit(EXIT_FAILURE);
    }
    
    //cream noul proces fiu
    int id = fork();
 
    if (id == -1)	//aici daca ia valoare <0 avem eroare, altfel este in regula
    {
        perror("Eroare la crearea procesului fiu.");
        exit(EXIT_FAILURE);
    }
 
    if (id != 0)	//daca valoarea nu este returnata in procesul fiu, adica sectiunea aceasta va fi executata de procesul parinte
    {
        close(pipe_caractere[0]); // se inchide citirea pipe-ului de caractere în procesul părinte
        close(pipe_statistica[1]); // se inchide scrierea pipe-ului de statistici în procesul părinte
 
        int i = 0;
        char c = 'e';
 
        signal(SIGUSR1, handler);//legam cu aceasta functie semnalul SIGUSR1 de functia handler
 
        // Pauza scurta inainte de a trimite primul semnal
        sleep(1);
 
        while (i < 7)
        {
            printf("Caracterul 'e' a fost trimis.\n");
            write(pipe_caractere[1], &c, sizeof(char));	//se trimite caracterul 'e' in pipe
            kill(id, SIGUSR1);	//se trimite semnalul SIGURS1 catre fiu
            sleep(1);	//asteptam o secunda intre iteratiile lui i (intre fiecare transmitere de semnal din cele 7)
            i++;
 
        }
 
        close(pipe_caractere[1]); // inchidem partea de scriere a pipe-ului de caractere în procesul părinte pentru ca am terminat scrierea in acest pipe
 
        // se asteapta ca procesul fiu sa se termine
        wait(NULL);
 
        // citim statistici din pipe
        read(pipe_statistica[0], &nrcaractere, sizeof(int));	//trecem numarul total de caractere
        read(pipe_statistica[0], &nrcaractere_primite, sizeof(int));	//trecem numarul de caractere care au fost primite dupa semnal
 
 
 
        // afisam statisticile
        printf("Statistica: Numar total de caractere: %d, Numar caractere dupa fiecare semnal: %d\n", nrcaractere, nrcaractere_primite);
        close(pipe_statistica[0]); // inchidem partea de citire a pipe-ului de statistici din procesul parinte
    }
 
    else	//altfel, aceasta sectiune va fi executata de procesul fiu
    {
        close(pipe_caractere[1]); // inchidem partea de scriere a pipe-ului de caractere in procesul fiu
        close(pipe_statistica[0]); // inchidem partea de citire a pipe-ului de statistici din procesul fiu
 
        char buffer[1];	//declaram un buffer in care stocam caracterele citite
 
        // folosim handlerul nostru pentru semnul SIGUSR1 
        signal(SIGUSR1, handler);
 
        while (read(pipe_caractere[0], buffer, sizeof(char)) > 0)//citim rand cu rand fiecare caracter primit si incrementam variabilele noastre pt caractere totale citite si caractere primite
        {
            nrcaractere++;
            nrcaractere_primite++;
            printf("Caracter primit: %c. Total: %d, După semnal: %d\n", buffer[0], nrcaractere, nrcaractere_primite);
        }
 
        // salvam date in pipe-ul de statistica
        write(pipe_statistica[1], &nrcaractere, sizeof(int));
        write(pipe_statistica[1], &nrcaractere_primite, sizeof(int));
 
        close(pipe_caractere[0]); // inchidem partea de citire a pipe-ului de caractere din procesul fiu
        close(pipe_statistica[1]); // inchidem partea de scriere a pipe-ului de statistici din procesul fiu
 
        exit(EXIT_SUCCESS);
    }
 
    return 0;
}
