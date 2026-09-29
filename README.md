
<p align="center">
  <img src="docs/images/hero.png" alt="ESP32 Logger – a battery-powered event counter with a big button and an 8-digit LED display" width="100%">
</p>

<p align="center">
  <a href="https://www.printables.com/model/1565668-esp32-logger"><img src="https://img.shields.io/badge/Printables-ESP32%20Logger-FA6831" alt="Printables"></a>
  <img src="https://img.shields.io/badge/ESP32--C3-Arduino-00979D" alt="ESP32-C3 · Arduino">
  <img src="https://img.shields.io/badge/log-CSV%20on%20microSD-3DDC97" alt="CSV on microSD">
  <a href="LICENSE"><img src="https://img.shields.io/github/license/18Markus1984/ESP_Logger?color=F5B83D" alt="MIT license"></a>
</p>

# ESP32 Logger

**ESP32 Logger** is a standalone, battery-powered **event counter** built around an ESP32-C3.
Press the big button and it counts one event, writes a timestamped line to a microSD card and shows
today's count on an 8-digit LED display. Pressed by mistake? A long press takes it back.

I built it to count visitors, but it works for anything you want to tally by hand: people
coming through a door, parts coming off a machine, defects on a line. It gets its time from the
internet, and when there is no Wi-Fi you set the clock with the same button. An Excel workbook
turns the log into heatmaps and daily summaries.

It is a slightly overengineered answer to a tally sheet, and that was the fun of it.

---

## Contents

