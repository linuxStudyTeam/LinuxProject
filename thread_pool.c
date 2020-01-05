
#include "thread_pool.h"

void pool_init(thread_pool *tp, int n)
{
    tp->socks = Calloc(n, sizeof(int)); 
    tp->cnt = n;                     
    tp->use_pos= tp->no_use_pos = 0;           
    sem_init(&tp->mutex, 0, 1);      
    sem_init(&tp->avail, 0, tp->cnt);
    sem_init(&tp->used, 0, 0);     
}


void pool_destroy(thread_pool *tp)
{
    Free(tp->socks);
}



void use_thread_of_pool (thread_pool *tp, int item)
{   sem_wait(&tp->avail);           /* 等待线程池中是否有可用的线程，若没有则阻塞，有则将avail信号量减1 */
    sem_wait(&tp->mutex);        /* 抢占互斥量，相当于加锁操作，保证原子性 */
    tp->socks[tp->use_pos] = item;        /* 将socket值写入到线程池结构体的socket数组中 */
    tp-> use_pos =(tp-> use_pos +1)%(tp->cnt); /* 调整指针 */
    sem_post(&tp->mutex);          /* 释放互斥量，让其他线程能够抢占 */
    sem_post(&tp->used);          /* 对外宣布线程池中已经有一个线程被用到了，即信号量used加1 */
}
 

int release_thread_of_pool(thread_pool *tp)
{
    int item;
    sem_wait(&tp->used);    /* 等待线程池中被使用的线程，若没有一个线程被使用过，则会阻塞，有则将used信号量减1 */
    sem_wait(&tp->mutex);    /* 抢占互斥量，相当于加锁操纵，保证原子性 */
    item = tp->socks[tp->no_use_pos];            /* 将socket值弹出 */
    tp->no_use_pos=(tp->no_use_pos+1)%(tp->cnt);     /* 调整指针 */
    sem_post(&tp->avail);                    /* 对外宣布线程池中有一个线程已经可用，即信号量avail加1 */
    sem_post(&tp->mutex);  /* 释放互斥量 */
    return item;
}



void *process_thread_function(void *arg)
{
	int conn_sock ;
   extern thread_pool tp;
       	pthread_detach(pthread_self()); 
    while (1) { 
	    conn_sock=release_thread_of_pool(&tp);   /* Remove a task from task pool */
	    //toggle(conn_sock);            /* Serve client */
	    printf("------%d-------\n",conn_sock);
	    close(conn_sock);
    }

}
