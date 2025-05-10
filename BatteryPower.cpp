
#include "pch.h"
#include <windows.h>
#include <string>
#include "BatteryPower.h"
#include <powerbase.h>
#include "DataManager.h"

CBatteryPowerPlugin::CBatteryPowerPlugin() {
}

const wchar_t* CBatteryPowerPlugin::GetItemName() const {
    return L"BRate";
}

const wchar_t* CBatteryPowerPlugin::GetItemId() const {
    return L"BatteryPowerPluginID";
}

const wchar_t* CBatteryPowerPlugin::GetItemLableText() const {
    return L"";
}

const wchar_t* CBatteryPowerPlugin::GetItemValueText() const {
    return CDataManager::Instance().m_cur_b_rate.c_str();
}

const wchar_t* CBatteryPowerPlugin::GetItemValueSampleText() const {
    return L"12.5 W";
}
