#include "../../rbx_engine.h"
#include "../../../driver/driver_impl.hpp"
#include "../../offsets/offsets.hpp"
void RBX::Instance::SetIntValue(int value)
{
    write<int>(this->address + Offsets::Value, value);
}
int RBX::Instance::getIntFromValue() const
{
    return    read<int>(this->address + Offsets::Value);
}