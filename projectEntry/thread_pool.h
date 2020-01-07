#ifndef THREAD_POOL_H_INCLUDED
#define THREAD_POOL_H_INCLUDED 
#include "../tools/wrapper.h"
#include "weblet.h"
#define SIZE_OF_POOL 4
typedef struct {
    int *socks;         /* 定义指针，指向用于初始化的线程池的数组空间（一般存放socket值） */         
    int cnt;            /* 线程池的线程数 */
    int use_pos;          /* 类似指针，使用一个线程则加一，满足运算关系为： use_pos = (use_pos+1) % cnt */
    int no_use_pos;         /* 类似指针，一个线程使用完则加一，满足运算关系为： no_use_pos = (no_use_pos+1) % cnt */
    // 上面两个数若相等则表示线程池为空闲状态
    sem_t mutex;      /* 互斥量，保证在使用线程（这里的使用线程其实表示的是函数调用那一瞬间），
    					不允许销毁线程；在销毁线程（同理，也是函数调用的一瞬间），不允许使用线程 */
    sem_t avail;       /* 信号量，表示线程池中实际可用的线程的数量，初值为cnt */
    sem_t used;      /* 信号量，表示线程池中正被使用的线程的数量，初值为0*/
} thread_pool;

/*  线程池函数 */
/*
	初始化线程池
	@param：
	   *tp: 线程池结构体对象指针
	   n： 线程池大小
*/

void pool_init(thread_pool *tp, int n); 
/*
	销毁线程池
	@param: 
		*tp: 线程池结构体对象指针
*/
void pool_destroy(thread_pool *tp);

/*
	使用线程池中的某个线程，将item（本项目是sock值）值赋给该线程
	@oaram:
		*tp: 线程池结构体对象指针
		item: socket值
*/
void use_thread_of_pool(thread_pool *tp, int item);

/*
	获取线程池中当前线程所存储的sock值（这里并没有做销毁线程的操作，只是指针偏移了，而线程销毁是等待它自然销毁）
	@oaram:
		*tp: 线程池结构体对象指针
*/
int release_thread_of_pool(thread_pool *tp);


/*
	线程处理函数
	@param:
		*arg: 指向线程所传入的参数
*/
void *process_thread_function(void *arg);

#endif