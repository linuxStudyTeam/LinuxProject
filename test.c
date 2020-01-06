#include <stdio.h>
#include <stdlib.h>
int  main(int argc, char const *argv[])
{
	        int len;
          FILE *fp1,*fp2,*fp3,*fp4;
           char *poststr,inputdata[512];
           char servername[10]={0},serverpasswd[10]={0},op[30]={0};
         
           printf("Content-Type:text/html\n\n");
           printf("<HTML>\n");
           printf("<BODY>\n");
           poststr=getenv("CONTENT_LENGTH");
           printf("%s\n",poststr);
          if(poststr==NULL)
               printf("<DIV STYLE=\"/\"mce_STYLE=\"/\"\"COLOR:RED/\">ERROR Input,no Data!</DIV>\n");
           else
            {
             printf("<DIV STYLE=\"/\"mce_STYLE=\"/\"\"COLOR:RED/\"> Input Data OK!</DIV>\n");
             len=atoi(poststr);
             fgets(inputdata,len+2,stdin);
             sscanf(inputdata,"username=%[^&]&password=%[^&]&op=%[^&]",servername,serverpasswd,op);
            }
           printf("</div>\n");
           printf("</BODY>\n");
           printf("</HTML>\n");
           fflush(stdout);
//printf("hello world\n");
	exit(0);
}