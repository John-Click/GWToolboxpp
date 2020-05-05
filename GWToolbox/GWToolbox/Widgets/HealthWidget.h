#pragma once

#include "ToolboxWidget.h"
#include <Color.h>

class HealthWidget : public ToolboxWidget {
	HealthWidget() {};
	~HealthWidget() {};
public:
	static HealthWidget& Instance() {
		static HealthWidget instance;
		return instance;
	}

	const char* Name() const override { return "Health"; }

    void LoadSettings(CSimpleIni *ini) override;
	void SaveSettings(CSimpleIni *ini) override;
    void DrawSettingInternal() override;

	// Draw user interface. Will be called every frame if the element is visible
	void Draw(IDirect3DDevice9* pDevice) override;

    bool click_to_print_health = false;

    std::wstring agent_name_ping;
	bool hide_in_outpost = false;
	Color color_default = 0;
	Color color_below_ninety = 0;
	Color color_below_half = 0;
};
