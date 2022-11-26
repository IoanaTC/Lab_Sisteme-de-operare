#include<sys/syscall.h>
#include<unistd.h>|

int main(){
    syscall(331, "Hello!");
    return 0;
}