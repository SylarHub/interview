# c++基础

[toc]

##  1. volatile关键字有什么作用
结论：在编译器优化时可能对变量进行各种各样的优化处理，用该关键字修饰变量就告知编译器，不要对其进行优化，每次从内存中取值，而不是使用寄存器中备份值.
volatile可以和const一起修饰变量，如只读状态寄存器。也可以执行一个指针。这个关键字大多用嵌入式编程中，平时不怎么需要使用。
看如下面试题：
```
int square(volatile int *p)
{
    return （*p） * (*p);
}
```
这个代码的作用是什么，有没有什么问题？
其实就是间接考察了volatile关键字，*p的值可能会处于意想不到的变化中，所以可能不是期望返回平方值. 需要如下做
```
int a = *p;
return a * a;
```
至于网上常说的结论，多线程中共享的变量用volatile来修饰，其实就是错误说法。这两者没有根本的关系，这个关键字就是让编译器不做过多优化，多线程共享变量一般就是用锁和原子变量atomic去修饰。具体的可以看如下这篇文章 [volatile和多线程关系](https://www.zhihu.com/question/31459750/answer/52069135)


## 2. const关键字
既然谈到关键字，就谈一谈其他的关键字。
const意思是可读，用于修饰函数、变量等，标明不可读。注意只是在编译器阶段限定了可读性，在编译期的保护.实际运行中还是可以用const_cast进行 取消const操作.而且得注意    对全局const变量，存在于bss或data区域，调用const_cast转换会报错.

const修饰类成员函数时，不能修改变量，不能调用非const函数。  实现的机制就是函数内传入this指针是const指针. 这里还有c++11的知识点，const函数内要修改变量怎么做，这时候用mutable修饰变量即可，代表是可变的。

面试题 ： 在函数f中怎么调用到两个fun函数
```
struct S
{
	int value;
	void foo()
	{
		printf("in foo()\n");
	}
	void foo() const
	{
		printf("in foo() const\n");
	}
    void f()const
    {
        //TODO
    }
};

```
const函数 可以有const对象或者const函数调用，非const解绑就行
```
void f()const
{
    foo();
    const_cast<S *>(this)->foo();
}
```

## 3. constexpr关键字
谈到上面const关键字，c++11标准新出了关键字constexpr，可以用来修饰变量，函数，告知编译器在编译时候就可以算出具体的值，让编译器尽情的进行优化，不需要每次运行时都重新计算一遍.
constexpr修饰函数时只能调用全局constexpr对象，只能有一个return，只能调用其他consntexpr函数(核心就是为了在编译时就能算出具体的值)
一个函数返回constexpr值时，它的实现必须放到头文件中
```
constexpr int x = 12;

//计算阶乘
constexpr int factorial(int n) {return n <=0 ? 1 : (n*factorial(n-1));}

```
constexpr是一种比const 更严格的束缚,

## 4. static关键字
static可以用来修饰变量和函数. 主要是修改了存储位置和作用域.
一个作用是隐藏，保证不被其他文件所使用。第二个作用是保持变量的持久，离开作用域也能生效。第三个修饰C++类成员和函数，为所有类共用。

从内存布局来说，data段可以存放有初值的static对象和局部static对象。bss段可以存放未初始化的全局static对象

修饰局部static变量：如果未初始化，会被默认初始化为0. 离开作用域后会继续驻留在内存中，只会初始化一次，可以用来当计数器等.
修饰全局静态变量：在cpp1中定义的普通的全局变量，可以在同工程下其他cpp内通过extern使用到。静态全局变量只能在本文件生效。 所以可以用来隐藏变量，不被其他文件所使用。
修饰全局静态函数：同上，作用域问题。

修饰类成员变量：所有类的对象共有，不占用类的存储空间(求sizeof时不计入). 定义时在类外定义.
修饰类成员函数：所有类的对象共有，该类只能访问对应的静态成员变量，类的静态成员函数。如果需要访问具体非静态成员变量，需要提供具体的类对象. 核心就是不包含this指针.因为不属于任何对象，所以也不能用virtual修饰.

在C语言中静态变量在编译器时就分配好了内存，而在c++下是写时复制思想，只有第一次用到时才会对其进行内存分配，并通过atexit进行管理。初始化时也可以用其他变量来初始化。

## 5. extern关键字
  
1:用于拓展函数和变量的作用域，在A文件中的变量和函数，通过extern拓展到同工程下B文件使用. 提示编译器去其他文件下寻找定义.
2: 用于规范c++ 调用c。  由于c++实现了重载，编译时会对函数名进行重命名(加入参数类型等信息),在c++中调用c语言规范的函数时，需要通过extern c{}进行规范，用来解决链接时名字匹配问题。

## 6. explict关键字
用来修饰构造函数，标明不能进行隐式转换。
隐式转换，如下：
```
class Demo
{
private:
	int a;
public:
	 Demo(int a):a(a){}
};

Demo s = 12;  //隐式转换，调用单参数的构造函数 Demo(int a)


```
对构造函数前面标明explicit时，即可限制这种操作.

## 7. 类型转换
常知的类型转换有四种，下面一一介绍
const_cast<> ：       用于去除const属性.
reinterpret_cast<>：强制转换，从底层二进制数据进行强制转换，非必要不使用。整型可以转换成指针等.
dynamic_cast<>：   派生类之间的转换，常用来将下行转换( 指向派生类的基类指针  转换为 派生类指针）,在运行阶段动态转换.
static_cast<>：        用于基本数据类型之间的转换，也可用于上行转换(派生类对象转换为基类指针),任何类型和void、void *转换。建议在隐式转换中都用此代替.在编译器进行转换，效率更高.对比C隐式转换会进行一定的安全检查，如char *转为int *会出错

## 8. 虚函数实现
虚函数是通过虚函数表实现。当一个类中有virtual修饰的函数时，编译器会为该类创建一个虚函数表vtable(一维数组)，数组里存放的是每个虚函数的地址，在构造函数中会对虚函数指针vptr进行初始化，关键到具体的虚函数里（所以构造函数不可以是虚函数，假设是虚函数，需要通过虚函数表指针调用，但是虚函数表指针会未初始化，又找不到对应的实现）。 运行时多态指的是，根据指向对象的不同，来指向不同的虚函数表，从而实现多态.
在vs命令行中输入/d1 reportSingleClassLayoutXXX（XXX为类名），可以输出类的内存布局，并且可以从对象地址 直接调用到 所有的虚函数。


```
#define PRINT_FUN cout << __FUNCTION__ << endl;
class Base
{
public:
	int vB;
public:
	virtual void h1() { PRINT_FUN }
	virtual void k1() { PRINT_FUN }
};
class Derive : public Base
{
public:
	int vD;
public:
	virtual void h1() { PRINT_FUN }
	virtual void k2() {PRINT_FUN}
};


int main(int argc, char *argv[]) {
	typedef void(*Fun)();
	Derive d;
	int * p = reinterpret_cast<int *>(&d);	            //p就是虚函数表指针，对象首地址4个字节				
	int *pTable = reinterpret_cast<int *>(*p);		   // 取虚函数表指针里的值，就是对应的虚函数表位置,转换为数组首地址（每个元素都是函数地址）
	
	Fun pFun = (Fun)*(pTable);                        //调用的是第一个虚函数Dervie::h1，这个*就是取数组里某个值的意思 arr[i] = *(arr+i)
	pFun();

	Fun pFun1 = (Fun)*(pTable+1);                    ////调用的是第二个虚函数Dervie::k1
	pFun1();
	return 0;
}
```
输入/d1 reportSingleClassLayoutDerive，可以看到类的内存布局如下:
![内存布局](vx_images/241765916236646.png)

更详细的多重继承，虚继承可以参考 [虚函数实现](https://blog.csdn.net/futurewu/article/details/77197273)

## 9. new delete malloc free
malloc/free：负责内存分配
new/delete： 1：调用operator new/delete/new []/delete[] 进行内存分配 2：调用构造/析构函数 3：返回对应的对象指针。

在operator new[]中的时候会在对象前4字节位置分配好有多少个元素，在delete[]中通过偏移，找到有多少个元素. 通过以下例子重载operator new，可以看出传入函数指针数值和对象地址偏移了4个位置。
```
class Base
{
public:
	int vB;
	Base() { cout << "Base():" << this << endl; }
	~Base() { cout << "~Base():" << this << endl; }
public:
	static void * operator new[](size_t size)
	{
		Base* p = (Base*)malloc(size);
		cout << "operator new[]" << size << " p = " << p << endl;
		return p;
	}
	static void operator delete[](void *p, size_t size)
	{
		cout << "operator delete[]" << size << " p = " << p << endl;
		free(p);
	}
};

/*
Base *p = new Base[4];
delete []p;
*/
```
![运行结果](vx_images/99645217256812.png)

理解了上面原理，对应的new[]能不能用delete释放，new能不能用delete[]释放都可以轻松回答出来.(传入地址-4 +4)。
用new分配的空间用free释放，理论上来说没有问题，但是没有调用到析构函数，可能会造成内存泄漏。

类型的右placement new，也就是operator new的一种重载方式，不进行分配内存，在已分配的内存上进行构造.比如可用在共享内存中分配对象.
```
char * p = (char *)malloc(2000);
Base * pp = new (p) Base;
delete pp;
```

## 10. 数组名a、&a
数组名也是类似，它是一个别名.
看如下案例，可以得知a和&a
```
int a[] = { 1,2,3,4,5 };

cout << "**************************" << endl;
for (int i = 0; i < 5; ++i)
{
	cout << "a[i]地址" << &(a[i]) << endl;
}
cout << "**************************" << endl;
cout << "a= " << a << endl;
cout << "&a= " << &a << endl;
cout << "&a +1 = " << &a +1 << endl;
cout << "a +1 =  " << a + 1 << endl;
```
![运行结果](vx_images/49424621249482.png)
结论： 数组名本质是是一个地址，一个常量指针。  所以
一维数组名 &a+1偏移整个一维数组空间大小数组位置 a+1直接偏移一个元素位置。 。 对数组名&和sizeof操作时，有特殊处理，sizeof是整个数组大小，取&后+1是偏移一个数字空间位置.
二维数组名 &a+1偏移整个二维数组空间大小位置   a+1偏移一个一维数组长度,  &a[0] + 1偏移一个一维数组长度 a[0]+1偏移一个元素长度.

## 11. #和##
看如下例子
```
#define ToString(a)  #a			//转为字符串
#define Conn(x,y) x##y		//关键字拼接
int main()
{
	cout << ToString(456) << endl;
	int val1 = 10;
	cout << Conn(val, 1) << endl;		
	return 0;
}

```
结果是456 和 10.

## 12. 设计一个不可继承的类

第一个思想就是将类的构造函数设置为私有，然后提供一个GetInstance的静态接口给外部获取类对象.但是必须手动调用释放接口。而且必须得修改类的实现.
第二个方法很巧妙：利用虚继承，有虚继承时，最底层的类会直接调用顶层类的构造函数，此时将顶层类的构造函数设置为私有，设置一个友元类。当类不可被继承时，只需要虚继承这个模板类即可。代码如下
```
template<class T>
class ClassNoInHeritance
{
	friend T;
private:
	ClassNoInHeritance(){}
};

class Base : virtual  public ClassNoInHeritance<Base>
{

};

class Derive : public Base
{
    
}
```
//Derive类在构造时，由于是虚继承关系，直接调用ClassNoInHeritance，此时无法调用它的私有构造函数。 而对应的Base类由于是友元类的关系，所以能调用ClassNoInHeritance的构造函数。 
Base b; //可实现

## 13. 重写、重载、隐藏
重写：派生类对虚函数的重写，多态的基础之一
重载：函数参数不同，可重载，多态的基础之一
隐藏：子类的某一个函数名和父类的某一个函数名相同（不必要求参数），这时候默认调用子类的接口。  deervie.Base::fun() 也可以这样调用到父类的对应接口


## 14. 智能指针
为了更好的解决C++内存泄漏问题和多次释放等问题，提出智能指针解决方案。通过RAII思想(资源获取就是初始化)在智能指针对象 构造、析构时，同步对原始指针进行操作.
多次释放问题，如下
```
class Demo{}
Demo *p = new Demo;
vector<Demo *> v;
v.push_back(p);
delete p;
```
v[0]和p都是指向同一片内存，p释放后，对v操作会异常。

### 14.1 unique_ptr
独占式智能指针，对赋值运算符和拷贝构造函数设置为protect，可以进行移动构造。
智能指针离开作用域会析构，释放内部存储的原始指针，也可以调用release方法直接释放原始指针（此操作不会析构智能指针）
1：构造
```
Demo *p = new Demo();

unique_ptr<Demo> u1(p);

unique_ptr<Demo> u2 = make_unique<Demo>();

unique_ptr<Demo> u3(std::move(u1));
```
2:  释放
release操作：释放智能指针对原始指针的所有权。
reset操作： 对原始指针进行析构。
默认采用 std::default_delete<T> 方法释放堆内存，也可以自己定义删除器。具体参考下一节shared_ptr删除器
3:可用
重载了(),直接用 if( u )来判断可用性
### 14.2 shared_ptr
如果保持对象的共有权，此时用到shared_prt。其本质就是一个模板类。内部存储着对象指针数据和引用计数指针。
1：构造，可通过多种方式进行构造. 一般来说 std::make_shared 是最推荐的一种写法
```
class Demo
{}

int * a = new int(5);
shared_ptr<int> p1(a);    //不推荐使用，外部可能析构p，不符合RAII思想

std::shared_ptr<Demo> p2(new Demo[3](),default_delete<Demo[]>());		//Demo[]，支持对象数组
p2.get()[1].fun;

std::shared_ptr<Demo> p3 = make_shared<Demo>();		//不支持对象数组，C++20 支持 auto p = std::make_shared<int[5]>();

```
2：自定义释放规则，在对象释放时可以自定义，可支持如下几种方式
```
std::shared_ptr<Demo> p2(new Demo[3](),default_delete<Demo[]>());		//默认default_delete模板类，支持T和T[]，对()进行重载内部调用析构

void delObj(Demo *p)
{
    delete[] p;
}
std::shared_ptr<Demo> p2(new Demo[3](),delObj);                         //自定义函数

std::shared_ptr<Demo> p2(new Demo[3](),[](Demo *p){delete[] p;});    //lambada表达式

```
3：引用计数
增加：进行拷贝构造、赋值运算符等操作时。用shared_ptr指针给同类的指针赋值。作为函数参数(形参调用了拷贝构造)
减少：智能指针离开作用域-1/指针指向了另外一种对象/reset调用无参-1.
这里有篇很好的文章详细的解释了引用计数的原理-[引用计数原理](https://blog.csdn.net/qq_29108585/article/details/78027867)
一般人可能想到是定义了static变量共用计数(std::shared_ptr<Demo> p1(pOrignal);std::shared_ptr<Demo> p2(pOrignal)这种做法 按照static变量方式，引用计数应该是2，其实他们是独立的.).所以排除这种思路。
智能指针内部有一个引用计数区域指针，执行一片内存。在智能指针构造的时候，会在堆中创建一片引用计数区域(存储原子引用计数等信息)，当进行赋值运算符、拷贝构造等操作时，将指针指向这片引用计数区域。当智能指针析构时，取消关联并将引用计数-1. 进行reset操作时取消关联，引用计数-1。

![引用计数原理图](vx_images/509154410246042.png)

4：使用注意事项
不能让shared_ptr多次引用同一数据，会导致一次new多次delete
不能让shared_ptr指向栈上对象.也是多次释放问题.
不要手动delete get到的原始指针.    
使用get返回的指针，当唯一一个指向动态对象的智能指针销毁后，get指针也是失效了

### 14.3 weak_ptr
主要用来解决两个问题。
第一是shared_ptr的循环引用问题。死锁无法释放最终的资源。只需要对其中一个shared_ptr修改为weak_ptr即可
第二是悬浮指针问题，如有两个指针p1和p2，指向堆上的同一个对象Object，p1和p2位于不同的线程中。假设线程A通过p1指针将对象销毁了（尽管把p1置为了NULL），那p2就成了空悬指针。此时用weak_ptr可以判断指针释放还继续有效，有效则提升为shared_ptr继续操作.

从本质来说，是为了配合shared_ptr使用的，weak_ptr对象与shared_ptr对象共享一个计数器对象 (标明强引用计数和弱引用计数)在拷贝构造和赋值运算符等操作时，并未对强引用计数进行+1。
weak_ptr对象依赖shared_ptr/weak_ptr对象生成，并自动调用计数器递增弱引用计数。当某个weak_ptr生命期结束时，会自动调用析构函数，析构函数中会通过计数器递减弱引用计数，若弱引用计数为0，析构函数就会调用计数器的_Delete_this()函数删除计数器对象。



1：构造
```
shared_ptr<Demo> s = make_shared<Demo>();

weak_ptr<Demo> w1(s);
weak_ptr<Demo> w2(w1);
```

2 :其他
reset: 取消和shared_ptr对象的关联
lock: weak_ptr并不能直接访问资源，用当前的weak_ptr对象构造一个shared_ptr对象出来
expired()：看cppreference文档可知，主要是判断对应的强引用计数是否为0.

返回this指针，用weak_ptr，直接继承enable_shared_from_this即可。
### 14.4 线程安全
经典问题，智能指针是线程安全的吗。这里推荐一篇相当好的文章，[智能指针安全问题](https://www.zhihu.com/question/56836057/answer/2158966805)

线程安全，主要就是共享数据时，    能包装所以线程下的行为都是可预期的。
考虑如下问题

第一：引用计数增加是否是线程安全。
引用计数采用原子操作，自增自减都是线程安全
第二：修改指向时是否安全
线程处理是可以对智能指针修改指向. 这时分情况考虑，多线程代码操作的不是同一个shared_ptr的对象。
传递引用或者 指针的指针时，操作的是同一个对象，这时候修改指向时，涉及引用计数-1，重新指向其他原始指针等。涉及几个操作，这里不是原始的。 这是线程不安全的。
不是同一个对象时，因为对引用计数的修改是线程安全的，所以是安全的。
第三：获取获取进行操作释放安全
比如智能指针内部存储vector指针， 取出vector指针进行多线程操作，可以得知这也是线程不安全的。但是这个问题不是智能指针的问题，是原始数据 非线程安全的问题。


### 14.5 终极内部实现--
这里推荐两篇文章，从源码级的角度去分析 具体是怎么实现，可以帮助我们更好掌握智能指针
[从源码理解智能指针（二）—— shared_ptr、weak_ptr](https://blog.csdn.net/qq_28114615/article/details/101166744)

![继承关系](vx_images/279591512259676.png)



真正的计数器是子类来实现，_Ref_count_base的子类有三种:_Ref_count、_Ref_count_del和_Ref_count_del_alloc。从子类名也可以看出来，从前往后三种子类的功能更强大，体现在计数、删除器和分配器三个方面

reset接口：两种，一个是强引用一个是弱引用的。修改参数里对象的引用计数，修改现有对象的引用计数，修改指针指针和引用计数指针。
从无参或者shared_ptr 构造出对应的weak_ptr，都是调用上述resetxxx接口

建议详细看看上面这篇文章，对各种接口和实现都能了如于心。


## 15. atomic和无锁队列
### 15.1 atomic
[atomic官方文档](https://zh.cppreference.com/w/cpp/atomic/atomic)
首先谈到经典问题，多线程对同一变量进行自增，这是线程不安全的。因为++i或者sum=sum+1并不是一个原子操作，它会造成错误。
这是atomic就来了，它实现了若干类型的原子操作.
1:适用类型
基本类型都适用，int,char,bool,long等等。另外也支持自定义类型，满足五个表达式都为true就可（从文档中可以查看，满足支持拷贝，复制，并且是位可比的，也支持memcpy操作.）
2: 不支持拷贝构造函数，所以也无法作为形参。 传参时需要传递引用.
3: store    修改包含的值，内部也可指定相关的内存顺序(memory_order_seq_cst之类的.指令执行顺序之类的)
   load 读取包含的值。
   exchange: 读取并修改为新值，原子操作。
   
### 15.2 无锁队列
无锁队列通常是以下这种形式
```
do
{
    备份旧数据
    赋值新数据
}while（ **CAS(内存地址，旧数据，新数据**)）
```
现代cpu中都支持CAS的原子操作.

atomic也有两个CAS函数，compare_exchange_strong 和compare_exchange_weak. (weak版本的CAS允许偶然出乎意料的返回（比如在字段值和期待值一样的时候却返回了false），不过在一些循环算法中，这是可以接受的。通常它比起strong有更高的性能。)

a.compare_exchange_weak(b,c)
如果a==b,返回true,将c值赋值给a。
如果a!=b，返回false,将a赋值给b. ()
要么赋值，要么拉取最新的数据，重新循环

根据以上的思路，可以很简单的设计出一个无锁队列.
```
struct node { int val; node *next; };
atomic<node*> listHead;

void push(int val)
{
	//常规思路，构造新节点指向newNode，然后让头节点指向新节点
	node * oldHead = listHead;
	node * newNode = new node{ val,oldHead };
	while (!listHead.compare_exchange_weak(oldHead, newNode))
		newNode->next = oldHead;
	//没有其他线程在执行，此时listHead ==oldHead,返回true. 将头节点listHead指向新节点newNode
	//在分配完newNode后，可能其他线程push了数据，oldHead已经不是头节点，此时返回false，oldHead获取了最新的listHead, 执行将新节点的下一个指针挂在头节点上，然后继续判断，走上面的一步
	//为什么需要while.因为这是执行完newNode->next = oldHead;后，可能又有线程push了数据，oldHead又不是最新的了。
}
```

上面谈到的是C++11的atomic支持的CAS， 在windows中也可以用InterlockedCompareExchange。gcc下用compare_and_swap.


此时有一个经典的ABA问题，线程1取值A，修改为B，最终修改为A。然后线程2发现没有变化，继续执行。这是错。可以用doule Cas来实现，变量+引用计数。 虽然变量还是一样，但是结点内存引用计数变了。详情可看[无锁队列的实现](https://coolshell.cn/articles/8239.html)

## 16. lambada表达式
C++11中新增的一个语法糖，简洁高效。核心就是
[capture list] (params list) mutable exception-> return type { function body }
capture list：外部捕捉列表，引用捕获，值捕获，隐式捕获(=和&，eg:[=, &a, &b])。
param list：参数，必须有参数名，不支持可变参数，不能有默认参数
mutable：值捕获时也可以修改值
-> return type：没有返回值可以省略，返回明确值也可以由编译器进行类型推导。

它的本质好函数对象是一样的，定义一个lambada表达式，编译器会自动生成一个类，并重载()实现.

## 17. decltype
C++11提出的 用于推导类型的关键字。类型说明符，它的出现主要是解决复杂的类型声明。
三种用法
1：lambada表达式
```
如果需要lambda表达式的类型type，可以使用decltype；
auto cmp = [](const st &x1, const st &x2) {return false; };        
map<st, int,decltype(cmp)> m(cmp);            //注意m后面括号和decltype
```
![](vx_images/185694118257281.png)
lambada没有默认的构造和析构函数

2：表达式
decltype(expr)的结果根据expr的结果不同而不同：expr返回左值，得到该类型的左值引用；expr返回右值，得到该类型
3：变量
decltype(var)完美保留了变量的类型

## 18. 左值、右值
当对象被用作左值的时候，用的是对象的身份（在内存中的位置）；当一个对象被用作右值的时候，用的是对象的值（内容）
左值引用，就是平常说的引用。为左值引用初始化的一定是左值
```
int a = 1;
int &b = a;
```
右值引用，为右值引用初始化的一定是右值，引用右值引用，也为了对象移动. 函数返回值，表达式的值等都是临时变量,这些都是右值
```
int a = 1;
int &b = 1 + 3;
```
std::move 将一个左值转换为一个右值

## 19. 移动语义
资源的移动，类会默认生成一个移动构造函数.(位拷贝来进行)
```
String(String&& s): _str(s._str)
{ s._str = nullptr; }
```
[移动语义](https://theonegis.github.io/cxx/C-%E7%A7%BB%E5%8A%A8%E8%AF%AD%E4%B9%89%E5%8F%8A%E6%8B%B7%E8%B4%9D%E4%BC%98%E5%8C%96/)
## 20. 完美转发
```
template<typename T>
void fun(T a)
{
    otherFun(a);
}
```
类似参考上面函数，传入左值时，最终传入otherFun也是左值。传入右值时也需要传入右值。这是完美转发。
forward实现上述功能, 涉及引用折叠(所有右值引用折叠到右值引用上仍然是一个右值引用,.所有的其他引用类型之间的折叠都将变成左值引用).
```
template<typename T1, typename T2>
void set(T1 && var1, T2 && var2){
  m_var1 = std::forward<T1>(var1);
  m_var2 = std::forward<T2>(var2);
}
```

```
template <typename T>
constexpr T forward(typename std::remove_reference<T>::type & t)noexcept
{
    return static_cast<T &&>(t);    //在强制类型转换中，将参数 __t 传递给对应类 _Tp 的构造函数，然后调用了该类的构造函数从而完成对象创建过程。
}
```

解除引用
```
template <typename T>
class remove_reference
{
public:
   typedef T type;
};

template<typename T>
class remove_reference<T&>
{
public:
   typedef T type;
};

```
## 20. map自定义比较函数
默认的map的比较函数是less.以下几种方式可以自定义比较函数.
重载<
```
struct st
{
	int a;
	int b;
	bool operator < (const st & s)const        //注意不要漏了const
	{
		return false;     //测试用
	}
};
map<st,int> m;        //左键重载<
```
函数对象：
```
struct cmp
{
    bool operator ()(const st &x1,const st &x2)const
    {
        return false;    //测试用
    }
}
map<st,in,cmp> m;
```
lambada表达式
```
auto cmp = [](const st &x1, const st &x2) {return false; };        
map<st, int,decltype(cmp)> m(cmp);            //注意m后面括号和decltype

```

## 21. unordered_map自定义hash和比较函数
查看其定义可知
template < class Key,                        //键值对中键的类型
           class T,                          //键值对中值的类型
           class Hash = hash<Key>,           //容器内部存储键值对所用的哈希函数
           class Pred = equal_to<Key>,       //判断各个键值对键相同的规则
           class Alloc = allocator< pair<const Key,T> >  // 指定分配器对象的类型
           > class unordered_map;

可以通过函数对象或者重载运算符的方式。
hash函数需要重载(),返回size_t 用于hash_key.
equal_to需要重载==,返回bool类型。

用函数对象时，都是重载operator()，hash单参数返回size_t，equal双参数返回bool类型，  注意需要const.

hash表拓容： 当元素个数> 总个数*装载因子时，对buket下每个节点，都计算加入新bukket(放入的位置可能不同)
![](vx_images/183283320254783.png)

## 22 push_back和emplace_back
想详细区别，看源码。
1:C++11后，对push_back传入右值引用，内部也是调用emplace_back.
```
#if __cplusplus >= 201103L
void push_back(value_type &&__x) {
    emplace_back(std::move(__x));
}
#endif
```
2: emplace_back和push_back区别
主要是构造的区别，一个是原地构造。
```
_Alloc_traits::construct(this->_M_impl, this->_M_impl._M_finish,
                                 std::forward<_Args>(__args)...); // emplace_back()--完美转
_Alloc_traits::construct(this->_M_impl, this->_M_impl._M_finish,
                                 __x);                            // push_back()
```
对于多个参数而言，可以直接将构造的参数传过去，原地构造一个对象。



## 23. 构造函数和析构函数能不能调用虚函数
基类构造函数初始化时，子类还未构造，基类析构函数调用虚函数时，子类已经析构，虚函数指针未正确执行虚函数表，不能调用到虚函数表，编译为了安全处理，自行进行了静态绑定，直接寻址调用基类对应的函数。

## 24. 如何让类只能动态分配和静态分配
只能静态分配：重新operator new 和operator new[]操作，设置为私有。
只能动态分配：设置构造和析构函数为protected.并提供静态接口，动态生成类的对象.

## 25. 友元
用friend来修饰，不可以继承和传递。 友元类和友元函数都是外部的，不属于类内部。但是需要在类内部进行声明，告知编译器函数的详细情况，从而控制访问。


## 26. auto
auto关键字在编译期间可以进行自动推导类型,也可用来替代复杂的类型。auto仅仅是一个占位符，它并不是一个真正的类型
以下是若干细节：(可以自己写实例来测试，用typeid来输出对应的类型---注意，会忽略顶层的const和&输出。
typeid(int)  typeid(const int)  typeid(int &))都是int.

1：对const变量 进行auto会自动去除const属性，除非 const auto 变量
2：对引用变量 进行auto会自动去除&属性，除非  auto &变量
3：对数值进行auto 会退化成指针，如果用&则 保持类型一致

## 27. typename和泛型编程
typename和class功能一致，都是在泛型编程中声明一个类型。
```
template<typename T>
void fun(const T& proto, typename const T::const_iterator iter){
	T::const_iterator it =iter;
}
```
用typename 声明类型T，在参数中声明const T::const_iterator是一个类型(当前编译器会自动优化，不需要typename.对一些负责的类型，需要告知编译器这是一个类型，而不是其他的东西).

编译器会对函数模板进行两次编译：在声明的地方对模板代码本身进行编译，在调用的地方对参数替换后的代码进行编译(所以模板类声明和实现放在相关h文件中，一般其他文件在使用时需要去h文件进行二次编译)。被实例化后才有具体的函数.

## 28. 引用的本质
既然分析到本质，从编译到底层一步步分析。
从编译角度看，引用也在符号表中占据一个位置（符号表：存储变量和地址相关信息）
从汇编角度看，看如下
![引用](vx_images/550092409236650.png)
可以看出，引用和指针在底层没什么区别，都是将变量取址，然后赋值给对应内存位置.
从这个角度来看，引用也是一个受限的指针（不能++，--，&等等）.


## 29. 构造函数的初始化列表
初始化列表的构造函数 和类内部成员的声明顺序一致.
以下四种情况的初始化必须用到函数的初始化列表.
```
class Base
{
public:
	Base(int i){}
};
class Demo : public Base
{
public:
	Demo(int& x):b(1),a(2),c(x),Base(12){}
	void fun(){c += 1;}
private:
	const int a;
	Base b;
	int &c;
};
```
注意。构造函数x传引用，原理查看上一节。不能将刑参的地址 赋值给c.


## 30 动态链接库和静态链接库
这篇文章相当好
[动态链接库和静态链接的创建和使用](https://zhuanlan.zhihu.com/p/544022813)
动态链接库： so文件，dll文件，在程序运行过程直接链接，直接共享一份，节省内存.发布更新时直接修改dll文件
静态链接库： a文件(libxxx.a), lib文件。在程序编译过程中直接打包进入对应的执行程序中。发布更新时需要重新生产exe文件

linux的静态库的创建和使用 :
```
g++ -c xxx.cpp
ar rcs libxxx.a xxx.o

g++ -o demo TestStaticLibrary.cpp -L../StaticLibrary -lxxx()
```
windows下的静态库创建和生成：在工程中生成静态库文件，直接生成相应的文件，测试文件中直接调用对应的h文件，附加静态库的目录和静态库文件名，直接使用.

linux下动态库的创建和使用：
```
//gcc编译的执行文件默认是ELF格式，不需要初始化入口
g++ -fPIC -shared -o libxxx.so xxx.cpp
g++ TestDynamicLibrary.cpp -L../DynamicLibrary -lxxx     //需要修改/etc/ld.so.conf，将库文件目录加入重建cache

```
windows下动态库的创建和使用：执行文件是PE格式，需要入口，所以需要_declspec(dllexport)关键字
为同一h文件，直接根据宏定义，在创建定义宏即可。使用时不定义宏
```
#ifdef DLL_EXPORT
#    define DLL_API __declspec(dllexport)
#else
#    define DLL_API __declspec(dllimport)
#endif
```
生成是生成一个dll文件和lib文件(上静态库lib文件不同，只是一个导入库，包括地址符号表确保能正确找到程序而已)。使用时依旧需要附加对应的导入库lib文件，然后将dll文件放入对应的exe桶目录。


可重定位文件：包含二进制代码和数据，可以在编译时和其他可重定位文件 生成一个可执行目标文件。
![](vx_images/195591310248868.png)
systab：符号表，存储了程序中定义的函数和全局变量信息.可以用readelf来读取，局部静态变量符号表在运行时由栈进行管理。

链接的主要作用就是符号解析(将变量函数等引用和 符号表中一个确定的符号关联起来)和重定位(合并节并重定位节中的地址，有).

相关命令接口有readelf(显示一个目标文件的完整结构)、nm(显示一个目标文件的符号表中定义的符号)，ar(静态库命令)，objdump(二进制工具之母，显示一个目标文件中所有的信息，最足要的是进行反汇编)




## 31 随机数库
[c++11随机数库](https://blog.csdn.net/CSDNwei/article/details/113865349)
C++11提供了随机数引擎和分布类。   引擎有若干中生成随机数方式，可以生成一个随机数。分布类可以按照正太分布、均匀分布等方式生成一个对应分布下的随机数。

以前常用的rand可以看做是随机数，但是在rand() % m用于生成0，m-1的随机数时就会出错，此时分布有问题。用此方式生成的随机数不随机了。
c++11中提供了分布类，用于更好生成各种分布。
注意：
1：在函数中定义引擎的话，最好就定义一次，在全局地方或者设置为局部变量
2：引擎种子可以用真随机数 std::rand_device.
下面代码可以用于测试随机情况:

```

#include<iostream>
#include <vector>
#include <stdlib.h>
#include <ctime>
#include <list>
#include <map>
#include <string>
#include <bitset>
#include <stack>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <sys/types.h>
#include <stdarg.h>
#include <cmath>
#include <set>
#include <crtdbg.h>
#include "assert.h"
#include <deque>
#include <climits>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <random>
#include <windows.h>  
using namespace std;
int maxCount = 100000000;
int needProp = 2500;
int maxProp = 10000;
void RandTest()
{
	int meetCount = 0;
	for (int i = 0; i < maxCount ; ++i)
	{
		if (rand() % maxProp < needProp)
		{
			meetCount++;
		}
	}
	cout << "rand:" <<meetCount * 1.0 / maxCount * maxProp << endl;
}

void EngineRand()
{
	static bool randSeed = false;
	static default_random_engine e;
	static uniform_int_distribution<int> u(0,maxProp-1);
	if (!randSeed)
	{
		randSeed = true;
		e.seed(std::random_device());
	}
	int meetCount = 0;
	for (int i = 0; i < maxCount ; ++i)
	{
		if (u(e) < needProp)
		{
			meetCount++;
		}
	}
	cout << "EngineRand:" <<meetCount * 1.0 / maxCount * maxProp<< endl;
}
int main()
{
	srand(time(nullptr));
	cout << "输入循环测试数量:";
	cin >> maxCount;
	cout << "输入总概率:";
	cin >> maxProp;
	while (true)
	{
		cout << endl << "输入要计算的概率:"; 
		cin >> needProp ;
		RandTest();
		EngineRand();
	}
	
	EngineRand();

	return 0;
}
```
## 32 原地初始化
C++对类非静态成员可以就地初始化，用=或{}进行初始化，它的赋值顺序在参数初始化之后. 在其他定义变量，也可以这样原地初始化。
```
class Demo
{
private:
    int a = 1;
    vector<int> v{1,2,3,4};
public:
    Demo(){}
    Demo(int x):a(x){}
}
```

## 33 final关键字
修饰虚函数，这样派生类无法重载该接口
## 34 override关键字
修饰派生类的虚函数，该接口 必须在基类的虚函数中（清晰表现当前类的接口是 虚函数还是新定义接口）

