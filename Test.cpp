#include "Test.h"

Test * Test::only = nullptr;

Test::Test()
= default;

Test * Test::getTest()
{
    if(!only)
        only = new Test();

    return only;
}

void Test::print()
{
    int num;
    cout << "\n\n\n\n\t\t\t[ 研讨三 | 广义表多项式 | 功能测试 ]" << endl;
    cout << "\n" << endl;
    cout << "\t\t\t\t[1] 新建多项式" << endl;
    cout << "\t\t\t\t[2] 多项式相加" << endl;
    cout << "\t\t\t\t[3] 多项式计算" << endl;
    cout << "\t\t\t\t[4] 多项式求导" << endl;
    cout << "\t\t\t\t[0] 退出" << endl;

    cout << endl << "\t\t\t    ---[当前多项式列表]---" << endl;

    num = 0;
    for(auto &v : lst)
    {
        cout << "\t\t\t    [ " << num++ << " ] "
             << v << endl;
    }
    if(lst.empty())
        cout << "\t\t\t    (为空 请新建)" << endl;

    cout << "\t\t\t    ----------------------" << endl;
}

void Test::choose(int i)
{
    switch (i)
    {
    case 1:
        testInput();
        break;
    case 2:
        testAdd();
        break;
    case 3:
        testEval();
        break;
    case 4:
        testDer();
        break;
    default:
        break;
    }
}

void Test::testInput()
{
    cout << "\t\t\t\t [新建多项式]\n" << endl
         << "\t请输入多项式表达式(如: x^3+xy^5+7) 暂不支持括号" << endl;
    string s;
    cin >> s;
    lst.emplace_back(PolyList(s));
}

void Test::testAdd()
{
    int a,b;
    cout << "\t\t\t\t [多项式相加]\n" << endl
         << "\t请输入第一个要相加的序号" << endl;
    cin >> a;
    cout << "\t请输入第二个要相加的序号" << endl;
    cin >> b;

    try {
        cout << "\t结果是 " << (lst.at(a) + lst.at(b)) << endl;
    }
    catch (std::out_of_range&)
    {
        cout << "输入选项不正确哈" << endl;
    }

    system("pause");
}

void Test::testEval()
{
    int choice;
    cout << "\t\t\t\t [多项式计算]\n" << endl
         << "\t请输入要计算多项式的序号" << endl;
    cin >> choice;

    PolyList& p = lst.at(choice);

    ValMap vals = p.getValMap();

    cout << "\t该多项式有 " << vals.size() << " 个变量,请输入对应变量的值" << endl;
    for(auto &item : vals)
    {
        cout << "\t" << item.first << " = ";
        cin >> item.second;
        cout << endl;
    }

    cout << "\t计算结果为:" << p.calculate(vals) << endl;
    system("pause");



}
void inline Test::testDer()
{
    char v;
    int choice;
    cout << "\t\t\t\t [多项式求导]\n" << endl
         << "\t请输入要求导多项式的序号" << endl;
    cin >> choice;

    PolyList& p = lst.at(choice);

    cout << "\t输入要求导的变元:" << endl;
    cin >> v;

    p.derivation(v);

}
