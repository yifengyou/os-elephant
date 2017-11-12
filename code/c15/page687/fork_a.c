#include <unistd.h>
#include <stdio.h>

int main()
{
    printf("I will fork in 5 seconds\n");
    sleep(5);
    int pid = fork();
    if ( pid == -1 ){
        return 0;
    }
    if ( pid ){
        printf("I am father,my pid is %d\n",getpid());
        sleep(5);
        return 0;
    }else{
        printf("I am child,my pid is %d\n",getpid());
        sleep(5);
        return 0;
    }
}


