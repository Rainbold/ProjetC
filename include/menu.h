#ifndef MENU_H_
#define MENU_H_

#define MAX_FIELD 3

void new_menu(enum type_menu type);

enum state menu_update(enum state state, int key, key_event_t key_event);
void menu_display(int mid_w, int mid_h);

void menu_free();

#endif /* MENU_H_ */
