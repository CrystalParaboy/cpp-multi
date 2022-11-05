/**
这里以异步的方式启动了任务。它会返回一个future对象。future用来存储异步任务的执行结果，关于future我们在后面packaged_task的例子中再详细说明。在这个例子中我们仅仅用它来等待任务执行完成。
此处是等待异步任务执行完成。
 * */
static const int MAX = 10e8;
static double sum = 0;

void worker(int min, int max) {
  for (int i = min; i <= max; i++) {
    sum += sqrt(i);
  }
}

int main() {
  sum = 0;
  auto f1 = async(worker, 0, MAX);
  cout << "Async task triggered" << endl;
  f1.wait();
  cout << "Async task finish, result: " << sum << endl << endl;
}


/**
面象对象版本
这里通过一个类来描述任务。这个类是对前面提到的任务的封装。它包含了任务的输入参数，和输出结果。
work函数是任务的主体逻辑。
通过async执行任务：这里指定了具体的任务函数以及相应的对象。请注意这里是&w，因此传递的是对象的指针。如果不写&将传入w对象的临时复制。
**/
#include <stdio.h>
#include <cmath>
#include <future>
#include <iostream>
using namespace std;
class Worker{
public:
    Worker(int min, int max):mMin(min), mMax(max){}
    double work(){
        mResult=0;
        for(int i=mMin;i<mMax;i++){
            mResult+=sqrt(i);
        }
        return mResult;
    }
    double getResult(){
        return mResult;
    }
private:
    int mMin;
    int mMax;
    double mResult;
};
int main()
{
    Worker *w(0,10086);
    cout<<"Task in class triggered"<<endl;
    auto f=async(&Worker::work,&w);
    f.wait();
    cout << "Task in class finish, result: " << w.getResult() << endl << endl;
    return 0;
}
