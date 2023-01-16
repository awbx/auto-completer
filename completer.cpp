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
  cout << ((node->chr == '\0') ? '~' : node->chr);
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
    if (!root) root = new Node('@');
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

  void show_suggestions(Node *node, string word) {
    if (!node) return;
    if (node && node->chr == '\0') {
      cout << "- " << word << endl;
      return;
    }

    for (auto child : node->children) show_suggestions(child, word + string(1, node->chr));
  }

  Node *find(Node *node, const char *chr) {
    if (!chr || !*chr) return node;
    if (!node) return nullptr;
    auto it = find_if(node->children.begin(), node->children.end(), [chr](Node *x) { return x->chr == *chr; });

    if (it != node->children.end())
    {
        cout << node->chr << endl;
    return find(*it, chr + 1);
    }
    return nullptr;
  }
};

void load_command(Completer &suggestor) {
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
};

int main() {
  Completer suggestor;
  load_command(suggestor);

  while (true) {
    string word;
    cout << ">> ";
    cin >> word;
    if (!cin.good()) break;
    auto found = suggestor.find(suggestor.root, word.c_str());

    suggestor.show_suggestions(found, "");
  }
  // dump_dot(suggestor.root);
}