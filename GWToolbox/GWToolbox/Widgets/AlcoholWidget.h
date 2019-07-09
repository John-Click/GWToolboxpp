#pragma once

#include "ToolboxWidget.h"

#include <GWCA/Packets/StoC.h>
#include <GWCA\Constants\Maps.h>

class AlcoholWidget : public ToolboxWidget {
	AlcoholWidget() {};
	~AlcoholWidget() {};
private:
	DWORD alcohol_level = 0;
	time_t last_alcohol = 0;
	long alcohol_time = 0;
	long GetAlcoholTitlePointsGained(); // Returns amount of alcohol points gained since last check (or map load)
	long prev_alcohol_title_points = 0; // Used in GetAlcoholTitlePointsGained
	GW::Constants::MapID map_id;
	DWORD prev_packet_tint_6_level=0; // Record what last post processing packet was - for lunars check
public:
	static AlcoholWidget& Instance() {
		static AlcoholWidget instance;
		return instance;
	}
	
	const char* Name() const override { return "Alcohol"; }

	void Initialize() override;
	void Update(float delta) override;
	DWORD GetAlcoholLevel();
	long GetAlcoholTitlePoints(); // Gets current alcohol title points.
	bool AlcUpdate(GW::Packet::StoC::PostProcess *packet);

	// Draw user interface. Will be called every frame if the element is visible
	void Draw(IDirect3DDevice9* pDevice) override;

	void DrawSettingInternal() override;
};
