#include "NRF24.h"
#include "core/display.h"
#include "core/utils.h"
#include "modules/NRF24/nrf_common.h"
#include "modules/NRF24/nrf_jammer.h"
#include "modules/NRF24/nrf_spectrum.h"

void NRF24Menu::optionsMenu() {
    options.clear();
    options.push_back({"Information", nrf_info});

    options.push_back({
        "Active module: " + nrf_module_display_name(nrf_get_active_module()),
        [=]() { moduleSelectMenu(); }
    });

    options.push_back({
        "Spectrum",
        []() {
            if (!nrf_require_active_module()) return;
            nrf_spectrum();
        }
    });

    options.push_back({
        "NRF Jammer",
        []() {
            if (!nrf_require_active_module()) return;
            nrf_jammer();
        }
    });

    options.push_back({
        "CH Jammer",
        []() {
            if (!nrf_require_active_module()) return;
            nrf_channel_jammer();
        }
    });
    options.push_back({
        "CH hopper",
        []() {
            if (!nrf_require_active_module()) return;
            nrf_channel_hopper();
        }
    });

#if defined(ARDUINO_M5STICK_C_PLUS) || defined(ARDUINO_M5STICK_C_PLUS2)
    options.push_back({"Config pins", [=]() { configMenu(); }});
#endif

    addOptionToMainMenu();

    loopOptions(options, MENU_TYPE_SUBMENU, "NRF24");
}

void NRF24Menu::moduleSelectMenu() {
    options.clear();

    auto makeLabel = [](Nrf24Module module, const char *name) {
        String label = name;
        if (nrf_get_active_module() == module) label += " (active)";
        return label;
    };

    options.push_back({
        makeLabel(Nrf24Module::Primary, "Primary"),
        [=]() {
            if (nrf_set_active_module(Nrf24Module::Primary)) {
                optionsMenu();
            } else {
                displayError("Primary module not configured");
                delay(750);
                moduleSelectMenu();
            }
        }
    });

    if (nrf_has_module(Nrf24Module::Secondary)) {
        options.push_back({
            makeLabel(Nrf24Module::Secondary, "Secondary"),
            [=]() {
                if (nrf_set_active_module(Nrf24Module::Secondary)) {
                    optionsMenu();
                } else {
                    displayError("Secondary module not configured");
                    delay(750);
                    moduleSelectMenu();
                }
            }
        });
    } else {
        options.push_back({
            "Secondary (not configured)",
            []() {
                displayError("Wire the secondary module and update NRF pins");
                delay(750);
            }
        });
    }

    options.push_back({"Back", [=]() { optionsMenu(); }});

    loopOptions(options, MENU_TYPE_SUBMENU, "NRF Module");
}

void NRF24Menu::configMenu() {
    uint8_t opt = 0;
    options = {
        {"NRF24 (legacy)",     [&]() { opt = 1; }      },
        {"NRF24 (shared SPI)", [&]() { opt = 2; }      },
        {"Back",               [=]() { optionsMenu(); }},
    };

    loopOptions(options, MENU_TYPE_SUBMENU, "RF Config");
    if (opt == 1) {
        bruceConfigPins.setNrf24Pins(
            {(gpio_num_t)NRF24_SCK_PIN,
             (gpio_num_t)NRF24_MISO_PIN,
             (gpio_num_t)NRF24_MOSI_PIN,
             (gpio_num_t)NRF24_SS_PIN,
             (gpio_num_t)NRF24_CE_PIN,
             GPIO_NUM_NC}
        );
    }
    if (opt == 2) {
        bruceConfigPins.setNrf24Pins(
            {(gpio_num_t)SDCARD_SCK,
             (gpio_num_t)SDCARD_MISO,
             (gpio_num_t)SDCARD_MOSI,
             GPIO_NUM_33,
             GPIO_NUM_32,
             GPIO_NUM_NC}
        );
    }
}
void NRF24Menu::drawIconImg() {
    drawImg(
        *bruceConfig.themeFS(), bruceConfig.getThemeItemImg(bruceConfig.theme.paths.nrf), 0, imgCenterY, true
    );
}
void NRF24Menu::drawIcon(float scale) {
    clearIconArea();
    int iconW = scale * 80;
    int iconH = scale * 60;

    if (iconW % 2 != 0) iconW++;
    if (iconH % 2 != 0) iconH++;

    int caseW = 3 * iconW / 4;
    int caseH = 2 * iconH / 3;
    int caseX = iconCenterX - iconW / 2;
    int caseY = iconCenterY - iconH / 6;

    int antW = iconW / 8;
    int connR = iconH / 20;

    // Case
    tft.drawRect(caseX, caseY, caseW, caseH, bruceConfig.priColor);

    // Antenna
    tft.fillRect(caseX + caseW, caseY + caseH / 2 - antW / 2, antW, antW, bruceConfig.priColor);
    tft.fillRoundRect(
        caseX + caseW + antW,
        caseY + caseH - iconH,
        antW,
        iconH - caseH / 2 + antW / 2,
        antW / 2,
        bruceConfig.priColor
    );

    // Connectors
    tft.fillCircle(caseX + caseW / 6, caseY + 1 * caseH / 5, connR, bruceConfig.priColor);
    tft.fillCircle(caseX + caseW / 6, caseY + 2 * caseH / 5, connR, bruceConfig.priColor);
    tft.fillCircle(caseX + caseW / 6, caseY + 3 * caseH / 5, connR, bruceConfig.priColor);
    tft.fillCircle(caseX + caseW / 6, caseY + 4 * caseH / 5, connR, bruceConfig.priColor);

    tft.fillCircle(caseX + caseW / 3, caseY + 1 * caseH / 5, connR, bruceConfig.priColor);
    tft.fillCircle(caseX + caseW / 3, caseY + 2 * caseH / 5, connR, bruceConfig.priColor);
    tft.fillCircle(caseX + caseW / 3, caseY + 3 * caseH / 5, connR, bruceConfig.priColor);
    tft.fillCircle(caseX + caseW / 3, caseY + 4 * caseH / 5, connR, bruceConfig.priColor);

    // Chip
    tft.fillRect(
        caseX + caseW - 2 * antW - connR, caseY + caseH / 2 - antW / 2, antW, antW, bruceConfig.priColor
    );
}
