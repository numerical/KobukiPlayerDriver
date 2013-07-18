#include "kobuki.h"

Kobuki::Kobuki(ConfigFile *cf, int section)
    : Driver(cf, section, true, PLAYER_MSGQUEUE_DEFAULT_MAXLEN) {


    parameters.device_port = cf->readString(section,
                                            "device", "/dev/ttyUSB0");
    parameters.enable_acceleration_limiter=false;

    memset(&position_addr, 0, sizeof(position_addr));

    if(cf->readDeviceAddr(&position_addr, section, "provides",
                          PLAYER_POSITION2D_CODE, -1, NULL) == 0) {
        if(this->AddInterface(position_addr) != 0) {
            this->SetError(-1);
            return;
        }
    }

}

Kobuki::~Kobuki() {
}

int Kobuki::Setup() {

    try {
        kobuki.init(parameters);
    } catch (ecl::StandardException &e) {
        fprintf(stderr, "%s\n", e.what());
    }

    StartThread();
    return 0;
}

int Kobuki::Shutdown() {
    kobuki.setBaseControl(0, 0); // Stop robot
    kobuki.disable();
    StopThread();
    return 0;
}

void Kobuki::Main() {
    for(;;) {
        pthread_testcancel();

        if(InQueue->Empty() == false) {
            ProcessMessages();
        }
    }
}

int Kobuki::Subscribe(player_devaddr_t id) {
    return Driver::Subscribe(id);
}

int Kobuki::Unsubscribe(player_devaddr_t id) {
    return Driver::Unsubscribe(id);
}

int Kobuki::ProcessMessage(MessageQueue *queue, player_msghdr *hdr,
                           void* msg_data) {
    // TODO parse messages, for now just return an error
    PLAYER_WARN("Messages not implemented yet");
    return -1;

}

void Kobuki::PutData() {
    Publish(position_addr, NULL, PLAYER_MSGTYPE_DATA,
            PLAYER_POSITION2D_DATA, (void*)data, sizeof(data));
}

Driver* Kobuki_Init(ConfigFile* cf, int section) {
    return (Driver*)(new Kobuki(cf, section));
}

void Kobuki_Register(DriverTable* table) {
    table->AddDriver("kobuki", Kobuki_Init);
}

extern "C" {
    int player_driver_init(DriverTable *table) {
        PLAYER_MSG0(1, "Registering Kobuki driver");
        Kobuki_Register(table);
        return 0;
    }
}
