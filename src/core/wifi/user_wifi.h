#pragma once

#include <stdbool.h>

/**
 * @brief Attempt to connect using the Wi-Fi credentials defined in user_settings.h.
 *
 * @return true when a connection is established or already active.
 */
bool connectUserWifiFromSettings();

/**
 * @brief Ensure the WebUI is started according to the user settings.
 *
 * Tries to join the configured Wi-Fi network and, when enabled, launches the
 * WebUI without blocking the main menu. Falls back to AP mode if association
 * fails so the device remains reachable.
 */
void autoStartWebInterfaceFromSettings();
