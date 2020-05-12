#include <Arduino.h>
#include <x10.h>
#include <x10constants.h>


class x10_MySupport
{
    private:
        x10 myHouse;

    public:
        void init(int zcPin, int dataPin, int dataRecievePin);
        void unit_on(int unit_id, int repeatTimes);
        void unit_off(int unit_id, int repeatTimes);
        void unit_dim(int unit_id, int times, int repeatTimes);
        void unit_bright(int unit_id, int times, int repeatTimes);
        void getStatus(int repeatTimes);
        void getHAIL(int repeatTimes);
        void reset_all_lights(int repeatTimes);

        bool received();
        void reset();
        void process();
};
