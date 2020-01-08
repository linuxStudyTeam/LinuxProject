#include "service.h"
int main(int argc, char const *argv[])
{

    char firstNum[MAXLINE/2], secondNum[MAXLINE/2],op;
    char content[MAXLINE];
    int first_num=0,second_num=0;
    // 读取数据
    readData(firstNum,secondNum,&op);
    //验证字符串是否全为整型算法
   if(convertStrToInt(firstNum,&first_num)==CONVERT_FAILED||convertStrToInt(secondNum,&second_num)==CONVERT_FAILED){
        resultDisplay(first_num,second_num,TYPE_ERROR);
    }else{
        if (op==DIV && second_num==0){
            resultDisplay(first_num,second_num,LOGIC_ERROR);
        }else {
            resultDisplay(first_num,second_num,op);
        }
    }
    exit(0);
}