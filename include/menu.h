#ifndef MENU_H_
#define MENU_H_

#define MAX_FIELD 3

struct menu* new_menu(enum type_menu type, struct menu* sup_menu);

enum state menu_update(enum state state, int key, key_event_t key_event);
void menu_display(int mid_w, int mid_h, enum type_menu type);

void menu_free();

#endif /* MENU_H_ */
