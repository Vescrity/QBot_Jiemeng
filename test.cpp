#include <iostream>
using namespace std;
int main()
{
  bool a = 0;
  bool b = 1;
  bool result = !(a ^ b);
  cout << result << endl;
}
