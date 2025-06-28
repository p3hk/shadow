#define IMGUI_DEFINE_MATH_OPERATORS

#include "overlay.hpp"
#include "imgui/TextEditor.h"
#include <dwmapi.h>
#include "../../misc/output/output.hpp"
#include "../../cheats/features.h"

#include "../../misc/globals/globals.hpp"
#include "../../misc/configs/configs.hpp"
#include "../../library/json/json.hpp"

#include "ckey/keybind.hpp"
#include "../overlay/XorStr/xorstr.hpp"
#include "../overlay/XorStr/json.hpp"

#include <filesystem>
#include <thread>
#include <bitset>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <unordered_set>
#include "../../cheats/esp/fonts.h"
#ifdef min
#undef min
#endif
#include <stack>

#ifdef max
#undef max
#endif
#include <Psapi.h>
#include "imgui/backends/imgui_impl_win32.h"
#include "imgui/backends/imgui_impl_dx11.h"
#include "imgui/misc/freetype/imgui_freetype.h"
#include "imgui/addons/imgui_addons.h"
#include "fonts/IconsFontAwesome6.h"
#include "fonts/roboto_medium.h"

template<typename T>
T custom_clamp(T value, T min, T max) {
	return value < min ? min : (value > max ? max : value);
}
#pragma once

std::string GetConfigFolderPath()
{
	std::string configFolderPath = RBX::appdata_path() + XorStr("\\charm\\configs");

	if (!std::filesystem::exists(configFolderPath))
	{
		std::filesystem::create_directory(configFolderPath);
	}

	return configFolderPath;
}

ID3D11Device* overlay::d3d11Device = nullptr;
ID3D11DeviceContext* overlay::d3d11DeviceContext = nullptr;
IDXGISwapChain* overlay::dxgiSwapChain = nullptr;
ID3D11RenderTargetView* overlay::d3d11RenderTargetView = nullptr;

// CGui-style member variables
int overlay::m_iCurrentPage = 0;
std::vector<const char*> overlay::m_Tabs;

enum ImFonts_ : int
{
	ImFont_Main = 0,
	ImFont_Icons
};

static bool                     g_ShowMenu = false;
static float                    g_menu_alpha = 0.0f;
static const float              g_fade_speed = 6.5f;
std::vector<std::string> outputLines;
bool overlay::full_screen(HWND windowHandle)
{
	MONITORINFO monitorInfo = { sizeof(MONITORINFO) };
	if (GetMonitorInfo(MonitorFromWindow(windowHandle, MONITOR_DEFAULTTOPRIMARY), &monitorInfo))
	{
		RECT windowRect;
		if (GetWindowRect(windowHandle, &windowRect))
		{
			return windowRect.left == monitorInfo.rcMonitor.left
				&& windowRect.right == monitorInfo.rcMonitor.right
				&& windowRect.top == monitorInfo.rcMonitor.top
				&& windowRect.bottom == monitorInfo.rcMonitor.bottom;
		}
	}
}

void SetPropertiesPanelStyle() {

	ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.2f, 0.4f, 0.6f, 0.8f));
	ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.3f, 0.5f, 0.7f, 0.9f));
	ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.4f, 0.6f, 0.8f, 1.0f));

	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.9f, 0.9f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TextDisabled, ImVec4(0.6f, 0.6f, 0.6f, 1.0f));

	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.1f, 0.1f, 0.1f, 0.7f));

	ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
}

void ResetPropertiesPanelStyle() {
	ImGui::PopStyleColor(6);
	ImGui::PopFont();
}
bool WatermarkNshit = false;
#include "../../library/LuaSys/lua.hpp"
#include <string>
TextEditor edi;
lua_State* L = nullptr;
bool dislocatecam = false;
void ExecuteLuaCode(const std::string& lua_code) {
	if (L == nullptr) {

		L = luaL_newstate();
		luaL_openlibs(L);
	}

	if (luaL_dostring(L, lua_code.c_str()) != LUA_OK) {

		const char* error_msg = lua_tostring(L, -1);
		ImGui::Text("Lua Error: %s", error_msg);
		lua_pop(L, 1);
	}
}
bool keybindlistshitt = false;
bool targetlist = false;

