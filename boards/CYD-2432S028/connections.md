# CYD-2432W328R / CYD-2432S024R External Connections

## NRF24 (single CN1 harness + microSD sniffer)

| NRF24 pin | CYD connector | ESP32 GPIO | Notes |
|-----------|---------------|------------|-------|
| GND       | CN1 pin 1     | GND        | Shared ground with board. |
| VCC (3V3) | CN1 pin 4     | 3V3        | Use 3.3 V only. |
| CE        | CN1 pin 2     | IO22       | Marked "IO22" on the silkscreen. |
| CSN       | CN1 pin 3     | IO21       | Marked as a second "IO22" on some revisions. |
| SCK       | microSD sniffer SCK | IO18 | Shared VSPI clock line. Remove the SD card while using the NRF24. |
| MOSI      | microSD sniffer MOSI | IO23 | Shared VSPI MOSI line. |
| MISO      | microSD sniffer MISO | IO19 | Shared VSPI MISO line. |
| IRQ (optional) | — | — | Not used by the firmware. |

### Adding a second NRF24L01 module

You can attach a second 2.4 GHz radio to the **Extended IO** header while
sharing the same SPI bus used by the CN1 module:

| NRF24 pin | CYD connector | ESP32 GPIO | Notes |
|-----------|---------------|------------|-------|
| GND       | Extended IO GND | GND      | Common ground with the board. |
| VCC (3V3) | Extended IO 3V3 | 3V3      | Use the 3.3 V rail only. |
| CE        | Extended IO IO16 | IO16    | Dedicated enable for the secondary module. |
| CSN       | Extended IO IO17 | IO17    | Independent chip-select line. |
| SCK       | Shared with primary | IO18 | Tie into the same SCK line as the CN1 harness. |
| MOSI      | Shared with primary | IO23 | Tie into the same MOSI line as the CN1 harness. |
| MISO      | Shared with primary | IO19 | Tie into the same MISO line as the CN1 harness. |

Leave the microSD card removed while either radio is active. Both modules can
remain wired at the same time; use the on-device **Active module** menu entry to
select which NRF24L01 the jammer and spectrum tools should control.

> ℹ️  The CYD-2432S024R revision routes IO21 and IO22 to the CN1 JST socket even if the silkscreen shows "IO22" twice. The updated firmware build maps the NRF24 CSN signal to IO21 and CE to IO22 so you can wire the module using only the CN1 harness for control and power plus the SD sniffer (or the original P3 breakout) for the shared SPI bus.

After flashing the `CYD-2432W328R-or-S024R` firmware target the NRF24 menu will automatically use the new pinout. No further
runtime configuration is required.
