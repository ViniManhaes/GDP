#include "common.h"

//-------------------------------------------------------------------------
// FUNÇÕES PRINCIPAIS
void gameloop(void); 


int main(int argc, char **argv)
{
	if (!begin())
	{
		closing();
		return -1;
	}

	gameloop();
	
	
	closing();
	return 0;

}
END_OF_MAIN();

void gameloop(void)
{
	unsigned long long int vel_control = 0;
	
	
	Sprite hero(HERO00000001_PCX,8,4);
	Sprite other(HERO01800001_PCX,10,7);
			
	
	hero.CreateDamageBox();
	other.CreateDamageBox();
	
	Mobile teste(500);
	teste.addSprite(&hero);
	teste.addSprite(&other);
	teste.addSprite(HERO02200001_PCX,7,3);

	
	// Looping do Jogo-------------------------------
	do
	{
		cout << teste.getSize() << endl;
		if (counter >= vel_control)
		{
			cout << "!" << endl;		
			// Ações e Movimentos---------------------------
			if(key[KEY_DOWN])
			{
				cout << "#" << endl;
				teste.setIndex(2);
			}
			else if(key[KEY_UP])
			{
				cout << "#" << endl;
				teste.setIndex(1);
				teste.impulse(*(new Array(0, -20)));
			}
			else if(key[KEY_LEFT])
			{
				cout << "#" << endl;
				teste.setIndex(1);
				teste.impulse(*(new Array(-10, 0)));
			}
			else if(key[KEY_RIGHT])
			{
				cout << "#" << endl;
				teste.setIndex(1);
				teste.impulse(*(new Array(10, 0)));
			}
			else
			{
				cout << "#" << endl;
				teste.setIndex(0);
			}
			cout << "@" << endl;
			
			// Desenhando Sprites----------------------------
			teste.act();
			
			
			/*
			if (hero.CollisionCheck(other, 'd', 'd'))
				textprintf_ex(buffer, font, 400, 3, makecol(0,0,255), -1, "Colisao: detectada !");
			else
				textprintf_ex(buffer, font, 400, 3, makecol(255,0,0), -1, "Colisao: nao detectada");*/
			
			
			//Re-desenhando a tela---------------------------
			blit(buffer, screen, 0, 0, 0, 0,MAX_X,MAX_Y);
			clear(buffer);
			
			vel_control = counter + 1;
		}
		
		if (key[KEY_ESC])    break;
	}while(1);
	
	
	// destruição de Objetos Dinamicos
	
}
