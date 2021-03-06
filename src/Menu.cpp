#include "Menu.h"

Menu::Menu(){
  // Init fmod
  FSOUND_Init (44100, 32, 0);

  // Create buffer image
  buffer = create_bitmap( SCREEN_W, SCREEN_H);

  // Load images
  if(!(menu = load_bitmap( ("images/gui/menu.png"), NULL)))
    abort_on_error( "Cannot find image images/gui/menu.png \n Please check your files and try again");
  if(!(menuselect = load_bitmap( ("images/gui/menuSelector.png"), NULL)))
    abort_on_error( "Cannot find image images/gui/menuSelector.png \n Please check your files and try again");
  if(!(help = load_bitmap( ("images/gui/help.png"), NULL)))
    abort_on_error( "Cannot find image images/gui/help.png \n Please check your files and try again");
  if(!(cursor[0] = load_bitmap( ("images/gui/cursor1.png"), NULL)))
    abort_on_error( "Cannot find image images/gui/cursor1.png \n Please check your files and try again");
  if(!(cursor[1] = load_bitmap( ("images/gui/cursor2.png"), NULL)))
    abort_on_error( "Cannot find image images/gui/cursor2.png \n Please check your files and try again");
  if(!(levelSelectLeft = load_bitmap( ("images/gui/levelSelectLeft.png"), NULL)))
    abort_on_error( "Cannot find image images/gui/levelSelectLeft.png \n Please check your files and try again");
  if(!(levelSelectRight = load_bitmap( ("images/gui/levelSelectRight.png"), NULL)))
    abort_on_error( "Cannot find image images/gui/levelSelectRight.png \n Please check your files and try again");
  if(!(levelSelectNumber = load_bitmap( ("images/gui/levelSelectNumber.png"), NULL)))
    abort_on_error( "Cannot find image images/gui/levelSelectNumber.png \n Please check your files and try again");
  if(!(copyright = load_bitmap( ("images/gui/copyright.png"), NULL)))
    abort_on_error( "Cannot find image images/gui/copyright.png \n Please check your files and try again");
  if(!(credits = load_bitmap( ("images/gui/credits.png"), NULL)))
    abort_on_error( "Cannot find image images/gui/credits.png \n Please check your files and try again");

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
  newSelectorY = SCREEN_H - 323;
  selectorY = SCREEN_H - 323;
  selectorX = 60;
  mouse_control = false;
  selectorHovering = 0;

  // Create map for live background
  tile_map = new tileMap("data/levels/level_01");

  // Set background scroll direction
  scrollDirection = "right";
  tile_map -> y = random( 0, (tile_map -> height * 64) - SCREEN_H);
  tile_map -> x = 0;

  levelOn = 0;
}

