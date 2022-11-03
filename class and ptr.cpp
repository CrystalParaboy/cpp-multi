#include <iostream>
#include <thread>
using namespace std;
class A{
public:
    mutable int a;                 //mutable 保证const 中 a可以被修改，否则line12报错
    A(int i):a(i) {}
};

void myPrint(const A& temp)         // 在线程中转递类必须用const
{
    temp.a=2;
	cout << "子线程中的a:"<<temp.a << endl;
	cout << "子线程myPrint的参数地址是" << &temp << "thread = " << std::this_thread::get_id() << endl;
	
	//myPrint(const A& temp)中引用不能去掉，如果去掉会多创建一个对象
	//const也不能去掉，去掉会出错
	//即使是传递的const引用，但在子线程中还是会调用拷贝构造函数构造一个新的对象，
	//所以在子线程中修改a的值不会影响到主线程
	//如果希望子线程中修改a的值影响到主线程，可以用thread myThread(myPrint, std::ref(myObj));
	//这样const就是真的引用了，myPrint定义中的const就可以去掉了，类A定义中的mutable也可以去掉了
}

void ptr_myPrint(unique_ptr<int> ptn)
{
	cout << "thread = " << std::this_thread::get_id() << endl;
}

int main()
{
	A myObj(10);
	
	unique_ptr<int> up(new int(10));
	//独占式指针只能通过std::move()才可以传递给另一个指针
	//传递后up就指向空，新的ptn指向原来的内存
	//所以这时就不能用detach了，因为如果主线程先执行完，ptn指向的对象就被释放了
	thread ptr_myThread(ptr_myPrint, std::move(up));
	
	thread myThread(myPrint,myObj);
	myThread.join();
	
	cout << "Hello World!" << endl;
}
