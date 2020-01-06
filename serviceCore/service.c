#include "../wrapper.h"

int main(int argc, char const *argv[])
{
	char *buf, *p, *name, *password;
    char content[MAXLINE];
    

    /* Extract the two arguments from standard input */
    scanf("%s&%sd", name, password);
    
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