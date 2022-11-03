#include <iostream>
#include <thread>
using namespace std;

void myPrint(const int &i, char* pmybuf)
{
	//如果线程从主线程detach了
	//i不是mvar真正的引用，实际上值传递，即使主线程运行完毕了，子线程用i仍然是安全的，但仍不推荐传递引用
	//推荐改为const int i
	cout << i << endl;
	//pmybuf还是指向原来的字符串，所以这么写是不安全的
	cout << pmybuf << endl;
}

int main()
{
	int mvar = 1;
	int& mvary = mvar;
	char mybuf[] = "this is a test";
	thread myThread(myPrint, mvar, mybuf);//第一个参数是函数名，后两个参数是函数的参数
	myThread.join();
	
	cout << "Hello World!" << endl;
}
