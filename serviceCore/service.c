#include "service.h"

int main(int argc, char const *argv[])
{
	char *buf, *p;
    char content[MAXLINE],name[MAXLINE/2], password[MAXLINE/2],op;


    // 读取数据
    readData(name,password,&op);

    /* Make the response body */
    sprintf(content, "Welcome to add.com: ");
    sprintf(content, "%sTHE Internet addition portal.\r\n<p>", content);
    sprintf(content, "%syour name is %s, your passord is %s\r\n<p>", 
	    content, name, password);
    sprintf(content, "%sThanks for visiting!\r\n", content);

    /* Generate the HTTP response */
    printf("Content-length: %d\r\n", strlen(content));
    printf("Content-type: text/html\r\n\r\n");
    printf("%s", content);
    fflush(stdout);
    exit(0);
}