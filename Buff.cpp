#include "Buff.h"

bool Buff::IsAlive(float time) {
    return startTime < time && time < endTime;
}

BYTE Buff::getType() {

}
