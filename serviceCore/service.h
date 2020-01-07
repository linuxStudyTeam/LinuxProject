#ifndef SERVICE_H_INCLUDED
#define SERVICE_H_INCLUDED


#include "../tools/wrapper.h"
#include <ctype.h>
#define USER "2020"
#define PWD "123"
#define USER_ERROR 'A'
#define PWD_ERROR 'B'
#define TYPE_ERROR 'C'
#define LOGIC_ERROR 'D'
#define ADD '1'
#define SUB '2'
#define MUL '3'
#define DIV '4'
#define CONVERT_FAILED '5'
#define CONVERT_SUCCEED '6'

//void readDataByGet(char* name, char* pwd,char* op);
// 读取数据
void readData(char* name, char* pwd,char* op);
//void readDataLoginByPost(char* name, char* pwd);

// 验证用户
int comfirmUser(char* name, char* pwd);

// 登录成功
void loginSucceed(char* name, char* pwd);

// 登录失败
void loginFailed(char type);

// 计算器结果展示页面
void resultDisplay(int firstNum,int secondNum,char type);

// 字符串转成整型
char convertStrToInt(char *str_num,int *int_num);

#endif