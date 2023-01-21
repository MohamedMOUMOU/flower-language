#ifndef TYPES_H
#define TYPES_H

#include <vector>

#include <string>

using namespace std;

struct node {
  vector < string > value;
  vector < node * > children;
};

#endif