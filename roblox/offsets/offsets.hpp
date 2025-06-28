#pragma once
#include <cstdint>
#include <Windows.h>

namespace Offsets
{
    constexpr signed short int DataModel = 0x120; // Updated to FakeDataModelPointer
    constexpr signed short int Game = 0x1b8; // Updated to GameLoaded
    constexpr signed short int JobId = 0x140; // No change
    constexpr signed short int Workspace = 0x180; // No change
    constexpr signed short int GameID = 0x198; // Updated to GameId

    constexpr signed short int Visual_Engine = 0x10; // No change
    constexpr signed short int Dimensions = 0x740; // No change
    constexpr signed short int View_Matrix = 0x4D0; // Updated to viewmatrix

    constexpr signed short int Size = 0x8; // Updated to ChildrenEnd
    constexpr signed short int Children = 0x80; // No change
    constexpr signed short int Parent = 0x50; // No change
    constexpr signed short int Lighting_FogEnd = 0x138; // No change
    constexpr signed short int Lighting_FogStart = 0x13C; // No change
    constexpr signed short int Lighting_AmbientColor = 0xD0; // Updated to TeamColor
    constexpr signed short int Lighting_OutdoorAmbient = 0x110; // Updated to OutdoorAmbient
    constexpr signed short int Lighting_Brightness = 0x128; // Updated to ClickDetectorMaxActivationDistance
    constexpr signed short int Local_Entity = 0x128; // Updated to LocalPlayer
    constexpr signed short int Model_Instance = 0x330; // Updated to ModelInstance
    constexpr signed short int Humanoid_RigType = 0x1D0; // Updated to RigType
    inline constexpr uintptr_t MoveDirection = 0x160; // No change
    inline constexpr uintptr_t MeshPartColor3 = 0x1A0; // No change
    inline constexpr uintptr_t MaterialType = 0x2F0; // No change
    constexpr signed short int CanCollide = 0x313; // No change
    constexpr signed short int Primary_Part = 0x230; // Updated to UserId
    constexpr signed short int Move_Direction = 0x160; // No change
    inline constexpr uintptr_t Transparency = 0xF8; // No change

    constexpr signed short int CFrame = 0x11C; // No change
    constexpr signed short int WalkSpeed1 = 0x1D8; // Updated to WalkSpeed
    inline constexpr uintptr_t WalkSpeedCheck = 0x3B0; // Updated to WalkSpeedCheck

    constexpr signed short int WalkSpeed2 = 0x3B0; // Updated to FramePositionX
    constexpr signed short int JumpPower1 = 0x1B8; // Updated to JumpPower
    constexpr signed short int HipHeight = 0x1A8; // No change
    constexpr signed short int HipBoxes = 0x1B8; // Updated to JumpPower

    constexpr signed short int ping = 0xD0; // No change

    constexpr signed short int Name = 0x78; // No change
    constexpr signed short int Class_Name = 0x18; // Updated to ClassDescriptor
    constexpr signed short int Team = 0x218; // No change
    constexpr signed short int Value = 0xD8; // No change

    constexpr signed short int MousePosition = 0xF4; // No change

    inline constexpr uintptr_t Camera = 0x3F8; // No change
    inline constexpr uintptr_t CameraMaxZoomDistance = 0x280; // No change
    inline constexpr uintptr_t CameraMinZoomDistance = 0x284; // No change
    inline constexpr uintptr_t CameraMode = 0x288; // No change
    inline constexpr uintptr_t CameraPos = 0x124; // No change
    inline constexpr uintptr_t CameraRotation = 0x100; // No change
    inline constexpr uintptr_t CameraSubject = 0xF0; // No change
    inline constexpr uintptr_t CameraType = 0x160; // No change
    inline constexpr uintptr_t CameraFOV = 0x168; // Updated to FOV

    constexpr uintptr_t CameraFieldOfView = 0x140; // Updated to GetGlobalState

    constexpr signed short int Part_Primitive = 0x178; // Updated to Primitive
    constexpr signed short int Part_Position = 0x140; // No change
    constexpr signed short int Part_Velocity = 0x14C; // No change
    constexpr signed short int Part_Rotation = 0x11c; // No change
    constexpr signed short int Part_Size = 0x2B0; // Updated to PartSize

    constexpr signed short int Health = 0x19C; // No change
    constexpr signed short int Max_Health = 0x1BC; // Updated to MaxHealth

    namespace World {
        constexpr signed short int Ambience = 0xC8; // No equivalent found, kept the same
        constexpr signed short int Brightness = 0x128; // Updated to DisplayName
    }
}