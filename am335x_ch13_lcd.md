# [cite_start]13.1 Introduction [cite: 4]

## [cite_start]13.1.1 Purpose of the Peripheral [cite: 5]

[cite_start]The LCD controller consists of two independent controllers, the Raster Controller and the LCD Interface Display Driver (LIDD) controller[cite: 7]. [cite_start]Each controller operates independently from the other and only one of them is active at any given time[cite: 8].

### Raster Controller
[cite_start]The Raster Controller handles the synchronous LCD interface[cite: 9]. [cite_start]It provides timing and data for constant graphics refresh to a passive display[cite: 9]. [cite_start]It supports a wide variety of monochrome and full-color display types and sizes by use of programmable timing controls, a built-in palette, and a gray-scale/serializer[cite: 10]. [cite_start]Graphics data is processed and stored in frame buffers[cite: 11]. [cite_start]A frame buffer is a contiguous memory block in the system[cite: 11]. [cite_start]A built-in DMA engine supplies the graphics data to the Raster engine which, in turn, outputs to the external LCD device[cite: 12].

### LIDD Controller
[cite_start]The LIDD Controller supports the asynchronous LCD interface[cite: 13]. [cite_start]It provides full-timing programmability of control signals (CS, WE, OE, ALE) and output data[cite: 13].

### Architecture and Data Path
[cite_start]Figure 13-1 shows the LCD controller details[cite: 14]. [cite_start]The raster and LIDD Controllers are responsible for generating the correct external timing[cite: 14]. [cite_start]The DMA engine provides a constant flow of data from the frame buffer(s) to the external LCD panel via the Raster and LIDD Controllers[cite: 15]. [cite_start]In addition, CPU access is provided to read and write registers[cite: 16]. [cite_start]The solid, thick lines in Figure 13-1 indicate the data path[cite: 17]. [cite_start]The Raster Controller's data path is fairly complicated, for a thorough description of the Raster Controller data path, see Section 13.3.6[cite: 18].

[Figure: Block diagram of the LCD Controller (Figure 13-1). The diagram shows a DMA block communicating with the CPU and an LCD block. The LCD block contains an Input FIFO, Palette RAM, Gray-scaler/serializer, Output FIFO, Raster controller, LIDD controller, and configuration Registers. [cite_start]MUXes route the final signals to outputs including LCD_D[23:0], LCD_VSYNC, LCD_HSYNC, LCD_PCLK, LCD_AC_ENB_CS, and LCD_MCLK.] [cite: 20, 21, 22, 23, 29, 30, 31, 36, 37, 40, 41, 42, 43, 44, 45]

---

## [cite_start]13.1.2 Features [cite: 51]

[cite_start]General features of the LCD Controller include: [cite: 52]

* Supports up to 24-bit data output; [cite_start]8 bits-per-pixel (RGB). [cite: 53]
* [cite_start]Supports up to 2048x2048 resolution. [cite: 54]
* [cite_start]Integrated DMA engine to pull data from the external frame buffer without burdening the processor via interrupts or a firmware timer. [cite: 56]
* [cite_start]512 word deep internal FIFO with programmable threshold values. [cite: 57]

### [cite_start]Character Based Panels [cite: 58]
* [cite_start]Supports 2 Character Panels (CSO and CS1) with independent and programmable bus timing parameters when in asynchronous Hitachi, Motorola and Intel modes. [cite: 59]
* [cite_start]Supports 1 Character Panel (CSO) with programmable bus timing parameters when in synchronous Motorola and Intel modes. [cite: 60]
* [cite_start]Can be used as a generic 16 bit address/data interleaved MPU bus master with no external stall. [cite: 61]

### [cite_start]Passive Matrix LCD Panels [cite: 61]
* [cite_start]Panel types including STN, DSTN, and C-DSTN. [cite: 62]
* [cite_start]AC Bias Control. [cite: 63]

### [cite_start]Active Matrix LCD Panels [cite: 64]
* [cite_start]Panel types including TN TFT. [cite: 65]

### [cite_start]OLED Panels [cite: 66]
* [cite_start]Passive Matrix (PM OLED) with frame buffer and controller IC inside the Panel. [cite: 67]
* [cite_start]Active Matrix (AM OLED). [cite: 67]

# 13.2 Integration

[cite_start]The device includes an LCD Controller that reads display data from external memory and drives several different types of LCD displays[cite: 75]. [cite_start]The LCD Controller integration is shown in Figure 13-2[cite: 76].

