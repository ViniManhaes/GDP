#ifndef __MOBILE_H__
#define __MOBILE_H__

#define ATTRITION 0.6
#define GRAVITY	10

class Mobile: public Object
{
	private:
		Array *speed;
		Array *thrust;
		//bool free; //aparentemente e inutil, nao serve para o que foi planejado e se ninguem achar uma funcao vai ser cortada em definitivo
		int ground;

	public:
		Mobile(Sprite** animation, unsigned short int quat_sprite, Array *speed, Array *thrust, /*bool free,*/ int ground, int pos_x, int pos_y);
		Mobile(int ground);
		~Mobile();
		inline void setGround(int ground) { this->ground = ground; };
		inline int getGround(void) { return ground; };
		inline void setSpeed(Array *speed) { this->speed = speed; };
		inline Array getSpeed(void) { return *speed; };
		inline void setThrust(Array *thrust) { this->thrust = thrust; };
		inline Array getThrust(void) { return *thrust; };
		/*inline void setFree(bool free) { this->free = free; };
		inline bool getFree(void) { return free; };*/
		void impulse(Array thrust);
		void move(vector<Object> obstacle);
		bool collisionCheck(Object target, char self_box, char other_box);
		bool bumpCollision(Object target);
		bool attackCollision(Object target);
		int getDiff_x(Object target);
		int getDiff_y(Object target);
};

Mobile::Mobile(int ground): Object()
{
	speed = new Array(0, 0);
	thrust = new Array(0, 0);
	setGround(ground);
}

Mobile::Mobile(Sprite** animation, unsigned short int quant_sprite, Array *speed, Array *thrust, /*bool free,*/ int ground, int pos_x, int pos_y) : Object(animation, quant_sprite)
{
	setSpeed(speed);
	setThrust(thrust);
	//setFree(free);
	setGround(ground);
	setIndex(0);
	getAnimation()->SetPosition(pos_x, pos_y);
}

Mobile::~Mobile(void)
{
	delete speed;
	delete thrust;
}

//deve receber o somatorio das aceleracoes ques estao atuando sobre o corpo e modifica a velocidade de acordo
//gravidade e atrito estao embutidos no codigo
void Mobile::impulse(Array thrust)
{
	thrust.AddCoordinate(0, GRAVITY); //gravidade

	thrust.AddCoordinate(speed->GetX()*ATTRITION, speed->GetY()*ATTRITION); //atrito -> baseado na velocidade do corpo

	speed->AddCoordinate(thrust.GetX(), thrust.GetY()); //somatorio dos impulsos que ele esta recebendo
}

//move e trata as colisoes necessarias
//tambem aplica impulso no sentido contrario se houver colisao no caminho
void Mobile::move(vector<Object> obstacle)
{
	int x_atual, y_atual;
	int diff_x, diff_y;

	x_atual = getAnimation()->GetX();
	y_atual = getAnimation()->GetY();

	getAnimation()->Move(speed->GetX(), speed->GetY());

	for(unsigned int i = 0; i < obstacle.size(); i++)
	{
		if(bumpCollision(obstacle[i]))
		{
			//preciso ajustar essa parte
			diff_x = getDiff_x(obstacle[i]);
			diff_y = getDiff_y(obstacle[i]);
			thrust->AddCoordinate(diff_x,diff_y);
			getAnimation()->Move(diff_x, diff_y);
		}
	}

	if(getAnimation()->GetY() + getAnimation()->GetCollisionBox('d','h') - (getAnimation()->GetFrameHeight()>>1) > ground)
	{
		getAnimation()->Move(0, ground - getAnimation()->GetY() + getAnimation()->GetCollisionBox('d', 'h') - (getAnimation()->GetFrameHeight()>>1));
	}
}

//e uma simplificacao da mesma funcao do sprite, para objetos
bool Mobile::collisionCheck(Object target, char self_box, char other_box)
{
	return this->getAnimation()->CollisionCheck(*(target.getSprite()), self_box, other_box);
	//essa parte foi bizarra, tive que criar uma funcao para isso funcionar
	//mesmo considerando que eu ja tinha uma funcao que faz isso ai
}

//batida simples
bool Mobile::bumpCollision(Object target)
{
	return this->collisionCheck(target, 'd', 'd');
}

//batida de ataque
bool Mobile::attackCollision(Object target)
{
	return this->collisionCheck(target, 'a', 'd');
}

//detecta o quanto um objeto entrou em outro, pelo eixo x
int Mobile::getDiff_x(Object target)
{
	int x_menor1, x_maior1, x_menor2, x_maior2;

	x_menor1 = getAnimation()->GetX() - getAnimation()->GetCollisionBox('d', 'w') + (getAnimation()->GetFrameWidth()>>1);
	x_maior1 = getAnimation()->GetX() - getAnimation()->GetCollisionBox('d', 'w') + (getAnimation()->GetFrameWidth()>>1);
	x_menor2 = getAnimation()->GetX() - getAnimation()->GetCollisionBox('d', 'w') + (getAnimation()->GetFrameWidth()>>1);
	x_maior2 = getAnimation()->GetX() - getAnimation()->GetCollisionBox('d', 'w') + (getAnimation()->GetFrameWidth()>>1);

	if(x_maior1 >= x_menor2)
	{
		if(x_menor1 > x_maior2)
		{
			return 0;
		}
		return x_maior2 - x_menor1;
	}
	return 0;
}

//detecta o quanto um objeto entrou em outro, pelo eixo y
int Mobile::getDiff_y(Object target)
{
	int y_menor1, y_maior1, y_menor2, y_maior2;

	y_menor1 = getAnimation()->GetY() - getAnimation()->GetCollisionBox('d', 'h') + (getAnimation()->GetFrameHeight()>>1);
	y_maior1 = getAnimation()->GetY() - getAnimation()->GetCollisionBox('d', 'h') + (getAnimation()->GetFrameHeight()>>1);
	y_menor2 = getAnimation()->GetY() - getAnimation()->GetCollisionBox('d', 'h') + (getAnimation()->GetFrameHeight()>>1);
	y_maior2 = getAnimation()->GetY() - getAnimation()->GetCollisionBox('d', 'h') + (getAnimation()->GetFrameHeight()>>1);

	if(y_maior1 >= y_menor2)
	{
		if(y_menor1 > y_maior2)
		{
			return 0;
		}
		return y_maior2 - y_menor1;
	}
	return 0;
}

#endif
