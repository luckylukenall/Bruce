#pragma once

/**
 * @brief User-specific overrides for touch input, Wi-Fi credentials and WebUI behaviour.
 *
 * Update the values below to match your environment. Credentials are stored in flash on first
 * successful connection so future boots can reuse them.
 */

#ifndef USER_DISABLE_TOUCH_INPUT
#define USER_DISABLE_TOUCH_INPUT 1
#endif

#ifndef USER_WIFI_SSID
#define USER_WIFI_SSID "II"
#endif

#ifndef USER_WIFI_PASSWORD
#define USER_WIFI_PASSWORD "theconstant.6!"
#endif

#ifndef USER_AUTO_START_WEBUI
#define USER_AUTO_START_WEBUI 1
#endif
