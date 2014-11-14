#include <iostream>
#include <thread>

using namespace std; 

void thread_task() {
  cout << "hello thread" << std::endl;
}

int main(int argc, const char *argv[]) {
  thread t(thread_task);
  t.join();
  return EXIT_SUCCESS;
}