void colorslol() {
	ImGuiStyle& style = ImGui::GetStyle();


	style.Colors[ImGuiCol_WindowBg] = ImAdd::HexToColorVec4(0x2F2F2F, 1.00f);
	style.Colors[ImGuiCol_ChildBg] = ImAdd::HexToColorVec4(0x242424, 1.00f);
	style.Colors[ImGuiCol_PopupBg] = ImAdd::HexToColorVec4(0x242424, 1.00f);
	style.Colors[ImGuiCol_Border] = ImAdd::HexToColorVec4(0xFFFFFF, 0.50f);
	style.Colors[ImGuiCol_BorderShadow] = ImAdd::HexToColorVec4(0x000000, 0.00f);


	style.Colors[ImGuiCol_Text] = ImAdd::HexToColorVec4(0xFFFFFF, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImAdd::HexToColorVec4(0xAAAAAA, 1.00f);


	style.Colors[ImGuiCol_Header] = ImAdd::HexToColorVec4(0x404040, 1.00f);
	style.Colors[ImGuiCol_HeaderHovered] = ImAdd::HexToColorVec4(0x505050, 1.00f);
	style.Colors[ImGuiCol_HeaderActive] = ImAdd::HexToColorVec4(0x606060, 1.00f);


	style.Colors[ImGuiCol_Button] = ImAdd::HexToColorVec4(0x404040, 1.00f);
	style.Colors[ImGuiCol_ButtonHovered] = ImAdd::HexToColorVec4(0x505050, 1.00f);
	style.Colors[ImGuiCol_ButtonActive] = ImAdd::HexToColorVec4(0x606060, 1.00f);


	style.Colors[ImGuiCol_FrameBg] = ImAdd::HexToColorVec4(0x333333, 1.00f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImAdd::HexToColorVec4(0x444444, 1.00f);
	style.Colors[ImGuiCol_FrameBgActive] = ImAdd::HexToColorVec4(0x555555, 1.00f);


	style.Colors[ImGuiCol_SliderGrab] = ImAdd::HexToColorVec4(0xCCCCCC, 1.00f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImAdd::HexToColorVec4(0xFFFFFF, 1.00f);


	style.FrameBorderSize = 1.0f;
	style.FrameRounding = 4.0f;
	style.WindowRounding = 4.0f;
	style.ChildRounding = 4.0f;
	style.PopupRounding = 4.0f;
	style.ScrollbarRounding = 4.0f;
	style.GrabRounding = 4.0f;
	style.TabRounding = 4.0f;

	ImGui::SetNextWindowSize(ImVec2(176, 342), ImGuiCond_Once);

	ImGui::PushStyleColor(ImGuiCol_WindowShadow, style.Colors[ImGuiCol_SliderGrab]);
	ImGui::Begin("Colors", nullptr, ImGuiWindowFlags_NoTitleBar);
	ImGui::PopStyleColor();
	ImGui::PopStyleVar(2);
	ImGui::PushItemWidth(ImGui::GetWindowWidth() - 50);
	ImAdd::BeginChild("Colors", ImVec2(0, 0));
	{
		ImAdd::CheckBox("WaterMark", &WatermarkNshit);
		ImAdd::CheckBox("Keybinds", &keybindlistshitt);
		ImAdd::CheckBox("Vsync", &globals::vsync);
		const float width = ImGui::GetContentRegionAvail().x;
		ImAdd::SliderInt("Menu Glow Opacity", &globals::menu_glow, 0, 500);

		ImGui::Text("Window Background");
		ImAdd::ColorEdit4("WindowBg", (float*)&style.Colors[ImGuiCol_WindowBg]);
		ImGui::Text("Child Background");
		ImAdd::ColorEdit4("ChildBg", (float*)&style.Colors[ImGuiCol_ChildBg]);
		ImGui::Text("Popup Background");
		ImAdd::ColorEdit4("PopupBg", (float*)&style.Colors[ImGuiCol_PopupBg]);

		ImGui::Text("Text Color");
		ImAdd::ColorEdit4("Text", (float*)&style.Colors[ImGuiCol_Text]);
		ImGui::Text("Text Disabled Color");
		ImAdd::ColorEdit4("TextDisabled", (float*)&style.Colors[ImGuiCol_TextDisabled]);
		ImGui::Text("Title Background Color");
		ImAdd::ColorEdit4("TitleBg", (float*)&style.Colors[ImGuiCol_TitleBg]);

		ImGui::Text("Slider Grab Color");
		ImAdd::ColorEdit4("SliderGrab", (float*)&style.Colors[ImGuiCol_SliderGrab]);
		ImGui::Text("Slider Grab Active Color");
		ImAdd::ColorEdit4("SliderGrabActive", (float*)&style.Colors[ImGuiCol_SliderGrabActive]);

		ImGui::Text("Border Color");
		ImAdd::ColorEdit4("Border", (float*)&style.Colors[ImGuiCol_Border]);
		ImGui::Text("Separator Color");
		ImAdd::ColorEdit4("Separator", (float*)&style.Colors[ImGuiCol_Separator]);

		ImGui::Text("Button Color");
		ImAdd::ColorEdit4("Button", (float*)&style.Colors[ImGuiCol_Button]);
		ImGui::Text("Button Hovered Color");
		ImAdd::ColorEdit4("ButtonHovered", (float*)&style.Colors[ImGuiCol_ButtonHovered]);
		ImGui::Text("Button Active Color");
		ImAdd::ColorEdit4("ButtonActive", (float*)&style.Colors[ImGuiCol_ButtonActive]);

		ImGui::Text("Frame Background Color");
		ImAdd::ColorEdit4("FrameBg", (float*)&style.Colors[ImGuiCol_FrameBg]);
		ImGui::Text("Frame Background Hovered Color");
		ImAdd::ColorEdit4("FrameBgHovered", (float*)&style.Colors[ImGuiCol_FrameBgHovered]);
		ImGui::Text("Frame Background Active Color");
		ImAdd::ColorEdit4("FrameBgActive", (float*)&style.Colors[ImGuiCol_FrameBgActive]);

		ImGui::Text("Header Color");
		ImAdd::ColorEdit4("Header", (float*)&style.Colors[ImGuiCol_Header]);
		ImGui::Text("Header Hovered Color");
		ImAdd::ColorEdit4("HeaderHovered", (float*)&style.Colors[ImGuiCol_HeaderHovered]);
		ImGui::Text("Header Active Color");
		ImAdd::ColorEdit4("HeaderActive", (float*)&style.Colors[ImGuiCol_HeaderActive]);

		ImGui::Text("Scrollbar Background");
		ImAdd::ColorEdit4("ScrollbarBg", (float*)&style.Colors[ImGuiCol_ScrollbarBg]);
		ImGui::Text("Scrollbar Grab Color");
		ImAdd::ColorEdit4("ScrollbarGrab", (float*)&style.Colors[ImGuiCol_ScrollbarGrab]);
		ImGui::Text("Scrollbar Grab Hovered Color");
		ImAdd::ColorEdit4("ScrollbarGrabHovered", (float*)&style.Colors[ImGuiCol_ScrollbarGrabHovered]);
		ImGui::Text("Scrollbar Grab Active Color");
		ImAdd::ColorEdit4("ScrollbarGrabActive", (float*)&style.Colors[ImGuiCol_ScrollbarGrabActive]);

		ImGui::Text("Text Selected Background Color");
		ImAdd::ColorEdit4("TextSelectedBg", (float*)&style.Colors[ImGuiCol_TextSelectedBg]);
		ImGui::Text("Window Shadow Color");
		ImAdd::ColorEdit4("WindowShadow", (float*)&style.Colors[ImGuiCol_WindowShadow]);

		ImGui::Text("Resize Grip Color");
		ImAdd::ColorEdit4("ResizeGrip", (float*)&style.Colors[ImGuiCol_ResizeGrip]);
		ImGui::Text("Resize Grip Active Color");
		ImAdd::ColorEdit4("ResizeGripActive", (float*)&style.Colors[ImGuiCol_ResizeGripActive]);
		ImGui::Text("Resize Grip Hovered Color");
		ImAdd::ColorEdit4("ResizeGripHovered", (float*)&style.Colors[ImGuiCol_ResizeGripHovered]);
	}
	ImAdd::EndChild();
	ImGui::EndChild();

}
void RenderPropertiesPanel(RBX::Instance& selectedPart) {

}

bool link = false;
void RenderExplorer() {
	static RBX::Instance selectedPart;
	static std::unordered_set<uint64_t> spectatedParts;
	static std::unordered_map<uint64_t, std::vector<RBX::Instance>> nodeCache;
	static std::unordered_map<uint64_t, bool> nodeExpandedCache;
	static std::unordered_map<uint64_t, std::string> nodeNameCache;
	static std::unordered_map<uint64_t, std::string> nodeClassNameCache;
	static std::unordered_map<uint64_t, std::string> nodeTeamCache;
	static std::unordered_map<uint64_t, std::string> nodeUniqueIdCache;
	static std::unordered_map<uint64_t, std::string> nodeModel;
	static bool isCacheInitialized = false;
	static auto lastCacheRefresh = std::chrono::steady_clock::now();

	static std::unordered_map<uint64_t, std::string> nodePath;

	auto cacheNode = [&](RBX::Instance& node) {
		if (nodeCache.find(node.address) == nodeCache.end()) {
			nodeCache[node.address] = node.GetChildren(false);
			nodeNameCache[node.address] = node.GetName();
			nodeTeamCache[node.address] = node.GetTeam().GetName();
			nodeClassNameCache[node.address] = node.GetClass();
			nodeModel[node.address] = node.GetModelInstance().GetName();
			nodeUniqueIdCache[node.address] = std::to_string(node.address);

			std::string path = node.GetName();
			RBX::Instance parent = node.GetParent();
			while (parent.address != 0) {
				if (nodeCache.find(parent.address) == nodeCache.end()) {
					nodeCache[parent.address] = parent.GetChildren(false);
					nodeNameCache[parent.address] = parent.GetName();
				}
				std::string parentName = nodeNameCache[parent.address];
				if (!parentName.empty()) {
					path = parentName + "." + path;
				}
				parent = parent.GetParent();
			}
			nodePath[node.address] = path;
		}
		};

	try {
		auto& datamodel = globals::game;
		RBX::Instance root_instance(datamodel.address);

		auto now = std::chrono::steady_clock::now();
		if (std::chrono::duration_cast<std::chrono::seconds>(now - lastCacheRefresh).count() >= 2) {
			nodeCache.clear();
			nodeNameCache.clear();
			nodeClassNameCache.clear();
			nodeUniqueIdCache.clear();
			isCacheInitialized = false;
			lastCacheRefresh = now;
		}

		if (!isCacheInitialized) {
			cacheNode(root_instance);
			isCacheInitialized = true;
		}

		ImGuiStyle& style = ImGui::GetStyle();
		ImGui::PushStyleColor(ImGuiCol_Border, style.Colors[ImGuiCol_SliderGrab]);
		ImGui::PushStyleColor(ImGuiCol_WindowShadow, style.Colors[ImGuiCol_SliderGrab]);

		ImGui::Begin("Explorer", nullptr, ImGuiWindowFlags_NoTitleBar);
		ImGui::PopStyleColor(2);


		ImAdd::BeginChild("Explorer", ImVec2(0, ImGui::GetContentRegionAvail().y - 205));


		for (auto& child : nodeCache[root_instance.address]) {
			std::stack<std::pair<RBX::Instance, int>> stack;
			stack.push({ child, 0 });

			while (!stack.empty()) {
				auto [node, indentLevel] = stack.top();
				stack.pop();

				cacheNode(node);

				ImGui::SetCursorPosX(20.0f * indentLevel);
				ImGui::PushID(nodeUniqueIdCache[node.address].c_str());

				const std::vector<RBX::Instance>& children = nodeCache[node.address];
				bool hasChildren = !children.empty();

				std::string displayText;
				if (nodeClassNameCache[node.address] == "Workspace") {
				}
				else if (nodeClassNameCache[node.address] == "Players") {
				}
				else if (nodeClassNameCache[node.address] == "Folder") {
				}
				else if (nodeClassNameCache[node.address] == "Part" ||
					nodeClassNameCache[node.address] == "BasePart") {
				}
				else if (nodeClassNameCache[node.address].find("Script") != std::string::npos) {
				}
				else {
					displayText = "";
				}

				displayText += " " + nodeNameCache[node.address] + " [" + nodeClassNameCache[node.address] + "]";

				bool isExpanded = ImGui::TreeNodeEx(displayText.c_str(),
					hasChildren ? 0 : ImGuiTreeNodeFlags_Leaf);

				if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
					selectedPart = node;
				}

				if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
					ImGui::OpenPopup("NodeContextMenu");
				}

				if (ImGui::BeginPopup("NodeContextMenu")) {
					if (ImGui::MenuItem("Copy Hex Addy")) {
						ImGui::SetClipboardText(nodeUniqueIdCache[node.address].c_str());
					}

					if (ImGui::MenuItem("Teleport To Part")) {
						RBX::Vector3 partPosition = node.GetPosition();
						const float verticalOffset = 5.0f;
						partPosition.y += verticalOffset;
						globals::players.GetLocalPlayer().GetModelInstance().FindFirstChild("HumanoidRootPart", false, {}).SetPartPos(partPosition);
					}
					if (spectatedParts.count(node.address)) {
						if (ImGui::MenuItem("Unspectate Part")) {
							selectedPart.UnSpectate();
							spectatedParts.erase(node.address);
						}
					}
					else {
						if (ImGui::MenuItem("Spectate Part")) {
							selectedPart = node;
							selectedPart.Spectate(node);
							spectatedParts.insert(node.address);
						}
					}
					ImGui::EndPopup();
				}

				if (isExpanded) {
					for (auto it = children.rbegin(); it != children.rend(); ++it) {
						stack.push({ *it, indentLevel + 1 });
					}
					ImGui::TreePop();
				}

				ImGui::PopID();
			}
		}

		ImAdd::EndChild();

		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImAdd::HexToColorVec4(0x2a2a38, 1.00f));
		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImAdd::HexToColorVec4(0x24242f, 1.00f));
		ImGui::PushStyleColor(ImGuiCol_PopupBg, ImAdd::HexToColorVec4(0x24242f, 1.00f));
		ImGui::PushStyleColor(ImGuiCol_Text, ImAdd::HexToColorVec4(0xc8c8c8, 1.00f));

		//	ImGui::BeginChild("Properties", ImVec2(0, 200), true);
		ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "Properties");
		ImGui::Separator();

		if (selectedPart.address != 0) {
			if (ImGui::TreeNodeEx("Basic Information", ImGuiTreeNodeFlags_DefaultOpen)) {
				const std::string& nodeName = nodeNameCache[selectedPart.address];
				const std::string& className = nodeClassNameCache[selectedPart.address];
				const std::string& model = nodeModel[selectedPart.address];
				RBX::Instance parent = selectedPart.GetParent();
				std::string parentName = nodeNameCache[parent.address];


				ImGui::Text("Path:"); ImGui::SameLine(120);
				ImGui::TextWrapped("%s", nodePath[selectedPart.address].c_str());

				ImGui::Text("Name:"); ImGui::SameLine(120);
				ImGui::Text("%s", nodeName.c_str());

				ImGui::Text("Class:"); ImGui::SameLine(120);
				ImGui::Text("%s", className.c_str());

				ImGui::Text("Parent:"); ImGui::SameLine(120);
				ImGui::Text("%s", parentName.c_str());

				ImGui::Text("Model:"); ImGui::SameLine(120);
				ImGui::Text("%s", model.c_str());

				ImGui::TreePop();
			}

			if (ImGui::TreeNodeEx("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
				RBX::Vector3 position = selectedPart.GetPosition();
				ImGui::Text("Position:"); ImGui::SameLine(120);
				ImGui::Text("X: %.1f  Y: %.1f  Z: %.1f", position.x, position.y, position.z);

				RBX::Vector3 size = selectedPart.GetSize();
				ImGui::Text("Size:"); ImGui::SameLine(120);
				ImGui::Text("X: %.1f  Y: %.1f  Z: %.1f", size.x, size.y, size.z);

				ImGui::TreePop();
			}

			if (ImGui::TreeNodeEx("Appearance", ImGuiTreeNodeFlags_DefaultOpen)) {
				int color = selectedPart.GetColor3();
				ImGui::Text("Color:"); ImGui::SameLine(120);
				ImGui::Text("0x%X", color);

				ImGui::TreePop();
			}

			if (ImGui::TreeNodeEx("Part Properties", ImGuiTreeNodeFlags_DefaultOpen)) {
				static bool canCollide = false;
				if (ImGui::Checkbox("CanCollide", &canCollide)) {
					selectedPart.setCanCollide(canCollide);
				}

				ImGui::TreePop();
			}

			if (ImGui::TreeNodeEx("Instance Value", ImGuiTreeNodeFlags_DefaultOpen)) {
				const std::string& className = nodeClassNameCache[selectedPart.address];

				if (className == "BoolValue") {
					bool boolValue = selectedPart.getBoolFromValue();
					ImGui::Text("%s", boolValue ? "true" : "false");
				}
				else if (className == "IntValue") {
					int intValue = selectedPart.getIntFromValue();
					ImGui::Text("%d", intValue);
				}
				else if (className == "StringValue") {
					std::string stringValue = selectedPart.getStringFromValue();
					ImGui::Text("%s", stringValue.c_str());
				}
				else if (className == "NumberValue") {
					float floatValue = selectedPart.GetFloatFromValue();
					ImGui::Text("%.2f", floatValue);
				}
				else {
					ImGui::Text("No specific value");
				}

				ImGui::TreePop();
			}

			if (ImGui::TreeNodeEx("Network", ImGuiTreeNodeFlags_DefaultOpen)) {
				ImGui::Text("Address:"); ImGui::SameLine(120);
				ImGui::Text("0x%llX", selectedPart.address);

				ImGui::TreePop();
			}
		}

		ImAdd::EndChild();

		ImGui::PopStyleColor(4);


		ImAdd::EndChild();
	}
	catch (...) {
	}
}





