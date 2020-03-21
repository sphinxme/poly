#include <iostream>
#include <cstdlib>
#include "Test.h"

using namespace std;


int main()
{
    system("mode con cols=80 lines=30");
    //调整窗口大小
    system("chcp 65001 > nul");
    Test *me = Test::getTest();
    //获取单例测试对象

    long choice = 1;
    while (choice != 0)
    {
        system("cls");          //清屏
        me->print();            //打印测试信息
        cin >> choice;          //接收选项
        me->choose(choice);     //处理选项
    }

    //收尾
    delete me;

    return 0;
}


