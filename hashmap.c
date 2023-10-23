#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define STARTING_BUCKETS 8
#define MAX_KEY_SIZE 10 // TODO remove this constraint


struct Node
{
  char *key;
  void *value;
  struct Node *next;
};

typedef struct Node *node;

node createNode()
{
  node temp;
  temp = (node)malloc(sizeof(struct Node));
  temp->next = NULL;
  return temp;
}

node addNode(node head, char *key, void **value)
{
  node temp, p;
  temp = createNode();
  temp->key = key;
  temp->value = value;
  if (head == NULL)
  {
    head = temp;
  }
  else
  {
    p = head;
    while (p->next != NULL)
    {
      p = p->next;
    }
    p->next = temp;
  }
  return head;
}

typedef struct Hashmap
{
  node **items;
  int capacity;
  int count;
} Hashmap;

unsigned long hashCode(unsigned char *s, Hashmap *h)
{
  unsigned long a = 5381;
  int c;
  while (c = *s++)
  {
    a = ((a << 5) + a) + c;
  }
  return a % h->capacity;
}

Hashmap *Hashmap_new(void)
{
  Hashmap *h = malloc(sizeof(Hashmap));
  h->capacity = STARTING_BUCKETS;
  h->items = malloc(sizeof(node *) * STARTING_BUCKETS);
  h->count = 0;
  return h;
}

void *Hashmap_get(Hashmap *h, char *key)
{
  int index = hashCode(key, h);
  node p = h->items[index];
  {
    while (p->next != NULL)
    {
      p = p->next;
    }
  }
  return p->value;
}

void *Hashmap_set(Hashmap *h, char *key, void *value)
{
  if (h->capacity == h->count)
  {
    h->capacity = h->capacity * 2;
    h->items = realloc(h->items, h->capacity * sizeof(node *));
  }

  int index = hashCode(key, h);
  node p = h->items[index];
  if (p == NULL)
  {
    h->items[index] = addNode(p, key, value);
  }
  else
  {
    while (p->next != NULL)
    {
      p = p->next;
    }
    h->items[index] = addNode(p, key, value);
  }
  h->count++;
}

Hashmap *Hashmap_free()
{
  return NULL;
}

void *Hashmap_delete(Hashmap *h, char *key)
{
  int index = hashCode(key, h);
  node p = h->items[index];
  {
    while (p->next != NULL)
    {
      p->key = NULL;
      p->value = NULL;
      p = p->next;
    }
    p->key = NULL;
    p->value = NULL;
  }
}

int main()
{
  Hashmap *h = Hashmap_new();

  // basic get/set functionality
  int a = 5;
  float b = 7.2;
  Hashmap_set(h, "item a", &a);
  Hashmap_set(h, "item b", &b);
  assert(Hashmap_get(h, "item b") == &b);
  assert(Hashmap_get(h, "item a") == &a);

  // using the same key should override the previous value
  int c = 20;
  Hashmap_set(h, "item a", &c);
  assert(Hashmap_get(h, "item a") == &c);
  // basic delete functionality
  Hashmap_delete(h, "item a");
  assert(Hashmap_get(h, "item a") == NULL);
  

  // handle collisions correctly
  // note: this doesn't necessarily test expansion
  int i, n = STARTING_BUCKETS * 10, ns[n];
  char key[MAX_KEY_SIZE];
  for (i = 0; i < n; i++)
  {
    ns[i] = i;
    sprintf(key, "item %d", i);
    Hashmap_set(h, key, &ns[i]);
  }
  for (i = 0; i < n; i++)
  {
    sprintf(key, "item %d", i);
    assert(Hashmap_get(h, key) == &ns[i]);
  }
/*
      Hashmap_free(h);
      /*
         stretch goals:
         - expand the underlying array if we start to get a lot of collisions
         - support non-string keys
         - try different hash functions
         - switch from chaining to open addressing
         - use a sophisticated rehashing scheme to avoid clustered collisions
         - implement some features from Python dicts, such as reducing space use,
         maintaing key ordering etc. see https://www.youtube.com/watch?v=npw4s1QTmPg
         for ideas
         */
  printf("ok\n");
}
