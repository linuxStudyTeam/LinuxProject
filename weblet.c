
#include"weblet.h"

int main(int argc, char **argv)
{
	int listen_sock,conn_sock,port,clientlen;
	struct sockaddr_in clientaddr;
	
	/*cheek command line args*/
	if(argc!=2){
		fprintf(stderr,"usage:%s<port>\n",argv[0]);
		exit(1);
	}
	port=atoi(argv[1]);
	listen_sock = open_listen_sock(port);
	while(1){
		clientlen = sizeof(clientaddr);
		conn_sock = accept(listen_sock,(SA *)&clientaddr,&clientlen);
		process_trans(conn_sock);
		close(conn_sock);
	}
	
}
