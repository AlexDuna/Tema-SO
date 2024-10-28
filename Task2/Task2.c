/*Realizați un program C care primește 2 argumente din linia de comandă: un director sursă și un director destinație.

Se va parcurge recursiv structura de directoare din directorul sursă. Pentru fiecare intrare din directorul sursă, se vor executa următoarele operații, în funcție de tipul intrării:

1.Pentru directoare, se va crea un director echivalent în directorul destinație, cu aceleași drepturi ca directorul original. Astfel, structura de directoare din directorul destinație va fi asemănătoare cu cea din sursă.

2.Pentru fișierele obișnuite, în funcție de dimensiunea fișierului se va crea o copie sau o legătura simbolică:

3.Pentru fișierele mai mici de 500 octeți, se va crea o copie a fișierului în directorul destinație (și, după caz, în subfolderul echivalent). Aceste fișiere copie vor avea aceleași drepturi de read ca și fișierele originale (Ex: dacă fișierul original are –rwx-wxr-x, fisierul nou va avea –r-----r--).

4.Pentru celelalte fișiere, se vor crea legături simbolice către fișierul original. Aceste linkuri vor avea aceleași drepturi de acces ca și fișierele originale.

5.Pentru (soft) linkuri, nu se va executa nici o operație.

Tema va fi însoțita de fișierul makefile corespunzător.*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>

//functia de copiere a fisierului sursa in fisierul destinatie
void copiere(const char *path_sursa, const char *path_destinatie)
{
	int fisier_SURSA = open("path_sursa",O_RDONLY);
	int fisier_DESTINATIE = open("path_destinatie", O_WRONLY | O_CREATE | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH );
	
	char buffer[1024];
	ssize_t bytesRead;
	
	while((bytesRead = read(fisier_SURSA , buffer , sizeof(buffer)) > 0)
	{
		write(fisier_DESTINATIE, buffer, bytesRead);
	}
	
	close(fisier_SURSA);
	close(fisier_DESTINATIE);
}


//functie pentru crearea unei legaturi simbolice catre un fisier
void creare_link(const char *path_sursa, const char *path_destinatie)
{
	symlink(path_sursa,path_destinatie);
}


//functie pentru parcurgerea sau procesarea recursiva a fisierelor/directoarelor din sursa
void procesare_intrari(const char *path_sursa, const char *path_destinatie)
{
	struct stat statBuff; //ca sa stocam informatii despre fisierul/directorul curent
	
	//extragem informatiile despre fisierul/directorul curent
	if(lstat(path_sursa, &statBuff) == -1)
	{
		perror("lstat");
		exit(EXIT_FAILURE);
	}
	
	//verificam tipul de intrare
	if(S_ISDIR(statBuf.st_mode))
	{
		//cream directorul in destinatie si procesam continutul recursiv
		mkdir(path_destinatie, statBuf.st_mode);
		
		DIR *dir=opendir(path_sursa);
		if(!dir)
		{
			perror("opendir");
			exit(EXIT_FAILURE);
		}
		
		struct dirent *intrare;
		
		//parcurgem recursiv continutul unui director
		while((intrare = readdir(dir))!= NULL)
		{
			if(strcmp(intrare->d.name,".")!= 0 && strcmp(intrare->d.name,"..")!=0)	//verificam ca numele intrarii sa nu fie '.' sau '..' care reprezinta directorul curent sau directorul parinte pentru ca pe ele nu le mai parcurgem recursiv
			{
				char new_path_sursa[1024], new_path_destinatie[1024];				//declaram 2 cai noi pentru sursa si destinatie pentru ca se vor schimba
				snprintf(new_path_sursa,1024,"%s%s",path_sursa,intrare->d.name);		//
				snprintf(new_path_destinatie,1024,"%s%s",path_destinatie,intrare->d.name);
				procesare_intrare(new_path_sursa,new_path_destinatie);
			}	
		}
		
		//inchidem apoi directorul
		closedir(dir);
		
	}
	else if(S_ISREG(statBuf.st_mode))
	{
		//daca este fisier obisnuit se face o copie sau se creaza legatura simbolica
		if(statBuf.st_size < 500)
		{
			copiere(path_sursa,path_destinatie);
		}
		else
		{
			creare_link(path_sursa,path_destinatie);
		}
	}
	else
	{
		printf("Tipul de fisier este necunoscut");
		exit(EXIT_FAILURE);
	}
}

//verificam daca programul este apelat cu numarul corect de argumente de la linia de comanda
int main(int nr_argumente, int *vector_argumente[])
{
	if(nr_argumente != 3)
	{
		printf("Numar incorect de argumente!");
		exit(EXIT_FAILURE);
	}
	
	char *path_sursa = vector_argumente[1]; // numele sursei
	char *path_destinatie = vector_argumente[2]; //numele destinatiei
	
	procesare_intrari(path_sursa, path_destinatie);
	
	return 0;
}



























