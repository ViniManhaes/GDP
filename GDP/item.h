#ifndef __ITEM_H__
#define __ITEM_H__

using namespace std;

class Item: public Object
{
	protected:
		string name;
 
	public:
		Item(void);
		Item(Sprite **animation, unsigned short int quant_sprite);
		Item(Sprite **animation, unsigned short int quant_sprite, string name);
		virtual void Effect(Mobile target) = 0; //efeito do item
		virtual string Get(Mobile target) = 0; //deve retornar o nome do item
		~Item(void);//trabalhar esse aqui
};

Item::Item(void)
{
	name = "anonimo";
}

Item::Item(Sprite **animation, unsigned short int quant_sprite):Object(animation, quant_sprite)
{
	name = "anonimo";
}

Item::Item(Sprite **animation, unsigned short int quant_sprite, string name): Object(animation, quant_sprite)
{
	this->name = name;
}


#endif
