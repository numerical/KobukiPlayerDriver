#ifndef KOBUKI_H
#define KOBUKI_H

#include <libplayercore/playercore.h>

class Kobuki : public Driver {
    private:

    public:
        Kobuki(ConfigFile*, int);
        ~Kobuki();

        virtual void Main();
        virtual void Setup();
        virtual void Shutdown();

        virtual int Subscribe(player_devaddr_t);
        virtual int Unsubscribe(player_devaddr_t);
        virtual int ProcessMessage(MessageQueue, player_msghdr, void*)
};
#endif
