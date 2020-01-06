#include "thread_pool.h"
#include "weblet.h"

thread_pool tp; // 声明为外部变量
struct hostent *hp;
char *haddrp;
struct sockaddr_in clientaddr;
int main(int argc, char **argv)
{
	int listen_sock,conn_sock,port,clientlen,i,sign=1;

	pthread_t tid; // 定义线程变量

	pool_init(&tp, SIZE_OF_POOL); // 初始化线程池

	/*检查是否有输入端口，若没有则直接结束*/
	if(argc!=2){
		fprintf(stderr,"抱歉，您没有输入端口号，格式：%s <port>\n",argv[0]);
		exit(1);
	}
	port=atoi(argv[1]);


	
	listen_sock = open_listen_sock(port); // 监听端口

	for (i = 0; i < SIZE_OF_POOL; i++)       /* 给线程池创建线程 */
	    pthread_create(&tid, NULL, process_thread_function, NULL);
	while(1){
		clientlen = sizeof(clientaddr);
		// 设置存储值


		conn_sock = accept(listen_sock,(SA *)&clientaddr,&clientlen);
		//char *poststr=getenv("CONTENT_LENGTH");
        //printf("12312:  %s\n",poststr);

		// 因为服务器的网络名字与网络地址基本上是不会变化，所以加了个判断条件，只获取一次，减少没有必要的时间消耗
		if (sign==1)
		{
			// Gethostbyaddr 返回对应于给定地址的主机信息（封装在结构体中）
			hp = Gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr, sizeof(clientaddr.sin_addr.s_addr), AF_INET);
			// inet_ntoa 网络地址转换成“.”点隔的字符串格式
			haddrp = inet_ntoa(clientaddr.sin_addr);
			sign = 2;
		}
	    use_thread_of_pool(&tp, conn_sock);  /* 给线程池中的线程添加数据（赋sock值） */
	}
	
}
