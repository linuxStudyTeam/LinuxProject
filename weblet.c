#include "thread_pool.h"
#include "weblet.h"

thread_pool tp; // 声明为外部变量
int main(int argc, char **argv)
{
	int listen_sock,conn_sock,port,clientlen,i;
	struct sockaddr_in clientaddr;
	pthread_t tid; // 定义线程变量
	struct hostent *hp;
	char *haddrp;
	pool_init(&tp, SIZE_OF_POOL); // 初始化线程池

	/*cheek command line args*/
	if(argc!=2){
		fprintf(stderr,"usage:%s<port>\n",argv[0]);
		exit(1);
	}
	port=atoi(argv[1]);
	listen_sock = open_listen_sock(port);

	for (i = 0; i < SIZE_OF_POOL; i++)       /* Create worker threads */
	    pthread_create(&tid, NULL, process_thread_function, NULL);
	while(1){
		clientlen = sizeof(clientaddr);
		conn_sock = accept(listen_sock,(SA *)&clientaddr,&clientlen);
		process_trans(conn_sock);

		hp = Gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr, 
			   sizeof(clientaddr.sin_addr.s_addr), AF_INET);
	    haddrp = inet_ntoa(clientaddr.sin_addr);
	    printf("server connected to %s (%s)\n", hp->h_name, haddrp);

	    use_thread_of_pool(&tp, conn_sock);  /* Insert conn_sock in task pool */
		close(conn_sock);
	}
	
}
