#include <RF24.h>
#include <globals.h>

// Define the Macros case it hasn't been declared
#ifndef NRF24_CE_PIN
#define NRF24_CE_PIN -1
#endif
#ifndef NRF24_SS_PIN
#define NRF24_SS_PIN -1
#endif

extern RF24 NRFradio;
extern SPIClass *NRFSPI;

enum class Nrf24Module : uint8_t {
    Primary = 0,
    Secondary = 1,
};

bool nrf_has_module(Nrf24Module module);
bool nrf_set_active_module(Nrf24Module module);
Nrf24Module nrf_get_active_module();
const BruceConfigPins::SPIPins &nrf_active_pins();
String nrf_module_display_name(Nrf24Module module);
bool nrf_require_active_module();

bool nrf_start();

void nrf_info();
