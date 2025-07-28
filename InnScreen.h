//
// Created by jeremiah on 7/27/25.
//

#ifndef NARTHENIAN_INNSCREEN_H
#define NARTHENIAN_INNSCREEN_H


#include "Screen.h"
#include "MessageBox.h"
#include "Menu.h"
#include "Innkeeper.h"

class InnScreen : public Screen {
private:
    MessageBox message_box;
    MessageBox stats_box;
    Menu menu;
    Innkeeper *innkeeper = nullptr;
    enum InnScreenState { inn_none, inn_deciding, inn_sleeping1, inn_sleeping2, inn_nsf, inn_wait_ack, inn_finished };
    InnScreenState state = inn_none;
    int effect_frame;
    int wait_frames;
    void set_state(InnScreenState new_state);
    void sleep_effect(void);
    void do_menu(void);

public:
    InnScreen();
    void reset_inn(Innkeeper *new_innkeeper);
    void iterate(void) override;
    void draw(int frame) override;
};


#endif //NARTHENIAN_INNSCREEN_H
