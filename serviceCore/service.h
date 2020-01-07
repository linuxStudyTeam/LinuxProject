#ifndef SERVICE_H_INCLUDED
#define SERVICE_H_INCLUDED


#include "../tools/wrapper.h"

#define USER "2020"
#define PWD "123"
#define USER_ERROR 'A'
#define PWD_ERROR 'B'
void readDataByGet(char* name, char* pwd,char* op);
void readDataLoginByPost(char* name, char* pwd);
int comfirmUser(char* name, char* pwd);
void loginSucceed(char* name, char* pwd);
void loginFailed(char type);
#endif