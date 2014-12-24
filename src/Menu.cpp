#include "Menu.h"

Menu::Menu()
{
  buffer = create_bitmap( 1280, 960);

  // Load images
  if(!(menu = load_bitmap( ("images/gui/menu.png"), NULL))){
    abort_on_error( "Cannot find image images/gui/menu.png \n Please check your files and try again");
  }
  if(!(menu_edit = load_bitmap( ("images/gui/menu_edit.png"), NULL))){
    abort_on_error( "Cannot find image images/gui/menu_edit.png \n Please check your files and try again");
  }
  if(!(menuselect = load_bitmap( ("images/gui/menuSelector.png"), NULL))){
    abort_on_error( "Cannot find image images/gui/menuSelector.png \n Please check your files and try again");
  }
  if(!(help = load_bitmap( ("images/gui/help.png"), NULL))){
    abort_on_error( "Cannot find image images/gui/help.png \n Please check your files and try again");
  }
  if(!(cursor[0] = load_bitmap( ("images/gui/cursor1.png"), NULL))){
    abort_on_error( "Cannot find image images/gui/cursor1.png \n Please check your files and try again");
  }
  if(!(cursor[1] = load_bitmap( ("images/gui/cursor2.png"), NULL))){
    abort_on_error( "Cannot find image images/gui/cursor2.png \n Please check your files and try again");
  }
  if(!(levelSelectLeft = load_bitmap( ("images/gui/levelSelectLeft.png"), NULL))){
    abort_on_error( "Cannot find image images/gui/levelSelectLeft.png \n Please check your files and try again");
  }
  if(!(levelSelectRight = load_bitmap( ("images/gui/levelSelectRight.png"), NULL))){
    abort_on_error( "Cannot find image images/gui/levelSelectRight.png \n Please check your files and try again");
  }
  if(!(levelSelectNumber = load_bitmap( ("images/gui/levelSelectNumber.png"), NULL))){
    abort_on_error( "Cannot find image images/gui/levelSelectNumber.png \n Please check your files and try again");
  }

  //Load sound effects
  if(!(click = load_sample(("sounds/click.wav")))){
    abort_on_error( "Cannot find sound sounds/click.png \n Please check your files and try again");
  }

  // Temporary fonts
  FONT *f1, *f2, *f3, *f4, *f5;

  //Sets Font
  if(!(f1 = load_font(("fonts/arial_black.pcx"), NULL, NULL))){
    abort_on_error( "Cannot find font fonts/arial_black.png \n Please check your files and try again");
  }

  f2 = extract_font_range(f1, ' ', 'A'-1);
  f3 = extract_font_range(f1, 'A', 'Z');
  f4 = extract_font_range(f1, 'Z'+1, 'z');

  //Merge fonts
  font = merge_fonts(f4, f5 = merge_fonts(f2, f3));

  //Destroy temporary fonts
  destroy_font(f1);
  destroy_font(f2);
  destroy_font(f3);
  destroy_font(f4);
  destroy_font(f5);

  // Allow transparency
  set_alpha_blender();

  //Variables
  newSelectorY = 637;
  selectorY = 637;
  selectorX = 60;

}

