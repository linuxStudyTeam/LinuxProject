#include "service.h"

void readData(char* name, char* pwd,char* op)
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