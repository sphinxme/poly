#include "PolyList.h"

namespace Poly
{
/*---PolyList Start---*/

//新生成一个以name为变元的多项式广义表
	PolyList::PolyList(char name)
		: head(NULL), depth(1), length(0), constant(0), name(name)
	{
	}

	PolyList::PolyList(string str)
		: head(NULL), depth(1), length(0), constant(0), name('x')
	{
		getPolyList(*this, str);
	}

//深拷贝构造
	PolyList::PolyList(const PolyList& A)
	{
		//重新写
		*this = A;
	}

	PolyList::~PolyList()
	{
		delete head;
		head = NULL;
		depth = 1;
		length = 0;
		constant = 0;
	}

	PolyList& PolyList::operator=(const PolyList& B)
	{
		if (this == &B)          //排除自身赋值
			return (*this);

		//this->~PolyList();
		/*
		//删除之前自有的
		if(head)
			delete head;
		*/

		depth = B.depth;
		length = B.length;
		constant = B.constant;
		name = B.name;
		const PolyNode* tmp = B.head;                   //哎不是...为什么啊
		PolyNode::copyNode(tmp, this->head);
		return (*this);
	}

	PolyList PolyList::operator+(const PolyList& B) const
//再重写需要考虑的情况:两者都只有常数项并入大if 两者有一个是空的也并入大if
	{
		PolyList ret(name);
		if (!(this->head || B.head))
			//当AB都为空的时候(只有常数项)
		{
			if (ret.name > B.name)         //让ret总保持最上层的字母
				ret.name = B.name;
			ret.constant = this->constant + B.constant; //常数项加和

			ret.depth = 1;
			ret.length = 0;

			//至此ret准备完毕 可以返回
		}
		else if (!(this->head && B.head))
			//当AB当中有且仅有一个为空 另一个不为空的时候
		{
			ret = this->head ? (*this) : B;        //把非空的那个复制给ret
			ret.constant = this->constant + B.constant;      //实在是不知道怎么优化
			//至此ret准备完毕 可以返回
		}
		else
			//当AB两个都不为空的时候 又要分情况
		{
			if (this->name == B.name)
				//当两多项式变元相同时
			{
				//常数项相加
				ret.constant += B.constant;

				//单项式同幂次合并 非同幂次按从大到小排列插入


				const PolyNode* Aptr = this->head;
				const PolyNode* Bptr = B.head;

				//循环之前先做一次
				PolyNode::addNode(ret.head, Aptr, Bptr);
				PolyNode* tail = ret.head;
				ret.length = 1;
				while (Aptr || Bptr)
				{
					PolyNode::addNode(tail->nLink, Aptr, Bptr);
					tail = tail->nLink;
					ret.length++;
				}
				//mmp 不优化了

				//新生成的多项式的深度应该是两个多项式深度的最大值?
				//不管了 大不了之后不用深度值
				ret.depth = (this->depth) > (B.depth) ? this->depth : B.depth;
			}
			else
				//当两多项式变元不同时
			{
				ret = (this->name < B.name) ? (*this) : B; //把最上层的那个复制给ret
				const PolyList& add = (this->name > B.name) ? (*this) : B; //把下层的那个指定为add

				//把常数项相加
				ret.constant += add.constant;

				//新多项式深度等于 (原多项式的深度) 与 (add的深度+1) 这两者的最大值
				ret.depth = (ret.depth) > (add.depth + 1) ? ret.depth : add.depth + 1;

				//接下来把add接到/加到ret的0次项上面

				//先找到零次项在哪里
				PolyNode* tailptr = ret.head;
				while (tailptr->nLink)
				{
					if ((tailptr->nLink)->exp == 0)
						break;
					tailptr = tailptr->nLink;
				}

				PolyNode tmp(ptr, 0);
				tmp.lLink = new PolyList(add);
				tmp.lLink->constant = 0;

				if (tailptr->nLink)//有没有0次项
					//如果有0次项的话 这时候应该在ptr后面
				{
					tailptr->nLink = new PolyNode(*(tailptr->nLink) + tmp);
				}
				else
					//如果没有的话 给他加一个
				{
					const PolyNode* ptr = &tmp;
					PolyNode::copyNodeSingle(ptr, tailptr->nLink);
					ret.length++;
				}


				//至此 ret 准备完毕可以返回
			}
		}
		return ret;
	}

/*
PolyList &PolyList::operator+=(const PolyList &B)
{
    return <#initializer#>;
}

PolyList &PolyList::operator-(const PolyList &B)
{
    return <#initializer#>;
}

PolyList &PolyList::operator-=(const PolyList &B)
{
    return <#initializer#>;
}
*/

//private

