#ifndef WIIMOTE_H_
#define WIIMOTE_H_


// Initiate the wiimote module
void wiimote_init();

// Will be looking for wiimote during time seconds
void wiimote_link(int time);

// Disconnect all wiimote
void wiimote_disconnect();

// Manage the input of wiimote
enum state input_wiimote(struct game* game, enum state state);

// Rumble a wiimote
void wiimote_set_rumble(int id, int state);

#endif /* WIIMOTE_H_ */
