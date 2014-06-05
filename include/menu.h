#ifndef MENU_H_
#define MENU_H_

// Create a new menu
void new_menu(enum type_menu type);

// Update a menu
enum state menu_update(enum state state, int key, key_event_t key_event);

// Display a menu
void menu_display(int mid_w, int mid_h);

// Free a menu
void menu_free();

#endif /* MENU_H_ */
