#ifndef TEST_H
#define TEST_H

#include <vector>
#include <iostream>
#include "PolyList.h"
#include <cstdlib>

using std::vector;
using std::string;
using std::cout, std::cin, std::endl;
using namespace Poly;

class Test
{
    static Test * only;
public:
    vector<PolyList> lst;       //存储多项式的容器

    void choose(int i);         //管理选择函数

    void print();               //输出信息
    void testInput();           //输入测试
    void testAdd();             //加法测试
    void testEval();            //求值测试
    void inline tevistDer();      //求导测试

    static Test * getTest();    //单例模式相关

private:
    Test();


};


#endif //TEST_H
