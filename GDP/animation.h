#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "common.h"

class Array
{
	private:
		float module;
		float x;
		float y;
		float max;
		bool disable;

	public:
		Array(float x = 0, float y = 0, float max = 99999);
		void ToVersor(void);
		void Rotate(int degree);
		void SetDirection(float px, float py, float tx, float ty, float mod);
		void SetCoordinate(float x, float y);
		void AddCoordinate(float x, float y);
		void SetModule(float value);
		void SinFunction(float x, float y, float delay, char type);
		void CosFunction(float x, float y, float delay, char type);
		bool IsNull(void);
		void NullifyProgress(float speed);
		inline float GetModule(void) const { return module; };
		inline float GetX(void) const { return x; };
		inline float GetY(void) const { return y; };
		inline void Revert(void){x = -x; y = -y;};
		inline void SetMax(float value) { if (value >= 1.0) max = value; };
		inline void Disable(void){ disable = true; };
		inline void Able(void){ disable = false; };
		inline bool IsAbled(void) const { return (!disable); };

};

class Sprite
{
	private:
		BITMAP **frame;
		short frame_max;
		short frame_num;
		short *delay;
		short delay_num;
		Array *Pos;
		Box *DB;
		Box *AB;
		int rotation;
		float scale;
		bool alpha;
		bool reversed;

	public:
		Sprite(int id, int max, int gen_delay = 0);
		~Sprite(void);
		void SetDelay(int pos, int ticks);
		int Animate(void);
		int GetCollisionBox(char value,char box) const ;
		void PrintCollisionBox(void) const ;
		void CreateDamageBox(void);
		void CreateAttackBox(void);
		void SetDamageBox(int x, int y, int w, int h, int index);
		void SetAttackBox(int x, int y, int w, int h, int index);
		bool CollisionCheck(Sprite * &other, char self_box, char other_box);
		bool CollisionCheck(Sprite &other, char self_box, char other_box);
		void Move(float x, float y);
		void SetPosition(float x, float y);
		void LoadMask(int id, int x, int y );
		inline int GetX(void) const { return (int) Pos->GetX(); };
		inline int GetY(void) const { return (int) Pos->GetY(); };
		inline float GetPosModule(void) const  { return Pos->GetModule(); };
		inline int GetFrameNum(void) const { return (int) frame_num; };
		inline int GetFrameMax(void) const { return (int) frame_max; };
		inline int GetFrameHeight(void) const { return frame[frame_num]->h; };
		inline int GetFrameWidth(void) const { return frame[frame_num]->w; };
		inline int GetReversed(void) const { return reversed; };
		inline void SetFrameNum(short frame_num) { this->frame_num = frame_num; }; //adicionado por Daniel
		inline void SetScale(float value){ if (value > 0) scale = value; };
		inline void SetRotation(int value){ rotation = value; };
		inline void SetAlpha(void){ alpha = true; };
		inline void SwitchReversed(void){ reversed = !reversed; };
};

using namespace std;

//---------------------------------------------------------------
// Métodos da Classe Sprite

/*
	Não pode haver construtor default
	pois o minimo para a classe sprite existir
	eh ter algum sprite para armazenar
*/
Sprite::Sprite(int id, int max, int gen_delay)
{

	Pos = new Array;


	frame = (BITMAP **) malloc (max * sizeof(BITMAP*));
	this->delay = (short *) malloc (max * sizeof(short));
	frame_max = max;
	frame_num = 0;
	delay_num = 0;
	DB = NULL;
	AB = NULL;
	rotation = DISABLED;
	scale = DISABLED;
	alpha = false;
	reversed = false;

	for(int i = 0;i< max;i++)
	{
	   frame[i] = (BITMAP *) data_sprite[id + i].dat;
		this->delay[i] = gen_delay;
	}

	Pos->SetCoordinate(MAX_X/2, MAX_Y/2);

}

/*
Método destrutor da classe sprite
libera memória ao deletar alguns atributos da classe sprite
deve ser utilizado fora do looping
*/

Sprite::~Sprite(void)
{
	for(int i = 0;i< frame_max;i++)
	  destroy_bitmap(frame[i]);
	free(frame);
	free(delay);
	delete Pos;
}

/*
Seta o delay de um sprite, sendo pos a posição no vetor e ticks o novo valor
pode ser utilizado dentro ou fora do looping
*/

void Sprite::SetDelay(int pos, int ticks)
{
	if (pos < frame_max)	delay[pos] = ticks;
}

