#ifndef LIST_H_
#define LIST_H_

#include <stdlib.h>
#include <stdio.h>
#include <map.h>
#include <sprite.h>
#include <window.h>

struct list {
	void* data;
	int x, y;
	s_type type;
	struct list* next;
};

struct list* list_new();
struct list* list_get_tail(struct list* l);
struct list* list_insert_back(struct list* l, int x, int y, s_type type, void* data);
struct list* list_cut_head(struct list* l);
struct list* list_find(struct list* l, int x, int y);
struct list* list_remove(struct list* l, int x, int y);
struct list* list_delete(struct list* l);

#endif