static ImVec4 ui_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

bool overlay::init = false;

bool overlay::create_device_d3d(HWND hw) {
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferCount = 1;
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hw;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	D3D_FEATURE_LEVEL feature_levels[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0 };
	D3D_FEATURE_LEVEL selected_level;

	HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, D3D11_CREATE_DEVICE_DEBUG, feature_levels, _countof(feature_levels), D3D11_SDK_VERSION, &sd, &dxgiSwapChain, &d3d11Device, &selected_level, &d3d11DeviceContext);

	if (FAILED(hr)) {
		if (hr == DXGI_ERROR_UNSUPPORTED) {
			hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, 0, feature_levels, _countof(feature_levels), D3D11_SDK_VERSION, &sd, &dxgiSwapChain, &d3d11Device, &selected_level, &d3d11DeviceContext);
		}
	}

	create_render_target();
	return true;
}

void overlay::cleanup_device_d3d()
{
	cleanup_render_target();

	if (dxgiSwapChain)
	{
		dxgiSwapChain->Release();
		dxgiSwapChain = nullptr;
	}

	if (d3d11DeviceContext)
	{
		d3d11DeviceContext->Release();
		d3d11DeviceContext = nullptr;
	}

	if (d3d11Device)
	{
		d3d11Device->Release();
		d3d11Device = nullptr;
	}
}

void overlay::create_render_target()
{
	ID3D11Texture2D* back_buffer = nullptr;
	if (SUCCEEDED(dxgiSwapChain->GetBuffer(0, IID_PPV_ARGS(&back_buffer))))
	{
		d3d11Device->CreateRenderTargetView(back_buffer, nullptr, &d3d11RenderTargetView);
		back_buffer->Release();
	}
	else
	{
		std::cerr << "failed to get back buffer" << std::endl;
	}
}

void overlay::cleanup_render_target()
{
	if (d3d11RenderTargetView)
	{
		d3d11RenderTargetView->Release();
		d3d11RenderTargetView = nullptr;
	}
}

LRESULT __stdcall overlay::window_proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_SIZE:
		if (d3d11Device != nullptr && wParam != SIZE_MINIMIZED)
		{
			cleanup_render_target();
			dxgiSwapChain->ResizeBuffers(0, LOWORD(lParam), HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
			create_render_target();
		}
		return 0;

	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU)
			return 0;
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	default:
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

void overlay::move_window(HWND hw)
{
	static HWND target = NULL;
	static DWORD lastFindWindowTime = 0;
	DWORD currentTime = GetTickCount();

	// Cache the Roblox window but refresh frequently enough for ESP
	if (target == NULL || currentTime - lastFindWindowTime > 250) {
		target = FindWindowA(nullptr, "Roblox");
		lastFindWindowTime = currentTime;
	}

	// Don't cache foreground window since it's critical for ESP performance
	HWND foregroundWindow = GetForegroundWindow();

	if (target != foregroundWindow && hw != foregroundWindow)
	{
		RECT targetRect{};
		if (GetWindowRect(target, &targetRect))
		{
			UINT flags = SWP_NOACTIVATE | SWP_NOZORDER;
			if (full_screen(target))
				flags |= SWP_NOMOVE | SWP_NOSIZE;

			SetWindowPos(hw, nullptr, targetRect.left, targetRect.top, targetRect.right - targetRect.left, targetRect.bottom - targetRect.top, flags);
		}
	}
}

bool DrawMultiCombo(const char* label, const std::vector<std::string>& items, std::vector<bool>& selections)
{
	ImGuiContext& g = *GImGui;
	std::string preview;
	for (size_t i = 0; i < items.size(); ++i) {
		if (selections[i]) {
			if (!preview.empty()) {
				preview += ", ";
			}
			preview += items[i];
		}
	}
	if (preview.empty()) {
		preview = "Select items";
	}

	if (!ImAdd::BeginCombo(label, preview.c_str(), ImGuiComboFlags_None))
		return false;

	bool value_changed = false;
	for (size_t i = 0; i < items.size(); ++i)
	{
		ImGui::PushID(i);
		bool selected = selections[i];
		if (ImAdd::Selectable(items[i].c_str(), selected, ImVec2(-0.1f, 0)))
		{
			selections[i] = !selections[i];
			value_changed = true;
		}
		if (selections[i])
			ImGui::SetItemDefaultFocus();
		ImGui::PopID();
	}

	ImGui::EndCombo();

	if (value_changed)
		ImGui::MarkItemEdited(g.LastItemData.ID);

	return value_changed;
}

bool Keybind(CKeybind* keybind, const ImVec2& size_arg = ImVec2(0, 0), bool clicked = false, ImGuiButtonFlags flags = 0)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(keybind->get_name().c_str());
	const ImVec2 label_size = ImGui::CalcTextSize(keybind->get_name().c_str(), NULL, true);

	ImVec2 pos = window->DC.CursorPos;
	if ((flags & ImGuiButtonFlags_AlignTextBaseLine) &&
		style.FramePadding.y <
		window->DC.CurrLineTextBaseOffset)

		pos.y += window->DC.CurrLineTextBaseOffset - style.FramePadding.y;
	ImVec2 size = ImGui::CalcItemSize(
		size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

	const ImRect bb(pos, pos + size);
	ImGui::ItemSize(size, style.FramePadding.y);
	if (!ImGui::ItemAdd(bb, id))
		return false;

	if (g.CurrentItemFlags & ImGuiItemFlags_ButtonRepeat)
		flags |= ImGuiButtonFlags_Repeat;
	bool hovered, held;
	bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, flags);

	bool value_changed = false;
	int key = keybind->key;

	auto io = ImGui::GetIO();

	std::string name = keybind->get_key_name();

	if (keybind->waiting_for_input)
		name = "..";

	if (ImGui::GetIO().MouseClicked[0] && hovered)
	{
		if (g.ActiveId == id)
		{
			keybind->waiting_for_input = true;
		}
	}
	else if (ImGui::GetIO().MouseClicked[1] && hovered)
	{
		ImGui::OpenPopup(keybind->get_name().c_str());
	}
	else if (ImGui::GetIO().MouseClicked[0] && !hovered)
	{
		if (g.ActiveId == id)
			ImGui::ClearActiveID();
	}

	if (keybind->waiting_for_input)
	{
		if (ImGui::GetIO().MouseClicked[0] && !hovered)
		{
			keybind->key = VK_LBUTTON;

			ImGui::ClearActiveID();
			keybind->waiting_for_input = false;
		}
		else
		{
			if (keybind->set_key())
			{
				ImGui::ClearActiveID();
				keybind->waiting_for_input = false;
			}
		}
	}

	ImVec4 textcolor = ImLerp(ImVec4(201 / 255.f, 204 / 255.f, 210 / 255.f, 1.f), ImVec4(1.0f, 1.0f, 1.0f, 1.f), 1.f);

	window->DrawList->AddRectFilled(bb.Min, bb.Max, ImColor(33 / 255.0f, 33 / 255.0f, 33 / 255.0f, 1.f), 2.f);

	window->DrawList->AddText(
		bb.Min +
		ImVec2(
			size_arg.x / 2 - ImGui::CalcTextSize(name.c_str()).x / 2,
			size_arg.y / 2 - ImGui::CalcTextSize(name.c_str()).y / 2),
		ImGui::GetColorU32(ImGui::GetStyleColorVec4(ImGuiCol_Text)),
		name.c_str());

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_Popup |
		ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoScrollbar;
	ImGui::SetNextWindowPos(pos + ImVec2(0, size_arg.y - 1));
	ImGui::SetNextWindowSize(ImVec2(size_arg.x, 47 * 1.f));

	{
		if (ImGui::BeginPopup(keybind->get_name().c_str(), 0))
		{

			{
				{
					ImGui::BeginGroup();
					{
						if (ImGui::Selectable(XorStr("Toggle"), keybind->type == CKeybind::TOGGLE))
							keybind->type = CKeybind::TOGGLE;
						if (ImGui::Selectable(XorStr("Hold"), keybind->type == CKeybind::HOLD))
							keybind->type = CKeybind::HOLD;
					}
					ImGui::EndGroup();
				}
			}

			ImGui::EndPopup();
		}
	}

	return pressed;
}
void EnableBlurEffect(HWND hwnd) {

	DWM_BLURBEHIND bb = { 0 };
	bb.dwFlags = DWM_BB_ENABLE;
	bb.fEnable = TRUE;
	bb.hRgnBlur = NULL;
	bb.fTransitionOnMaximized = FALSE;

	DwmEnableBlurBehindWindow(hwnd, &bb);
}

