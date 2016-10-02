/*
 *  Abuse — dark 2D side-scrolling platform game
 *  Copyright © 1995 Crack dot Com
 *  Copyright © 2005—2016 Sam Hocevar <sam@hocevar.net>
 *
 *  This software was released into the Public Domain. As with most public
 *  domain software, no warranty is made or implied by Crack dot Com, by
 *  Jonathan Clark, or by Sam Hocevar.
 */

#pragma once

#include "objects.h"
#include "chars.h"


// The controlled character is you!

#define CHAR_HUMAN 0
#define CHAR_IGUANA 1

#define GAME_CHARACTERS 2
#define TOTAL_CHARACTERS 10

#define MORPH_TABLE_WIDTH (GAME_CHARACTERS-1)

extern sequence *morph_table[MORPH_TABLE_WIDTH*MORPH_TABLE_WIDTH];

class controlled_character : public GameObject
{
  GameObject *cur_char;
  signed char morphing_to;  // who you are right now
  unsigned long abilities;  // if 1<<character bit is set then you have that ability
  void change_character(int char_type);

public :
  controlled_character(long X, long Y);
  controlled_character(FILE *fp);
  virtual int size();
  virtual game_objects type() { return O_starting_position; }
  virtual void save(FILE *fp);
  virtual int decide();

  void do_damage(int amount, GameObject *who, GameObject *from);
};

