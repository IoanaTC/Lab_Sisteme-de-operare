// copiez datele dintr-un fisier sursa, intr-un fisier destinatie, date ca parametru la apel
// ./mycp file1.txt file2.txt

#include<fcntl.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<errno.h>
#include<stdlib.h>
#include <stdio.h>


int main(int argc, char *argv[]){

    // extragem denumirile fisierelor trimise ca param
    char* src_path = argv[1]; // sursa
    char* dst_path = argv[2]; // destinatia

    // folosesc o structura stat pentru a alfla informatii despre fisierele date
    struct stat stat_buf;

    if(stat(src_path, &stat_buf) < 0){
        // eroare
        perror("stat failed to load");
        return errno;
    }
    else{
        // facem un buffer pentru a transporta datele din fisierul src

        int file_descriptor = open(src_path, O_RDONLY);
        if(file_descriptor < 0){
            perror("Open src failed");
            return errno;
        }
        // deschidem fisierul, adaugandu-i flaguri si permisiuni    
        int file_descriptor_dst = open(dst_path, O_WRONLY | O_CREAT, S_IRWXU);
        if(file_descriptor_dst < 0){
            perror("Open dst failed");
            return errno;
        }

        char* buf = (char*) malloc(stat_buf.st_size * sizeof(char) + 1);
        if(buf == NULL){
            perror("Malloc failed");
            return errno;
        }

        // citim datele din fisier

        size_t bytes_read = read(file_descriptor, buf, stat_buf.st_size);
        if(bytes_read < 0){
            perror("Read failed");
            return errno;
        }
        // ne asiguram ca toate datele din sursa au fost transportate corect
        for(size_t index = bytes_read; index < stat_buf.st_size; index+=bytes_read){

            bytes_read = read(file_descriptor, buf + index, stat_buf.st_size - index);
            if(bytes_read < 0){
                perror("Read failed");
                return errno;
            }
        }
        buf[stat_buf.st_size] = '\0';
        
        // scriem datele citite in buf in fisierul destinatie

        size_t bytes_written = write(file_descriptor_dst, buf, stat_buf.st_size);
        if(bytes_written < 0){
            perror("Write failed");
            return errno;
        }
        // ne asiguram ca toate datele din buf au fost transportate corect
        for(size_t index = bytes_written; index < stat_buf.st_size; index+=bytes_written){

            bytes_written = write(file_descriptor_dst, buf + index, stat_buf.st_size - index);
            if(bytes_written < 0){
                perror("Write error");
                return errno;
            }
        }

        if(close(file_descriptor) < 0){
            perror("Close fd failed");
            return errno;
        }
        if(close(file_descriptor_dst) < 0){
            perror("Close fd_dst failed");
            return errno;
        }
    }
    return 0;
}