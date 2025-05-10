#pragma once
#include "PluginInterface.h"
#include "BatteryPower.h"

class CBatteryPowerRatePlugin : public ITMPlugin
{
private:
    CBatteryPowerRatePlugin();

public:
    static CBatteryPowerRatePlugin& Instance();

    // 通过 ITMPlugin 继承
    virtual IPluginItem* GetItem(int index) override;
    virtual void DataRequired() override;
    virtual const wchar_t* GetInfo(PluginInfoIndex index) override;
    virtual const wchar_t* GetTooltipInfo();

private:
    CBatteryPowerPlugin m_battery_power;

    static CBatteryPowerRatePlugin m_instance;
};

#ifdef __cplusplus
extern "C" {
#endif
    __declspec(dllexport) ITMPlugin* TMPluginGetInstance();

#ifdef __cplusplus
}
#endif
