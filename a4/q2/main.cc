#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "Expression.h"
using namespace std;

// You will need to make changes to this file

// Read and construct expression object.  Returns a pointer to a heap-allocated
// object (caller must delete it).

vector <Expression *> stk;

istream &operator>>(istream &in, Expression *&e) {
  string s;
  int stktop = -1;

  while (cin >> s) {
    istringstream ss(s);
    int n;
    if (ss >> n) {
      Loneint *integer = new Loneint{n};

      stktop++;
      stk.emplace_back(integer);
    }
    else if (s == "NEG" || s == "ABS") {
      Unary *unary;
      if (stktop >= 0) {
        unary = new Unary{s, stk[stktop--]};
        stk.erase(stk.end() - 1);
      }
      else {
        cerr << "Invalid input" << endl;
        return in;
      }

      stktop++;
      stk.emplace_back(unary);
    }
    else if (s == "+" || s == "-" || s == "*" || s == "/") {
      Expression *r, *l;
      if (stktop >= 0) {
        r = stk[stktop--];
        stk.erase(stk.end() - 1);
      }
      else {
        cerr << "Invalid input" << endl;
        return in;
      }
      if (stktop >= 0) {
        l = stk[stktop--];
        stk.erase(stk.end() - 1);
      }
      else {
        cerr << "Invalid input" << endl;
        return in;
      }

      Binary *binary = new Binary{s, l, r};
      stktop++;
      stk.emplace_back(binary);
    }
    else if (s == "done") {
      break;
    }
    else {
      Variable *variable = new Variable{s};

      stktop++;
      stk.emplace_back(variable);
    }
  }

  e = stk[stktop];
  return in;
}

int main () {
  string s;

  Expression *e;
  cin >> e;
  cout << e->prettyprint() << endl;

  // Command interpreter
  while (cin >> s) {
    if (s == "eval") {
      try {
        cout << e->evaluate() << endl;
      } catch (exception e) {}
    }
    else if (s == "set") {
      string name;
      int value;
      cin >> name >> value;
      e->set(name, value);
    }
    else if (s == "unset") {
      string name;
      cin >> name;
      e->unset(name);
    }
    else if (s == "print") {
      try {
        cout << e->prettyprint() << endl;
      } catch (exception e) {}
    }
    else if (s == "copy") {
      Expression *theCopy = e->clone();
      cout << theCopy->prettyprint() << endl;
      theCopy->set("x", 1);
      cout << theCopy->prettyprint() << endl;
      try {
        cout << theCopy->evaluate() << endl;
      }
      catch (exception e) {
        /* Do the appropriate thing*/
      }
      delete theCopy;
    }
  }

  delete e;
}
