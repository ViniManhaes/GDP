#ifndef __GAMEBASICS_H__
#define __GAMEBASICS_H__

#include "common.h"

volatile unsigned long long int counter;
BITMAP *buffer;
DATAFILE *data_sprite;

void game_timer(void);
int begin(void);
void closing(void);

void game_timer(void)
{
	counter++;
}
END_OF_FUNCTION(game_timer);

int begin(void)
{

	allegro_init();

	install_keyboard();
	install_mouse();

	install_timer();

	srand((unsigned)time(NULL));

	set_color_depth(COLOR_BITS);
	if (set_gfx_mode(VIDEO_CARD, MAX_X, MAX_Y, V_MAX_X, V_MAX_Y) < 0)
	{
		if (set_gfx_mode(GFX_AUTODETECT, MAX_X, MAX_Y, V_MAX_X, V_MAX_Y) < 0)
		{
			allegro_message("Erro ao inicializar o modo grafico");
			return (FALSE);
		}
	}

	if (COLOR_BITS == 32)
		set_alpha_blender();

	/*
	//corte para usar em Linux :)
	if (install_sound(DIGI_CARD, MIDI_CARD, NULL) < 0)
	{
		if (install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL) < 0)
		{
			allegro_message("Erro ao inicializar o som");
			return (FALSE);
		}
	}
	*/
	
	buffer = create_bitmap(MAX_X,MAX_Y);
	clear(buffer);
	
	// Datas---------------------------------------------
	
	// Senha do Data:
	packfile_password("870SDFG4rewt4D7y34t");
	
	// Carregando data de sprites do jogo
	data_sprite = load_datafile("data/sprites.dat");
	//outros datas...
	
	// Removendo o Registro da Senha
	packfile_password(NULL);
	
	LOCK_VARIABLE(counter);
	LOCK_FUNCTION(game_timer);
	install_int(game_timer, 1000/FPS);
	
	
	#undef FPS
	#undef VIDEO_CARD
	#undef DIGI_CARD
	#undef MIDI_CARD
	#undef V_MAX_X
	#undef V_MAX_Y

	return (TRUE);
}

void closing(void)
{
	destroy_bitmap(buffer);
	remove_timer();
	remove_mouse();
	remove_sound();
	remove_keyboard();
	allegro_exit();
}

#endif
