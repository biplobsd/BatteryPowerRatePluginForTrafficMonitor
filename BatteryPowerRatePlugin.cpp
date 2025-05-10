#include "pch.h"
#include "BatteryPowerRatePlugin.h"
#include "DataManager.h"

#include <Windows.h>
#include <SetupAPI.h>
#include <Batclass.h>
#include <devguid.h>
#include <winioctl.h>
#include <string>
#include <iostream>

#pragma comment(lib, "setupapi.lib")

// If GUID_DEVCLASS_BATTERY is still undefined, define it manually
#ifndef GUID_DEVCLASS_BATTERY
DEFINE_GUID(GUID_DEVCLASS_BATTERY, 0x72631E54L, 0x78A4, 0x11D0, 0xBC, 0xF7, 0x00, 0xAA, 0x00, 0xB7, 0xB3, 0x2A);
#endif

// If battery IOCTLs are undefined, define them manually
#ifndef IOCTL_BATTERY_QUERY_TAG
#define BATTERY_IOCTL_INDEX 0x0800
#define IOCTL_BATTERY_QUERY_TAG \
    CTL_CODE(FILE_DEVICE_BATTERY, BATTERY_IOCTL_INDEX + 0, METHOD_BUFFERED, FILE_READ_ACCESS)
#define IOCTL_BATTERY_QUERY_STATUS \
    CTL_CODE(FILE_DEVICE_BATTERY, BATTERY_IOCTL_INDEX + 3, METHOD_BUFFERED, FILE_READ_ACCESS)
#endif

#ifndef FILE_DEVICE_BATTERY
#define FILE_DEVICE_BATTERY 0x00000029
#endif

std::wstring GetBatteryPowerRate()
{
    std::wstring result = L"0.00 W";
    HDEVINFO hdev = SetupDiGetClassDevs(&GUID_DEVCLASS_BATTERY, 0, 0, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
    if (hdev == INVALID_HANDLE_VALUE)
        return result;

    SP_DEVICE_INTERFACE_DATA did = { 0 };
    did.cbSize = sizeof(did);

    // Use double for higher precision
    double totalRateMilliwatts = 0.0;
    bool foundBattery = false;

    // Enumerate all batteries in the system
    for (DWORD index = 0; SetupDiEnumDeviceInterfaces(hdev, 0, &GUID_DEVCLASS_BATTERY, index, &did); ++index)
    {
        DWORD cbRequired = 0;
        SetupDiGetDeviceInterfaceDetail(hdev, &did, 0, 0, &cbRequired, 0);
        if (cbRequired == 0)
            continue;

        PSP_DEVICE_INTERFACE_DETAIL_DATA pdidd = (PSP_DEVICE_INTERFACE_DETAIL_DATA)LocalAlloc(LPTR, cbRequired);
        if (!pdidd)
            continue;

        pdidd->cbSize = sizeof(*pdidd);
        if (!SetupDiGetDeviceInterfaceDetail(hdev, &did, pdidd, cbRequired, &cbRequired, 0))
        {
            LocalFree(pdidd);
            continue;
        }

        // Open a handle to the battery
        HANDLE hBattery = CreateFile(pdidd->DevicePath, GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        LocalFree(pdidd);

        if (hBattery == INVALID_HANDLE_VALUE)
            continue;

        // Get battery information
        BATTERY_QUERY_INFORMATION bqi = { 0 };
        DWORD dwWait = 0;
        DWORD dwOut;

        // First, get the battery tag
        if (DeviceIoControl(hBattery, IOCTL_BATTERY_QUERY_TAG, &dwWait, sizeof(dwWait), &bqi.BatteryTag, sizeof(bqi.BatteryTag), &dwOut, NULL)
            && bqi.BatteryTag)
        {
            // Now get the battery status - try multiple times to ensure accuracy
            BATTERY_WAIT_STATUS bws = { 0 };
            bws.BatteryTag = bqi.BatteryTag;

            BATTERY_STATUS bs = { 0 };

            // Get an accurate reading by averaging multiple samples
            const int NUM_SAMPLES = 3;
            int validSamples = 0;

            for (int sample = 0; sample < NUM_SAMPLES; sample++)
            {
                if (DeviceIoControl(hBattery, IOCTL_BATTERY_QUERY_STATUS, &bws, sizeof(bws), &bs, sizeof(bs), &dwOut, NULL))
                {
                    totalRateMilliwatts += bs.Rate;
                    validSamples++;
                    foundBattery = true;
                }

                // Small delay between samples
                Sleep(50);
            }

            // Average the readings if we got more than one
            if (validSamples > 1)
            {
                totalRateMilliwatts /= validSamples;
            }
        }
        CloseHandle(hBattery);
    }
    SetupDiDestroyDeviceInfoList(hdev);

    if (foundBattery)
    {
        // Convert milliwatts to watts with high precision
        double watts = totalRateMilliwatts / 1000.0;

        wchar_t buffer[32];

        // Use different format based on charging or discharging
        // Include 2 decimal places as requested
        if (watts > 0) {
            swprintf_s(buffer, L"%.2f W+", watts); // Charging (positive)
            result = buffer;
        }
        else if (watts < 0) {
            swprintf_s(buffer, L"%.2f W-", -watts); // Discharging (negative)
            result = buffer;
        }
        else {
            result = L"0.00 W"; // No power flow
        }
    }

    return result;
}

CBatteryPowerRatePlugin CBatteryPowerRatePlugin::m_instance;

CBatteryPowerRatePlugin::CBatteryPowerRatePlugin()
{
}

CBatteryPowerRatePlugin& CBatteryPowerRatePlugin::Instance()
{
    return m_instance;
}

IPluginItem* CBatteryPowerRatePlugin::GetItem(int index)
{
    switch (index)
    {
    case 0:
    return &m_battery_power;
    default:
        break;
}
    return nullptr;
}


void CBatteryPowerRatePlugin::DataRequired()
{
    CDataManager::Instance().m_cur_b_rate = GetBatteryPowerRate().c_str();
}

const wchar_t* CBatteryPowerRatePlugin::GetInfo(PluginInfoIndex index)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    static CString str;
    switch (index)
    {
    case TMI_NAME:
        return L"BatteryPowerPlugin";
    case TMI_DESCRIPTION:
        return L"Battery Power Rate Plugin for TrafficMonitor";
    case TMI_AUTHOR:
        return L"biplobsd";
    case TMI_COPYRIGHT:
        return L"Copyright (C) by Biplob Kumar Sutradhar 2025";
    case TMI_VERSION:
        return L"1.0";
    case ITMPlugin::TMI_URL:
        return L"https://github.com/biplobsd/BatteryPowerRatePluginForTrafficMonitor.git";
        break;
    default:
        break;
    }
    return L"";
}

const wchar_t* CBatteryPowerRatePlugin::GetTooltipInfo()
{   
	static CString str;
	str.Format(L"Battery power rate: %s", m_battery_power.GetItemValueText());
	return str;
}

ITMPlugin* TMPluginGetInstance()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    return &CBatteryPowerRatePlugin::Instance();
}