/*
Faz a animação do sprite
pode ser utilizado para animar sprite com alpha ou rotacionado e/ou em escala
deve ser utilizado dentro do looping
*/

int Sprite::Animate(void)
{
	if (alpha)
		draw_trans_sprite(buffer, frame[frame_num], (int) Pos->GetX() - (frame[frame_num]->w)/2, (int) Pos->GetY() - (frame[frame_num]->h)/2);
	else
	{
		if ((rotation != DISABLED) && (scale != DISABLED))
			rotate_scaled_sprite(buffer, frame[frame_num], (int) Pos->GetX() - (frame[frame_num]->w)/2, (int) Pos->GetY() - (frame[frame_num]->h)/2, itofix(rotation), ftofix(scale));
		else if (rotation != DISABLED)
			rotate_scaled_sprite(buffer, frame[frame_num], (int) Pos->GetX() - (frame[frame_num]->w)/2, (int) Pos->GetY() - (frame[frame_num]->h)/2, itofix(rotation), ftofix(1.0));
		else if (scale != DISABLED)
			rotate_scaled_sprite(buffer, frame[frame_num], (int) Pos->GetX() - (frame[frame_num]->w)/2 + (int) ((1.0 - scale)*(frame[frame_num]->w/2)), (int) Pos->GetY() - (frame[frame_num]->h)/2 + (int) ((1.0 - scale)*(frame[frame_num]->h/2)), itofix(0), ftofix(scale));
		else
			draw_sprite(buffer, frame[frame_num], (int) Pos->GetX() - (frame[frame_num]->w)/2, (int) Pos->GetY() - (frame[frame_num]->h)/2);
	}

	if (frame_max == 1)	return ANIMATION_END;
	if (delay_num < delay[frame_num])
		delay_num++;
	else
	{
			delay_num = 0;
			if (!reversed)
			{	
				if (frame_num < frame_max - 1)	frame_num++;
				else
				{
					frame_num = 0;
					return ANIMATION_END;
				}
			}
			else
			{
				if (frame_num > 0)	frame_num--;
				else
				{
					frame_num = frame_max;
					return ANIMATION_END;
				}
			}
	}
	
	return frame_num;
}

/*
Retorna o box de colisão
o primeiro parametro se refere a qual dos atributos da estrutura Box vai ser retornado
o segundo parametro  é utlizado para saber qual o tipo do box de colisão(damage ou attack)
deve ser utilizado dentro do looping
*/

int Sprite::GetCollisionBox(char value,char box) const
{
	switch(box)
	{
		case 'd':
			if (!DB)	return ERROR;
			switch(value)
			{
				case 'w':
					return DB[frame_num].w;
				case 'h':
					return DB[frame_num].h;
				case 'x':
					return DB[frame_num].x;
				case 'y':
					return DB[frame_num].y;
			}
		case 'a':
			if (!AB)	return ERROR;
			switch(value)
			{
				case 'w':
					return AB[frame_num].w;
				case 'h':
					return AB[frame_num].h;
				case 'x':
					return AB[frame_num].x;
				case 'y':
					return AB[frame_num].y;
			}
	}
	return ERROR;
}

/*
desenha na tela o damage e o attack box(se houver) do sprite no frame atual
deve ser utilizado dentro do looping
*/

void Sprite::PrintCollisionBox(void) const
{
	if (DB)
		rect(buffer,DB[frame_num].x + (int) Pos->GetX()  - (frame[frame_num]->w)/2, DB[frame_num].y + (int) Pos->GetY() - (frame[frame_num]->h)/2, DB[frame_num].x + DB[frame_num].w + (int) Pos->GetX()  - (frame[frame_num]->w)/2, DB[frame_num].y + DB[frame_num].h + (int) Pos->GetY() - (frame[frame_num]->h)/2,makecol(0,128,255));
	if (AB)
	{
		if (AB[frame_num].x != DISABLED)
			rect(buffer,AB[frame_num].x + (int) Pos->GetX()  - (frame[frame_num]->w)/2,AB[frame_num].y + (int) Pos->GetY() - (frame[frame_num]->h)/2, AB[frame_num].x + AB[frame_num].w + (int) Pos->GetX()  - (frame[frame_num]->w)/2, AB[frame_num].y + AB[frame_num].h + (int) Pos->GetY() - (frame[frame_num]->h)/2,makecol(255,128,0));
	}
	if ( (!DB) && (!AB) )
		textprintf_ex(buffer, font, 3, 3, makecol(255,255,0), -1, "COLLISION BOXES NOT DEFINED");
}