	void PolyList::copyList(const PolyList*& source, PolyList*& dest)
	{
		if (!source)                                             // 递归终点
		{
			dest = NULL;
			return;
		}
		/*
		if(dest)
			delete dest;                                        // 有隐患
		*/ //TODO:不知道怎么处理

		// 新建同名对象和基本量赋值
		dest = new PolyList(source->name);
		dest->length = source->length;
		dest->constant = source->constant;
		dest->depth = source->depth;

		// 有
		const PolyNode* tmp = source->head;
		PolyNode::copyNode(tmp, dest->head);           // 递归复制节点

	}

	vector<Mono> PolyList::initPrint() const
	{
		vector<Mono> ret;

		PolyNode* ptrNode = head;

		//遍历所有的节点 收缴单项式字符串聚合
		while (ptrNode)
		{
			vector<Mono> tmp = ptrNode->initprint();
			ret.insert(ret.end(), tmp.begin(), tmp.end());
			ptrNode = ptrNode->nLink;
		}

		//处理字符串
		for (auto& val : ret)
		{
			if (val.zero)
			{
				val.zero = false;//是0次方的话就不显示指数和字母了
			}
			else
			{
				val.vars.insert(0, 1, this->name);
			}
		}

		//末尾还有常数项
		if (constant)
		{
			Mono cst;
			cst.zero = true;
			cst.coef = constant;
			ret.push_back(cst);
		}

		//处理完毕
		return ret;
	}
	string PolyList::print() const
	{
		std::stringstream ss;
		vector<Mono> me = initPrint();


		for (auto& val : me)
		{
			if (val.coef >= 0)
				ss << '+';
			else
				ss << '-';

			if (!(val.coef == 1 && !val.vars.empty()))//只有当 系数为1 并且 变元不为空的时候不显示前面的系数 其他都要输出系数
				ss << val.coef;

			ss << val.vars;
		}

		ss.get();


		string ret;
		ss >> ret;
		return ret;
	}
	vector<Mono> PolyList::cutStr(string str)//将读入字符串切片并存储为Mono列返回
	{
		vector<Mono> ret;

		str.erase(std::remove(str.begin(), str.end(), ' '), str.end());//去除字符串中所有空格

		if (!str.empty())
		{
			auto first = str.begin(), second = str.begin();
			while (second != str.end())
			{
				// 寻找(second, end) 之间的'+'和'-'
				second = std::find_if(second + 1, str.end(), [](char a)
				{ return (a == '+') || (a == '-'); });
				ret.push_back(catchStr(string(first, second)));   // 子串送去处理 然后加给返回对象
				first = second; // 迭代器更新
			}
		}

		return ret;
	}
	Mono PolyList::catchStr(string monoStr)
	{
		Mono ret = Mono();

		if (monoStr.empty()) //判空
		{
			ret.coef = 0;
			return ret;
		}

		std::stringstream ss(monoStr);

		switch (ss.peek())          //检测并取走符号位
		{
		case '-':
			ret.coef = -1;
			ss.get();
			break;
		case '+':
			ss.get();
			break;
		}

		if ((ss.peek() >= '0') && (ss.peek() <= '9'))
		{
			int tmp;
			ss >> tmp;
			ret.coef *= tmp;
		}
		if (!ss.eof())
			ss >> ret.vars;

		return ret;
	}

//考虑换成递归
	PolyList* PolyList::convert(const Mono& m)
	{
		PolyList* ret = NULL;
		list<variable> varList;
		std::stringstream ss(m.vars);
		int coefficient = m.coef;

		if (m.vars.empty())                  //如果字符串为空设置流状态标志
			ss.setstate(std::ios_base::eofbit);

		while (!ss.eof())         //每一循环对应一个变元
		{
			//准备变元
			variable v;
			ss.get(v.first);
			if (ss.peek() == '^')
			{
				ss.get();   //吸收'^'
				ss >> v.second;
			}
			else
				v.second = 1;
			if (v.second)        //如果次方不为0 有序插入链表 如果次方为0 直接丢弃
				insertInOrder(varList, v); //有序插入变元
		}

		//至此varList里是从顶层到底层依次排序的字母变元序列
		//准备函数递归

		if (varList.empty())
			//如果为空 直接返回常数项 默认变元为x
		{
			ret = new PolyList('x');
			ret->constant = coefficient;
		}
		else
		{
			ret = align(varList.begin(), varList.end(), coefficient);
		}

		return ret;
	}
	void PolyList::insertInOrder(list<variable>& list, const variable& v)
	{
		//找位置
		auto iter = std::lower_bound(list.begin(), list.end(), v, [](variable a, variable b)
		{ return a.first < b.first; });

		if (iter == list.end()) // 没有的话直接尾插
		{
			list.push_back(v);
		}
		else
		{
			if (iter->first == v.first)  // 如果是同一变元合并
				(*iter).second += v.second;
			else                        // 如果是不同变元插入
				list.insert(iter, v);
		}
	}

