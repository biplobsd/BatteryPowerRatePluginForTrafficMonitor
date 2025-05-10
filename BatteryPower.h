#pragma once

#include "PluginInterface.h"
#include <string>

class CBatteryPowerPlugin : public IPluginItem {
public:
    CBatteryPowerPlugin();
    virtual ~CBatteryPowerPlugin() = default;

    const wchar_t* GetItemName() const override;
    const wchar_t* GetItemId() const override;
    const wchar_t* GetItemLableText() const override;
    const wchar_t* GetItemValueText() const override;
    const wchar_t* GetItemValueSampleText() const override;

private:
    std::wstring m_valueText;
    void UpdatePowerInfo(); // Function to fetch power info
};