/*
cria o box de colisão do tipo damage
deve ser utilizado fora do looping
*/

void Sprite::CreateDamageBox(void)
{
	if (DB)	return;

	DB = (Box *) malloc (sizeof(Box) *frame_max);
	for (int x = 0; x < frame_max;x++)
	{
		DB[x].x = 0;
		DB[x].y = 0;
		DB[x].w = frame[x]->w;
		DB[x].h = frame[x]->h;
	}
}

/*
cria o box de colisão do tipo attack
deve ser utilizado fora do looping
*/

void Sprite::CreateAttackBox(void)
{
	if (AB)	return;

	AB = (Box *) malloc (sizeof(Box) *frame_max);

	for (int x = 0; x < frame_max;x++)
	{
		AB[x].x = DISABLED;
		AB[x].y = DISABLED;
		AB[x].w = DISABLED;
		AB[x].h = DISABLED;
	}
}

/*
modifica os atributos do box de colisão do tipo damage
a posição é determinada pelo atributo index
pode ser utilizado dentro ou fora do looping
*/

void Sprite::SetDamageBox(int x, int y, int w, int h, int index)
{
	if (!DB)	return;
	DB[index].x = x;
	DB[index].y = y;
	DB[index].w = w;
	DB[index].h = h;
}

/*
modifica os atributos do box de colisão do tipo attack
a posição é determinada pelo atributo index
pode ser utilizado dentro ou fora do looping
*/

void Sprite::SetAttackBox(int x, int y, int w, int h, int index)
{
	if (!AB)	return;
	AB[index].x = x;
	AB[index].y = y;
	AB[index].w = w;
	AB[index].h = h;
}

/*
verifica colisão
o atributo self_box determina qual o tipo de colisão(atraves do damage ou do attack box)
os atributos other e other_box se referem a um segundo sprite com o qual vai se testar
a colisão,de forma que o atributo other_box define o tipo do box de colisão do segundo
sprite(damage ou attack)
deve ser utilizado dentro de um looping
*/

bool Sprite::CollisionCheck(Sprite &other, char self_box, char other_box)
{

	if (self_box == 'd')
	{
		if (!DB)
		{
			textprintf_ex(buffer, font, 3, 3, makecol(255,255,0), -1, "DAMAGE BOX NOT DEFINED");
			return false;
		}

		if (other_box == 'a')
		{
			if (other.GetCollisionBox('x','a') == DISABLED)
				return false;
		}

		if (Pos->GetX() + DB[frame_num].x - (frame[frame_num]->w)/2 > other.GetX() + other.GetCollisionBox('x',other_box) + other.GetCollisionBox('w',other_box) - (other.GetFrameWidth()/2))
			return false;

		if (Pos->GetY() + DB[frame_num].y - (frame[frame_num]->h)/2 > other.GetY() + other.GetCollisionBox('y',other_box) + other.GetCollisionBox('h',other_box) - (other.GetFrameHeight()/2))
			return false;

		if (Pos->GetX() + DB[frame_num].x + DB[frame_num].w   - (frame[frame_num]->w)/2< other.GetX() + other.GetCollisionBox('x',other_box) - (other.GetFrameWidth()/2))
			return false;

		if (Pos->GetY() + DB[frame_num].y + DB[frame_num].h   - (frame[frame_num]->h)/2< other.GetY() + other.GetCollisionBox('y',other_box) - (other.GetFrameHeight()/2))
			return false;
	}
	else
	{
		if (!AB)
		{
			textprintf_ex(buffer, font, 3, 3, makecol(255,255,0), -1, "ATTACK BOX NOT DEFINED");
			return false;
		}


		if (AB[frame_num].x == DISABLED) //corrigido por Daniel = -> ==
			return false;

		if (other_box == 'a')
		{
			if (other.GetCollisionBox('x','a') == DISABLED)
				return false;
		}

		if (Pos->GetX() + AB[frame_num].x - (frame[frame_num]->w)/2 > other.GetX() + other.GetCollisionBox('x',other_box) + other.GetCollisionBox('w',other_box) - (other.GetFrameWidth()/2))
			return false;

		if (Pos->GetY() + AB[frame_num].y - (frame[frame_num]->h)/2 > other.GetY() + other.GetCollisionBox('y',other_box) + other.GetCollisionBox('h',other_box) - (other.GetFrameHeight()/2))
			return false;

		if (Pos->GetX() + AB[frame_num].x + AB[frame_num].w   - (frame[frame_num]->w)/2< other.GetX() + other.GetCollisionBox('x',other_box) - (other.GetFrameWidth()/2))
			return false;

		if (Pos->GetY() + AB[frame_num].y + AB[frame_num].h   - (frame[frame_num]->h)/2< other.GetY() + other.GetCollisionBox('y',other_box) - (other.GetFrameHeight()/2))
			return false;
	}

	return true;
}