[Figure: Block diagram of LCD Controller Integration (Figure 13-2). Shows the LCD Controller interfacing with the L3Fast Interconnect via DMA Master Interface, L4Peripheral Interconnect via CFG Interface, MPU Subsystem Interrupts, and PRCM. [cite_start]It outputs to LCDC Pads including LCD_PCLK, LCD_DATA[15:0], LCD_DATA[23:16], LCD_HSYNC, LCD_VSYNC, and LCD_MEMORY_CLK.] [cite: 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98]

---

## 13.2.1 LCD Controller Connectivity Attributes

[cite_start]The general connectivity attributes for the LCDC subsystems are shown in Table 13-1[cite: 100].

| Attributes | Type |
| :--- | :--- |
| **Power Domain** | [cite_start]Peripheral Domain [cite: 101] |
| **Clock Domain** | [cite_start]PD_PER_LCD_L3_GCLK (OCP Master Clock) [cite: 101][cite_start]<br>PD_PER_LCD_L3_GCLK (OCP Slave Clock) [cite: 101][cite_start]<br>PD_PER_LCD_GCLK (Functional Clock) [cite: 101] |
| **Reset Signals** | [cite_start]PER_DOM_RST_N [cite: 101] |
| **Idle/Wakeup Signals** | [cite_start]Standby [cite: 101][cite_start]<br>Smart Idle [cite: 101] |
| **Interrupt Requests** | [cite_start]1 Interrupt to MPU Subsystem (LCDCINT) [cite: 101] |
| **DMA Requests** | [cite_start]None [cite: 101] |
| **Physical Address** | [cite_start]L4 Peripheral Slave Port [cite: 101] |

---

## 13.2.2 LCD Controller Clock and Reset Management

[cite_start]The LCDC module uses the following functional and OCP interface clocks[cite: 112]. [cite_start]The L4 Slave interface runs at half the frequency of the L3 Master interface but uses the same clock[cite: 113]. [cite_start]The clock is divided within the LCDC through the `l4_clkdiv` input using the `pd_per_lcd_l4s_gclk_ien` signal from the PRCM[cite: 114]. [cite_start]The functional clock comes from the Display PLL[cite: 115]. [cite_start]When the Display PLL is in bypass mode, its output is sourced by either `CORE_CLKOUTM6` or `PER_CLKOUTM2`[cite: 115].

| Clock Signal | Max Freq | Reference / Source | Comments |
| :--- | :--- | :--- | :--- |
| [cite_start]**l3_clk**<br>Master Interface Clock [cite: 117] | [cite_start]200 MHz [cite: 117] | [cite_start]CORE_CLKOUTM4 [cite: 117] [cite_start]| pd_per_lcd_l3_gclk<br>From PRCM [cite: 117] |
| [cite_start]**l4_clk**<br>Slave Interface Clock [cite: 117] | [cite_start]100 MHz [cite: 117] | [cite_start]CORE_CLKOUTM4<br>(divided within LCDC) [cite: 117] [cite_start]| pd_per_lcd_l3_gclk<br>From PRCM [cite: 117] |
| [cite_start]**lcd_clk**<br>Functional Clock [cite: 117] | [cite_start]200 MHz [cite: 117] | [cite_start]Display PLL CLKOUT [cite: 117] [cite_start]| pd_per_lcd_gclk<br>From PRCM [cite: 117] |

---

## 13.2.3 LCD Controller Pin List

[cite_start]The LCD Controller external interface signals are shown in Table 13-3[cite: 119].

