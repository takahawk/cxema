#include "radix_tree.h"

#include <string.h>

#include "util.h"

static _RadixTreeNode* _form_node(char *prefix, void *data, size_t data_size)
{
  _RadixTreeNode* node = malloc(sizeof(*node));

  node->prefix   = cpystr(prefix);
  node->data     = cpymem(data, data_size);
  node->next     = NULL;
  node->children = NULL;

  return node;
}

static void _add(_RadixTreeNode *node, char *key, void *data, size_t data_size)
{
  if (NULL == node)
    return;
  char *fullkey = key;
  char *prefix = node->prefix;

  while (*prefix && *prefix == *key)
    prefix++, key++;

  if (!prefix) {
    if (!key) {
      free(node->data);
      node->data = cpymem(data, data_size);
      return;
    } else {
      if (node->children) {
        _add(node->children, key, data, data_size);
      } else {
        node->children = _form_node(key, data, data_size);
        node->children->parent = node;
      }
      return;
    }
  } else {
    if (!key) {
      _form_node(node,)
    }

    if (*prefix < *key) {
      _RadixTreeNode *next = node->next;
      newNode
    }
  }
}

void add(RadixTree *self, char *key, void *data, size_t data_size)
{
  if (NULL == self->head) {
    self->head = _form_node(NULL, key, data, data_size);
    return;
  }

}

static void* get(RadixTree *self, char *key)
{
  // TODO: impl
}

static void remove(RadixTree *self, char *key)
{
  // TODO: impl
}

static void release(RadixTree **pself)
{
  // TODO: impl
}

static RadixTree* form(void)
{
  // TODO: impl
}
