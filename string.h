#ifndef STRING_H
#define STRING_H

#include "template.h"

//------------------------------------------------------------------------------
// Knuth-Morris-Pratt (prefix function, max length of prefix == suffix): O(N).
//------------------------------------------------------------------------------

template <class C, int N>
class KMP {
  const C* pattern;
  int n, prefixF[N+1];

public:
  void build(const C* s, int length) {
    pattern = s;
    n = length;
    prefixF[0] = prefixF[1] = 0;
    for (int i = 1; i < n; ++i) {
      int j = prefixF[i];
      while (j > 0 && pattern[i] != pattern[j]) j = prefixF[j];
      prefixF[i+1] = j + (pattern[i] == pattern[j] ? 1 : 0);
    }
  }

  void search(const C* text, int length) {
    for (int i = 0, j = 0; j < length; )
      if (i < n && pattern[i] == text[j]) {
        ++i;
        ++j;
        if (i == n) printf("occurrence at %d\n", j - n);
      } else {
        if (i > 0) i = prefixF[i];
        else ++j;
      }
  }
};

//------------------------------------------------------------------------------
// Z-function (max length of string prefix starting at position i): O(N).
//------------------------------------------------------------------------------

template <class C>
void zFunction(const C* s, int n, int* z) {
  z[0] = n;
  for (int i = 1, l = 0, r = 1; i < n; ++i) {
    z[i] = i < r ? min(r-i, z[i-l]) : 0;
    while (i+z[i] < n && s[z[i]] == s[i+z[i]]) ++z[i];
    if (i+z[i] > r) l = i, r = i+z[i];
  }
}

//------------------------------------------------------------------------------
// Aho-Corasick (multiple patterns matching): O(N A), M(N A).
//------------------------------------------------------------------------------

template <int N, char Aa = 'a', char Az = 'z'>
class AhoCorasick {
  static const int A = Az - Aa + 1;
  int trie[N][A], size;
  int end[N], link[N], endLink[N];

  int addNode() {
    fill_n(trie[size], A, 0);
    end[size] = 0;
    return size++;
  }

public:
  AhoCorasick() { init(); }

  void init() {
    size = 0;
    addNode();
  }

  void addString(const char* s, int n) {
    int v = 0;
    for (int i = 0; i < n; ++i) {
      int c = s[i] - Aa;
      if (trie[v][c] == 0) trie[v][c] = addNode();
      v = trie[v][c];
    }
    end[v] = n;
  }

  void build(bool addEdges = false) {
    int queue[N];
    queue[0] = 0;
    link[0] = 0;
    for (int *v = queue, *tail = v + 1; v != tail; ++v) {
      for (int c = 0; c < A; ++c)
        if (trie[*v][c] != 0) {
          int u = link[*v];
          while (u != 0 && trie[u][c] == 0) u = link[u];
          link[trie[*v][c]] = *v == 0 ? 0 : trie[u][c];
          *tail++ = trie[*v][c];
        } else {
          if (addEdges) trie[*v][c] = trie[link[*v]][c];
        }
      //if (end[*v] == 0) end[*v] = end[link[*v]];
      endLink[*v] = end[link[*v]] ? link[*v] : endLink[link[*v]];
    }
  }

  void search(const char* text, int m) {
    for (int v = 0, i = 0; i < m; ++i) {
      char c = text[i] - Aa;
      while (v != 0 && trie[v][c] == 0) v = link[v];
      v = trie[v][c];
      for (int u = v; u != 0; u = endLink[u]) if (end[u]) {
        printf("occurrence at [%d,%d): ", i+1 - end[u], i+1);
        print("%c", text + i+1-end[u], text + i+1, "");
      }
    }
  }
};

//------------------------------------------------------------------------------
// Suffix array: O(N log^2 N).
// Kasai algorithm (longest common prefixes): O(N).
// !Don't forget to append '\0' character at the end and pass length+1 to get
//  suffixes sorted instead of cyclic shifts!
//------------------------------------------------------------------------------

template <int N>
class SuffixArray {
  int rank[N];
  bool diff[N];
  inline int trim(int i, int n) { return i < n ? i : i - n; }

public:
  int array[N];  //, lcp[N];
  SegmentTree<Tmin<int>> lcp;

  template <class C>
  void build(const C* s, int n) {
    for (int i = 0; i < n; ++i) array[i] = i;
    sort(array, array + n, [s](int x, int y) { return s[x] < s[y]; });
    for (int i = 0; i+1 < n; ++i) diff[i] = s[array[i]] != s[array[i+1]];
    diff[n-1] = true;
    for (int length = 1; length < n; length *= 2) {
      for (int i = 0, group = 0; i < n; group += diff[i], ++i)
        rank[trim(array[i] - length + n, n)] = group;
      for (int i = 0, j = 1; j <= n; ++j) if (diff[j-1]) {
        sort(array + i, array + j,
             [this](int x, int y) { return rank[x] < rank[y]; });
        i = j;
      }
      for (int i = 0; i+1 < n; ++i)
        diff[i] |= rank[array[i]] != rank[array[i+1]];
    }
    for (int i = 0, j = 1; j <= n; ++j) if (diff[j-1])
      sort(array + i, array + j), i = j;
  }

  template <class C>
  void computeLcp(const C* s, int n) {
    build(s, n);
    lcp.init(n-1);
    for (int i = 0; i < n; ++i) rank[array[i]] = i;
    int length = lcp[n-1] = 0;
    for (int i = 0; i < n; ++i) if (rank[i] > 0) {
      int u = trim(i + length, n);
      int v = trim(array[rank[i] - 1] + length, n);
      for (; length < n && s[u] == s[v]; ++length) {
        u = trim(u + 1, n);
        v = trim(v + 1, n);
      }
      lcp[rank[i] - 1] = length;
      if (length > 0) --length;
    }
    lcp.build();
  }

