#ifndef __OBJECT_H_
#define __OBJECT_H__

#include <vector>

class Object
{
	private:
	vector<Sprite*> *animation;
	unsigned short int index;
	unsigned short int last;

	public:
	Object(void);
	Object(Sprite** animation, unsigned short int quant_sprite);
	virtual ~Object(void); //o compilador alerta que a classe possui funcoes virtuais, por isso o destrutor deve ser virtual
	inline unsigned short int getIndex(void) const { return index; };
	inline unsigned short int getSize(void) const { return last; };
	void setIndex(unsigned short int index);
	inline Sprite* getAnimation(void) const { return animation->at(index); }; //sem retornar ponteiro pode dar problema.
	inline Sprite* getSprite(void) const { return animation->at(index); };
	void addSprite(int id, int max, int gen_delay);
	void addSprite(Sprite* sprite);
	void setPosition(float pos_x, float pos_y);
	virtual void act(void);
};

//recebe um vetor de ponteiros de sprites para o objeto
Object::Object(Sprite** animation, unsigned short int quant_sprite)
{
	Object();
	for(unsigned short int i = 0; i< quant_sprite; i++)
	{
		(this->animation)->push_back(animation[i]);
	}
}

//cria um object sem sprites
Object::Object(void)
{
	animation = new vector<Sprite*>;
	last = 0;
}

//destroi o object!
Object::~Object(void)
{
	for(unsigned short int i = 0; i < animation->size(); i++)
	{
		delete animation->at(i);
	}
	delete animation;
}

//muda a animacao do object, mantendo a posicao
//algo aqui esta errado
void Object::setIndex(unsigned short int index)
{
	int pos_x, pos_y;

	cout << "%" << endl;
	pos_x = getAnimation()->GetX();
	pos_y = getAnimation()->GetY();
	cout << "$" << endl;
	this->index = index;
	getAnimation()->SetPosition(pos_x, pos_y);
	if(!(getAnimation()->GetReversed()))
	{
		getAnimation()->SetFrameNum(0);
	}
	else
	{
		getAnimation()->SetFrameNum(getAnimation()->GetFrameMax());
	}
}

//adiciona um novo sprite ao fim do vector, criado a partir dos parametros do construtor de sprite
void Object::addSprite(int id, int max, int gen_delay = 0)
{
	animation->push_back(new Sprite(id, max, gen_delay));
	last++;
}

//mesmo que o anterior, mas recebe um sprite ja criado
void Object::addSprite(Sprite* sprite)
{
	animation->push_back(sprite);
	last++;
}

//metodo que deve ser chamado para animar o object
void Object::act(void)
{
	getAnimation()->Animate();
}

void Object::setPosition(float pos_x, float pos_y)
{
	getAnimation()->SetPosition(pos_x, pos_y);
}

#endif

