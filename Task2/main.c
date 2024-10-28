/*
Realizați un program C care primește 2 argumente din linia de comandă: un director sursă și un director destinație.

Se va parcurge recursiv structura de directoare din directorul sursă. Pentru fiecare intrare din directorul sursă, se vor executa următoarele operații, în funcție de tipul intrării:

1. Pentru directoare, se va crea un director echivalent în directorul destinație, cu aceleași drepturi ca directorul original. Astfel, structura de directoare din directorul destinație va fi asemănătoare cu cea din sursă.

2. Pentru fișierele obișnuite, în funcție de dimensiunea fișierului se va crea o copie sau o legătura simbolica:

2.1. Pentru fișierele mai mici de 500 octeți, se va crea o copie a fișierului în directorul destinație (și, după caz, în subfolderul echivalent). Aceste fișiere copie vor avea aceleași drepturi de read ca și fișierele originale (Ex: dacă fișierul original are –rwx-wxr-x, fisierul nou va avea –r-----r--).

2.2. Pentru celelalte fișiere, se vor crea legături simbolice către fișierul original. Aceste linkuri vor avea aceleași drepturi de acces ca și fișierele originale.

2.3. Pentru (soft) linkuri, nu se va executa nici o operație.

Tema va fi însoțita de fișierul makefile corespunzător.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>

void copiere(const char *path_sursa, const char *path_destinatie) {
    int fisier_SURSA = open(path_sursa, O_RDONLY);
    int fisier_DESTINATIE = open(path_destinatie, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

    char buffer[1024];
    ssize_t bytesRead;

    while ((bytesRead = read(fisier_SURSA, buffer, sizeof(buffer))) > 0) {
        write(fisier_DESTINATIE, buffer, bytesRead);
    }

    close(fisier_SURSA);
    close(fisier_DESTINATIE);
}

void creare_link(const char *path_sursa, const char *path_destinatie) {
    symlink(path_sursa, path_destinatie);
}

void procesare_intrare(const char *path_sursa, const char *path_destinatie) {
    struct stat statBuff;

    if (lstat(path_sursa, &statBuff) == -1) {
        perror("lstat");
        exit(EXIT_FAILURE);
    }

    if (S_ISDIR(statBuff.st_mode)) {
        mkdir(path_destinatie, statBuff.st_mode);

        DIR *dir = opendir(path_sursa);
        if (!dir) {
            perror("opendir");
            exit(EXIT_FAILURE);
        }

        struct dirent *intrare;

        while ((intrare = readdir(dir)) != NULL) {
            if (strcmp(intrare->d_name, ".") != 0 && strcmp(intrare->d_name, "..") != 0) {
                char new_path_sursa[1024], new_path_destinatie[1024];
                snprintf(new_path_sursa, 1024, "%s/%s", path_sursa, intrare->d_name);
                snprintf(new_path_destinatie, 1024, "%s/%s", path_destinatie, intrare->d_name);
                procesare_intrare(new_path_sursa, new_path_destinatie);
            }
        }

        closedir(dir);
    } else if (S_ISREG(statBuff.st_mode)) {
        if (statBuff.st_size < 500) {
            copiere(path_sursa, path_destinatie);
        } else {
            creare_link(path_sursa, path_destinatie);
        }
    } else {
        fprintf(stderr, "Tip de fisier necunoscut: %s\n", path_sursa);
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <director_sursa> <director_destinatie>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *path_sursa = argv[1];
    char *path_destinatie = argv[2];

    procesare_intrare(path_sursa, path_destinatie);

    return 0;
}

