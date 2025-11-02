#include "nrf_common.h"
#include "../../core/display.h"
#include "../../core/mykeyboard.h"

RF24 NRFradio(NRF24_CE_PIN, NRF24_SS_PIN);
SPIClass *NRFSPI;

static Nrf24Module activeModule = Nrf24Module::Primary;

static BruceConfigPins::SPIPins &getBusForModule(Nrf24Module module) {
    return (module == Nrf24Module::Secondary) ? bruceConfigPins.NRF24_secondary_bus : bruceConfigPins.NRF24_bus;
}

bool nrf_has_module(Nrf24Module module) { return getBusForModule(module).isConfigured(); }

Nrf24Module nrf_get_active_module() { return activeModule; }

bool nrf_set_active_module(Nrf24Module module) {
    if (!nrf_has_module(module)) return false;
    activeModule = module;
    return true;
}

const BruceConfigPins::SPIPins &nrf_active_pins() {
    if (!nrf_has_module(activeModule)) {
        if (nrf_has_module(Nrf24Module::Primary)) activeModule = Nrf24Module::Primary;
        else if (nrf_has_module(Nrf24Module::Secondary)) activeModule = Nrf24Module::Secondary;
    }
    return getBusForModule(activeModule);
}

String nrf_module_display_name(Nrf24Module module) {
    switch (module) {
    case Nrf24Module::Secondary:
        return String(F("Secondary"));
    case Nrf24Module::Primary:
    default:
        return String(F("Primary"));
    }
}

bool nrf_require_active_module() {
    if (nrf_has_module(nrf_get_active_module())) return true;

    displayError("NRF module pins not configured");
    delay(750);
    return false;
}

static SPIClass *resolveSpiForPins(const BruceConfigPins::SPIPins &pins) {
    if (pins.mosi == (gpio_num_t)TFT_MOSI && pins.mosi != GPIO_NUM_NC) {
#if TFT_MOSI > 0
        return &tft.getSPIinstance();
#else
        return &SPI;
#endif
    }

    if (pins.mosi == bruceConfigPins.SDCARD_bus.mosi) {
        return &sdcardSPI;
    }

    if (pins.mosi == bruceConfigPins.CC1101_bus.mosi && pins.mosi != bruceConfigPins.SDCARD_bus.mosi) {
        return &CC_NRF_SPI;
    }

    return &SPI;
}

void nrf_info() {
    tft.fillScreen(bruceConfig.bgColor);
    tft.setTextSize(FM);
    tft.setTextColor(TFT_RED, bruceConfig.bgColor);
    tft.drawCentreString("_Disclaimer_", tftWidth / 2, 10, 1);
    tft.setTextColor(TFT_WHITE, bruceConfig.bgColor);
    tft.setTextSize(FP);
    tft.setCursor(15, 33);
    padprintln("These functions were made to be used in a controlled environment for STUDY only.");
    padprintln("");
    padprintln("DO NOT use these functions to harm people or companies, you can go to jail!");
    tft.setTextColor(bruceConfig.priColor, bruceConfig.bgColor);
    padprintln("");
    padprintln(
        "This device is VERY sensible to noise, so long wires or passing near VCC line can make "
        "things go wrong."
    );
    padprintln("");
    padprintln("Active module: " + nrf_module_display_name(nrf_get_active_module()));
    if (nrf_has_module(Nrf24Module::Secondary)) padprintln("Secondary module: available");
    else padprintln("Secondary module: not configured");
    padprintln("Use 'Active module' in the NRF24 menu to switch radios.");
    delay(1000);
    while (!check(AnyKeyPress));
}

bool nrf_start() {
    const auto &pins = nrf_active_pins();
    if (!pins.isConfigured()) return false;

    pinMode(pins.cs, OUTPUT);
    digitalWrite(pins.cs, HIGH);
    pinMode(pins.io0, OUTPUT);
    digitalWrite(pins.io0, LOW);

    NRFSPI = resolveSpiForPins(pins);
    NRFSPI->begin((int8_t)pins.sck, (int8_t)pins.miso, (int8_t)pins.mosi);
    delay(10);

    if (NRFradio.begin(
            NRFSPI,
            rf24_gpio_pin_t(pins.io0),
            rf24_gpio_pin_t(pins.cs)
        )) {
        return true;
    } else return false;
}