  int queryLcp(int i, int j) {
    i = rank[i];
    j = rank[j];
    if (i > j) swap(i, j);
    return lcp.query(i, j);
  }
};

//------------------------------------------------------------------------------
// Generalized suffix automaton: O(N), M(N A).
// Transitions using map: O(N log A), M(N).
// Maximum 2*N-1 nodes, 3*N-4 edges.
//------------------------------------------------------------------------------

template <int N, char Aa = 'a', char Az = 'z'>
class SuffixAutomaton {
  static const int A = Az - Aa + 1;

  struct State {
    int length;
    State* next[A];
    void clearNext() { fill_n(next, A, nullptr); }
    bool hasNext(char c) { return next[c] != nullptr; }
    //map<char, State*> next;
    //void clearNext() { next.clear(); }
    //bool hasNext(char c) { return next.count(c) > 0; }
    State* link;
  } state[2*N-1];
  State* last, *free;

  State* computeLink(State* p, char c) {
    State* q = p->next[c];
    if (p->length + 1 == q->length) return q;
    State* clone = free++;
    clone->length = p->length + 1;
    copy_n(q->next, A, clone->next);
    //clone->next = q->next;
    clone->link = q->link;
    for (; p && p->next[c] == q; p = p->link) p->next[c] = clone;
    return q->link = clone;
  }

public:
  //long long differentSubstrings = 0;

  void init() {
    last = state;
    free = last + 1;
    last->clearNext();
    last->length = 0;
    last->link = nullptr;
  }

  void addLetter(char c, bool newString = false) {
    if (newString) last = state;
    c -= Aa;
    State* p = last;
    if (p->hasNext(c)) return void(last = computeLink(p, c));
    last = free++;
    last->clearNext();
    last->length = p->length + 1;
    for (; p && !p->hasNext(c); p = p->link) p->next[c] = last;
    last->link = p ? computeLink(p, c) : state;
    //differentSubstrings += last->length - last->link->length;
  }

  int size() const { return static_cast<int>(free - state); }

  void build(const char* s) {
    for (init(); *s; ++s) addLetter(*s);
  }
};

//------------------------------------------------------------------------------
// Ukkonen algorithm (suffix tree) by adamant: O(N log A), M(N).
//------------------------------------------------------------------------------

template <int N>
class SuffixTree {
  int n, pos;
  int size, node;
  char s[N+1];

  struct Node {
    int start, length, link;
    map<char, int> next;
  } tree[2*N-1];

  int addNode(int pos, int length = numeric_limits<int>::max()) {
    tree[size].start = pos;
    tree[size].length = length;
    return size++;
  }

public:
  void init() {
    n = size = 0;
    addNode(0);
  }

  void addLetter(char c = '\0') {
    assert(size > 0);
    if (c) s[n] = c;
    else c = s[n];
    ++n;
    ++pos;
    int last = 0;
    while (pos > 0) {
      while (pos > tree[tree[node].next[s[n-pos]]].length) {
        node = tree[node].next[s[n-pos]];
        pos -= tree[node].length;
      }
      int& v = tree[node].next[s[n-pos]];
      char t = s[tree[v].start + pos - 1];
      if (v == 0) {
        v = addNode(n - pos);
        tree[last].link = node;
        last = 0;
      } else if (t == c) {
        tree[last].link = node;
        break;
      } else {
        int u = addNode(tree[v].start, pos - 1);
        tree[u].next[c] = addNode(n - 1);
        tree[u].next[t] = v;
        tree[v].start += pos - 1;
        tree[v].length -= pos - 1;
        v = u;
        tree[last].link = u;
        last = u;
      }
      if (node == 0) --pos;
      else node = tree[node].link;
    }
  }

  char* data() { return s + n; }

  int differentSubstrings() {
    int res = 0;
    for (int i = 1; i < size; ++i)
      res += min(n - tree[i].start, tree[i].length);
    return res;
  }
};

//------------------------------------------------------------------------------
// Duval algorithm and derived: O(N).
// Builds Lindon decomposition of S: S1 S2 S3 .. Sm, where Si - simple string
// (there is no itself shift, that is lexicographically smaller
// than the string itself) and S1 >= S2 >= ... >= Sm.
//------------------------------------------------------------------------------

template <class C>
vector<int> duval(const C* s, int n) {
  vector<int> res;
  for (int i = 0, j, k; i < n; ) {
    for (k = i, j = i + 1; j < n && s[k] <= s[j]; ++j)
      s[k] < s[j] ? k = i : ++k;
    for (; i <= k; i += j-k) res.push_back(i);
  }
  return res;
}

template <class C>
int minCyclicShift(const C* s, int n) {
  for (int i = 0, j, k, r = 0; ; ) {
    for (k = i, j = i + 1; s[k<n ? k : k-n] <= s[j<n ? j : j-n]; ++j)
      s[k<n ? k : k-n] < s[j<n ? j : j-n] ? k = i : ++k;
    for (r = i; i <= k; i += j-k);
    if (i >= n) return r;
  }
}

template <class C>
int minSuffix(const C* s, int n) {
  for (int i = 0, j, k, r = 0; ; ) {
    for (k = i, j = i + 1; s[k] <= s[j]; ++j)
      s[k] < s[j] ? k = i : ++k;
    for (; i <= k; i += j - k) r = i;
    if (i >= n) return r;
  }
}

#endif  // STRING_H