void Menu::update(){
  // Disable/Enable mouse control on input
  if(key[KEY_UP] || key[KEY_DOWN] || key[KEY_LEFT] || key[KEY_RIGHT] || key[KEY_W] || key[KEY_A] || key[KEY_S] || key[KEY_D] || joy[0].stick[0].axis[1].d2 || joy[0].stick[0].axis[1].d1 || joy[0].button[4].b || joy[0].button[5].b)
    mouse_control=false;
  else if (mouse_x!=old_mouse_x || mouse_y!=old_mouse_y)
    mouse_control=true;

  old_mouse_x = mouse_x;
  old_mouse_y = mouse_y;

  menuOpen = false;
  poll_joystick();

  // Move around live background
  if( scrollDirection == "right"){
    if( tile_map -> x + 1 < (tile_map -> width * 64 - SCREEN_W)){
      tile_map -> x += 1;
    }
    else{
      scrollDirection = "left";
    }
  }
  else if( scrollDirection == "left"){
    if( tile_map -> x - 1 > 0){
      tile_map -> x -= 1;
    }
    else{
      scrollDirection = "right";
    }
  }


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
  if((key[KEY_W] || key[KEY_UP] || joy[0].stick[0].axis[1].d1) && step>10){
    if(selectorHovering!=0)
      selectorHovering--;
    else
      selectorHovering=3;
    step=0;
  }
  if((key[KEY_S] || key[KEY_DOWN] || joy[0].stick[0].axis[1].d2) && step>10){
    if(selectorHovering!=3)
      selectorHovering++;
    else
      selectorHovering=0;
    step=0;
  }

  // Hover play
  if((mouse_control && (collisionAny(mouse_x,mouse_x,60,270,mouse_y,mouse_y, SCREEN_H-323, SCREEN_H-278)) || !mouse_control && ( selectorHovering==0))){
    if(newSelectorY != SCREEN_H-323){
      newSelectorY = SCREEN_H-323;
      selectorX = 60;
      play_sample(click,255,125,1000,0);
    }

  }
  // Hover edit
  else if((mouse_control && (collisionAny(mouse_x,mouse_x,60,270,mouse_y,mouse_y, SCREEN_H-260, SCREEN_H-215)) || !mouse_control && ( selectorHovering==1))){
    if(newSelectorY != SCREEN_H-260){
      newSelectorY = SCREEN_H-260;
      selectorX = 60;
      play_sample(click,255,125,1000,0);
    }

  }
  // Hover help
  else if((mouse_control && (collisionAny(mouse_x,mouse_x,60,270,mouse_y,mouse_y, SCREEN_H-197, SCREEN_H-152)) || !mouse_control && ( selectorHovering==2))){
    if(newSelectorY != SCREEN_H-197){
      newSelectorY = SCREEN_H-197;
      selectorX = 60;
      play_sample(click,255,125,1000,0);
    }
    menuOpen = true;
  }
  //Hover exit
  else if((mouse_control && (collisionAny(mouse_x,mouse_x,60,270,mouse_y,mouse_y, SCREEN_H-132, SCREEN_H-87)) || !mouse_control && ( selectorHovering==3))){
    if(newSelectorY != SCREEN_H-132){
      newSelectorY = SCREEN_H-132;
      selectorX = 60;
      play_sample(click,255,125,1000,0);
    }
  }

  // Select button
  // level select left
  if(((collisionAny(mouse_x,mouse_x,SCREEN_W-180,SCREEN_W-140,mouse_y,mouse_y, 80, 120) && mouse_b & 1) || (key[KEY_A] || key[KEY_LEFT] || joy[0].button[4].b)) && step>10) {
    play_sample(click,255,125,1000,0);
    if( levelOn > 0){
      levelOn--;
    }
    else{
      levelOn = 3;
    }
    if( levelOn == 0){
      tile_map -> load( "data/levels/level_01");
    }
    else if( levelOn == 1){
      tile_map -> load( "data/levels/level_test");
    }
    else if( levelOn == 2){
      tile_map -> load( "data/saves/danny");
    }
    else if( levelOn == 3){
      tile_map -> load( "data/saves/dannyII");
    }
    tile_map -> y = random( 0, (tile_map -> height * 64) - SCREEN_H);
    tile_map -> x = 0;
    step=0;
  }
  if(((collisionAny(mouse_x,mouse_x,SCREEN_W-80,SCREEN_W-40,mouse_y,mouse_y, 80, 120) && mouse_b & 1) || (key[KEY_D] || key[KEY_RIGHT] || joy[0].button[5].b)) && step>10){
  // level select right
    play_sample(click,255,125,1000,0);
    if( levelOn < 3){
      levelOn++;
    }
    else{
      levelOn = 0;
    }
    if( levelOn == 0){
      tile_map -> load( "data/levels/level_01");
    }
    else if( levelOn == 1){
      tile_map -> load( "data/levels/level_test");
    }
    else if( levelOn == 2){
      tile_map -> load( "data/saves/danny");
    }
    else if( levelOn == 3){
      tile_map -> load( "data/saves/dannyII");
    }
    tile_map -> y = random( 0, (tile_map -> height * 64) - SCREEN_H);
    tile_map -> x = 0;
    step=0;
  }
  // Start
  if(((collisionAny(mouse_x,mouse_x,60,270,mouse_y,mouse_y,  SCREEN_H-323, SCREEN_H-278)&& mouse_b & 1)|| ((key[KEY_ENTER] || joy[0].button[0].b) && selectorHovering==0)) && step>10){
    set_next_state( STATE_GAME);
  }
  // Edit
  if(((collisionAny(mouse_x,mouse_x,60,270,mouse_y,mouse_y, SCREEN_H-260, SCREEN_H-215)&& mouse_b & 1)|| ((key[KEY_ENTER] || joy[0].button[0].b) && selectorHovering==1)) && step>10){
    set_next_state( STATE_EDIT);
  }
  // Quit
  if(((collisionAny(mouse_x,mouse_x,60,270,mouse_y,mouse_y,  SCREEN_H-132, SCREEN_H-87)&& mouse_b & 1)|| ((key[KEY_ENTER] || joy[0].button[0].b) && selectorHovering==3)) && step>10){
    set_next_state( STATE_EXIT);
  }
  step++;
}

void Menu::draw()
{
  // Draw background to screen
  rectfill( buffer, 0, 0, SCREEN_W, SCREEN_H, makecol( 255,255,255));

  // Draw live background
  tile_map -> draw_map(buffer);

  // Overlay
  draw_trans_sprite(buffer, credits, 0, 0);
  draw_trans_sprite(buffer, menu, 0, SCREEN_H-461);
  draw_trans_sprite(buffer, menuselect, selectorX, selectorY);

  // Level selection
  draw_trans_sprite(buffer, levelSelectLeft, SCREEN_W-180, 80);
  draw_trans_sprite(buffer, levelSelectNumber, SCREEN_W-160, 80);
  textprintf_centre_ex(buffer,font,SCREEN_W-112,73,makecol(0,0,0),-1,"%i",levelOn + 1);
  draw_trans_sprite(buffer, levelSelectRight, SCREEN_W-80, 80);

  // Hover select left
  if(collisionAny(mouse_x,mouse_x,SCREEN_W-180,SCREEN_W-140,mouse_y,mouse_y, 80, 120)){
    draw_trans_sprite(buffer, levelSelectLeft, SCREEN_W-180, 80);
  }
  // Hover select right
  if(collisionAny(mouse_x,mouse_x,SCREEN_W-80,SCREEN_W-40,mouse_y,mouse_y, 80, 120)){
    draw_trans_sprite(buffer, levelSelectRight, SCREEN_W-80, 80);
  }

  // Cursor
  draw_sprite(buffer, cursor[0], mouse_x, mouse_y);

  // Select button
  if (mouse_b & 1 || key[KEY_ENTER] || joy[0].button[0].b){
    if (selectorY==610){
      do{
        draw_sprite(buffer, menu, 0, 0);
        draw_sprite(buffer, help,0,0);
        draw_sprite(screen,buffer,0,0);
      }
      while(!key[KEY_ESC] && !mouse_b & 1 && !joy[0].button[0].b);
    }
  }
  if( menuOpen){
    draw_trans_sprite( buffer,help,0,0);
  }
  draw_trans_sprite(buffer,copyright,SCREEN_W-350,SCREEN_H-40);
  draw_sprite(buffer,cursor[0],mouse_x,mouse_y);
  // Draw buffer
  stretch_sprite( screen, buffer, 0, 0, SCREEN_W, SCREEN_H);
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
  destroy_sample( intro);

  // Fade out
  highcolor_fade_out(16);

  // Clean up fmod
  FSOUND_Close();
}
