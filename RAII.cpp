#include <thread>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <mutex>
using namespace std;
/**
全局的互斥体g_i_mutex用来保护全局变量g_i
这是一个设计为可以被多线程环境使用的方法。因此需要通过互斥体来进行保护。这里没有调用lock方法，而是直接使用lock_guard来锁定互斥体。
在方法结束的时候，局部变量std::lock_guard<std::mutex> lock会被销毁，它对互斥体的锁定也就解除了。
在多个线程中使用这个方法。
**/
int al=0;
mutex al_mutex;
void safe_increment(){
    lock_guard<mutex> lock(al_mutex);
    ++al;
    cout<<this_thread::get_id()<<":"<<al<<endl;
}


int main() {
  cout<<"main"<<al<<'\n';
  thread t1(safe_increment);
  thread t2(safe_increment);
  t1.join();
  t2.join();
  cout<<"main"<<al<<'\n';
}