/*
verifica colisão
o atributo self_box determina qual o tipo de colisão(atraves do damage ou do attack box)
os atributos other e other_box se referem a um segundo sprite com o qual vai se testar
a colisão,de forma que o atributo other_box define o tipo do box de colisão do segundo
sprite(damage ou attack)
a diferença para a primeira função é que nesta o primeiro atributo é um objeto dinâmico
deve ser utilizado dentro de um looping
*/

bool Sprite::CollisionCheck(Sprite * &other, char self_box, char other_box)
{

	if (self_box == 'd')
	{
		if (!DB)
		{
			textprintf_ex(buffer, font, 3, 3, makecol(255,255,0), -1, "DAMAGE BOX NOT DEFINED");
			return false;
		}

		if (other_box == 'a')
		{
			if (other->GetCollisionBox('x','a') == DISABLED)
				return false;
		}

		if (Pos->GetX() + DB[frame_num].x - (frame[frame_num]->w)/2 > other->GetX() + other->GetCollisionBox('x',other_box) + other->GetCollisionBox('w',other_box) - (other->GetFrameWidth()/2))
			return false;

		if (Pos->GetY() + DB[frame_num].y - (frame[frame_num]->h)/2 > other->GetY() + other->GetCollisionBox('y',other_box) + other->GetCollisionBox('h',other_box) - (other->GetFrameHeight()/2))
			return false;

		if (Pos->GetX() + DB[frame_num].x + DB[frame_num].w   - (frame[frame_num]->w)/2< other->GetX() + other->GetCollisionBox('x',other_box) - (other->GetFrameWidth()/2))
			return false;

		if (Pos->GetY() + DB[frame_num].y + DB[frame_num].h   - (frame[frame_num]->h)/2< other->GetY() + other->GetCollisionBox('y',other_box) - (other->GetFrameHeight()/2))
			return false;
	}
	else
	{
		if (!AB)
		{
			textprintf_ex(buffer, font, 3, 3, makecol(255,255,0), -1, "ATTACK BOX NOT DEFINED");
			return false;
		}

		if (AB[frame_num].x == DISABLED) //igualmente corrigido
			return false;

		if (other_box == 'a')
		{
			if (other->GetCollisionBox('x','a') == DISABLED)
				return false;
		}

		if (Pos->GetX() + AB[frame_num].x - (frame[frame_num]->w)/2 > other->GetX() + other->GetCollisionBox('x',other_box) + other->GetCollisionBox('w',other_box) - (other->GetFrameWidth()/2))
			return false;

		if (Pos->GetY() + AB[frame_num].y - (frame[frame_num]->h)/2 > other->GetY() + other->GetCollisionBox('y',other_box) + other->GetCollisionBox('h',other_box) - (other->GetFrameHeight()/2))
			return false;

		if (Pos->GetX() + AB[frame_num].x + AB[frame_num].w   - (frame[frame_num]->w)/2< other->GetX() + other->GetCollisionBox('x',other_box) - (other->GetFrameWidth()/2))
			return false;

		if (Pos->GetY() + AB[frame_num].y + AB[frame_num].h   - (frame[frame_num]->h)/2< other->GetY() + other->GetCollisionBox('y',other_box) - (other->GetFrameHeight()/2))
			return false;
	}

	return true;
}

/*
Soma à posição de um sprite o vetor (x, y)
deve ser utilizado dentro ou fora do looping
*/

void Sprite::Move(float x, float y)
{
	Pos -> AddCoordinate(x,y);
}

/*
Seta posições do sprite.
pode ser utilizado dentro ou fora do looping
*/

void Sprite::SetPosition(float x, float y)
{
	Pos->SetCoordinate(x,y);
}

/*
Desenha uma imagem por cima do sprite.
deve ser usado fora do looping
*/

void Sprite::LoadMask(int id, int x, int y )
{
	BITMAP *temp;

	temp = (BITMAP *) data_sprite[id].dat;


	for(int x = 0; x < frame_max;x++)
		draw_sprite(frame[x], temp, x, y);


	destroy_bitmap(temp);
}

//---------------------------------------------------------------
// Métodos da Classe Array