- [Highlights](#highlights)
- [How it works](#how-it-works)
- [Hardware](#hardware)
- [Wiring](#wiring)
- [PCB](#pcb)
- [3D printing](#3d-printing)
- [Assembly](#assembly)
- [Firmware](#firmware)
- [Log file](#log-file)
- [Evaluation workbook](#evaluation-workbook)
- [Repository layout](#repository-layout)
- [Lessons learned](#lessons-learned)
- [Credits and license](#credits-and-license)

---

## Highlights

| | |
|---|---|
| 🔴 **One big button** | Click to count, long press to undo. The button's LED doubles as a status light. |
| 🔢 **8-digit LED display** | MAX7219 7-segment display: all-time total at start-up, then today's count. |
| 💾 **CSV on microSD** | Every press is one line `Date;Time;Counter`. Opens directly in Excel. |
| 🕒 **Network time** | NTP sync on start-up with automatic summer/winter time (CET/CEST). |
| 📶 **Wi-Fi setup portal** | Hold the button while switching on and a captive portal opens, including fields for a static IP. |
| ✏️ **Offline clock editor** | No Wi-Fi? Set date and time with the button, prefilled with the last logged time. |
| 🔋 **Battery powered** | 1000 mAh LiPo, USB-C charging, four charge LEDs visible through translucent printed windows. |
| 📊 **Evaluation workbook** | Heatmap day × time, daily summary, time-of-day and weekday patterns. Paste the CSV, done. |

---

## How it works

<p align="center">
  <img src="docs/images/startup.gif" alt="Start-up: the total count, the blinking LED while syncing the time, then today's count" width="60%">
</p>

At start-up the display shows the **total number of presses** stored on the card. While the logger
fetches the time, the LED in the button blinks. As soon as the clock is set, the display switches
to **today's count**, and every click raises it by one.

<p align="center">
  <img src="docs/images/controls.svg" alt="Button controls: switching on, counting and the offline clock editor" width="100%">
</p>

| LED | Meaning |
|---|---|
| blinking (0.5 s) | waiting for network time |
| on | Wi-Fi portal is open, or the button is being pressed |
| three short blinks | last entry removed, or next field in the clock editor |

---

## Hardware

<p align="center">
  <img src="docs/images/gallery-1.png" width="32%">
  <img src="docs/images/gallery-2.png" width="32%">
  <img src="docs/images/gallery-3.png" width="32%">
</p>

| Part | Qty | Notes |
|---|:-:|---|
| [ESP32-C3 SuperMini](https://de.aliexpress.com/item/1005005967641936.html) | 1 | the brain, plugs into female headers on the PCB |
| [MAX7219 8-digit 7-segment module](https://www.amazon.de/dp/B07Z7RLGC2) | 1 | shows the count and the clock editor |
| [microSD card module](https://de.aliexpress.com/item/1005006297859728.html) | 1 | SPI, plus a microSD card (FAT32) |
| [Big arcade button with LED](https://www.amazon.de/dp/B0C1K7T5SG) | 1 | sold as a 12 V button, the LED is swapped for a 3 V one |
| [3 V LED](https://www.amazon.de/dp/B0CXDS8LDL) | 1 | goes into the button as status LED |
| [LiPo 3.7 V, 802540, 1000 mAh](https://de.aliexpress.com/item/1005002970377289.html) | 1 | held in place with a strip of insulating tape |
| [Charge / boost module, 3.7 V in, 5 V 2 A out](https://de.aliexpress.com/item/1005006156633451.html) | 1 | battery management with four charge LEDs |
| [USB-C port](https://de.aliexpress.com/item/1005007593502706.html) | 1 | charging socket, clamped by the case |
| [Slide switch](https://www.amazon.de/dp/B008R50AA0) | 1 | fixed with two M2 screws |
| [JST connectors](https://www.amazon.de/dp/B07PRWF4BV) | – | every part is pluggable |
| [Female pin headers 2.54 mm](https://www.amazon.de/dp/B0BZHBMF15) | – | sockets for the ESP32 on the PCB |
| M3 nuts + M3 countersunk screws | 3 | close the case |
| M2 screws and nuts | ~6 | switch, SD module and PCB |
| Wires, copper-clad board | – | for the PCB, see below |

---

## Wiring

<p align="center">
  <img src="docs/images/wiring.svg" alt="Wiring diagram: button, LED, MAX7219 and SD module to the ESP32-C3 SuperMini" width="100%">
</p>

| From | To (GPIO) |
|---|---|
| Button switch | **2** (internal pull-up, other side to GND) |
| Button LED + | **3** |
| MAX7219 **DIN** / **CLK** / **CS** | **4** / **5** / **6** |
| SD module **CS** / **SCK** / **MOSI** / **MISO** | **9** / **10** / **20** / **21** |
| Charge module 5 V out (via switch) | **5V** of the ESP32, display and SD module |

The pins are defined at the top of [`ESPCounter.ino`](ESPCounter/ESPCounter.ino) if you want to
move something. I like having the battery management on its own module instead of connecting the
battery directly to the microcontroller board. The PCB only sees a clean 5 V.

<p align="center">
  <img src="docs/images/circuit-breadboard.webp" alt="Breadboard view of the circuit" width="49%">
  <img src="docs/images/circuit-soldered.webp" alt="Soldered PCB" width="49%">
</p>

---

## PCB

<p align="center">
  <img src="docs/images/pcb-layout.webp" alt="Two-layer PCB layout in Fritzing" width="70%">
</p>

The PCB is designed in **Fritzing** and made in a local makerspace with **three different lasers**,
starting from a plain copper-clad board:

1. **CO₂ laser:** cut the outline, scaled up by 10 %. The board gets properly burned here, so it is ground clean afterwards.
2. **Fiber laser:** remove all copper except traces and lettering. This takes a while: the fiber laser only takes off a little copper per pass, and too much power burns the glass fibre underneath.
3. **Acrylic paint:** spray the whole board as a solder mask.
4. **Diode laser:** free the pads. It removes the paint but not the copper, so only the pads are exposed.
5. **Drill** the four mounting holes in the corners.

<p align="center">
  <img src="docs/images/pcb-finished.jpg" alt="Finished laser-made PCB" width="60%">
</p>

The Fritzing projects and the per-layer exports (copper, mask, silk; normal and mirrored, PDF/SVG)
for every revision are in [`PCB/`](PCB).

---

## 3D printing

<p align="center">
  <img src="docs/images/print-parts.webp" alt="Printed parts; the battery windows are marked in the bottom right" width="70%">
</p>

- The model is in [`3d/Counter.3mf`](3d/Counter.3mf) and on [Printables](https://www.printables.com/model/1565668-esp32-logger).
- Print the top and bottom cover with **tree supports**. Take care when removing the support at the USB-C opening of the bottom cover.
- The small marked parts in the bottom right are the **battery windows**. Print them in a translucent material or a different colour so the charge LEDs shine through.

---

## Assembly

### 1. Nuts

Press three M3 nuts into the case: two in the top cover, one directly below the USB-C port.
The bottom one is tight. Put the nut in, pull it into place with a long screw, then swap to a
shorter one.

<p align="center">
  <img src="docs/images/nuts-1.webp" width="32%">
  <img src="docs/images/nuts-2.webp" width="32%">
  <img src="docs/images/nuts-3.webp" width="32%">
</p>

### 2. Power section

Mount the charge module with the battery and the switch. The switch is held by two M2 screws
straight into the plastic, the battery by a strip of insulating tape. Push the USB-C cable through
the slit and into place; the outer case keeps it there. Connect the battery and test the
module: **if all four LEDs light up, it works.**

<p align="center">
  <img src="docs/images/power-1.webp" width="19%">
  <img src="docs/images/power-2.webp" width="19%">
  <img src="docs/images/power-3.webp" width="19%">
  <img src="docs/images/power-4.webp" width="19%">
  <img src="docs/images/power-5.webp" width="19%">
</p>

### 3. Top cover

Push the LED display through the rectangular opening. Insert the big button without its
electronics into the round hole, then use the button's mechanism to push it into the button housing.

<p align="center">
  <img src="docs/images/top-1.webp" width="32%">
  <img src="docs/images/top-2.webp" width="32%">
  <img src="docs/images/top-3.webp" width="32%">
</p>

### 4. Electronics

Screw the SD module to the right side with two M2 screws. Plug everything into the PCB with the
JST connectors and place the PCB, with M2 nuts from the other side. Connect power last.

<p align="center">
  <img src="docs/images/mount-1.webp" width="24%">
  <img src="docs/images/mount-2.webp" width="24%">
  <img src="docs/images/mount-3.webp" width="24%">
  <img src="docs/images/mount-4.webp" width="24%">
</p>

### 5. Close

Close the case with three M3 countersunk screws from the outside into the nuts from step 1.

<p align="center">
  <img src="docs/images/close-1.webp" width="40%">
  <img src="docs/images/close-2.webp" width="40%">
</p>

---

## Firmware

### Toolchain

| | |
|---|---|
| Arduino IDE | 2.x |
| Board package | **esp32 by Espressif** |
| [LedControl](https://github.com/wayoda/LedControl) | MAX7219 display |
| [WiFiManager](https://github.com/tzapu/WiFiManager) (tzapu) | Wi-Fi setup portal |
| [OneButton](https://github.com/mathertel/OneButton) (Matthias Hertel) | click, double click, long press |
| SPI, SD, WiFi | included in the board package |

Board settings: **ESP32C3 Dev Module** and **USB CDC On Boot: Enabled**, so the serial monitor
works over the SuperMini's USB port.

### Flash and first start

1. Open [`ESPCounter/ESPCounter.ino`](ESPCounter/ESPCounter.ino) and upload via USB.
2. Insert a FAT32 microSD card. The logger creates `/log.csv` with a header on first start.
3. **Hold the button and switch on.** Connect your phone to the Wi-Fi **`EventCounter-Setup`**, choose your network and enter the password. Static IP, gateway and DNS can be set there too.
4. From now on it connects by itself. If no known network is found, the portal opens again for up to three minutes, then the logger carries on.

> **Different time zone?** Change the POSIX string in `initTime()`:
> `configTzTime("CET-1CEST,M3.5.0/2,M10.5.0/3", ...)`.

### Program flow

<p align="center">
  <img src="docs/images/flow-setup.png" alt="Flowchart of setup()" width="49%">
  <img src="docs/images/flow-loop.png" alt="Flowchart of loop() and the button event handlers" width="49%">
</p>

`setup()` connects to Wi-Fi, mounts the SD card and reads the last counter value from `log.csv`,
then waits up to 100 s for network time. Without it, the offline clock editor takes over and
the manually set time becomes the system time. `loop()` only has to call `button.tick()`.
OneButton does the rest and calls the click and long-press handlers.

A small detail: the Wi-Fi transmit power is lowered to 8.5 dBm (`WIFI_POWER_8_5dBm`). Some C3
SuperMini boards fail to connect at full power, so only raise it if you need the extra range.

---

## Log file

Every click appends one line to `/log.csv`, every long press removes the last one:

```
Date;Time;Counter
08.05.2026;9:39;1
08.05.2026;9:42;2
08.05.2026;9:43;3
```

`Counter` is the running total over all days. On start-up the logger reads it from the last line,
so the card is the only memory. Swap the card and you start from zero. The same file also
tells the clock editor where to start.

---

## Evaluation workbook

<p align="center">
  <img src="docs/images/data-flow.svg" alt="From log.csv to the Time-of-Day chart" width="100%">
</p>

[`Evaluation excel/Event_Frequency_Analysis.xlsx`](Evaluation%20excel/Event_Frequency_Analysis.xlsx)
turns the raw CSV into something you can read at a glance. Paste the rows of `log.csv` into the
**RawData** sheet and everything else fills in by itself:

| Sheet | What it shows |
|---|---|
| **Heatmap Day x Time** | one row per day, 48 half-hour slots, colour scale shows when things happen |
| **Daily Summary** | total per day, weekday, first and last event |
| **Time-of-Day Distribution** | all events in half-hour bins over 24 h, with percentage share |
| **Weekday x Time** | the same grid aggregated by weekday, e.g. quiet Mondays, busy Friday afternoons |

It is built on `COUNTIFS` and `SUMPRODUCT` against the RawData table, so new rows at the bottom
are picked up automatically. The labels say "Event" instead of "Visitor", so it fits whatever
you count. The **Instructions** sheet explains the details.

---

## Repository layout

```
ESP_Logger/
├── ESPCounter/ESPCounter.ino      firmware (Arduino sketch)
├── 3d/Counter.3mf                 case, cover and battery windows
├── PCB/
│   ├── *.fzz                      Fritzing projects
│   └── BesucherTasterPlatine/     laser exports per revision (V1 … V2.2)
├── Evaluation excel/              Event_Frequency_Analysis.xlsx
└── docs/images/                   pictures for this README
```

---

## Lessons learned

I thought this would take a week or two. It took about three months as a side project. The
biggest time sink was not the counter but getting a reliable Wi-Fi connection on my university
network. It works now. Along the way I learned a lot about fast PCB production and about what
a fiber laser can and cannot do to a copper board.

<p align="center">
  <img src="docs/images/gallery-4.png" width="32%">
  <img src="docs/images/gallery-5.png" width="32%">
  <img src="docs/images/gallery-6.png" width="32%">
</p>

---

## Credits and license

- [LedControl](https://github.com/wayoda/LedControl) by Eberhard Fahle
- [WiFiManager](https://github.com/tzapu/WiFiManager) by tzapu
- [OneButton](https://github.com/mathertel/OneButton) by Matthias Hertel
- Case files and full build log on [Printables](https://www.printables.com/model/1565668-esp32-logger)

Released under the [MIT license](LICENSE). Made by [@18Markus1984](https://github.com/18Markus1984)
(Max Siebenschläfer on Printables). Issues and pull requests are welcome.
