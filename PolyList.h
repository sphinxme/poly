#ifndef POLYLIST_POLYLIST_H
#define POLYLIST_POLYLIST_H
#include <iostream>
#include <vector>
#include <list>
#include <sstream>
#include <algorithm>
#include <utility>
#include <map>

using std::vector;
using std::string;
using std::list;

namespace Poly
{
    /**
     * 广义表多项式
     *
     * PolyList 广义表结构多项式类
     * PolyNode 多项式类节点类
     * Mono     单项式结构体(多项式输入输出时使用)
     * variable 变元-幂次 对(多项式读入时使用)
     * ValMap   变元-值 图 (赋值计算时使用)
     *
     */
    class PolyList;
    class PolyNode;
    struct Mono;
    typedef std::pair<char, int> variable;
    typedef std::map<char, double> ValMap;
    enum polyNodeType {ptr, num};


    /**
     * 准备输出字符串时所用的单项式类
     * 如:3x^2y^6z^3
     */
    struct Mono
    {
        int coef = 1;               //每个单项的常系数            //如:3
        string vars;                //每个单项的字母次幂数值       //如: x^2y^6z^3
        bool zero = false;          //工具变量 辅助作用 (为ture时 x^0隐藏变量)
    };



    /**
     * 多项式类
     */
    class PolyList
    {

    private:
        //成员变量
        PolyNode* head;                     //
        int depth;                          // 深度(从1开始 初始为1)
        int length;                         // 长度(从1开始) 不算常数项
        int constant;                       // 常数项值 初始为0
        char name;                          // 本层变元

    public:

        friend class PolyNode;

        explicit PolyList(char name);                       // 以变元为名构造函数
        explicit PolyList(string str);                      // 字符串构造函数
        PolyList(const PolyList& A);                        // 拷贝构造函数
        ~PolyList();


        friend std::ostream& operator<<(                        // 输出运算符
            std::ostream& os, const PolyList& p);
        PolyList& operator= (const PolyList& B);                // 等于运算符
        PolyList& operator= (const string& B);                  // 以字符串构建
        PolyList  operator+ (const PolyList& B) const;          // 加法运算符
      //PolyList  operator+=(const PolyList& B);                // 自加运算符
      //PolyList  operator- (const PolyList& B);                // 减法运算符
      //PolyList  operator-=(const PolyList& B);

        PolyList& operator*=(int i);                            // 自乘系数 求导的时候用

        static void getPolyList(PolyList& me,                   // 将字符串转化为多项式类对象
                                const string& str);
        ValMap getValMap();                                     // 获取变元列表(未赋值)
        double calculate(const ValMap& vals);                   // 代入含有给定值的map并计算
        bool   derivation (char name);                          // 对所给变元求导


    private:

        string print() const;                                   // 底层输出打印函数(不进行空检查)
        static void copyList(const PolyList*& source,           // 工具函数 将copyList
            PolyList*& dest);

        static vector<Mono> cutStr(string str);
        inline static Mono catchStr(string monoStr);

        static PolyList* convert(const Mono& m);
        inline static void insertInOrder(list<variable>& list, const variable& v);
        static PolyList* align(list<variable>::iterator _iter, list<variable>::iterator _end, int endCoef);

        vector<Mono> initPrint() const;                     // 返回自己之下的单项式队列

        void deleteNode(PolyNode*& ptr);

    };

    class PolyNode
    {
    public:
        friend class PolyList;
        polyNodeType tag;      //结点类别标志
        union
        {
            PolyList* lLink;  //tag=ptr,存放系数本身又是多项式
            int coef;          //tag=num,系数是常数
        };
        int exp;               //指数
        PolyNode* nLink;       //同一层下一结点指针

        //构造函数
        //新建空节点 下一节点指针默认为NULL
        explicit PolyNode(polyNodeType _tag = num, int _exp = 0);
        PolyNode(const PolyNode& B);

        ~PolyNode();

        //复制函数
        //递归复制本节点及本节点后续的所有节点
        static void copyNode(const PolyNode*& source, PolyNode*& dest);
        //只复制本节点及其下属的所有多项式层
        static void copyNodeSingle(const PolyNode*& source, PolyNode*& dest);

        PolyNode operator+(const PolyNode& B) const;

        vector<Mono> initprint();//准备输出的单项数列

        ValMap getValMap();
        double calculate(const ValMap& vals);

        bool derivation(char name);

        PolyNode& operator*=(int i);

        //内联函数 用于把ABptr当中指数大的那个接到dest上
        // 然后被接入的指针自动迭代
        // 如果两者指数一样大 就将两者相加后接入
        inline static void addNode(PolyNode*& dest, const PolyNode*& Aptr, const PolyNode*& Bptr);

    };


}

namespace MyTools
{
    string to_string(int val); //fxxk

}
#endif //POLYLIST_POLYLIST_H
