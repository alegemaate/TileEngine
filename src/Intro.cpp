#include "Intro.h"
#include "tools.h"

Intro::Intro()
{
  // Loading font
  FONT *f1, *f2, *f3, *f4, *f5;

  //Sets Font
  f1 = load_font("fonts/introFont.pcx", NULL, NULL);
  f2 = extract_font_range(f1, ' ', 'A'-1);
  f3 = extract_font_range(f1, 'A', 'Z');
  f4 = extract_font_range(f1, 'Z'+1, 'z');

  //Merge fonts
  font = merge_fonts(f4, f5 = merge_fonts(f2, f3));

  // Say its loading
  textout_centre_ex(screen, font, "Loading...", SCREEN_W/2, (SCREEN_H - text_height(font))/2, makecol(255,255,255), -1);

  // Sound
  introSound = load_sample( "sounds/intro.wav");

  // Load background
  intro = load_bitmap( "images/title.png", NULL);

  string fileName;
  for( int i = 1; i < 63; i++){
    fileName = "images/intro/ (" + convertIntToString( i) + ").png";
    images[i] = load_bitmap( fileName.c_str(), NULL);
  }

  buffer = create_bitmap( SCREEN_W, SCREEN_H);

  // Sound
  play_sample( introSound, 128, 255, 1000, false);
}

void Intro::update()
{
  // Wait 2 seconds then go to the menu
  set_next_state(STATE_MENU);
}

void Intro::draw()
{
  // Intro stuffs
  highcolor_fade_in( images[1], 64);
  clear_keybuf();

  // Show background
  //play_sample( introSound, 255, 128, 1000, 0);
  for( int i = 1; i < 63; i++){
    poll_joystick();
    if(keyboard_keypressed() || joy_buttonpressed()){
      break;
    }
    rectfill( buffer, 0, 0, SCREEN_W, SCREEN_H, makecol( 0,0,0));
    rectfill( buffer, 0, 0, SCREEN_W, SCREEN_H, makecol( 0,0,0));
    stretch_sprite( buffer, images[i], 0, 0, SCREEN_W, SCREEN_H);
    stretch_sprite( screen, buffer, 0, 0, SCREEN_W, SCREEN_H);
    // Wheres my car?
    rest(70);
  }
  highcolor_fade_out( 32);
  rectfill( buffer, 0, 0, SCREEN_W, SCREEN_H, makecol( 0,0,0));
  highcolor_fade_in( intro, 32);
    rest(3000);
  highcolor_fade_out( 32);
}

Intro::~Intro()
{
  // Clear memory
  for( int i = 1; i < 63; i++){
    if(images[i]){
      destroy_bitmap( images[i]);
    }
  }
  destroy_bitmap( buffer);
  destroy_bitmap( intro);
  destroy_sample( introSound);
}
