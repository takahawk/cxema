#ifndef CXEMA_RADIX_TREE_H_
#define CXEMA_RADIX_TREE_H_
#include <stddef.h>
typedef struct _RadixTreeNode _RadixTreeNode;
typedef struct RadixTree RadixTree;

struct _RadixTreeNode {
  char      *prefix;
  void      *data;
  RadixTree *next;
  RadixTree *children;
  RadixTree *parent;
};

struct RadixTree {
  _RadixTreeNode *head;

  void  (*add)    (RadixTree *self, char *key, void *data, size_t size);
  void* (*get)    (RadixTree *self, char *key);
  void  (*remove) (RadixTree *self, char *key);

  void  (*release) (RadixTree **pself);
};

struct _RadixTreeStatic {
  RadixTree* (*form) (void);
};

extern const RadixTree RADIX_TREE;

#endif
