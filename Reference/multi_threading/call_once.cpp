#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>

using namespace std;

int winner;
once_flag winner_flag;
void set_winner(int id) { winner = id; }
void wait_1000ms(int id) {
  for (int i = 0; i < 1000; ++i) {
    this_thread::sleep_for(chrono::milliseconds(1));
  }
  call_once(winner_flag, set_winner, id);
}

int main() {
  thread threads[10];
  for (int i = 0; i < 10; ++i) {
    threads[i] = thread(wait_1000ms, i + 1);
  }
  cout << "waiting for the first among 10 threads to count 100ms...\n";
  for (auto& th : threads) th.join();
  cout << "winner thread: " << winner << "\n";
  return 0;
}
