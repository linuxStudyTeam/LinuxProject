#include "service.h"

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


void readDataLoginByPost(char* name, char* pwd){
	// 读取数据
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