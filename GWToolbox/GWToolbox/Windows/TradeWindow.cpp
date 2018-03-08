#include "TradeWindow.h"

#include <GWCA\GWCA.h>
#include <GWCA\Managers\ChatMgr.h>

#include <imgui.h>
#include <imgui_internal.h>
#include <json.hpp>

#include "logger.h"
#include "GuiUtils.h"
#include "GWToolbox.h"

#include <list>

unsigned int TradeWindow::Alert::uid_count = 0;

void TradeWindow::Initialize() {
	// skip the Window initialize to avoid registering with ourselves
	ToolboxWindow::Initialize();
}

void TradeWindow::DrawSettingInternal() {

}

void TradeWindow::Update(float delta) {
	chat.fetch();
	std::string message;
	std::string final_chat_message;
	for (unsigned int i = 0; i < chat.new_messages.size(); i++) {
		message = chat.new_messages.at(i)["message"].dump();
		std::transform(message.begin(), message.end(), message.begin(), ::tolower);
		for (unsigned j = 0; j < alerts.size(); j++) {
			// ensure the alert isnt empty
			if (strncmp(alerts.at(i).match_string, "", 128)) {
				if (message.find(alerts.at(j).match_string) != std::string::npos) {
					final_chat_message = "<c=#f96677><a=1> " + chat.new_messages.at(i)["name"].get<std::string>() + "</a>: " +
						chat.new_messages.at(i)["message"].get<std::string>() + "</c>";
					GW::Chat::WriteChat(GW::Chat::CHANNEL_TRADE, final_chat_message.c_str());
					// break to stop multiple alerts triggering the same message
					break;
				}
			}
		}
	}
}

void TradeWindow::Draw(IDirect3DDevice9* device) {
	if (!visible) { return; }
	ImGui::SetNextWindowPosCenter(ImGuiSetCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 0), ImGuiSetCond_FirstUseEver);
	if (ImGui::Begin(Name(), GetVisiblePtr(), GetWinFlags())) {
		ImGui::PushTextWrapPos();
		if (ImGui::Button("Alerts", ImVec2(ImGui::GetWindowContentRegionWidth(), 0))) {
			show_alert_window = true;
		}
		ImGui::BeginChild("trade_scroll");
		ImGui::Columns(2);
		ImGui::Text("Player Name");
		ImGui::SetColumnWidth(-1, 175);
		ImGui::NextColumn();
		ImGui::Text("Message");
		ImGui::SetColumnWidth(-1, 500);
		ImGui::NextColumn();
		std::string name;
		std::string message;
		for (unsigned int i = 0; i < chat.messages.size(); i++) {
			name = chat.messages.at(i)["name"].get<std::string>();
			message = chat.messages.at(i)["message"].get<std::string>();
			if (ImGui::Button(name.c_str())) {
				// TODO open whisper to the player when that functionality is added
			}
			ImGui::NextColumn();
			ImGui::Text("%s", message.c_str());
			ImGui::NextColumn();
		}
		ImGui::EndChild();
		if (show_alert_window) {
			if (ImGui::Begin("Trade Alerts", &show_alert_window)) {
				if (ImGui::Button("New Alert", ImVec2(ImGui::GetWindowContentRegionWidth(), 0))) {
					alerts.insert(alerts.begin(), Alert());
				}
				if (ImGui::IsItemHovered()) {
					ImGui::SetTooltip("Click to add a new keyword. Trade messages with matched keywords\n" \
						"will be send to the Guild Wars chat. The keywords are not case sensitive.");
				}
				for (unsigned int i = 0; i < alerts.size(); i++) {
					ImGui::PushID(alerts.at(i).uid);
					ImGui::InputText("", alerts.at(i).match_string, 128);
					ImGui::SameLine();
					if (ImGui::Button("x", ImVec2(24.0f, 0))) {
						alerts.erase(alerts.begin() + i);
					}
					ImGui::PopID();
				}
			}
			ImGui::End();
		}
		ImGui::PopTextWrapPos();
	}
	ImGui::End();
}

void TradeWindow::LoadSettings(CSimpleIni* ini) {
	ToolboxWindow::LoadSettings(ini);
	show_menubutton = ini->GetBoolValue(Name(), VAR_NAME(show_menubutton), true);
}

void TradeWindow::SaveSettings(CSimpleIni* ini) {
	ToolboxWindow::SaveSettings(ini);
}

void TradeWindow::Terminate() {
	chat.stop();
	ToolboxWindow::Terminate();
}