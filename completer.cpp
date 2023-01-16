#include <dirent.h>

#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Node {
 public:
  char           chr;
  vector<Node *> children;
  Node(const char _chr) : chr(_chr) {}
};

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

class Completer {
 public:
  Node *root;
  Completer() : root(nullptr) { this->init(); }

  void init() {
    if (!root) root = new Node(-1);
  }
  void insert_word(Node *node, char *word, int len) {
    if (!node || len < 0) return;

    auto it = find_if(node->children.begin(), node->children.end(), [word](Node *x) { return x->chr == *word; });

    if (it == node->children.end()) {
      Node *new_node = new Node(*word);
      node->children.push_back(new_node);
      insert_word(new_node, word + 1, len - 1);
      return;
    }
    insert_word(*it, word + 1, len - 1);
  }

  void insert_word(char *word) { insert_word(root, word, strlen(word)); }

  void show_completions(Node *node, string word, string prefix) {
    if (!node) return;
    if (node && node->chr == '\0') {
      cout << "- " << prefix.substr(0, prefix.size() - 1) << word << endl;
      return;
    }

    for (auto child : node->children) show_completions(child, word + string(1, node->chr), prefix);
  }

  Node *find(Node *node, const char *chr) {
    if (!chr || !*chr) return node;
    if (!node) return nullptr;
    auto it = find_if(node->children.begin(), node->children.end(), [chr](Node *x) { return x->chr == *chr; });

    if (it != node->children.end()) {
      return find(*it, chr + 1);
    }
    return nullptr;
  }
};

void load_commands(Completer &suggestor) {
  char *path = strdup("/bin");

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
};

int main() {
  Completer completer;
  load_commands(completer);

  dump_dot(completer.root);
  // while (true) {
  //   string prefix;

  //   cout << ">> ";
  //   cin >> prefix;
  //   if (!cin.good()) break;
  //   auto found = completer.find(completer.root, prefix.c_str());
  //   completer.show_completions(found, "", prefix);
  // }
}