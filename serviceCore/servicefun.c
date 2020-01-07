#include "service.h"

// get请求获取数据
void readDataGet(char* name, char* pwd,char* op)
{
	// 读取数据
	char c;
	int i=0,sign=0;
	while(1){
		c = getchar();
		if (c=='\0')
		{
			break;
		}
		if(c=='&'){		
			name[i] = '\0';
			i = 0;
			sign++;
			//break;
			continue;
		}
		if(sign==0){ // 读用户名
			name[i] = c;
		}else if(sign==1){ // 读密码
			pwd[i] = c;
		}else if(sign==2){
			*op = c;
		}	
		i++;
	}

}

// post请求获取数据
void readDataLoginByPost(char* name, char* pwd){
	char c;
	/*
		mutex 为自定义的互斥量，表示若没有读取到=，即不需要给name或pwd赋值，因为传递过来的参数是"A=值&B=值"形式
		sign: 表示按顺序赋值
	*/
	int i=0,mutex=0,sign=0;
	while(1){
		c = getchar();
		if (c=='\0')
		{
			break;
		}
		if (mutex==1)
		{
			if(c=='&'){		
				name[i] = '\0';
				i = 0;
				sign++;
				//break;
				mutex=0;
				continue;
			}
			if(sign==0){ // 读用户名
				name[i] = c;
			}else if(sign==1){ // 读密码
				pwd[i] = c;
			}/*else if(sign==2){
				*op = c;
			}*/
			i++;
		}
		if (c == '=')
		{
			mutex = 1;
		}	
	}	

}

// 验证用户
int comfirmUser(char *name, char *pwd)
{
	if (strcmp(name, USER)!=0)
	{
		return 2;
	}
	if (strcmp(pwd, PWD)!=0)
	{
		return 3;
	}
	return 1;
}

void loginSucceed(char* name, char* pwd){
	char content[MAXLINE];
	/* Make the response body */
   	sprintf(content, "Welcome to add.com: ");
    sprintf(content, "%sTHE Internet addition portal.\r\n<p>", content);
    sprintf(content, "%syour name is %s, your passord is %s\r\n<p>", 
            content, name, pwd);
    sprintf(content, "%sThanks for visiting!\r\n", content);

    /* Generate the HTTP response */
     printf("Content-length: %d\r\n", strlen(content));
     printf("Content-type: text/html\r\n\r\n");
     printf("%s", content);
     fflush(stdout);
}

void loginFailed(char type)
{
	if (type == USER_ERROR)
	{
		char content[MAXLINE];
		/* Make the response body */	
   		sprintf(content, "<div style='text-align:center;margin-top:200px'>Welcome to cal.com </div>");
    	sprintf(content, "%s<div style='text-align:center'> THE Internet addition portal.\r\n </div>", content);
    	sprintf(content, "%s<div style='text-align:center;color:red'>your username is illegal</div>\r\n", content);
    	sprintf(content, "%s<div style='text-align:center'>the legal name is 2020</div>\r\n", content);
	    /* Generate the HTTP response */
	     printf("Content-length: %d\r\n", strlen(content));
	     printf("Content-type: text/html\r\n\r\n");
	     printf("%s", content);
	     fflush(stdout);
	} else if (type == PWD_ERROR)
	{
		char content[MAXLINE];
		/* Make the response body */
   		sprintf(content, "<div style='text-align:center;margin-top:200px'>Welcome to cal.com </div>");
    	sprintf(content, "%s<div style='text-align:center'> THE Internet addition portal.\r\n </div>", content);
    	sprintf(content, "%s<div style='text-align:center;color:red'>your password is illegal</div>\r\n", content);
    	sprintf(content, "%s<div style='text-align:center'>please contact to the manager</div>\r\n", content);
	    /* Generate the HTTP response */
	     printf("Content-length: %d\r\n", strlen(content));
	     printf("Content-type: text/html\r\n\r\n");
	     printf("%s", content);
	     fflush(stdout);
	}
}