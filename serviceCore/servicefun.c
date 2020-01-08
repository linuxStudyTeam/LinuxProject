#include "service.h"

// get与post请求获取数据
void readData(char* firstParm, char* secondParm,char* op)
{
	char c;
	/*
		mutex 为自定义的互斥量，表示若没有读取到=，即不需要给firstParm或secondParm赋值，因为传递过来的参数是"A=值&B=值"形式
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
				if (sign==0){
					firstParm[i] = '\0';
				}else if(sign==1){
					secondParm[i] = '\0';
				}	
				i = 0;
				sign++;
				//break;
				mutex=0;
				continue;
			}
			if(sign==0){ // 读用户名
				firstParm[i] = c;
			}else if(sign==1){ // 读密码
				secondParm[i] = c;
			}else if(sign==2){
				*op = c;
			}
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
   	sprintf(content, "<div style='text-align:center;margin-top:200px'>Welcome to cal.com </div>");
    sprintf(content, "%s<div style='text-align:center'> THE Internet addition portal.</div>\r\n", content);
    sprintf(content, "%s<div style='text-align:center'>your name is %s, your passord is %s</div>\r\n", 
            content, name, pwd);
    sprintf(content, "%s<div style='text-align:center'>use the cal system now!!!</div>\r\n", content);
    sprintf(content, "%s<div style='text-align:center'><form action='calService' method='get'>please input first num: <input name='firstNum'></br>please input second num: <input name='secondNum'></br>add:<input type='radio' name='op' value=1>sub:<input type='radio' name='op' value=2>mul:<input type='radio' name='op' value=3>div:<input type='radio' name='op' value=4><br/><input type='submit' value='submit'/></form></div>\r\n", content);
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
    	sprintf(content, "%s<div style='text-align:center'> THE Internet addition portal. </div>\r\n", content);
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
    	sprintf(content, "%s<div style='text-align:center'> THE Internet addition portal.</div>\r\n ", content);
    	sprintf(content, "%s<div style='text-align:center;color:red'>your password is illegal</div>\r\n", content);
    	sprintf(content, "%s<div style='text-align:center'>please contact to the manager</div>\r\n", content);
	    /* Generate the HTTP response */
	     printf("Content-length: %d\r\n", strlen(content));
	     printf("Content-type: text/html\r\n\r\n");
	     printf("%s", content);
	     fflush(stdout);
	}
}



void resultDisplay(int firstNum,int secondNum,char type){

    char content[MAXLINE];
    sprintf(content, "<div style='text-align:center;margin-top:200px'>Welcome to cal.com </div>");
    if (type==ADD){
    	sprintf(content, "%s<div style='text-align:center'>The answer is: %d + %d = %d</div>\r\n", content, firstNum, secondNum, firstNum + secondNum);
    }else if (type==SUB){
    	sprintf(content, "%s<div style='text-align:center'>The answer is: %d - %d = %d</div>\r\n", content, firstNum, secondNum, firstNum - secondNum);
    }else if (type==MUL){
    	sprintf(content, "%s<div style='text-align:center'>The answer is: %d * %d = %d</div>\r\n", content, firstNum, secondNum, firstNum * secondNum);
    }else if(type==DIV){
    	sprintf(content, "%s<div style='text-align:center'>The answer is: %d / %d = %d</div>\r\n", content, firstNum, secondNum, firstNum / secondNum);
    }else if (type==TYPE_ERROR){
    	sprintf(content, "%s<div style='text-align:center;color:red'>TYPE_ERROR: your must input the integer data</div>\r\n", content);
    }else if(type==LOGIC_ERROR){
    	sprintf(content, "%s<div style='text-align:center;color:red'>LOGIC_ERROR: divisor can't be zero</div>\r\n", content);
    }else {
    	sprintf(content, "%s<div style='text-align:center;color:red'>UNKNOWN_ERROR: please contact the manager</div>\r\n", content);
    }
    sprintf(content, "%s<div style='text-align:center'>Thanks for visiting!</div>\r\n", content);
    printf("Content-length: %d\r\n", strlen(content));
    printf("Content-type: text/html\r\n\r\n");
    printf("%s", content);
    fflush(stdout);
    exit(0);
}


char convertStrToInt(char *str_num,int *int_num){
	char content[MAXLINE];
	int i,j,len = strlen(str_num),temp;
	for(i=0;i<len;i++){
		if (isdigit(str_num[i])) {
			temp = str_num[i] - 48; // 隐式转换
			for(j=1;j<=len-i-1;j++){
				temp *= 10;	
			}
			*int_num += temp;
			
	    }else {
	    	return CONVERT_FAILED;  
	    }
	}
	return CONVERT_SUCCEED;
}
