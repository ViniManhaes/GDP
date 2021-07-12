#ifndef __COMMON_H__
#define __COMMON_H__

// Includes Padrões
#include <iostream>
#include <string>
#include <time.h>
#include <allegro.h>
#include <math.h>

//coloquei porque muitas coisas usam
using namespace std;

// Defines do Sistema
#define MAX_X 						640
#define MAX_Y 						480
#define V_MAX_X 					0
#define V_MAX_Y 					0
#define COLOR_BITS 				32
#define VIDEO_CARD 				GFX_SAFE//GFX_AUTODETECT_FULLSCREEN
#define DIGI_CARD 				DIGI_AUTODETECT
#define MIDI_CARD 				MIDI_AUTODETECT
#define FPS 						60

// Defines Gerais
#define ERROR		-999999
#define DISABLED	-888888
#define ANIMATION_END	-38284

typedef struct _Box
{
	int x;
	int y;
	int w;
	int h;
}Box;




// Includes do Data
#include "data/sprite_id.h"


// Includes do Jogo
#include "gamebasics.h"
#include "animation.h"


// Includes da Fisica
#include "object.h"
#include "mobile.h"
//#include "item.h" //removi por problemas com a vtable, depois eu coloco de volta, acredito que deva ter algo a ver com ser virtual e nao ter descendentes -> pobre item, e uma solteirona sem filhos :'(
#include "teleporter.h"

#endif
