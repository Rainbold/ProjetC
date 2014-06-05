#ifndef LIST_H_
#define LIST_H_

#include <stdlib.h>
#include <stdio.h>
#include <map.h>
#include <sprite.h>
#include <window.h>

struct list;

// Create a new list
struct list* list_new();


int list_get_x(struct list* l);
int list_get_y(struct list* l);
void list_inc_x(struct list* l);
void list_dec_x(struct list* l);
void list_inc_y(struct list* l);
void list_dec_y(struct list* l);

// Return the data
void* list_get_data(struct list* l);

struct list* list_get_next(struct list* l);
struct list* list_get_tail(struct list* l);
struct list* list_insert_back(struct list* l, int x, int y, s_type type, void* data);
struct list* list_cut_head(struct list* l);
struct list* list_find(struct list* l, int x, int y);

// Remove an elevement of the list
struct list* list_remove(struct list* l, int x, int y);
struct list* list_remove2(struct list* l_head, struct list* l);
struct list* list_delete(struct list* l);

#endif
