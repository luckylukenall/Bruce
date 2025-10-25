#ifndef BRUCE_USER_SETTINGS_H
#define BRUCE_USER_SETTINGS_H

/**
 * @brief User-specific overrides for touch input, Wi-Fi credentials and WebUI behaviour.
 *
 * Update the values below to match your environment. Credentials are stored in flash on first
 * successful connection so future boots can reuse them.
 */

#ifndef USER_DISABLE_TOUCH_INPUT
#define USER_DISABLE_TOUCH_INPUT 0
#endif

#ifndef USER_WIFI_SSID
#define USER_WIFI_SSID ""
#endif

#ifndef USER_WIFI_PASSWORD
#define USER_WIFI_PASSWORD ""
#endif

#ifndef USER_AUTO_START_WEBUI
#define USER_AUTO_START_WEBUI 1
#endif

#endif // BRUCE_USER_SETTINGS_H
