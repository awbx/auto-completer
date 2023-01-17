#ifndef __COMPLETER_H__
#define __COMPLETER_H__

#include <readline/history.h>
#include <readline/readline.h>

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Node {
 public:
  char           chr;
  vector<Node *> children;
  Node(const char);
  ~Node();
};

class Completer {
 public:
  Node *root;
  Completer();
  ~Completer();

  void  init();
  void  insert_word(Node *, const char *, int);
  void  insert_word(const string &);
  void  display_paths(Node *, string, const string &);
  void  show_completions(string);
  Node *find(Node *, const char *);
};

// utils prototypes

void dump_dot(Node *);
void load_commands(Completer &);
void load_words(Completer &, const string &);

#endif