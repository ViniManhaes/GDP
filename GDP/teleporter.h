#ifndef __PHISIC_H__
#define __PHISIC_H__

#define OPEN	0
#define CHANGE	1
#define CLOSED	2

/*
O Teleporter precisa ter exatamente tres sprites conforme o esquema acima, a menos que nao seja usada a mudanca (metodo change())
O segundo sprite (CHANGE) precisa estar organizado para que em ordem direta (partindo do frame 0) a porta esteja abrindo, e em ordem inversa a porta esteja fechando
Se nao houver animacao de abertura para a porta, deve ser criada uma, mesmo que seja apenas um frame
*/
class Teleporter : public Object
{
	private:
		bool open;

	public:
		Teleporter(void);
		Teleporter(Sprite** animation, unsigned short int quant_sprite, bool open);
		virtual void teleport(void);
		void change(void);
		virtual void act(void);
};

//construtor padrao, faltam os sprites antes de poder ser usado
Teleporter::Teleporter(void)
{
	open = true;
}

//constroi com os sprites
Teleporter::Teleporter(Sprite** animation, unsigned short int quant_sprite, bool open = true) : Object(animation, quant_sprite)
{
	this->open = open;
}

//muda o personagem de lugar, precisa de mais informacao antes de poder ser terminado
void Teleporter::teleport(void)
{
	//trabalho em progresso
}

//muda o estado do teleporter (aberto ou fechado)
void Teleporter::change(void)
{
	if (open)
	{
		getAnimation()->SwitchReversed();
		open = false;
	}
	
	else 
	{
		open = true;
	}
	setIndex(CHANGE);
}

//anima o teleportador, fazendo a animacao de transicao
void Teleporter::act(void)
{
	getAnimation()->Animate();

	if(getIndex() == CHANGE)
	{
		if(open)
		{
			if(getAnimation()->GetFrameNum() >= getAnimation()->GetFrameMax())
			{
				setIndex(OPEN);
			}
		}
		else
		{
			if(getAnimation()->GetFrameNum() == 0) 
			{
				getAnimation()->SwitchReversed();
				setIndex(CLOSED);
			}
		}
	}
}

#endif
