#include <thread>
#include <time.h>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <sstream>
//这里创建了两个线程。它们都会有一些输出，其中一个会先停止3秒钟，然后再输出。主线程调用join会一直卡住等待它运行结束。
using namespace std;
void print_time() {
  auto now = std::chrono::system_clock::now();
  auto in_time_t = std::chrono::system_clock::to_time_t(now);

  std::stringstream ss;
  ss << std::put_time(localtime(&in_time_t), "%Y-%m-%d %X");
  cout << "now is: " << ss.str() << endl;
}

void sleep_thread() {
  this_thread::sleep_for(chrono::seconds(3));
  cout << "[thread-" << this_thread::get_id() << "] is waking up" << endl;
}

void loop_thread() {
  for (int i = 0; i < 10; i++) {
    cout << "[thread-" << this_thread::get_id() << "] print: " << i << endl;
  }
}

int main() {
  print_time();

  thread t1(sleep_thread);
  thread t2(loop_thread);
  
  t2.detach();
  t1.join();

  print_time();
  return 0;
}