	PolyList* PolyList::align(list<variable>::iterator _iter,
		list<variable>::iterator _end,
		int endCoef)
	{
		auto iter = _iter++;
		PolyList* ret = new PolyList(iter->first);

		if (_iter == _end)   //递归终点
		{
			ret->head = new PolyNode(num, iter->second);
			ret->head->coef = endCoef;
			ret->length = 1;
			return ret;
		}
		else
		{
			ret->head = new PolyNode(ptr, iter->second);
			ret->head->lLink = align(_iter, _end, endCoef);
			ret->length = 1;
			ret->depth = ret->head->lLink->depth + 1;
		}
		return ret;
	}
	void PolyList::getPolyList(PolyList& me, const string& str)
	{

		me.~PolyList();         //清空

		//字符串切片
		vector<Mono> monoCuts = cutStr(str);

		//分别转换每个Mono为PolyList并相加
		PolyList* tmp = NULL;
		for (auto& mono : monoCuts)
		{
			tmp = convert(mono);
			me = me + *tmp;
			tmp->~PolyList();   //清空释放
		}

	}
	double PolyList::calculate(const ValMap& vals)
	{
		//定义个容器pair
		//monoval.first用来放值 monova.second用来放次方
		typedef std::pair<double, int> MonoVal;
		vector<MonoVal> mv;//用来收缴每个单项的系数值和次方值

		//收缴每个单项的系数值和次方值
		PolyNode* ptr = head;
		while (ptr)
		{
			mv.emplace_back(ptr->calculate(vals), ptr->exp);
			ptr = ptr->nLink;
		}
		//进行计算
		double myVal = vals.at(name);
		double result = 0;
		for (auto item = mv.begin(); item != mv.end();)//每个幂次
		{
			result += item->first;
			int start = item->second;
			int end = (++item == mv.end()) ? (0) : (item->second);
			for (int i = start; i > end; i--)
				result *= myVal;
		}
		result += constant;

		return result;
	}
	ValMap PolyList::getValMap()
	{
		ValMap ret;

		PolyNode* ptr = head;
		while (ptr)
		{
			ret.merge(ptr->getValMap());
			ptr = ptr->nLink;
		}

		ret[name] = 0;

		return ret;
	}
	bool PolyList::derivation(char name)
	{
		bool ret = false;
		//检查: 自己是否就是要被求导的变元/要求导的在自己上层/要求导的在自己下层

		if (name < this->name)
			//要求导变元在自己上层(即自己下面不可能有被求导的变元了)
		{
			return false;//请求上面的把自己干掉
		}
		else if (name > this->name)
			//要求导的变元可能在自己的下层(那去下面找一找有没有)
		{
			PolyNode* ptr = head;//指向检测节点
			while (ptr)
			{
				if (ptr->derivation(name))
					//如果传回来true代表这个节点已经自行求导过了
				{
					ret = true;
					ptr = ptr->nLink;
				}
				else
					//如果这个节点下面没有了那就干掉这个节点
				{
					deleteNode(ptr);
					//干掉了
				}
			}
		}
		else
			//没错正是在下
		{
			ret = true;
			//对自己求导了
			constant = 0;

			PolyNode* ptr = head;
			while (ptr)
			{
				if (ptr->exp == 0)
					//0次项直接删除
				{
					deleteNode(ptr);
				}
				else if ((ptr->exp == 1) && (ptr->tag == num))
					//如果是1次项而且1次项系数是常数的话 直接并到常数项
				{
					constant = ptr->coef;
					deleteNode(ptr);
				}
				else
					//其他情况就直接系数减一然后系数乘一波
				{
					(*ptr) *= (ptr->exp)--;
					ptr = ptr->nLink;
				}
			}
		}
		return ret;
	}
	void PolyList::deleteNode(PolyNode*& ptr)
	{
		PolyNode* tmp = ptr;
		if (ptr == head)
			//在头节点位置
		{
			head = head->nLink;
			ptr->nLink = NULL;
			delete ptr;
			ptr = head;
		}
		else
			//不在 需要搜索
		{
			PolyNode* pre = head;
			while ((pre->nLink != ptr) && pre)
			{
				pre = pre->nLink;
			}
			if (pre->nLink == ptr)
			{
				pre->nLink = ptr->nLink;
				ptr->nLink = NULL;
				delete ptr;
				ptr = pre->nLink;
			}
		}
	}
	PolyList& PolyList::operator*=(int i)
	{
		constant *= i;

		PolyNode* ptr = head;

		while (ptr)
		{
			*ptr *= i;
			ptr = ptr->nLink;
		}

		return *this;
	}
	std::ostream& operator<<(std::ostream& os, const PolyList& p)
	{
		string out = p.print();
		if (out.empty())
			os << "0";
		else
			os << out;
		return os;
	}
	PolyList& PolyList::operator=(const string& B)
	{
		getPolyList(*this, B);
		return *this;
	}






/*---PolyList End-----*/

/*---PolyNode Start---*/
//广义表结点类的实现部分

