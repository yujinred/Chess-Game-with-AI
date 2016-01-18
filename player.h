#ifndef _PLAYER_H_
#define _PLAYER_H_

class Player
{
protected:
	char colour; //'b' or 'w' for black or white
public:
	Player(char);
	virtual ~Player();
};

#endif
