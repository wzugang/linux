```
#include <stdio.h>
#include <pthread.h>

int flag=0;

void push(void* arg)
{
    printf("push.\n");
}

void* test(void* arg)
{
    pthread_cleanup_push(push, NULL);
    flag = 1;
    while(flag)
    {
        sleep(1);
    }
    pthread_cleanup_pop(0);
    printf("pop.\n");
}

void* cancel(void* arg)
{
    pthread_t* p1 = (pthread_t*)arg;
    sleep(5);
    pthread_cancel(*p1);
    printf("cancel.\n");
}

int main()
{
    pthread_t t1;
    pthread_t t2;

    pthread_create(&t1, NULL, test, NULL);
    pthread_create(&t2, NULL, cancel, &t1);

    pthread_join(t2,NULL);
    pthread_join(t1,NULL);

    return 0;
}

gcc test.c -lpthread -o test
```