	PolyNode::PolyNode(polyNodeType _tag, int _exp)
		: tag(_tag), exp(_exp), nLink(NULL)
	{
		switch (_tag)
		{
		case ptr:
			lLink = NULL;
			break;
		case num:
			coef = 0;
			break;
		default:
			tag = num;
			coef = 0;
			break;
		}
	}

	PolyNode::PolyNode(const PolyNode& B)
	{
		//重新写
		this->tag = B.tag;
		this->exp = B.exp;

		if (this->tag == num)
			this->coef = B.coef;
		else
		{
			const PolyNode* tmp = B.nLink;
			copyNode(tmp, this->nLink);
		}

		const PolyList* tmp = B.lLink;
		PolyList::copyList(tmp, this->lLink);
	}

	PolyNode::~PolyNode()
	{
		if (tag == ptr)                //如果有系数多项式
			if (lLink)               //而且还不为空的话
				delete lLink;       //那就把它删掉

		if (nLink)                   //如果下一节点不为空的话
			delete nLink;           //那也把它删掉
		else
			return;                 //删掉
	}

	void PolyNode::copyNode(const PolyNode*& source, PolyNode*& dest)
	{
		if (!source)                                 //递归终点
		{
			dest = NULL;
			return;
		}
		//TODO:检测dest原先是不是存在有东西 有的话清掉防止内存泄漏
		copyNodeSingle(source, dest);

		const PolyNode* tmp = source->nLink;
		copyNode(tmp, dest->nLink);       //递归

	}

