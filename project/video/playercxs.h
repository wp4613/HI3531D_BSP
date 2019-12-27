#ifndef PLAYERCXS_H
#define PLAYERCXS_H
#include "player.h"
#include <CXStreamer/factory.h>
class PlayerCxs : public Player
{
    CXS::Element* mElemHead1;
    CXS::Element* mElemHead2;
    PlayerCxs();
public:
    static PlayerCxs* getInstance(int chn1,int chn2,uint16_t port1,uint16_t port2);

    int start();
};

#endif // PLAYERCXS_H
