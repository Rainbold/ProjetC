#ifndef WIIMOTE_H_
#define WIIMOTE_H_

void wiimote_init();
void wiimote_link(int time); // time in s to find wiimotes
enum state input_wiimote(void* thing, enum state state);

void wiimote_set_rumble(int id, int state);

#endif /* WIIMOTE_H_ */