	void PolyNode::copyNodeSingle(const PolyNode*& source, PolyNode*& dest)
	{
		if (!source)
		{
			dest = NULL;
			return;
		}
		dest = new PolyNode(source->tag, source->exp);
		dest->nLink = NULL;
		//信息复制
		if (source->tag == num)
			dest->coef = source->coef;//复制常系数
		else
		{
			const PolyList* tmp = source->lLink;
			PolyList::copyList(tmp, dest->lLink);//反复横跳 复制下一层的多项式对象
		}
	}

	PolyNode PolyNode::operator+(const PolyNode& B) const
	{
		if (exp != B.exp)
			throw "两个指数不同单项相加";

		PolyNode ret(num, exp);

		if ((this->tag == num) && (B.tag == num))
			//俩都是常系数
		{
			ret.coef = (*this).coef + B.coef;
			//准备完毕
		}
		else if ((this->tag == ptr) && (B.tag == ptr))
			//俩都是多项式系数
		{
			ret.tag = ptr;
			ret.lLink = new PolyList(*(this->lLink) + *(B.lLink));
		}
		else
			//有一个是多项式系数 有一个是常系数
		{
			ret.tag = ptr;
			const PolyList* tmp;
			if (tag == ptr)
				tmp = this->lLink;
			else
				tmp = B.lLink;

			PolyList::copyList(tmp, ret.lLink);
		}
		return ret;
	}

	vector<Mono> PolyNode::initprint()
	{
		vector<Mono> ret;

		if (tag == num)
		{
			Mono me;
			me.coef = coef;
			ret.push_back(me);
		}
		else
		{
			vector<Mono> lst = lLink->initPrint();
			ret.insert(ret.end(), lst.begin(), lst.end());
		}

		if (exp == 0)
			for (auto& val : ret)
				val.zero = true;

		else if (exp != 1)
		{
			for (auto& val : ret)
			{
				val.vars.insert(0, MyTools::to_string(exp));
				val.vars.insert(0, "^");
			}
		}

		return ret;
	}

	void PolyNode::addNode(PolyNode*& dest, const PolyNode*& Aptr, const PolyNode*& Bptr)
	{
		if (Aptr && Bptr)
		{
			if (Aptr->exp > Bptr->exp)
				//如果A大 把A接上并迭代A
			{
				copyNodeSingle(Aptr, dest);
				Aptr = Aptr->nLink;
			}
			else if (Aptr->exp < Bptr->exp)
				//如果B大 把B街上并迭代B
			{
				copyNodeSingle(Bptr, dest);
				Bptr = Bptr->nLink;
			}
			else
				//如果AB一样大 生成AB相加后的结果然后接上 然后AB都迭代
			{
				PolyNode tmp = *Aptr + *Bptr;
				const PolyNode* tmptr = &tmp;
				copyNodeSingle(tmptr, dest);

				Aptr = Aptr->nLink;
				Bptr = Bptr->nLink;
			}
		}
		else if (Aptr)
		{
			copyNodeSingle(Aptr, dest);
			Aptr = Aptr->nLink;
		}
		else if (Bptr)
		{
			copyNodeSingle(Bptr, dest);
			Bptr = Bptr->nLink;
		}
	}
	double PolyNode::calculate(const ValMap& vals)
	{
		switch (tag)
		{
		case num:
			return coef;

		case ptr:
			return lLink->calculate(vals);
		}
		return 0;
	}
	ValMap PolyNode::getValMap()
	{

		if (tag == num)
			return ValMap();
		else
			return lLink->getValMap();
	}
	bool PolyNode::derivation(char name)
	{
		if (tag == num)
			return false;
		else
			return lLink->derivation(name);
	}
	PolyNode& PolyNode::operator*=(int i)
	{
		if (tag == num)
			coef *= i;
		else
			*lLink *= i;


		return *this;
	}

/*---PolyNode End-----*/



}

string MyTools::to_string(int val)
{
	string ret;
	while (val)
	{
		ret.insert(0, 1, (char)(val % 10 + '0'));
		val /= 10;
	}
	return ret;
}