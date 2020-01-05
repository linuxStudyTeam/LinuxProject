/*weblet 事务处理函数process_trans的源代码,位于weblet.c中*/

void process_trans(int fd){
        int static_flag;
        struct stat sbuf;
        char buf[MAXLINE],method[MAXLINE],uri[MAXLINE],version[MAXLINE];
        char filename[MAXLINE],cgiargs[MAXLINE];
        rio_t rio;

        /*read request line and headers*/
        rio_readinitb(&rio,fd);
        rio_readlineb(&rio,buf,MAXLINE);
        sscanf(buf,"%s %s %s\n",method,uri,version);
        if(strcasecmp(method,"GET")){
                error_request(fd,method,"501","Not Implemented","weblet does not implement this method");
                return;
        }

        read_requesthdrs(&rio);

        static_flag=is_static(uri);
        if(static_flag){
                parse_static_uri(uri,filename);
        }else{
                parse_dynamic_uri(uri,filename,cgiargs);
        }

        if(stat(filename,&sbuf)<0){
                error_request(fd,filename,"404","Not Found","weblet could not find this file");
                return;
        }

        if(static_flag){/*feed static content*/
                if(!(S_ISREG(sbuf.st_mode))||!(S_IRUSR & sbuf.st_mode)){
                        error_request(fd,filename,"403","Forbidden","weblet is not permtted to read the file");
                        return;
                }
                feed_static(fd,filename,sbuf.st_size);
        }else{/*feed dynamic content*/
                if(!(S_ISREG(sbuf.st_mode))||!(S_IXUSR & sbuf.st_mode)){
                        error_request(fd,filename,"403","Forbidden","weblet coould not run the CGI progra");
                        return;
                }
                feed_dynamic(fd,filename,cgiargs);
        }
}

int is_static(char *uri){
        if(!strstr(uri,"cgi-bin"))
                return 1;
        else
                return 0;
}


/*weblet 读取额外请求报头的处理函数read_requesthdrs 的代码,位于weblet.c中*/

void read_requesthdrs(rio_t *rp)
{
        char buf[MAXLINE];

        rio_readlineb(rp,buf,MAXLINE);
        while(strcmp(buf,"\r\n")){
                printf("%s",buf);
                rio_readlineb(rp,buf,MAXLINE);
        }
        return;
}


/*parse_static_uri 和 parse_dynamic_uri 的代码,位于weblet.c中*/

void parse_static_uri(char *uri,char *filename)
{
        char *ptr;
        strcpy(filename,".");
        strcat(filename,uri);
        if(uri[strlen(uri)-1]=='/')
                strcat(filename,"home.html");
}

void parse_dynamic_uri(char *uri,char *filename,char *cgiargs){
        char *ptr;
        ptr = index(uri,"?");
        if(ptr){
                strcpy(cgiargs,ptr+1);
                *ptr='\0';
        }else{
                strcpy(cgiargs,"");
        }
        strcpy(filename,".");
        strcpy(filename,uri);
}


/*weblet 实现静态网页功能的feed_static 的代码,位于weblet.c中*/

void feed_static(int fd,char *filename,int filesize)
{
        int srcfd;

        char *srcp,filetype[MAXLINE],buf[MAXBUF];

        /*send response headers to client*/
        get_filetype(filename,filetype);
        sprintf(buf,"HTTP/1.0 200 OK\r\n");
        sprintf(buf,"%sServer:weblet Web Server\r\n",buf);
        sprintf(buf,"%sContent-length:%d\r\n",buf,filesize);
        sprintf(buf,"%sContent-type:%s\r\n",buf,filetype);
        rio_writen(fd,buf,strlen(buf));

        /*send response body to client*/
        srcfd=open(filename,O_RDONLY,0);
        srcp=mmap(0,filesize,PROT_READ,MAP_PRIVATE,srcfd,0);
        close(srcfd);
        rio_writen(fd,srcp,filesize);
        munmap(srcp,filesize);
}
/*get_filetype-derive file type from file name*/
void get_filetype(char *filename,char *filetype){
        if(strstr(filename,".html"))
                strcpy(filetype,"text/html");
        else if(strstr(filename,".jpg"))
                strcpy(filetype,"image/jpeg");
        else if(strstr(filename,".mpeg"))
                strcpy(filename,"video/mpeg");
        else
                                                                                                                                                                                               149,1-8       67%
                strcpy(filetype,"text/html");
}



/*weblet 实现动态网页功能的feed_static 的代码,位于weblet.c中*/

void feed_dynamic(int fd,char *filename,char *cgiargs)
{
        char buf[MAXLINE],*emptylist[]={NULL};
        int pfd[2];

        /*Return first part of http response*/
        sprintf(buf,"HTTP/1.0 200 OK\r\n");
        rio_writen(fd,buf,strlen(buf));
        sprintf(buf,"Server:weblet Web Server\r\n");
        rio_writen(fd,buf,strlen(buf));

        pipe(pfd);
        if(fork()==0){
                close(pfd[1]);
                dup2(pfd[0],STDIN_FILENO);
                dup2(fd,STDOUT_FILENO);
                execve(filename,emptylist,environ);
        }

        close(pfd[0]);
        write(pfd[1],cgiargs,strlen(cgiargs)+1);
        wait(NULL);   /* parent waits for and reaps child*/
        close(pfd[1]);
}


/*weblet 对错误请求的处理函数error_request 返回一条出错消息,位于weblet.c中*/

void error_request(int fd,char *cause,char *errnum,char *shortmsg,char *description){
        char buf[MAXLINE],body[MAXBUF];

        /*build the http response body*/
        sprintf(body,"<html><title>error request</title>");
        sprintf(body,"%s<body bgcolor=""ffffff"">\r\n",body);
        sprintf(body,"%s %s:%s\r\n",body,errnum,shortmsg);
        sprintf(body,"%s<p>%s:%s\r\n",body,description,cause);
                                                                                                                                                                                               192,1-8       93%
        /*send the http respose*/
        sprintf(buf,"HTTP/1.0 %s %s\r\n",errnum,shortmsg);
        rio_writen(fd,buf,strlen(buf));
        sprintf(buf,"Content-type:text/html\r\n");
        rio_writen(fd,buf,strlen(buf));
        sprintf(buf,"Content-length: %d\r\n\r\n",(int)strlen(body));
        rio_writen(fd,buf,strlen(buf));
        rip_writen(fd,body,strlen(body));
}

