#ifndef MENU_H_
#define MENU_H_

#define MAX_FIELD 3

struct menu* new_menu(enum type type);

enum state menu_update(struct menu* menu, int key);
void menu_display(struct menu* menu);

void menu_free(struct menu* menu);

#endif /* MENU_H_ */
