#include <unistd.h>
#include <stdio.h>
int main()
{
    int pid = fork();
    if ( pid == -1 ){
        return 1;
    }
    printf("Who am I ? my pid is:%d\n", getpid() );
    sleep(5);
    return 0;
}
