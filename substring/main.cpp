#include <iostream>
#include "substring.h"

using namespace std;

int main() {
  cout << str_len("this") << endl;
  cout << is_prefix("indent", "indentation") << endl;
  cout << "================" << endl;

  cout << substring_position("this", "this is a simple exercise") << endl;
  cout << substring_position("is", "this is a simple exercise") << endl;
  cout << substring_position("is a", "this is a simple exercise") << endl;
  cout << substring_position("is an", "this is a simple exercise") << endl;
  cout << substring_position("exercise", "this is a simple exercise") << endl;
  cout << substring_position("simple exercise", "this is a simple") << endl;
  cout << substring_position("", "this is a simple exercise") << endl;
  cout << substring_position("", "") << endl;

  cout << "================" << endl;

  cout << substring_position_recursion("this", "this is a simple exercise") << endl;
  cout << substring_position_recursion("is", "this is a simple exercise") << endl;
  cout << substring_position_recursion("is a", "this is a simple exercise") << endl;
  cout << substring_position_recursion("is an", "this is a simple exercise") << endl;
  cout << substring_position_recursion("exercise", "this is a simple exercise") << endl;
  cout << substring_position_recursion("simple exercise", "this is a simple") << endl;
  cout << substring_position_recursion("", "this is a simple exercise") << endl;
  cout << substring_position_recursion("", "") << endl;
  return 0;
}
