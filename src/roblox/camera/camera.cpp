#include "../rbx_engine.h"
#include "../../Graphics/overlay/overlay.hpp"




#include "../../driver/driver_impl.hpp"
#include "../../misc/output/output.hpp"
#include "../offsets/Offsets.hpp"
#include "../../misc/globals/globals.hpp"




#include <windows.h> // For Windows API functions like OpenProcess, GetModuleHandleEx, etc.
#include <TlHelp32.h> // For process and module enumeration (e.g., PROCESSENTRY32, MODULEENTRY32)
#include <string> // For working with strings
#include <iostream> // For debugging with std::cout (optional)
#include "../../misc/output/output.hpp"
#include "../rbx_engine.h"

#include <windows.h>
#include <iostream>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <filesystem>
#include <fstream>
#include <wininet.h>
#include <string>
#include <ShlObj.h>
#include "../../misc/log_system/log_system.h"
RBX::Instance RBX::Instance::GetCameraInstance()
{
    return read<RBX::Instance>(this->address + Offsets::Camera);
}

RBX::Vector3 RBX::Instance::GetCameraPosition()
{
    return read<RBX::Vector3>(this->address + Offsets::CameraPos);
}
RBX::Matrix3x3 RBX::Instance::GetCameraRotation()
{
    return read<RBX::Matrix3x3>(this->address + Offsets::CameraRotation);
}

float RBX::Instance::GetFov()
{
    return read<float>(this->address + Offsets::CameraFieldOfView);
}

void RBX::Instance::SetCameraRotation(RBX::Matrix3x3 Rotation)
{
    write<RBX::Matrix3x3>(this->address + Offsets::CameraRotation, Rotation);
}

RBX::Instance RBX::Instance::Spectate(RBX::Instance stringhere) {
    RBX::Instance placeholder;

    // Add validation checks to prevent crashes
    RBX::Instance workspace = globals::game.FindFirstChild("Workspace", false, {});
    if (!workspace.address) {
        utils::output::error("Failed to find Workspace");
        return placeholder;
    }

    RBX::Instance camera = workspace.FindFirstChild("Camera", false, {});
    if (!camera.address) {
        utils::output::error("Failed to find Camera");
        return placeholder;
    }

    // Validate target instance
    if (!stringhere.address) {
        utils::output::error("Invalid spectate target");
        return placeholder;
    }

    // Log addresses for debugging
    std::string logMsg = "Spectating - Camera address: " + std::to_string(camera.address) + ", Target address: " + std::to_string(stringhere.address);
    RBX::Log_System::Info(logMsg);

    try {
        // Use CameraSubject offset instead of Value offset for spectating
        RBX::Log_System::Info("Attempting to write camera subject...");
        write<std::uint64_t>(camera.address + Offsets::CameraSubject, stringhere.address);
        RBX::Log_System::Info("Camera subject write completed");
    }
    catch (...) {
        RBX::Log_System::Error("Exception occurred during spectate memory write operation");
        utils::output::error("Exception occurred during spectate operation");
    }

    return placeholder;
}

RBX::Instance RBX::Instance::UnSpectate() {
    RBX::Instance placeholder;

    // Add validation checks to prevent crashes
    RBX::Instance workspace = globals::game.FindFirstChild("Workspace", false, {});
    if (!workspace.address) {
        utils::output::error("Failed to find Workspace");
        return placeholder;
    }

    RBX::Instance camera = workspace.FindFirstChild("Camera", false, {});
    if (!camera.address) {
        utils::output::error("Failed to find Camera");
        return placeholder;
    }

    RBX::Instance localPlayer = globals::players.GetLocalPlayer();
    if (!localPlayer.address) {
        utils::output::error("Failed to find LocalPlayer");
        return placeholder;
    }

    RBX::Instance modelInstance = localPlayer.GetModelInstance();
    if (!modelInstance.address) {
        utils::output::error("Failed to find ModelInstance");
        return placeholder;
    }

    RBX::Instance humanoid = modelInstance.FindFirstChild("Humanoid", false, {});
    if (!humanoid.address) {
        utils::output::error("Failed to find Humanoid");
        return placeholder;
    }

    // Log addresses for debugging
    std::string logMsg = "UnSpectating - Camera address: " + std::to_string(camera.address) + ", Humanoid address: " + std::to_string(humanoid.address);
    RBX::Log_System::Info(logMsg);

    try {
        // Use CameraSubject offset instead of Value offset for spectating
        RBX::Log_System::Info("Attempting to write camera subject for unspectate...");
        write<std::uint64_t>(camera.address + Offsets::CameraSubject, humanoid.address);
        RBX::Log_System::Info("Camera subject write for unspectate completed");
    }
    catch (...) {
        RBX::Log_System::Error("Exception occurred during unspectate memory write operation");
        utils::output::error("Exception occurred during unspectate operation");
    }

    return placeholder;
}
//"CameraMaxZoomDistance": "0x240",
//""CameraMinZoomDistance" : "0x244",
//""CameraMode" : "0x248"
void RBX::Instance::SetCameraMaxZoom(int zoom) {
    write<int>(globals::camera.address + 0x240, zoom);
}
void RBX::Instance::SetCameraMinZoom(int zoom) {
    write<int>(globals::camera.address + 0x244, zoom);
}