#include <dirent.h>

#include <fstream>

#include "completer.hpp"

static void label(Node *node, int &id) {
  if (!node) return;

  cout << "\tNode" << id << "[label=\"";
  cout << ((node->chr <= 0) ? '~' : node->chr);
  cout << "\"]\n";
}

static void edge(int from, int to) {
  cout << "\tNode" << from << " -> "
       << "Node" << to << "[wieght=9]\n";
}

static int dfs(Node *node, int &id) {
  int my_id = 0, n;

  my_id = id++;

  if (node) {
    label(node, my_id);

    for (auto child : node->children) {
      n = dfs(child, id);
      edge(my_id, n);
    }
  }

  return my_id;
}

void dump_dot(Node *Node) {
  if (!Node) return;
  cout << "digraph {\n";
  int id = 0;
  dfs(Node, id);
  cout << "}\n";
}

// this function will all command avialable on your os to completer
void load_commands(Completer &suggestor) {
  char *path = strdup(getenv("PATH"));

  char *token = strtok(path, ":");

  while (token) {
    DIR *dir = opendir(token);
    if (!dir) {
      token = strtok(NULL, ":");
      continue;
    }
    dirent *entry;

    while ((entry = readdir(dir))) {
      if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) continue;
      suggestor.insert_word(entry->d_name);
    }

    closedir(dir);
    token = strtok(NULL, ":");
  };
  free(path);
};

void load_words(Completer &completer, const string &filename) {
  fstream file(filename, ios::in);

  if (!file.good()) {
    perror(filename.c_str());
    exit(1);
  }
  string line;
  while (!getline(file, line).eof()) {
    completer.insert_word(line.c_str());
  }
}