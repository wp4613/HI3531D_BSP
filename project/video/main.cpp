#include <iostream>
#include<unistd.h>
#include <CXStreamer/factory.h>
#include <playercxs.h>
#include <QCoreApplication>
int main(int argc,  char *argv[])
{
    QCoreApplication a(argc, argv);
    std::cout << "Hello World!" << std::endl;

    argc = argc;
    argv = argv;
    long chn1 = 0;
    long chn2 = 1;
    uint16_t port1 = 8081,port2 = 8082;
    if(argc >= 2)
        chn1 = strtol( argv[1],NULL,0);
    if(argc >= 3)
        chn2 = strtol( argv[2],NULL,0);

    if(argc >= 4)
        port1 = strtol( argv[3],NULL,0);

    if(argc >= 5)
        port2 = strtol( argv[4],NULL,0);

    Player * player = PlayerCxs::getInstance(chn1,chn2,port1,port2);
    if(player != nullptr)
        player->start();



    return a.exec();
}


