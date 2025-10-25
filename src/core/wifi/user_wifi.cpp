#include "user_wifi.h"

#include "core/utils.h"
#include "core/wifi/webInterface.h"
#include "core/wifi/wifi_common.h"
#include <Arduino.h>
#include <globals.h>
#include <user_settings.h>

namespace {
bool credentialsConfigured() {
    const char *ssid = USER_WIFI_SSID;
    return ssid && ssid[0] != '\0';
}
}

bool connectUserWifiFromSettings() {
    if (wifiConnected || WiFi.status() == WL_CONNECTED) {
        wifiConnected = true;
        wifiIP = WiFi.localIP().toString();
        return true;
    }

    if (!credentialsConfigured()) { return false; }

    const char *ssid = USER_WIFI_SSID;
    const char *pwd = USER_WIFI_PASSWORD;

    Serial.printf("Connecting to configured WiFi \"%s\"...\n", ssid);
    WiFi.mode(WIFI_MODE_STA);
    bool connected = _connectToWifiNetwork(String(ssid), String(pwd));
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
