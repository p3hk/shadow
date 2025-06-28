#include "../../rbx_engine.h"
#include "../../../driver/driver_impl.hpp"
#include "../../offsets/offsets.hpp"
void RBX::Instance::SetStringValue(const std::string& value)
{
    write<std::string>(this->address + Offsets::Value, value);
}
//
std::string RBX::Instance::getStringFromValue() const
{
    return read<std::string>(this->address + Offsets::Value);
}
// doesnt work ill add the one from gamesneeze:
/*
   template <typename T, typename = std::enable_if_t<std::is_same_v<T, std::string>>>
    void SetStringValue(const T& value) {
        constexpr uintptr_t offset = Offsets::Value;
        static_assert(sizeof(T) == sizeof(std::string), "Type mismatch in memory write.");
        write<std::remove_cv_t<T>>(address + offset, value);
    }

    [[nodiscard]] std::string getStringFromValue() const {
        constexpr uintptr_t offset = Offsets::Value;
        using ReturnType = std::invoke_result_t<decltype(&read<std::string>), uintptr_t>;
        static_assert(std::is_same_v<ReturnType, std::string>, "Return type must be std::string.");
        return read<ReturnType>(address + offset);
    }*/