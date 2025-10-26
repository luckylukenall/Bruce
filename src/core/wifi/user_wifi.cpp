#include "user_wifi.h"

#include "core/utils.h"
#include "core/wifi/webInterface.h"
#include "core/wifi/wifi_common.h"
#include <Arduino.h>
#include <globals.h>
#include <user_settings.h>

namespace {
String readConfiguredSsid() {
    return String(USER_WIFI_SSID);
}

String readConfiguredPassword() {
    return String(USER_WIFI_PASSWORD);
}

bool credentialsConfigured() {
    String ssid = readConfiguredSsid();
    ssid.trim();
    return !ssid.isEmpty();
}
}

bool connectUserWifiFromSettings() {
    if (wifiConnected || WiFi.status() == WL_CONNECTED) {
        wifiConnected = true;
        wifiIP = WiFi.localIP().toString();
        return true;
    }

    if (!credentialsConfigured()) { return false; }

    String ssid = readConfiguredSsid();
    String pwd = readConfiguredPassword();

    ssid.trim();
    pwd.trim();

    Serial.printf("Connecting to configured WiFi \"%s\"...\n", ssid.c_str());
    WiFi.mode(WIFI_MODE_STA);
    bool connected = _connectToWifiNetwork(ssid, pwd);
    if (connected) {
        wifiConnected = true;
        wifiIP = WiFi.localIP().toString();
        bruceConfig.addWifiCredential(ssid, pwd);
        updateClockTimezone();
        Serial.printf("Connected! IP address: %s\n", wifiIP.c_str());
    } else {
        wifiDisconnect();
        Serial.println("Failed to connect using configured WiFi credentials");
    }
    return connected;
}

void autoStartWebInterfaceFromSettings() {
#if USER_AUTO_START_WEBUI
    Serial.println("Auto-start WebUI: checking stored WiFi credentials before launch");
    bool connected = connectUserWifiFromSettings();
    if (isWebUIActive) return;

    if (connected) {
        Serial.println("Starting WebUI with existing WiFi connection");
        startWebUiBackground(false);
    } else {
        if (credentialsConfigured()) {
            Serial.println("Starting WebUI in AP mode because WiFi association failed");
        } else {
            Serial.println("No WiFi credentials configured, starting WebUI in AP mode");
        }
        startWebUiBackground(true);
    }
#else
    (void)connectUserWifiFromSettings();
#endif
}
