#include "service.h"
int main(int argc, char const *argv[])
{
	char *buf, *p;
    char content[MAXLINE],name[MAXLINE/2], password[MAXLINE/2];
    int sign = 0;

    // 读取数据
    readDataLoginByPost(name,password);

    sign = comfirmUser(name,password);

    if (sign==1)
    {
        // 登录成功页面 
        loginSucceed(name,password);

    }else if(sign==2){
        // 登录失败页面(用户名输错)
        loginFailed(USER_ERROR);

    }else {
        // 登录失败页面(密码输错)
        loginFailed(PWD_ERROR);
    }
    exit(0);
}