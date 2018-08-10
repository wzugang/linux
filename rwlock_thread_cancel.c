#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<stdlib.h>

int g_var = 10;
pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;
typedef void* (*thread_func)(void*);

void* write_work(void* arg)
{
    pthread_t tid = pthread_self();
    while(1)
    {
        static int count = 0;
        pthread_rwlock_wrlock(&rwlock);
        g_var = count;
        printf("write tid=%d, value=%d\n",(int)tid,g_var);
        pthread_rwlock_unlock(&rwlock);
        count++;
        pthread_testcancel();
        usleep(1000*100);
    }
}

void* read_work(void* arg)
{
    pthread_t tid = pthread_self();
    while(1)
    {
        pthread_rwlock_rdlock(&rwlock);
        printf("read  tid=%d, value=%d\n",(int)tid,g_var);
        pthread_rwlock_unlock(&rwlock);
        pthread_testcancel();
        usleep(1000*100);
    }
}

pthread_t new_thread(thread_func func)
{
    pthread_t tid;
    pthread_create(&tid,NULL,func,NULL);
    return tid;
}

void del_pthread(pthread_t tid)
{
    pthread_cancel(tid);
}

int main()
{
    int i;
    pthread_t tid[20];
    for(i=0;i<5;++i)
    {
        tid[i] = new_thread(write_work);
    }
    for(i=5;i<20;++i)
    {
        tid[i] = new_thread(read_work);
    }
    while(1)
    {
        sleep(300);
        break;
    }
    for(i=0;i<20;++i)
    {
        pthread_cancel(tid[i]);
        pthread_join(tid[i],NULL);
    }
}

