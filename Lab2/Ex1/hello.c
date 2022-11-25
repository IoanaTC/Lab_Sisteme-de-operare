#include<unistd.h>
#include <stdio.h>
#include<errno.h>

int main(){
    // printeaza la std.out mesajul "Hello World!", cu lungimea 14
    if(write(1, "Hello World!\n", 14) < 0){
        perror("Write failed");
        return errno;
    }
    return 0;
}