| Pin | Type | Description |
| :--- | :--- | :--- |
| [cite_start]`lcd_cp` [cite: 120] | [cite_start]O [cite: 120] | [cite_start]Pixel Clock in Raster mode / Read Strobe or ReadWrite /Strobe in LIDD mode [cite: 120] |
| [cite_start]`lcd_pixel_i[15:0]` [cite: 120] | [cite_start]I [cite: 120] | [cite_start]LCD Data Bus input (for LIDD mode only) [cite: 120] |
| [cite_start]`lcd_pixel_o[23:0]` [cite: 120] | [cite_start]- [cite: 120] | [cite_start]LCD Data Bus output [cite: 120] |
| [cite_start]`lcd_lp` [cite: 120] | [cite_start]O [cite: 120] | Line Clock or HSYNC in Raster mode; [cite_start]Write Strobe or Direction bit in LIDD mode [cite: 120] |
| [cite_start]`lcd_fp` [cite: 120] | [cite_start]- [cite: 120] | Frame Clock or VSYNC in Raster mode; [cite_start]Address Latch Enable in LIDD mode [cite: 120] |
| [cite_start]`lcd_ac` [cite: 120] | [cite_start]O [cite: 120] | AC bias or Latch Enable in Raster mode; [cite_start]Primary Chip Select/Primary Enable in LIDD MPU/Hitachi mode [cite: 120] |
| [cite_start]`lcd_mclk` [cite: 120] | [cite_start]O [cite: 120] | N/A in Raster mode; [cite_start]Memory Clock/Secondary chip Select/Secondary Enable in LIDD Synchronous/Async MPU/Hitachi mode [cite: 120] |

# [cite_start]13.3 Functional Description [cite: 127]

## [cite_start]13.3.1 Clocking [cite: 128]

[cite_start]This section details the various clocks and signals[cite: 130]. [cite_start]Figure 13-3 shows input and output LCD controller clocks[cite: 130].

[Figure: Figure 13-3. Input and Output Clocks. [cite_start]Diagram illustrating the LCD Controller receiving the LCD_CLK input and providing the "Pixel Clock Derived from LCD_CLK", "HSYNC/Line Clock", and "VSYNC/Frame Clock" to the Display.] [cite: 131, 132, 133, 134, 135, 136, 137]

### [cite_start]13.3.1.1 Pixel Clock (LCD_PCLK) [cite: 138]

[cite_start]The pixel clock (`LCD_PCLK`) frequency is derived from `LCD_CLK`, the reference clock to this LCD module[cite: 139]. [cite_start]The pixel clock is used by the LCD display to clock the pixel data into the line shift register[cite: 140]. 

[cite_start]$LCD\_PCLK = \frac{LCD\_CLK}{CLKDIV}$ [cite: 141, 142]

[cite_start]Where `CLKDIV` is a field in the `LCD_CTRL` register and should not be 0 or 1[cite: 143].

* [cite_start]**Passive (STN) mode:** `LCD_PCLK` only transitions when valid data is available for output[cite: 144]. It does not transition when the horizontal clock (`HSYNC`) is asserted or during wait state insertion[cite: 145].
* [cite_start]**Active (TFT) mode:** `LCD_PCLK` continuously toggles as long as the Raster Controller is enabled[cite: 146].

### [cite_start]13.3.1.2 Horizontal Clock (LCD_HSYNC) [cite: 153]

[cite_start]`LCD_HSYNC` toggles after all pixels in a horizontal line have been transmitted to the LCD and a programmable number of pixel clock wait states has elapsed both at the beginning and end of each line[cite: 155]. The `RASTER_TIMING_0` register fully defines the behavior of this signal[cite: 156]. `LCD_HSYNC` can be programmed to be synchronized with the rising or falling edge of `LCD_PCLK`[cite: 157]. The configuration field is bits 24 and 25 in the `RASTER_TIMING_2` register[cite: 158].

* [cite_start]**Active (TFT) mode:** The horizontal clock or the line clock is also used by TFT displays as the horizontal synchronization signal (`LCD_HSYNC`)[cite: 159]. The timings of the horizontal clock (line clock) pins are programmable to support:
    * [cite_start]Delay insertion both at the beginning and end of each line[cite: 161].
    * [cite_start]Line clock polarity[cite: 162].
    * [cite_start]Line clock pulse width, driven on rising or falling edge of pixel clock[cite: 163].

### [cite_start]13.3.1.3 Vertical Clock (LCD_VSYNC) [cite: 164]

[cite_start]`LCD_VSYNC` toggles after all lines in a frame have been transmitted to the LCD and a programmable number of line clock cycles has elapsed both at the beginning and end of each frame[cite: 165]. [cite_start]The `RASTER_TIMING_1` register fully defines the behavior of this signal[cite: 166]. [cite_start]`LCD_VSYNC` can be programmed to be synchronized with the rising or falling edge of `LCD_PCLK`[cite: 167]. [cite_start]The configuration field is bits 24 and 25 in the `RASTER_TIMING_2` register[cite: 168].

