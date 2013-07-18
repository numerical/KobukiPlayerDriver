#ifndef KOBUKI_H
#define KOBUKI_H

#include <cstdio>
#include <libplayercore/playercore.h>
#include <kobuki_driver/kobuki.hpp>

class Kobuki : public Driver {
    private:
        position2d_id position_addr;
        player_position2d_data data;

        kobuki::Parameters parameters;
        kobuki::Kobuki kobuki;

    public:
        Kobuki(ConfigFile*, int);
        ~Kobuki();

        virtual void Main();
        virtual int Setup();
        virtual int Shutdown();

        virtual int Subscribe(player_devaddr_t);
        virtual int Unsubscribe(player_devaddr_t);
        virtual int ProcessMessage(MessageQueue*, player_msghdr*, void*);

        virtual void PutData();
};
#endif
