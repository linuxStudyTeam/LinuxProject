#include "weblet.h"
/*weblet 事务处理函数process_trans的源代码,位于weblet.c中*/

void process_trans(int fd){
        int static_flag;
        struct stat sbuf;
        char buf[MAXLINE],method[MAXLINE],uri[MAXLINE],version[MAXLINE];
        char filename[MAXLINE],cgiargs[MAXLINE];
        rio_t rio;

        /*读取请求行，请求头*/
        rio_readinitb(&rio,fd);
        rio_readlineb(&rio,buf,MAXLINE);
        sscanf(buf,"%s %s %s\n",method,uri,version);
        //printf("123:%s\n", buf);
        rio_readlineb(&rio,buf,MAXLINE);

        // strcasecmp 忽略大小写比较字符串 相等返回0
        if(!strcasecmp(method,"GET")){
            // GET请求代码
            read_requesthdrs(&rio);
            // 判断是否为静态页面
            static_flag=is_static(uri);
            if(static_flag){
                   // printf("静态页面%d\n",static_flag);
                    parse_static_uri(uri,filename);
            }else{
                  //  printf("动态页面%d\n",static_flag);
                    parse_dynamic_uri(uri,filename,cgiargs);

            }
            // stat 获取文本信息
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

        }else if (!strcasecmp(method,"POST")){
       
           //printf("123: %s\n", rio.rio_buf);
           deal_parm_post(rio.rio_buf,cgiargs);
          // printf("------:%d\n", rio.rio_cnt);
          // printf("%s, %d", rio.rio_buf, strlen(rio.rio_buf));
  
            // stat 获取文本信息
            /*if(stat(filename,&sbuf)<0){
                    error_request(fd,filename,"404","Not Found","weblet could not find this file");
                    return;
            }
            
            if(!(S_ISREG(sbuf.st_mode))||!(S_IRUSR & sbuf.st_mode)){
                    error_request(fd,filename,"403","Forbidden","weblet is not permtted to read the file");
                    return;
            }  
            if(fork()==0){ // 子进程
                    printf("创建子进程\n");
                    execve("./test",NULL,environ);
            }
            wait(NULL);   /* parent waits for and reaps child
            printf("运行结束\n");*/
            // }else{

               /* error_request(fd,method,"501","Not Implemented","weblet does not implement this method");
                return;
            }*/
       }

        
}

int is_static(char *uri){
        /*
            strstr(str1,str2) 函数用于判断字符串str2是否是str1的子串。
            如果是，则该函数返回 str1字符串从 str2第一次出现的位置开始到 str1结尾的字符串；
            否则，返回NULL。
        */
        // 含有？ 证明有参数传入，即需要做动态响应
        if(!strstr(uri,"?"))
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
        //printf("前： %s\n", filename);
        char *ptr;
        strcpy(filename,".");
        strcat(filename,uri);
        //printf("后： %s\n", filename);

        /*if(uri[strlen(uri)-1]=='/')
                strcat(filename,"./WEB/home.html");*/
}

void parse_dynamic_uri(char *uri,char *filename,char *cgiargs){
        char *ptr;
        ptr = index(uri,'?');
        if(ptr){
                strcpy(cgiargs,ptr+1);
                *ptr='\0';
        }else{
                strcpy(cgiargs,"");
        }
        strcpy(filename,".");
        strcat(filename,uri);
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
       // printf("文件名%s\n", filename);
        /*send response body to client*/
        srcfd=open(filename,O_RDONLY,0);
        //printf("%d\n", filesize);

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
                strcpy(filetype,"text/html");
       // printf("%s\n", filetype);
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
        if(fork()==0){ // 子进程
                close(pfd[1]); // pfd[0] r; pfd[1] w
                dup2(pfd[0],STDIN_FILENO);
                dup2(fd,STDOUT_FILENO);
                /*
                    execve()用来执行参数filename字符串所代表的文件路径，
                    第二个参数是利用指针数组来传递给执行文件，
                    并且需要以空指针(NULL)结束，最后一个参数则为传递给执行文件的新环境变量数组
                */
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
        /*send the http respose*/
        sprintf(buf,"HTTP/1.0 %s %s\r\n",errnum,shortmsg);
        rio_writen(fd,buf,strlen(buf));
        sprintf(buf,"Content-type:text/html\r\n");
        rio_writen(fd,buf,strlen(buf));
        sprintf(buf,"Content-length: %d\r\n\r\n",(int)strlen(body));
        rio_writen(fd,buf,strlen(buf));
        rio_writen(fd,body,strlen(body));
}



// 处理post请求所提交的数据
void deal_parm_post(char *p, char *cgiargs)
{
    int len = strlen(p);
    char str[2][];
    // 440 请求头长度（不需要用到的部分）
    for (int i = 440,int j=0,int k=0; i < len; ++i)
    {
        str[k][j++]= p[i];
        if (p[i])
        {
            /* code */
        }
    }



    

    /*if(ptr){
            strcpy(cgiargs,ptr+1);
            *ptr='\0';
    }else{
            strcpy(cgiargs,"");
    }
    strcpy(filename,".");
    strcat(filename,uri);
    //printf("222:%s\n", p);*/
}

