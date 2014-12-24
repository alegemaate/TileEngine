#ifndef MENU_H
#define MENU_H

#include "GameState.h"

#include <allegro.h>
#include <alpng.h>
#include <string>
#include <vector>

#include "globals.h"
#include "tools.h"

class Menu : public GameState
{
  private:
    // Menu/GUI
    BITMAP *buffer, *levelSelectLeft, *levelSelectRight, *levelSelectNumber, *cursor[2], *menuselect, *menu, *menu_edit, *help;

    SAMPLE* click;

    // Menu
    int selectorY, selectorX, newSelectorY, selected_object;
    int cursor_x, cursor_y;
    int menu_view_x, menu_view_y;
    bool menuOpen, editMode;
  protected:
  public:
    //Main loop functions
    Menu();
    void update();
    void draw();
    ~Menu();
};

#endif // MENU_H
