#include <iostream>
#include <thread>
using namespace std;

void myPrint()
{
	cout << "我的线程开始运行" << endl;
	cout << "我的线程运行完毕" << endl;
	return;
}

int main()
{
	//(1)创建了线程，线程执行起点（入口）是myPrint；(2)执行线程
	//main函数中
    auto lambdaThread=[](int a){
        cout<<"开始线程"<<a;
        cout<<"结束线程"<<a<<endl;
    };

	thread myThread(lambdaThread,1);
	myThread.join();

	if (myThread.joinable())
	{
		cout << "可以调用join()或者detach()" << endl;
	}
	else
	{
		cout << "不能调用join()或者detach()" << endl;
	}
	
	cout << "Hello World!" << endl;
	return 0;
}