void SetUpWindowForBlur(HWND hwnd) {

	SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED | WS_EX_TRANSPARENT);
	SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 0, LWA_COLORKEY);

	EnableBlurEffect(hwnd);
}

#include <Windows.h>
#include <dwmapi.h>

#pragma comment(lib, "dwmapi.lib")

void ApplyBlurEffect(HWND hwnd, float blurAmount) {
	DWM_BLURBEHIND bb = { 0 };
	bb.dwFlags = DWM_BB_ENABLE;
	bb.fEnable = TRUE;
	bb.hRgnBlur = NULL;

	DwmEnableBlurBehindWindow(hwnd, &bb);
}

void render_fullscreen_background(HWND hwnd, float alpha, float blurAmount) {

	ApplyBlurEffect(hwnd, blurAmount);

	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, alpha));
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
	ImGui::Begin("background", nullptr,
		ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoSavedSettings);
	ImGui::End();
	ImGui::PopStyleColor();
}

TextEditor editor;
ImGuiStyle& style = ImGui::GetStyle();

void RenderFrame(ImVec2 p_min, ImVec2 p_max, ImU32 fill_col, bool border, float rounding)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = g.CurrentWindow;
	window->DrawList->AddRectFilled(p_min, p_max, fill_col, rounding);
	const float border_size = g.Style.FrameBorderSize;
	if (border && border_size > 0.0f)
	{
		window->DrawList->AddRect(p_min + ImVec2(1, 1), p_max + ImVec2(1, 1), ImGui::GetColorU32(ImGuiCol_BorderShadow), rounding, 0, border_size);
		window->DrawList->AddRect(p_min, p_max, ImGui::GetColorU32(ImGuiCol_Border), rounding, 0, border_size);
	}
}

void RenderFrameBorder(ImVec2 p_min, ImVec2 p_max, float rounding)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = g.CurrentWindow;
	const float border_size = g.Style.FrameBorderSize;
	if (border_size > 0.0f)
	{
		window->DrawList->AddRect(p_min + ImVec2(1, 1), p_max + ImVec2(1, 1), ImGui::GetColorU32(ImGuiCol_BorderShadow), rounding, 0, border_size);
		window->DrawList->AddRect(p_min, p_max, ImGui::GetColorU32(ImGuiCol_Border), rounding, 0, border_size);
	}
}
void RenderCustomTabs(int& selectedTab) {
	ImDrawList* drawList = ImGui::GetWindowDrawList();
	ImVec2 pos = ImGui::GetCursorScreenPos();
	float tabWidth = 80.0f;
	float tabHeight = 30.0f;
	const char* tabs[] = { "Legit", "Visuals", "Players", "Misc", "Rage" };
	int tabCount = 5;
	float spacing = 5.0f;
	for (int i = 0; i < tabCount; i++) {
		ImVec2 tabPos = ImVec2(pos.x + i * (tabWidth + spacing), pos.y);
		ImVec2 tabEnd = ImVec2(tabPos.x + tabWidth, tabPos.y + tabHeight);
		bool isActive = (selectedTab == i);
		ImU32 fillColor = isActive ? IM_COL32(114, 137, 218, 255) : IM_COL32(30, 30, 30, 255);
		ImU32 borderColor = isActive ? IM_COL32(134, 157, 238, 255) : IM_COL32(80, 80, 80, 255);
		drawList->AddRectFilled(tabPos, tabEnd, fillColor, 4.0f);
		drawList->AddRect(tabPos, tabEnd, borderColor, 4.0f, 0, 2.0f);
		ImVec2 textSize = ImGui::CalcTextSize(tabs[i]);
		ImVec2 textPos = ImVec2(tabPos.x + (tabWidth - textSize.x) * 0.5f, tabPos.y + (tabHeight - textSize.y) * 0.5f);
		drawList->AddText(textPos, IM_COL32(220, 220, 220, 255), tabs[i]);
		ImGui::SetCursorScreenPos(tabPos);
		if (ImGui::InvisibleButton((std::string("CustomTab") + std::to_string(i)).c_str(), ImVec2(tabWidth, tabHeight))) {
			selectedTab = i;
		}
		if (i < tabCount - 1) {
			ImGui::SameLine();
		}
	}
	ImGui::Dummy(ImVec2((tabWidth + spacing) * tabCount, tabHeight));
}

