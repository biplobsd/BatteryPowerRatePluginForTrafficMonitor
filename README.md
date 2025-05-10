# Battery Power Rate Plugin for TrafficMonitor

![GitHub release](https://img.shields.io/github/v/release/biplobsd/BatteryPowerRatePluginForTrafficMonitor)
![License](https://img.shields.io/github/license/biplobsd/BatteryPowerRatePluginForTrafficMonitor)

This is a plugin for [TrafficMonitor](https://github.com/zhongyang219/TrafficMonitor) that displays the battery power rate (in milliwatts) — indicating how fast your laptop is charging or discharging — in real time. It integrates seamlessly into the TrafficMonitor plugin system.

## 🖼️ Screenshot
![image](https://github.com/user-attachments/assets/1e26ee4d-0943-4dd6-80d9-d21d64f48c42)

## 🔌 Features

- Shows current battery power rate (charging/discharging in mW)

## 📦 Download

Head over to the [Releases](https://github.com/biplobsd/BatteryPowerRatePluginForTrafficMonitor/releases) section to download the latest compiled `BatteryPowerRatePlugin.dll`.

## 📂 Installation

1. Ensure you are using **TrafficMonitor v1.84.4 or later**.
2. Find or create a folder named `plugins` inside your TrafficMonitor installation directory.
3. Copy the `BatteryPowerRatePlugin.dll` into the `plugins` folder.
4. Restart TrafficMonitor.
5. Right-click on TrafficMonitor → `Plugin` → enable `BatteryPowerRatePlugin`.

## 🧑‍💻 Build Instructions

### Prerequisites

- Windows OS
- Visual Studio 2019 or later
- Desktop development with C++ workload

### Steps

1. Clone the repository:
   `git clone https://github.com/biplobsd/BatteryPowerRatePluginForTrafficMonitor.git`

2. Open the project by **double-clicking `PluginDemo.vcxproj`**.
3. In Visual Studio, select `Release` mode.
4. Build the project.
5. The output `BatteryPowerRatePlugin.dll` will appear in the `Release` folder.

