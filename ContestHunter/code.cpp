#include <cmath>
#include <cstdio>

#define MAX 30005
#define MOD 998244353
#define INF 0x3f3f3f3f
#define EPS 1e-6
#define pb push_back
#define fi first
#define se second
#define mapa make_pair

using L = long;
using LL = long long;  // c++11
using ULL = unsigned long long;

using namespace std;

int t;
int cur_count[MAX], parent[MAX], p_count[MAX];

int find(int x) {
  if (x == parent[x]) return x;
  int p_x = parent[x];
  int root = find(parent[x]);
  p_count[x] += p_count[p_x];
  parent[x] = root;
  return root;
}

int main() {
  int num;
  int cows[5 + 1];
  while (scanf("%d\n", &num) && num != 0) {
    for (int i = 0; i < num; ++i) {
      scanf("%d ", &num);
    }
  }
  return 0;
}