bool overlay::render() {
	//

	::SetThreadPriority(::GetCurrentThread(), THREAD_PRIORITY_HIGHEST);
	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	ImFontConfig font_config;
	font_config.PixelSnapH = false;
	font_config.OversampleH = 5;
	font_config.OversampleV = 5;
	font_config.RasterizerMultiply = 1.2f;

	static const ImWchar ranges[] = {
		0x0020, 0x00FF,
		0x0400, 0x052F,
		0x2DE0, 0x2DFF,
		0xA640, 0xA69F,
		0xE000, 0xE226,
		0,
	};

	ImGui_ImplWin32_EnableDpiAwareness();

	WNDCLASSEX wc = {};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = window_proc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetModuleHandle(nullptr);
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0));
	wc.lpszMenuName = nullptr;

	wc.lpszClassName = TEXT("quiet_overlay_class");
	wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

	if (!RegisterClassEx(&wc)) {
		DWORD error = GetLastError();
		char errMsg[128];
		sprintf_s(errMsg, "RegisterClassEx failed: GetLastError = %lu", error);
		MessageBoxA(nullptr, errMsg, "error", MB_OK | MB_ICONERROR);
		return false;
	}

	const HWND hw = CreateWindowEx(
		WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_NOACTIVATE,
		wc.lpszClassName,
		TEXT("externalbase"),
		WS_POPUP,
		0,
		0,
		GetSystemMetrics(SM_CXSCREEN),
		GetSystemMetrics(SM_CYSCREEN),
		nullptr,
		nullptr,
		wc.hInstance,
		nullptr
	);

	if (!hw) {
		UnregisterClass(wc.lpszClassName, wc.hInstance);
		return false;
	}

	SetLayeredWindowAttributes(hw, 0, 255, LWA_ALPHA);
	const MARGINS margin = { -1 };
	if (FAILED(DwmExtendFrameIntoClientArea(hw, &margin))) {
		DestroyWindow(hw);
		UnregisterClass(wc.lpszClassName, wc.hInstance);
		return false;
	}

	if (!create_device_d3d(hw)) {
		cleanup_device_d3d();
		DestroyWindow(hw);
		UnregisterClass(wc.lpszClassName, wc.hInstance);
		return false;
	}

	ShowWindow(hw, SW_SHOW);
	UpdateWindow(hw);

	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui::GetIO().IniFilename = nullptr;
	ImGuiIO& io = ImGui::GetIO();

	ImGui_ImplWin32_Init(hw);
	ImGui_ImplDX11_Init(d3d11Device, d3d11DeviceContext);

	const ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	init = true;
	static ImFont* pixelatedFont{ NULL };
	bool SpotifyBar = true;
	bool draw = false;
	static float fogEnd = 500.0f;
	static float fogStart = 100.0f;
	static float clockTime = 12.0f;
	static float brightness = 1.0f;
	static float environmentScale = 1.0f;
	static float fps = 1.0f;
	static float shadowsoftness = 0.0f;
	static ImVec4 fogColor = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
	static ImVec4 ambienceColor = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
	static ImVec4 backDoorAmbience = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
	static ImVec4 colorShiftBottom = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
	static ImVec4 colorShiftTop = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);
	static ImVec4 outdoorAmbient = ImVec4(0.6f, 0.6f, 0.6f, 1.0f);

	static bool globalShadows = true;
	bool done = false;
	int tab = 0;
	int theme = 0;
	bool teamcheck = false;
	auto last_time = std::chrono::high_resolution_clock::now();
	bool watermark = false;
	static float rot_angle = 0.0f;
	static const float rot_speed = 10.0f;
	static std::map<std::string, bool> strafe_map;
	float alpha = g_menu_alpha;
	bool gamelol = false;
	char buffer[256];
	std::string wmtext = "charm.hook";
	char textInput[256] = "charm.hook";
	ImFont* font = io.Fonts->AddFontFromMemoryTTF(font_Bytes, sizeof(font_Bytes), 13.5f, &font_config, io.Fonts->GetGlyphRangesJapanese());
	ImFont* font223 = io.Fonts->AddFontFromMemoryTTF(font_bytes232, sizeof(font_bytes232), 13.5f, &font_config, io.Fonts->GetGlyphRangesJapanese());
	static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
	ImFontConfig cfg;
	cfg.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags::ImGuiFreeTypeBuilderFlags_ForceAutoHint;
	cfg.SizePixels = 12;
	ImFontConfig fa_config; fa_config.MergeMode = true; fa_config.PixelSnapH = true;
	fa_config.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags::ImGuiFreeTypeBuilderFlags_ForceAutoHint;

	io.Fonts->AddFontFromMemoryCompressedTTF(roboto_medium_compressed_data, roboto_medium_compressed_size, 12, &cfg, io.Fonts->GetGlyphRangesDefault());
	io.Fonts->AddFontDefault(&cfg);
	ImFont* font2 = io.Fonts->AddFontFromMemoryCompressedTTF(fa6_solid_compressed_data, fa6_solid_compressed_size, 14, &cfg, icons_ranges);
	std::time_t t = std::time(nullptr);
	std::tm local_tm = *std::localtime(&t);

	while (!done) {
		MSG msg;
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if (msg.message == WM_QUIT) {
				done = true;
			}
		}

		if (done)
			break;

		move_window(hw);
		// Keep key checking at a reasonable rate but don't delay too much
		static DWORD lastKeyCheckTime = 0;
		DWORD currentTime = GetTickCount();

		if (currentTime - lastKeyCheckTime > 50) { // Check keys slightly more often for responsiveness
			if (GetAsyncKeyState(VK_F1) & 1 || GetAsyncKeyState(VK_INSERT) & 1)
				draw = !draw;
			lastKeyCheckTime = currentTime;
		}

		auto current_time = std::chrono::high_resolution_clock::now();
		float delta_time = std::chrono::duration<float>(current_time - last_time).count();
		last_time = current_time;

		if (g_ShowMenu) {
			g_menu_alpha += g_fade_speed * delta_time;
			if (g_menu_alpha > 1.0f) g_menu_alpha = 1.0f;
		}
		else {
			g_menu_alpha -= g_fade_speed * delta_time;
			if (g_menu_alpha < 0.0f) g_menu_alpha = 0.0f;
		}

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		ImGuiStyle& style = ImGui::GetStyle();

		ImGuiIO& io = ImGui::GetIO();

		// Initialize tabs array if empty (CGui-style)
		if (m_Tabs.empty()) {
			m_Tabs.push_back("Aimbot");
			m_Tabs.push_back("Visuals");
			m_Tabs.push_back("Players");
			m_Tabs.push_back("Misc");
			m_Tabs.push_back("Scripts");
			m_Tabs.push_back("Settings");
		}

		ImGui::StyleColorsDark();
		style.WindowRounding = 0;
		style.ChildRounding = 0;
		style.FrameRounding = 0;
		style.PopupRounding = 0;
		style.ScrollbarRounding = 0;
		style.TabRounding = 0;
		style.ButtonTextAlign = { 0.5f, 0.5f };
		style.WindowTitleAlign = { 0.5f, 0.5f };
		style.FramePadding = { 3.0f, 3.0f };
		style.WindowPadding = { 6.0f, 6.0f };
		style.ItemSpacing = style.WindowPadding;
		style.ItemInnerSpacing = { style.ItemSpacing.x, 5 };
		style.WindowBorderSize = 1;
		style.FrameBorderSize = 1;
		style.PopupBorderSize = 1;
		style.ScrollbarSize = 4.f;
		style.GrabMinSize = 4;
		style.WindowShadowSize = globals::menu_glow;
		if (globals::fonttype == 0) {

		}
		static HWND robloxWindow = NULL;
		static DWORD lastRobloxCheck = 0;
		//DWORD currentTime = GetTickCount();

		// Cache the Roblox window handle but refresh it frequently enough for ESP
		if (robloxWindow == NULL || currentTime - lastRobloxCheck > 250) {
			robloxWindow = FindWindowA(0, XorStr("Roblox"));
			lastRobloxCheck = currentTime;
		}

		// Don't cache foreground window - too important for ESP performance
		HWND currentForeground = GetForegroundWindow();

		if (currentForeground == robloxWindow || currentForeground == hw) {

			ImVec4 babyBlue = ImVec4(0.537f, 0.811f, 0.941f, 1.0f);
			ImVec4 darkBlue = ImVec4(0.0f, 0.0f, 0.5f, 1.0f);

			ImU32 accent_color_u32 = ImGui::ColorConvertFloat4ToU32(babyBlue);
			ImU32 border_accent_color_u32 = ImGui::ColorConvertFloat4ToU32(darkBlue);

			ImGui::Begin(XorStr("ESP"), nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);
			{

				ImGui::PushFont(font);

				RBX::hook_esp();

				ImGui::End();

				ImGui::PopFont();
			}
			if (targetlist) {
				ImGuiStyle& style = ImGui::GetStyle();
				static std::vector<std::string> keys;
				keys.clear();


				if (aimbot_target.address) keys.emplace_back("Aimbot Target - " + aimbot_target.name);
				if (aimbot_target2.address) keys.emplace_back("Silent Target - " + aimbot_target2.name);




				if (!keys.empty()) {
					ImGui::PushStyleColor(ImGuiCol_WindowShadow, style.Colors[ImGuiCol_SliderGrab]);
					ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
					ImGui::Begin("Target List", nullptr, ImGuiWindowFlags_NoDecoration);
					ImGui::PopStyleVar();
					ImGui::PopStyleColor();

					int max_lenght = 0;
					for (const auto& key : keys) {
						int textSize = ImGui::CalcTextSize(key.c_str()).x;
						if (textSize > max_lenght) max_lenght = textSize;
					}

					ImGui::SetWindowSize(ImVec2(max_lenght + style.WindowPadding.x * 2,
						(ImGui::GetFontSize() + style.WindowPadding.y * 2) + style.WindowPadding.y * 2 +
						(ImGui::GetFontSize() * keys.size()) + (style.ItemSpacing.y * (keys.size() - 1))));

					ImAdd::BeginChild("Targets", ImVec2(0, ImGui::GetFontSize() + style.WindowPadding.y * 2));
					ImAdd::EndChild();

					ImGui::SetCursorPosY(ImGui::GetFontSize() + style.WindowPadding.y * 2);
					ImGui::BeginChild("Target", ImVec2(0, 0), ImGuiChildFlags_Border, ImGuiWindowFlags_NoBackground);

					for (const auto& key : keys) {
						ImGui::TextColored(style.Colors[ImGuiCol_Text], "%s", key.c_str());
						if (key.empty()) {
							ImGui::TextColored(style.Colors[ImGuiCol_Text], "None");
						}
					}

					ImGui::EndChild();
					ImGui::End();
				}
			}
			if (keybindlistshitt) {
				ImGuiStyle& style = ImGui::GetStyle();
				static std::vector<std::string> keys;
				keys.clear();

				if (globals::aimbot_bind.enabled) keys.emplace_back("[ Toggle ] Aimbot - " + globals::aimbot_bind.get_key_name());
				if (globals::free_aim_bind.enabled) keys.emplace_back("[ Toggle ] Silent - " + globals::free_aim_bind.get_key_name());
				if (globals::silent_Aim_Bind.enabled) keys.emplace_back("[ Toggle ] Walkspeed - " + globals::silent_Aim_Bind.get_key_name());
				if (globals::fly_bind.enabled) keys.emplace_back("[ Toggle ] Fly - " + globals::fly_bind.get_key_name());

				if (!keys.empty()) {
					ImGui::PushStyleColor(ImGuiCol_WindowShadow, style.Colors[ImGuiCol_SliderGrab]);
					ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
					ImGui::Begin("KeyList", nullptr, ImGuiWindowFlags_NoDecoration);
					ImGui::PopStyleVar();
					ImGui::PopStyleColor();

					int max_lenght = 0;
					for (const auto& key : keys) {
						int textSize = ImGui::CalcTextSize(key.c_str()).x;
						if (textSize > max_lenght) max_lenght = textSize;
					}

					ImGui::SetWindowSize(ImVec2(max_lenght + style.WindowPadding.x * 2,
						(ImGui::GetFontSize() + style.WindowPadding.y * 2) + style.WindowPadding.y * 2 +
						(ImGui::GetFontSize() * keys.size()) + (style.ItemSpacing.y * (keys.size() - 1))));

					ImAdd::BeginChild("Keybinds", ImVec2(0, ImGui::GetFontSize() + style.WindowPadding.y * 2));
					ImAdd::EndChild();

					ImGui::SetCursorPosY(ImGui::GetFontSize() + style.WindowPadding.y * 2);
					ImGui::BeginChild("Content", ImVec2(0, 0), ImGuiChildFlags_Border, ImGuiWindowFlags_NoBackground);

					for (const auto& key : keys) {
						ImGui::TextColored(style.Colors[ImGuiCol_Text], "%s", key.c_str());
					}

					ImGui::EndChild();
					ImGui::End();
				}
			}
			if (WatermarkNshit) {
				ImGuiStyle& style = ImGui::GetStyle();
				auto now = std::chrono::system_clock::now();
				std::time_t now_time = std::chrono::system_clock::to_time_t(now);
				std::tm local_tm = *std::localtime(&now_time);

				char buffer[128];
				std::strftime(buffer, sizeof(buffer), "charm  %B %d %Y - %H:%M:%S", &local_tm);

				std::string watermark_text = std::string(buffer) + " - " + std::to_string(static_cast<int>(io.Framerate)) + " FPS";

				static ImVec2 watermark_pos = ImVec2(10, 10);
				ImVec2 text_size = ImGui::CalcTextSize(watermark_text.c_str());

				ImGui::SetNextWindowPos(watermark_pos, ImGuiCond_Once);
				ImGui::SetNextWindowSize(ImVec2(text_size.x + style.WindowPadding.x * 2, text_size.y + style.WindowPadding.y * 6), ImGuiCond_Always);

				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 5));
				ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0.5f));
				ImGui::Begin("Watermark", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBackground);
				ImGui::PopStyleVar();
				ImGui::PopStyleColor();

				ImGui::TextColored(ImAdd::HexToColorVec4(0xFFFFFF, 1.00f), "%s", watermark_text.c_str());

				ImGui::End();
			}

		}
		if (draw && (currentForeground == robloxWindow || currentForeground == hw)) {
			ImGuiStyle& style = ImGui::GetStyle();
			ImGuiIO& io = ImGui::GetIO();

			// Apply CGui-style colors and rounding  
			style.WindowRounding = 3;
			style.ChildRounding = 2;
			style.FrameRounding = 0;
			style.PopupRounding = 0;
			style.ScrollbarRounding = 0;
			style.ButtonTextAlign = { 0.5f, 0.5f };
			style.WindowTitleAlign = { 0.5f, 0.5f };
			style.FramePadding = { 6.0f, 6.0f };
			style.WindowPadding = { 10.0f, 10.0f };
			style.ItemSpacing = { 10.0f, 6.0f };
			style.ItemInnerSpacing = { style.WindowPadding.x, 2 };
			style.WindowBorderSize = 1;
			style.FrameBorderSize = 1;
			style.ScrollbarSize = 7.f;
			style.GrabMinSize = 1.f;
			style.DisabledAlpha = 0.5f;
			style.WindowMinSize = ImVec2(400, 450);

			// Apply CGui color scheme using ImAdd::HexToColorVec4
			style.Colors[ImGuiCol_WindowBg] = ImAdd::HexToColorVec4(0x0c0c0c, 1.00f);
			style.Colors[ImGuiCol_ChildBg] = ImAdd::HexToColorVec4(0x121212, 1.00f);
			style.Colors[ImGuiCol_PopupBg] = ImAdd::HexToColorVec4(0x0d0d0d, 1.00f);
			style.Colors[ImGuiCol_Text] = ImAdd::HexToColorVec4(0xb4b4b4, 1.00f);
			style.Colors[ImGuiCol_TextDisabled] = ImAdd::HexToColorVec4(0x8c8c8c, 1.00f);
			style.Colors[ImGuiCol_SliderGrab] = ImAdd::HexToColorVec4(0x5e7aa6, 1.00f);
			style.Colors[ImGuiCol_SliderGrabActive] = ImAdd::HexToColorVec4(0x3e5780, 1.00f);
			style.Colors[ImGuiCol_BorderShadow] = ImAdd::HexToColorVec4(0x000000, 1.00f);
			style.Colors[ImGuiCol_Border] = ImAdd::HexToColorVec4(0x333333, 1.00f);
			style.Colors[ImGuiCol_Separator] = style.Colors[ImGuiCol_Border];
			style.Colors[ImGuiCol_Button] = ImAdd::HexToColorVec4(0x2a2a2a, 1.00f);
			style.Colors[ImGuiCol_ButtonHovered] = ImAdd::HexToColorVec4(0x3f3f3f, 1.00f);
			style.Colors[ImGuiCol_ButtonActive] = ImAdd::HexToColorVec4(0x191919, 1.00f);
			style.Colors[ImGuiCol_FrameBg] = style.Colors[ImGuiCol_Button];
			style.Colors[ImGuiCol_FrameBgHovered] = style.Colors[ImGuiCol_ButtonHovered];
			style.Colors[ImGuiCol_FrameBgActive] = style.Colors[ImGuiCol_ButtonActive];
			style.Colors[ImGuiCol_Header] = ImAdd::HexToColorVec4(0x2a2a2a, 1.00f);
			style.Colors[ImGuiCol_HeaderHovered] = ImAdd::HexToColorVec4(0x3f3f3f, 1.00f);
			style.Colors[ImGuiCol_HeaderActive] = ImAdd::HexToColorVec4(0x191919, 1.00f);
			style.Colors[ImGuiCol_ScrollbarBg] = style.Colors[ImGuiCol_Border];
			style.Colors[ImGuiCol_ScrollbarGrab] = ImAdd::HexToColorVec4(0x414141, 1.00f);
			style.Colors[ImGuiCol_ScrollbarGrabHovered] = style.Colors[ImGuiCol_ScrollbarGrab];
			style.Colors[ImGuiCol_ScrollbarGrabActive] = style.Colors[ImGuiCol_ScrollbarGrab];
			style.Colors[ImGuiCol_ResizeGrip] = ImVec4();
			style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4();
			style.Colors[ImGuiCol_ResizeGripActive] = ImVec4();
			style.Colors[ImGuiCol_SeparatorActive] = ImVec4();
			style.Colors[ImGuiCol_SeparatorHovered] = ImVec4();

			// CGui window structure (590x455 with sidebar)
			ImVec2 MenuSize = ImVec2(590, 455);
			float HeaderHeight = ImGui::GetFontSize() + style.WindowPadding.y * 2;
			float SideBarWidth = 120;

			ImGui::SetNextWindowPos(io.DisplaySize / 2 - MenuSize / 2, ImGuiCond_Once);
			ImGui::SetNextWindowSize(MenuSize, ImGuiCond_Once);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2());
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
			ImGui::Begin("charm.hook", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground);
			ImGui::PopStyleVar(2);

			// Rendering (CGui-style header and layout)
			{
				ImVec2 pos = ImGui::GetWindowPos();
				ImVec2 size = ImGui::GetWindowSize();
				ImDrawList* pDrawList = ImGui::GetWindowDrawList();

				float fGlowAlpha = 0.14f;

				// Draw main window sections
				pDrawList->AddRectFilled(pos, pos + ImVec2(size.x, HeaderHeight), ImGui::GetColorU32(ImGuiCol_ChildBg), style.WindowRounding, ImDrawFlags_RoundCornersTop);
				pDrawList->AddRectFilled(pos + ImVec2(0, HeaderHeight), pos + size, ImGui::GetColorU32(ImGuiCol_WindowBg), style.WindowRounding, ImDrawFlags_RoundCornersBottomRight);
				pDrawList->AddRectFilled(pos + ImVec2(0, HeaderHeight), pos + ImVec2(SideBarWidth, size.y), ImGui::GetColorU32(ImGuiCol_ChildBg), style.WindowRounding, ImDrawFlags_RoundCornersBottomLeft);

				// Add gradient effect
				pDrawList->AddRectFilledMultiColor(pos + ImVec2(style.WindowBorderSize, style.WindowBorderSize + style.WindowRounding), pos + ImVec2(size.x - style.WindowBorderSize, HeaderHeight), ImGui::GetColorU32(ImGuiCol_SliderGrab, 0.0f), ImGui::GetColorU32(ImGuiCol_SliderGrab, 0.0f), ImGui::GetColorU32(ImGuiCol_SliderGrab, fGlowAlpha), ImGui::GetColorU32(ImGuiCol_SliderGrab, fGlowAlpha));

				// Draw borders and lines
				if (style.WindowBorderSize > 0)
				{
					pDrawList->AddRect(pos, pos + size, ImGui::GetColorU32(ImGuiCol_Border), style.WindowRounding);
					pDrawList->AddLine(pos + ImVec2(style.WindowBorderSize, HeaderHeight), pos + ImVec2(size.x - style.WindowBorderSize, HeaderHeight), ImGui::GetColorU32(ImGuiCol_SliderGrab));
					pDrawList->AddLine(pos + ImVec2(SideBarWidth, HeaderHeight + style.WindowBorderSize), pos + ImVec2(SideBarWidth, size.y - style.WindowBorderSize), ImGui::GetColorU32(ImGuiCol_Border), style.WindowBorderSize);
				}

				// Header branding (imgui.space style)
				pDrawList->AddText(pos + style.WindowPadding, ImGui::GetColorU32(ImGuiCol_Text), "charm");
				pDrawList->AddText(pos + style.WindowPadding + ImVec2(ImGui::CalcTextSize("charm").x + 1, 0), ImGui::GetColorU32(ImGuiCol_SliderGrab), ".hook");
			}

			// Content (CGui-style sidebar + main content)
			{
				// Sidebar with tabs
				ImGui::SetCursorScreenPos(ImGui::GetWindowPos() + ImVec2(0, HeaderHeight));
				ImGui::BeginChild("SideBar", ImVec2(SideBarWidth, 0), ImGuiChildFlags_AlwaysUseWindowPadding, ImGuiWindowFlags_NoBackground);
				{
					for (int i = 0; i < m_Tabs.size(); i++)
					{
						ImAdd::RadioFrameGradient(m_Tabs[i], &m_iCurrentPage, i, ImVec2(-0.1f, 0));
					}
				}
				ImGui::EndChild();

				// Main content area
				ImGui::SetCursorScreenPos(ImGui::GetWindowPos() + ImVec2(SideBarWidth + style.WindowBorderSize, HeaderHeight));
				ImGui::BeginChild("Main", ImVec2(0, 0), ImGuiChildFlags_AlwaysUseWindowPadding, ImGuiWindowFlags_NoBackground);
				{
					float fGroupWidth = (ImGui::GetWindowWidth() - style.ItemSpacing.x - style.WindowPadding.x * 2) / 2;

					// Main content pages based on current tab
					if (m_iCurrentPage == ImPage_Aimbot)
					{
						ImGui::BeginGroup(); // Left column
						{
							ImAdd::BeginChild("Aim Assist", ImVec2(fGroupWidth, 0));
							{
								ImAdd::CheckBox("Aimbot", &globals::aimbot);
								Keybind(&globals::aimbot_bind, ImVec2(40, 15));

								const char* aimbot_type_options[] = { "Memory", "Mouse" };
								ImAdd::Combo("Type", &globals::aimbot_type, aimbot_type_options, IM_ARRAYSIZE(aimbot_type_options));
								std::vector<std::string> item_names = {
									  "Knocked Check",
									  "Dead Check",
									 "Grabbed Check",
									  "Team Check"
								};
								DrawMultiCombo("Aim Assist Checks", item_names, globals::aimbot_checks);
								ImAdd::CheckBox(XorStr("AA Resolver"), &globals::resolver);
								const char* aimbot_hitpart_options[] = { "Head", "RootPart", "UpperRoot", "LowerRoot" };
								ImAdd::Combo("Hitpart", &globals::aimbot_part, aimbot_hitpart_options, IM_ARRAYSIZE(aimbot_hitpart_options));

								if (globals::aimbot_type == 0) {
									ImAdd::SliderFloat("Smoothness", &globals::smoothness_camera, 1.0f, 100.0f);
								}
								else if (globals::aimbot_type == 1) {
									ImAdd::SliderFloat("Smoothness", &globals::mouse_smoothness, 2.0f, 100.0f);
									ImAdd::SliderFloat("Strength", &globals::mouse_sensitivity, 1.0f, 100.0f);
								}

								ImAdd::CheckBox(XorStr("Sticky Aim"), &globals::aimbot_sticky);
								ImAdd::CheckBox(XorStr("Stutter Aim"), &globals::shake);
								ImGui::Spacing();
								if (globals::aimbot_type == 1) {
									ImAdd::CheckBox(XorStr("Prediction"), &globals::free_aim_prediction);
									ImAdd::SliderFloat("Prediction X", &globals::free_aim_prediction_x, 1.0f, 50.f);
									ImAdd::SliderFloat("Prediction Y", &globals::free_aim_prediction_y, 1.0f, 50.f);
								}
								else {
									ImAdd::CheckBox(XorStr("Prediction"), &globals::camera_prediction);
									ImAdd::SliderFloat("Prediction X", &globals::camera_prediction_x, 5.0f, 50.f);
									ImAdd::SliderFloat("Prediction Y", &globals::camera_prediction_y, 5.0f, 50.f);
									ImAdd::SliderFloat("Prediction Z", &globals::camera_prediction_z, 5.0f, 50.f);
								}

								ImAdd::SliderFloat("Stutter X", &globals::shake_x, 1.0f, 50.f);
								ImAdd::SliderFloat("Stutter Y", &globals::shake_y, 1.0f, 50.f);
							}
							ImAdd::EndChild();
						}
						ImGui::EndGroup();

						ImGui::SameLine();

						ImGui::BeginGroup(); // Right column
						{
							ImAdd::BeginChild("Silent Aim", ImVec2(fGroupWidth, 0));
							{
								ImAdd::CheckBox("Silent Aim", &globals::free_aim);
								Keybind(&globals::free_aim_bind, ImVec2(40, 15));
								ImAdd::CheckBox(XorStr("Sticky Aim"), &globals::free_aim_sticky);
								ImAdd::CheckBox(XorStr("Prediction"), &globals::free_aim_prediction);
								ImAdd::CheckBox(XorStr("Closest Part"), &globals::free_aim_closest_part);
								ImAdd::CheckBox(XorStr("Out Of Fov Check"), &globals::free_aim_is_in_fov);
								ImAdd::CheckBox(XorStr("Resolver"), &globals::free_aim_resolver);
								if (ImAdd::CheckBox(XorStr("Link To Aimbot"), &link)) {
									globals::free_aim_bind = globals::aimbot_bind;
									globals::free_aim_prediction_x = globals::camera_prediction_x;
									globals::free_aim_prediction_y = globals::camera_prediction_y;
									globals::camera_prediction_z = globals::free_aim_prediction_x;
								}
								ImAdd::SliderFloat("Prediction X", &globals::free_aim_prediction_x, 1.0f, 50.f);
								ImAdd::SliderFloat("Prediction Y", &globals::free_aim_prediction_y, 1.0f, 50.f);

								const char* free_aim_hitpart_options[] = { "Head", "RootPart", "UpperRoot", "LowerRoot" };
								ImAdd::Combo("Hitpart", &globals::free_aim_part, free_aim_hitpart_options, IM_ARRAYSIZE(free_aim_hitpart_options));
							}
							ImAdd::EndChild();
						}
						ImGui::EndGroup();
					}
					else if (m_iCurrentPage == ImPage_Visuals)
					{
						ImGui::BeginGroup(); // Left column
						{
							ImAdd::BeginChild("Visuals", ImVec2(fGroupWidth, 0));
							{
								ImAdd::CheckBox("ESP", &globals::esp);
								ImAdd::CheckBox("Bounding Box", &globals::box_esp);
								const char* box_type[] = { "2D Bounding", "2D Cornered Bounding" };
								ImAdd::Combo("Box Type", &globals::box_type, box_type, IM_ARRAYSIZE(box_type));

								ImAdd::CheckBox("Fill", &globals::fill_box);
								ImAdd::CheckBox("Fov", &globals::draw_aimbot_fov);
								ImAdd::CheckBox("CrossHair", &globals::crosshair);
								ImAdd::SliderInt("Crosshair Gap", &globals::crosshair_gap, 1, 15);
								ImAdd::SliderInt("Crosshair Size", &globals::crosshair_size, 1, 40);
								ImAdd::SliderInt("Crosshair Speed", &globals::crosshair_speed, 1, 20);
								ImAdd::CheckBox("Tracers", &globals::tracer_esp);
								ImAdd::CheckBox("Name", &globals::name_esp);
								ImAdd::CheckBox("Name Outline", &globals::name_outline);
								ImAdd::CheckBox("Tool", &globals::tool_esp);
								ImAdd::CheckBox("Tool Outline", &globals::tool_outline);
								ImAdd::CheckBox("Distance", &globals::distance_esp);
								ImAdd::CheckBox("Distance Outline", &globals::distance_outline);
								ImAdd::CheckBox("Healthbar", &globals::health_bar);

								ImAdd::CheckBox("cash esp (dh)", &globals::cash_esp);
								ImAdd::CheckBox("Spawn esp", &globals::cashier_esp);
								ImAdd::CheckBox("SoccerBall esp", &globals::soccer_ball_esp);
								ImAdd::CheckBox("Dahood Dropped Items Esp", &globals::dropped_items);
								ImAdd::CheckBox("Dahood Vehicle Esp", &globals::vehicle_esp);
								ImAdd::CheckBox("Jail ESP", &globals::jail_esp);
								ImAdd::CheckBox("Armor Bar", &globals::shield_bar);
								ImAdd::CheckBox("Flame Bar", &globals::flame_bar);
								ImAdd::CheckBox("Chams", &globals::chams);
								ImAdd::CheckBox("Skeleton ESP", &globals::skeleton_esp);
							}
							ImAdd::EndChild();
						}
						ImGui::EndGroup();

						ImGui::SameLine();

						ImGui::BeginGroup(); // Right column
						{
							ImAdd::BeginChild("Settings", ImVec2(fGroupWidth, 0));
							{
								ImAdd::SliderFloat("Chams Opacity", &globals::chams_opacity, 0.1f, 50.0f);
								ImAdd::CheckBox("Anti Aliasing", &globals::autoparry);
								ImAdd::SliderFloat("Alpha", &globals::alpha, 0.0f, 150.0f);
								ImAdd::SliderFloat("Glow Size", &globals::glow_size, 0.0f, 150.0f);
								ImAdd::SliderInt("Glow Opacity", &globals::glow_opacity, 0, 2);
								ImAdd::CheckBox("Use Distance Limit", &globals::distancecheck);
								ImAdd::SliderFloat("Max Distance", &globals::max_render_distance, 0.0f, 10000.0f);
								ImAdd::CheckBox("Health Bar Text", &globals::health_bar_text);
								ImAdd::SliderFloat("Fov Size", &globals::aimbot_fov_size, 1.0f, 200.0f);
								ImAdd::ColorEdit4("Box Color", globals::box_color);
								ImAdd::ColorEdit4("Fill Color", globals::color_5);
								ImAdd::ColorEdit4("Glow Color", globals::color_1);
								ImAdd::ColorEdit4("Tool Color", globals::color_2);
								ImAdd::ColorEdit4("Chams Color", globals::color_4);
								ImAdd::ColorEdit4("Name Color", globals::name_color);
								ImAdd::ColorEdit4("Distance Color", globals::distance_color);
								ImAdd::CheckBox("Off Screen Check", &globals::offscreen_Check);
							}
							ImAdd::EndChild();
						}
						ImGui::EndGroup();
					}
					else if (m_iCurrentPage == ImPage_Players)
					{
						static std::unordered_map<std::string, int> playerStatuses;
						static int selectedPlayerIndex = -1;
						std::vector<std::string> playerNames;

						const char* statusItems[] = { "Neutral", "Friend", "Enemy" };

						ImAdd::BeginChild("Players", ImVec2(0, 0));
						{
							ImGui::BeginGroup();
							{
								ImGui::TextColored(style.Colors[ImGuiCol_Text], "Player List");

								playerNames.clear();
								for (const auto& player : globals::players.GetChildren(false)) {
									playerNames.push_back(player.GetName());
								}

								if (ImGui::BeginTable("PlayerTable", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
									ImGui::TableSetupColumn("Name");
									ImGui::TableSetupColumn("Status");
									ImGui::TableHeadersRow();

									for (int i = 0; i < playerNames.size(); ++i) {
										const auto& playerName = playerNames[i];

										ImGui::TableNextRow();
										ImGui::TableSetColumnIndex(0);

										ImVec4 textColor = (playerName == globals::localplayername) ? ImVec4(0.6f, 0.3f, 1.0f, 1.0f) : ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
										if (ImAdd::Selectable((playerName == globals::localplayername) ? "Self" : playerName.c_str(), selectedPlayerIndex == i)) {
											selectedPlayerIndex = i;
										}

										ImGui::TableSetColumnIndex(1);
										const std::string& selectedPlayerName = playerNames[i];

										if (playerStatuses.find(selectedPlayerName) == playerStatuses.end()) {
											playerStatuses[selectedPlayerName] = 0;
										}

										ImGui::TextColored(style.Colors[ImGuiCol_Text], "%s", statusItems[playerStatuses[selectedPlayerName]]);
									}
									ImGui::EndTable();
								}
							}
							ImGui::EndGroup();

							ImGui::Text("Set Status:");

							if (selectedPlayerIndex >= 0 && selectedPlayerIndex < playerNames.size()) {
								const std::string& selectedPlayerName = playerNames[selectedPlayerIndex];

								ImVec4 statusColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

								switch (playerStatuses[selectedPlayerName]) {
								case 1: statusColor = ImVec4(0.0f, 1.0f, 0.0f, 1.0f); break;
								case 2: statusColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f); break;
								}

								ImGui::PushStyleColor(ImGuiCol_Text, statusColor);
								if (ImAdd::BeginCombo("##GlobalStatus", statusItems[playerStatuses[selectedPlayerName]])) {
									for (int i = 0; i < IM_ARRAYSIZE(statusItems); ++i) {
										bool isSelected = (playerStatuses[selectedPlayerName] == i);
										if (ImAdd::Selectable(statusItems[i], isSelected)) {
											playerStatuses[selectedPlayerName] = i;
										}
									}
									ImGui::EndCombo();
								}
								ImGui::PopStyleColor();
							}
							else {
								ImGui::BeginDisabled();
								if (ImAdd::BeginCombo("##GlobalStatus", "Select a Player", ImGuiComboFlags_NoArrowButton)) {
									ImGui::EndCombo();
								}
								ImGui::EndDisabled();
							}
						}
						ImAdd::EndChild();
					}
					else if (m_iCurrentPage == ImPage_Misc)
					{
						ImAdd::BeginChild("Misc", ImVec2(0, 0));
						{
							ImGui::TextColored(style.Colors[ImGuiCol_Text], "Settings");
							ImAdd::CheckBox("Lplayer", &globals::localplayercheck);
							ImAdd::CheckBox("Only Lplayer", &globals::onlylocal);
							ImAdd::CheckBox("Auto Rescan", &globals::rescan);

							if (ImAdd::Button("Rescan", ImVec2(-0.1f, 0))) {
								RBX::load_game_data();
							}

							// World controls (from original iTabID == 2)
							ImAdd::SliderFloat("Fog End", &fogEnd, 0.0f, 1000.0f);
							ImAdd::SliderFloat("Fog Start", &fogStart, 0.0f, 1000.0f);
							ImAdd::SliderFloat("charm Softness", &shadowsoftness, 0.0f, 1000.0f);
							ImAdd::SliderFloat("Clock Time", &clockTime, 0.0f, 24.0f);
							ImAdd::SliderFloat("Brightness", &brightness, 0.0f, 5.0f);
							ImAdd::SliderFloat("Environment Scale", &environmentScale, 0.0f, 5.0f);

							// Apply lighting changes
							//RBX::Instance Cool;
							//RBX::Instance lighting = Cool.GetLighting();
							//lighting.setFogEnd(fogEnd);
							//lighting.setFogStart(fogStart);
							//lighting.setBrightness(brightness);
							//lighting.setFogColor({ fogColor.x, fogColor.y, fogColor.z });
							//lighting.setAmbientColor({ ambienceColor.x, ambienceColor.y, ambienceColor.z });
							//lighting.setBackDoorAmbienceColor({ backDoorAmbience.x, backDoorAmbience.y, backDoorAmbience.z });
							//lighting.setColorShiftBottom({ colorShiftBottom.x, colorShiftBottom.y, colorShiftBottom.z });
							//lighting.setColorShiftTop({ colorShiftTop.x, colorShiftTop.y, colorShiftTop.z });
							//lighting.setOutdoorAmbient({ outdoorAmbient.x, outdoorAmbient.y, outdoorAmbient.z });
							//lighting.setGlobalShadows(globalShadows);

							// Color controls
							ImAdd::ColorEdit4("Fog Color", (float*)&fogColor);
							ImAdd::ColorEdit4("Ambience Color", (float*)&ambienceColor);
							ImAdd::ColorEdit4("Back Door Ambience Color", (float*)&backDoorAmbience);
							ImAdd::ColorEdit4("Color Shift Bottom", (float*)&colorShiftBottom);
							ImAdd::ColorEdit4("Color Shift Top", (float*)&colorShiftTop);
							ImAdd::ColorEdit4("Outdoor Ambient", (float*)&outdoorAmbient);

							// Rage features (from original iTabID == 4)
							ImAdd::SliderFloat("WalkSpeed", &globals::walkspeed_amount, 1.0f, 500.f);
							Keybind(&globals::silent_Aim_Bind, ImVec2(40, 15));
							ImAdd::CheckBox("Fly", &globals::fly);
							Keybind(&globals::fly_bind, ImVec2(40, 15));
							ImAdd::SliderInt("Fly Speed", &globals::fly_speed, 3, 45);
						}
						ImAdd::EndChild();
					}
					else if (m_iCurrentPage == ImPage_Scripts)
					{
						ImAdd::BeginChild("Scripts", ImVec2(0, 0));
						{
							static char config_name[64] = "";
							std::vector<std::string> configFiles;
							std::string configFolderPath = GetConfigFolderPath();

							for (const auto& entry : std::filesystem::directory_iterator(configFolderPath)) {
								if (!entry.is_directory() && entry.path().extension() == ".cfg") {
									configFiles.push_back(entry.path().filename().string());
								}
							}

							static std::string current_item = configFiles.empty() ? "" : configFiles[0];

							for (const auto& file : configFiles) {
								if (file.empty()) continue;
								bool is_selected = (current_item == file);

								if (ImAdd::Selectable(file.c_str(), is_selected)) {
									current_item = file;
									size_t extensionPos = current_item.rfind(".cfg");
									if (extensionPos != std::string::npos) {
										current_item.erase(extensionPos);
									}
									strncpy(config_name, current_item.c_str(), sizeof(config_name) - 1);
									config_name[sizeof(config_name) - 1] = '\0';
								}
							}

							ImGui::InputText("Config Name", config_name, sizeof(config_name));

							if (ImAdd::Button("Save Config", ImVec2(-0.1f, 0))) {
								RBX::configs::save(config_name);
							}

							if (ImAdd::Button("Load Config", ImVec2(-0.1f, 0))) {
								RBX::configs::load(config_name);
							}

							if (ImAdd::Button("Open Folder", ImVec2(-0.1f, 0))) {
								ShellExecute(NULL, "open", "explorer.exe", (RBX::appdata_path() + "\\Shadow\\configs").c_str(), NULL, SW_SHOWNORMAL);
							}
						}
						ImAdd::EndChild();
					}
					else if (m_iCurrentPage == ImPage_Settings)
					{
						ImAdd::BeginChild("Settings", ImVec2(0, 0));
						{
							ImGui::TextColored(style.Colors[ImGuiCol_Text], "Settings");
							ImAdd::CheckBox("Target List", &targetlist);
							ImAdd::CheckBox("Cache Off Screen Check", &globals::cframe);
							ImAdd::CheckBox("Slow Cache", &globals::HeavyOptimize);
							ImAdd::SliderInt("Player Cache Speed", &globals::threadrestarttime, 0.0f, 10000.0f);
							ImAdd::SliderInt("Player Cache Speed Centered", &globals::threadtime, 0.0f, 10000.0f);
						}
						ImAdd::EndChild();
					}
				}
				ImGui::EndChild();
			}
			ImGui::End();

		}

		if (draw) {
			SetLayeredWindowAttributes(hw, 0, 255, LWA_ALPHA);
			const MARGINS margin = { -1 };
			DwmExtendFrameIntoClientArea(hw, &margin);
			SetWindowLong(hw, GWL_EXSTYLE, WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TOOLWINDOW);

			BringWindowToTop(hw);
			SetForegroundWindow(hw);
		}
		else {
			SetWindowLong(hw, GWL_EXSTYLE, WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_TOOLWINDOW);
			SetLayeredWindowAttributes(hw, 0, 255, LWA_ALPHA);
			const MARGINS margin = { -1 };
			DwmExtendFrameIntoClientArea(hw, &margin);
		}

		ImGui::Render();
		const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
		d3d11DeviceContext->OMSetRenderTargets(1, &d3d11RenderTargetView, nullptr);
		d3d11DeviceContext->ClearRenderTargetView(d3d11RenderTargetView, clear_color_with_alpha);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		dxgiSwapChain->Present(globals::vsync, 0);

		// Only add a minimal sleep to prevent excessive CPU usage
		// Much lighter frame limiting to maintain ESP performance
		if (!draw) {
			// Only sleep when overlay is not being drawn
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}

	init = false;

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	cleanup_device_d3d();
	DestroyWindow(hw);
	UnregisterClass(wc.lpszClassName, wc.hInstance);
}