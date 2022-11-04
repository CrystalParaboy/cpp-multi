#include <thread>
#include <time.h>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <cmath>
#include <vector>
#include <mutex>
using namespace std;

/**
thread::hardware_concurrency()可以获取到当前硬件支持多少个线程并行执行。
根据处理器的情况决定线程的数量。
对于每一个线程都通过worker函数来完成任务，并划分一部分数据给它处理。
等待每一个线程执行结束。
在访问共享数据之前加锁
访问完成之后解锁
在多线程中使用带锁的版本
**/
static const int MAX = 10e8; 
static double sum = 0; 
static mutex exclu;

void worker(int min, int max) { 
  double temp_sum=0;
  for (int i = min; i <= max; i++) {
    temp_sum += sqrt(i);
  }
  exclu.lock();
  sum+=temp_sum;
  exclu.unlock();
  
}

void concurrent_task(int min, int max) {
  auto start_time = chrono::steady_clock::now();

  unsigned concurrent_count = thread::hardware_concurrency(); // ①
  cout << "hardware_concurrency: " << concurrent_count << endl;
  vector<thread> threads;
  min = 0;
  sum = 0;
  for (int t = 0; t < concurrent_count; t++) { // ②
    int range = max / concurrent_count * (t + 1);
    threads.push_back(thread(worker, min, range)); // ③
    min = range + 1;
  }
  for (int i = 0; i < threads.size(); i++) {
    threads[i].join();
  }

  auto end_time = chrono::steady_clock::now();
  auto ms = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();
  cout << "Concurrent task finish, " << ms << " ms consumed, Result: " << sum << endl;
}




int main() {
  concurrent_task(0,MAX);
  return 0;
}