* [cite_start]**Passive (STN) mode:** The vertical, or frame, clock toggles during the first line of the screen[cite: 169].
* [cite_start]**Active (TFT) mode:** The vertical, or frame, clock is also used by TFT displays as the vertical synchronization signal (`LCD_VSYNC`)[cite: 170]. The timings of the vertical clock pins are programmable to support:
    * [cite_start]Delay insertion both at the beginning and end of each frame[cite: 172].
    * [cite_start]Frame clock polarity[cite: 173].

### [cite_start]13.3.1.4 LCD_AC_BIAS_EN [cite: 174]

* [cite_start]**Passive (STN) mode:** To prevent a dc charge within the screen pixels, the power and ground supplies of the display are periodically switched[cite: 175]. [cite_start]The Raster Controller signals the LCD to switch the polarity by toggling this pin (`LCD_AC_BIAS_EN`)[cite: 176].
* [cite_start]**Active (TFT) mode:** This signal acts as an output enable (OE) signal[cite: 177]. [cite_start]It is used to signal the external LCD that the data is valid on the data bus (`LCD_DATA`)[cite: 178].

---

## [cite_start]13.3.2 LCD External I/O Signals [cite: 185]

[cite_start]Table 13-4 shows the details of the LCD controller external signals[cite: 186].

| Signal | Type | Description |
| :--- | :--- | :--- |
| `LCD_VSYNC` | OUT | **Raster controller:** Frame clock the LCD uses to signal the start of a new frame of pixels. Also used by TFT displays as the vertical synchronization signal. <br>**LIDD character:** Register select (RS) or address latch enable (ALE). [cite_start]<br>**LIDD graphics:** Address bit 0 (A0) or command/data select (C/D). [cite: 189] |
| `LCD_HSYNC` | OUT | **Raster controller:** Line clock the LCD uses to signal the end of a line of pixels that transfers line data from the shift register to the screen and to increment the line pointer(s). Also used by TFT displays as the horizontal synchronization signal. <br>**LIDD character:** Not used. <br>**LIDD graphics:** 6800 mode = read or write enable; [cite_start]8080 mode = not write strobe. [cite: 189] |
| `LCD_PCLK` | OUT | **Raster controller:** Pixel clock the LCD uses to clock the pixel data into the line shift register. In passive mode, the pixel clock transitions only when valid data is available on the data lines. In active mode, the pixel clock transitions continuously, and the ac-bias pin is used as an output enable to signal when data is available on the LCD pin. <br>**LIDD character:** Not used. <br>**LIDD graphics:** 6800 mode = enable strobe; [cite_start]8080 mode = not read strobe. [cite: 189] |
| `LCD_AC_BIAS_EN` | OUT | **Raster controller:** AC-bias used to signal the LCD to switch the polarity of the power supplies to the row and column axis of the screen to counteract DC offset. Used in TFT mode as the output enable to signal when data is latched from the data pins using the pixel clock. <br>**LIDD character:** Primary enable strobe. [cite_start]<br>**LIDD graphics:** Chip select 0 (CS0). [cite: 189] |
| `LCD_MCLK` | OUT | **Raster controller:** Not used. <br>**LIDD character:** Secondary enable strobe. [cite_start]<br>**LIDD graphics:** Chip select 1 (CS1). [cite: 189] |
| `LCD_D[23:0]` | Raster: OUT<br>LIDD: OUT/IN | **Raster controller (OUT):** LCD data bus, providing a 4-, 8-, 16-or 24-bit data path. For monochrome displays, each signal represents a pixel; for passive color displays, groupings of three signals represent one pixel (red, green, and blue). `LCD_DATA[3:0]` is used for monochrome displays of 2, 4, and 8 BPP; `LCD_DATA[7:0]` is used for color STN displays and `LCD_DATA[15:0]` or `LCD_DATA[23:0]` is used for active (TFT) mode. <br>**LIDD character (OUT/IN):** `LCD_DATA[15:0]` Read and write the command and data registers. [cite_start]<br>**LIDD graphics (OUT/IN):** `LCD_DATA[15:0]` Read and write the command and data registers. [cite: 189] |

---

## [cite_start]13.3.3 Pin Mapping and Color Assignments [cite: 190]

[cite_start]Due to a silicon bug, pin mapping for the data signals for RGB888 and RGB565 are not as designed[cite: 191]. [cite_start]Refer to the AM335x Silicon Errata (SPRZ360) for proper pin mapping and color assignments when using these modes with an LCD panel[cite: 192].
