#include <stdio.h>
#include <cmath>
#include <future>
#include <iostream>
#include <vector>
using namespace std;
/**
首先创建一个集合来存储future对象。我们将用它来获取任务的结果。
同样的，根据CPU的情况来创建线程的数量。
将任务包装成packaged_task。请注意，由于concurrent_worker被包装成了任务，我们无法直接获取它的return值。而是要通过future对象来获取。
获取任务关联的future对象，并将其存入集合中。
通过一个新的线程来执行任务，并传入需要的参数。
通过future集合，逐个获取每个任务的计算结果，将其累加。这里r.get()获取到的就是每个任务中concurrent_worker的返回值。
**/
double concurrent_worker(int min, int max) {
  double sum = 0;
  for (int i = min; i <= max; i++) {
    sum += sqrt(i);
  }
  return sum;
}

void concurrent_task(int min, int max, promise<double>* result) { // ①
  vector<future<double>> results;

  unsigned concurrent_count = thread::hardware_concurrency();
  min = 0;
  for (int i = 0; i < concurrent_count; i++) {
    packaged_task<double(int, int)> task(concurrent_worker);
    results.push_back(task.get_future()); 

    int range = max / concurrent_count * (i + 1);
    thread t(std::move(task), min, range);
    t.detach();

    min = range + 1;
  }

  cout << "threads create finish" << endl;
  double sum = 0;
  for (auto& r : results) {
    sum += r.get();
  }
  result->set_value(sum); // ②
  cout << "concurrent_task finish" << endl;
}

int main() {
  auto start_time = chrono::steady_clock::now();

  promise<double> sum; // ③
  concurrent_task(0, 10095, &sum);

  auto end_time = chrono::steady_clock::now();
  auto ms = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();
  cout << "Concurrent task finish, " << ms << " ms consumed." << endl;
  cout << "Result: " << sum.get_future().get() << endl; // ④
  return 0;
}