/*
Construtor da Classe Array em que se define x, y e seu tamanho maximo.
deve ser utilizado fora do looping
*/

Array::Array(float x, float y, float max)
{
    this->x = x;
    this->y = y;
    this->max = max;
    module = (float) sqrt( (this->x)*(this->x) + (this->y)*(this->y) );
}

/*
Calcula o versor de um vetor (o vetor de modulo um)
pode ser utilizado dentro ou fora de um looping
*/

void Array::ToVersor(void)
{
	if(module != 0)
	{
       x /= module;
       y /= module;
	   module = 1;
    }
	else
	{
	   x = 0;
	   y = 0;
	}
}

/*
Rotaciona um vetor
pode ser utilizado dentro ou fora de um looping
*/

void Array::Rotate(int degree)
{
	float a = x, b = y;
	this->x = a*cos(degree)-b*sin(degree);
	this->y = a*sin(degree)+b*cos(degree);
}

/*
Faz o vetor apontar na direcao do ponto (px, py) para o ponto (tx, ty), com tamanho mod.
pode ser utilizado dentro ou fora de um looping
*/

void Array::SetDirection(float px, float py, float tx, float ty, float mod)
{
	this->x = (float) tx - px;
	this->y = (float) ty - py;

	module = (float) sqrt( (this->x)*(this->x) + (this->y)*(this->y) );

	if (!module)
	{
		this->x = 0.0;
		this->y = 0.0;
	}
	else
	{
		this->x /= module;
		this->y /= module;
	}

	if (mod > max)
		mod = max;

	this->x *= mod;
	this->y *= mod;

	module = mod;
}

/*
Seta as cordenadas x e y do vetor e atualiza seu modulo.
pode ser utilizado dentro ou fora de um looping
*/

void Array::SetCoordinate(float x, float y)
{
	this->x = x;
	this->y = y;

	module =  sqrt( (this->x * this->x) + (this->y * this->y) );

	if(module > max)
	{
		this->x /= module;
		this->y /= module;
		module = max;
		this->x *= module;
		this->y *= module;
	}
}

/*
Soma os valores x e y às cordenadas de um vetor.
deve ser utilizado dentro de um looping
*/

void Array::AddCoordinate(float x, float y)
{
	this -> x += x;
	this -> y += y;

	module = sqrt( (this->x * this->x) + (this->y * this->y) );

	if(module > max)
	{
		this->x /= module;
		this->y /= module;
		module = max;
		this->x *= module;
		this->y *= module;
	}
}

/*
Seta o modulo de um vetor.
pode ser utilizado dentro ou fora de um looping
*/

void Array::SetModule(float value)
{
	x/=module;
	y/=module;

	if(value > max)
	{
		module = max;
		x*= module;
		y*= module;
	}
	else
	{
		x *= value;
		y *= value;
		module = value;
	}
}

/*
faz com o que o vetor se comporte como uma função seno (f(x) = sen(x))
deve ser chamado dentro de um looping
*/

void Array::SinFunction(float x, float y, float delay, char type)
{
	if(!delay)
		return;
	switch(type)
	{
		case 'x':
			AddCoordinate(x,sin(counter/delay));
			break;
		case 'y':
			AddCoordinate(sin(counter/delay),y);
		   	break;
	}
}

/*
faz com o que o vetor se comporte como uma função cosseno (f(x) = cos(x))
deve ser chamado dentro de um looping
*/

void Array::CosFunction(float x, float y, float delay, char type)
{
	if(!delay)
		return;
	switch(type)
	{
		case 'x':
			AddCoordinate(x,cos(counter/delay));
			break;
		case 'y':
			AddCoordinate(cos(counter/delay),y);
			break;
	}
}

/*
Verifica se um vetor é nulo.
pode ser utilizado dentro ou fora de um looping
*/

bool Array::IsNull(void)
{
	if(module == 0)
		return true;
	else
		return false;
}

/*
Gradualmente decrementa um vetor, com velocidade speed.
deve ser utilizado dentro de um looping
*/

void Array::NullifyProgress(float speed)
{
    if (x >= 0)
    {
        x -= speed;
        if(x < 0)
            x = 0;
    }
    else
    {
        x += speed;
        if(x > 0)
            x = 0;
    }
    if (y >= 0)
    {
		y -= speed;
		if(y < 0)
            y = 0;
    }
    else
    {
        y += speed;
        if (y > 0)
            y = 0;
    }
	module = sqrt(x*x+y*y);
}


#endif
