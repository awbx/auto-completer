#include "completer.hpp"

Node::Node(const char _chr) : chr(_chr) {}

Node::~Node() {
  for (auto child : children) delete child;
}

Completer::Completer() : root(nullptr) { this->init(); }

Completer::~Completer() { delete root; }

void Completer::init() {
  if (!root) root = new Node(-1);
}
void Completer::insert_word(Node *node, const char *word, int len) {
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

void Completer::insert_word(const string &word) { insert_word(root, word.c_str(), word.size()); }

void Completer::display_paths(Node *node, string word, const string &prefix) {
  if (!node) return;
  if (node && node->chr == '\0') {
    cout << "- " << prefix.substr(0, prefix.size() - 1 < 0 ? 0 : prefix.size() - 1) << word << endl;
    return;
  }

  for (auto child : node->children) display_paths(child, (word[0] == -1 ? "" : word) + string(1, node->chr), prefix);
}

void Completer::get_completions(Node *node, string word, vector<string> &words, const string &prefix) {
  if (!node) return;
  if (node && node->chr == '\0') {
    words.push_back(prefix.substr(0, prefix.size() - 1) + word);
    return;
  }

  for (auto child : node->children) get_completions(child, (word[0] == -1 ? "" : word) + string(1, node->chr), words, prefix);
}

char **Completer::get_completions(string word) {
  vector<string> words;

  get_completions(find(root, word.c_str()), "", words, word);

  char **completion_words = new char *[words.size() + 1];
  int    idx = 0;
  for (auto w : words) completion_words[idx++] = strdup(w.c_str());
  completion_words[idx] = NULL;
  return completion_words;
}

Node *Completer::find(Node *node, const char *chr) {
  if (!chr || !*chr) return node;
  if (!node) return nullptr;
  auto it = find_if(node->children.begin(), node->children.end(), [chr](Node *x) { return x->chr == *chr; });

  if (it != node->children.end()) {
    return find(*it, chr + 1);
  }
  return nullptr;
}
void Completer::show_completions(string prefix) {
  Node *found = find(root, prefix.c_str());
  display_paths(found, "", prefix);
}

Completer completer;

char *command_generator(const char *str, int state) {
  static char **words = completer.get_completions(string(str));

  if (!*words && !state) words = completer.get_completions(string(str));
  if (!*words && state) return NULL;

  return *(words++);
}

char **command_completion(const char *text, int start, int end) {
  rl_attempted_completion_over = 1;

  return rl_completion_matches(text, command_generator);
}

int main(int ac, char **av) {
  if (ac > 1)
    for (size_t idx = 1; idx < ac; idx++) load_words(completer, string(av[idx]));
  else
    load_commands(completer);

  rl_attempted_completion_function = command_completion;
  while (true) {
    char *line = readline("complete : ");
    if (!line) break;

    string prefix(line);
    if (prefix == "tree")
      dump_dot(completer.root);
    else
      cout << "prefix -> " << prefix << endl;
    free(line);
  }
}