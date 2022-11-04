#include <thread>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <mutex>
#include <stdlib.h>
#include <condition_variable>
#include <set>
using namespace std;

class Account {
public:
  Account(string name, double money): mName(name), mMoney(money) {};
/**
 * 这里声明了一个条件变量，用来在多个线程之间协作。
这里使用的是unique_lock，这是为了与条件变量相配合。因为条件变量会解锁和重新锁定互斥体。
这里是比较重要的一个地方：通过条件变量进行等待。此时：会通过后面的lambda表达式判断条件是否满足。如果满足则继续；如果不满足，则此处会解锁互斥体，并让当前线程等待。解锁这一点非常重要，因为只有这样，才能让其他线程获取互斥体。
这里是条件变量等待的条件。如果你不熟悉lambda表达式，请自行网上学习，或者阅读我之前写的文章。
此处也很重要。当金额发生变动之后，我们需要通知所有在条件变量上等待的其他线程。此时所有调用wait线程都会再次唤醒，然后尝试获取锁（当然，只有一个能获取到）并再次判断条件是否满足。除了notify_all还有notify_one，它只通知一个等待的线程。wait和notify就构成了线程间互相协作的工具。
请注意：wait和notify_all虽然是写在一个函数中的，但是在运行时它们是在多线程环境中执行的，因此对于这段代码，需要能够从不同线程的角度去思考代码的逻辑。这也是开发并发系统比较难的地方。
 **/
public:
  void changeMoney(double amount) {
    unique_lock lock(mMoneyLock); // ②
    mConditionVar.wait(lock, [this, amount] { 
      return mMoney + amount > 0; 
    });
    mMoney += amount;
    mConditionVar.notify_all();
  }
  string getName() {
    return mName;
  }
  double getMoney() {
    return mMoney;
  }
  mutex* getLock() {
    return &mMoneyLock;
  }

private:
  string mName;
  double mMoney;
  mutex mMoneyLock;
  condition_variable mConditionVar;
};
class Bank {
public:
  void addAccount(Account* account) {
    mAccounts.insert(account);
  }

  void transferMoney(Account* accountA, Account* accountB, double amount) {
    accountA->changeMoney(-amount);
    accountB->changeMoney(amount);
}

  double totalMoney() const {
    double sum = 0;
    for (auto a : mAccounts) {
      sum += a->getMoney();
    }
    return sum;
  }

private:
  set<Account*> mAccounts;
};

mutex sCoutLock;
void randomTransfer(Bank* bank, Account* accountA, Account* accountB) {
  for(int i=0;i<=20;i++) {
    double randomMoney = ((double)rand() / RAND_MAX) * 100;
    {
      lock_guard guard(sCoutLock);
      cout << "Try to Transfer " << randomMoney
           << " from " << accountA->getName() << "(" << accountA->getMoney()
           << ") to " << accountB->getName() << "(" << accountB->getMoney()
           << "), Bank totalMoney: " << bank->totalMoney() << endl;
    }
    bank->transferMoney(accountA, accountB, randomMoney);
  }
}

int main() {
  Account a("Paul", 100);
  Account b("Moira", 100);

  Bank aBank;
  aBank.addAccount(&a);
  aBank.addAccount(&b);

  thread t1(randomTransfer, &aBank, &a, &b);
  thread t2(randomTransfer, &aBank, &b, &a);

  t1.join();
  t2.join();

  return 0;
}