void Menu::update()
{
  menuOpen = false;

  // Move selector
  if( selectorY != newSelectorY){
    int selectorVelocity;
    if( newSelectorY != selectorY){
      selectorVelocity = (newSelectorY - selectorY )/ 6;
      if( newSelectorY - selectorY < 0){
        selectorVelocity -= 1;
      }
      else if( newSelectorY - selectorY > 0){
        selectorVelocity += 1;
      }
    }
    selectorY += selectorVelocity;
  }

  if( !editMode){
    //Hover play
    if(collisionAny(mouseX(),mouseX(),60,270,mouseY(),mouseY(), 637, 637 + 45)){
      if(newSelectorY != 637){
        newSelectorY = 637;
        selectorX = 60;
        play_sample(click,255,125,1000,0);
      }
    }
    //Hover edit
    else if(collisionAny(mouseX(),mouseX(),60,270,mouseY(),mouseY(), 700, 700 + 45)){
      if(newSelectorY != 700){
        newSelectorY = 700;
        selectorX = 60;
        play_sample(click,255,125,1000,0);
      }
    }
    //Hover help
    else if(collisionAny(mouseX(),mouseX(),60,270,mouseY(),mouseY(), 763, 763 + 45)){
      if(newSelectorY != 763){
        newSelectorY = 763;
        selectorX = 60;
        play_sample(click,255,125,1000,0);
      }
      menuOpen = true;
    }
    //Hover exit
    else if(collisionAny(mouseX(),mouseX(),60,270,mouseY(),mouseY(), 828, 828 + 45)){
      if(newSelectorY != 828){
        newSelectorY = 828;
        selectorX = 60;
        play_sample(click,255,125,1000,0);
      }
    }

    //Select button
    if(mouse_b & 1 || key[KEY_ENTER] || joy[0].button[0].b){
      // level select left
      if(collisionAny(mouseX(),mouseX(),1100,1140,mouseY(),mouseY(), 80, 120)){
        play_sample(click,255,125,1000,0);
        while( mouse_b & 1){}
      }
      // level select right
      else if(collisionAny(mouseX(),mouseX(),1200,1240,mouseY(),mouseY(), 80, 120)){
        play_sample(click,255,125,1000,0);
        while( mouse_b & 1){}
      }
      // Start
      else if(collisionAny(mouseX(),mouseX(),60,270,mouseY(),mouseY(), 637, 637 + 45)){
        set_next_state( STATE_GAME);
      }
      // Edit
      else if(collisionAny(mouseX(),mouseX(),60,270,mouseY(),mouseY(), 700, 700 + 45)){
        set_next_state( STATE_EDIT);
      }
      // Help
      else if(collisionAny(mouseX(),mouseX(),60,270,mouseY(),mouseY(), 763, 763 + 45)){

      }
      // Quit
      else if(collisionAny(mouseX(),mouseX(),60,270,mouseY(),mouseY(), 828, 828 + 45)){
        set_next_state( STATE_EXIT);
      }
    }
  }
}

void Menu::draw()
{
  // Draw background to screen
  rectfill( buffer, 0, 0, 1280, 960, makecol( 255,255,255));

  /* PLACE MENU BACKDROP HERE */

  // Overlay
  draw_trans_sprite(buffer, menu, 0, 0);
  draw_trans_sprite(buffer, menuselect, selectorX, selectorY);

  // Level selection
  draw_trans_sprite(buffer, levelSelectLeft, 1100, 80);
  draw_trans_sprite(buffer, levelSelectNumber, 1145, 80);
  //textprintf_centre_ex(buffer,font,1168,73,makecol(0,0,0),-1,"%i",levelOn);
  draw_trans_sprite(buffer, levelSelectRight, 1200, 80);

  // Hover select left
  if(collisionAny(mouseX(),mouseX(),1100,1140,mouseY(),mouseY(), 80, 120)){
    draw_trans_sprite(buffer, levelSelectLeft, 1100, 80);
  }
  // Hover select right
  if(collisionAny(mouseX(),mouseX(),1200,1240,mouseY(),mouseY(), 80, 120)){
    draw_trans_sprite(buffer, levelSelectRight, 1200, 80);
  }

  // Cursor
  stretch_sprite(buffer, cursor[0], mouseX(), mouseY(), 21 * resDiv, 26 * resDiv);

  // Select button
  if (mouse_b & 1 || key[KEY_ENTER] || joy[0].button[0].b){
    if (selectorY==610){
      do{
        draw_sprite(buffer, menu, 0, 0);
        draw_sprite(buffer,help,0,0);
        stretch_sprite(buffer, cursor[0], mouseX(), mouseY(), 21 * resDiv, 26 * resDiv);
        draw_sprite(screen,buffer,0,0);
      }
      while(!key[KEY_ESC] && !mouse_b & 1 && !joy[0].button[0].b);
    }
  }
  if( menuOpen){
    draw_trans_sprite( buffer,help,0,0);
  }

  // Draw buffer
  draw_sprite( screen, buffer, 0, 0);
}

Menu::~Menu()
{
  // Destory Bitmaps
  destroy_bitmap( buffer);
  destroy_bitmap( menu);
  destroy_bitmap( menuselect);
  destroy_bitmap( help);
  destroy_bitmap( cursor[0]);
  destroy_bitmap( cursor[1]);
  destroy_bitmap( levelSelectLeft);
  destroy_bitmap( levelSelectRight);
  destroy_bitmap( levelSelectNumber);

  // Destory Samples
  destroy_sample( click);
}
