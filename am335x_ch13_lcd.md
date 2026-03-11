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
### 13.3.4 DMA Engine

[cite_start]The DMA engine provides the capability to output graphics data to constantly refresh LCDs, without burdening the CPU, via interrupts or a firmware timer[cite: 200]. [cite_start]It operates on one or two frame buffers, which are set up during initialization[cite: 201]. [cite_start]Using two frame buffers (ping-pong buffers) enables the simultaneous operation of outputting the current video frame to the external display and updating the next video frame[cite: 202]. [cite_start]The ping-pong buffering approach is preferred in most applications[cite: 203].

[cite_start]When the Raster Controller is used, the DMA engine reads data from a frame buffer and writes it to the input FIFO (as shown in Figure 13-1)[cite: 204]. [cite_start]The Raster Controller requests data from the FIFO for frame refresh; [cite: 205] [cite_start]as a result, the DMA's job is to ensure that the FIFO is always kept full[cite: 206].

[cite_start]When the LIDD Controller is used, the DMA engine accesses the LIDD Controller's address and/or data registers[cite: 207]. [cite_start]To program the DMA engine, configure the following registers, as shown in Table 13-5[cite: 208].

**Table 13-5. Register Configuration for DMA Engine Programming**

| Register | Configuration |
| :--- | :--- |
| `LCDDMA_CTRL` | [cite_start]Configure DMA data format [cite: 210] |
| `LCDDMA_FB0_BASE` | [cite_start]Configure frame buffer 0 [cite: 210] |
| `LCDDMA_FB0_CEILING` | [cite_start]Configure frame buffer 0 [cite: 210] |
| `LCDDMA_FB1_BASE` | [cite_start]Configure frame buffer 1. (If only one frame buffer is used, these two registers will not be used.) [cite: 210] |
| `LCDDMA_FB1_CEILING` | [cite_start]Configure frame buffer 1. (If only one frame buffer is used, these two registers will not be used.) [cite: 210] |

[cite_start]In addition, the `LIDD_CTRL` register (for LIDD Controller) or the `RASTER_CTRL` register (for Raster Controller) should also be configured appropriately, along with all the timing registers[cite: 211]. [cite_start]To enable DMA transfers, the `LIDD_DMA_EN` bit (in the `LIDD_CTRL` register) or the `LCDEN` bit (in the `RASTER_CTRL` register) should be written with 1[cite: 212].

> [cite_start]**CAUTION:** Writes to RAM will fail when they are in the vicinity of where the DMA Engine is reading[cite: 214]. [cite_start]Thus, for screen updates to occur smoothly, rendering must be done to a buffer that is not currently in use by the DMA engine[cite: 215]. [cite_start]The between-frames period (immediately after the end-of-frame interrupts) works well to BLIT a "back buffer" (double-buffering) to the "front buffer" where DMA reads next[cite: 216]. [cite_start]This works well, and enables application firmware to let the DMA engine continue reading from the same frame buffer[cite: 217]. [cite_start]Alternatively, the location of where the DMA engine is reading from can be changed to an alternate (pre-prepared) frame buffer[cite: 218]. [cite_start]When doing so at the end-of-frame interrupt, by the time the ISR knows it is an EOF0 interrupt (for example), the DMA engine has already read its BASE and CEILING addresses for the next frame (FrameBuffer1)[cite: 219]. [cite_start]So the DMA BASE and CEILING addresses that can be changed safely at this point would be for FrameBuffer0 (the same buffer as the frame that just completed)[cite: 220]. [cite_start]Then the DMA engine starts using these addresses on the next frame[cite: 221]. [cite_start]If FrameBuffer1 BASE and CEILING are also updated, those take effect the next time the DMA engine reads them (after 2 frames)[cite: 222].

---

#### 13.3.4.1 Interrupts

[cite_start]Interrupts in this LCD module are related to DMA engine operation[cite: 224]. [cite_start]Four registers are used to control and monitor the interrupts: [cite: 225]

* [cite_start]The `IRQENABLE_SET` register allows the user to enable any of the interrupt sources[cite: 226].
* [cite_start]The `IRQENABLE_CLEAR` register allows the user to disable interrupts sources[cite: 227].
* [cite_start]The `IRQSTATUS_RAW` register collects all the interrupt status information[cite: 227].
* [cite_start]The `IRQSTATUS` register collects the interrupt status information for all enabled interrupts[cite: 234].

[cite_start]Any interrupt source not enabled in the `IRQENABLE_SET` register is masked out[cite: 235].

##### 13.3.4.1.1 LIDD Mode
[cite_start]When operating in LIDD mode, the DMA engine generates one interrupt signal every time the specified frame buffer has been transferred completely[cite: 237]. [cite_start]The `DONE` bit in the `LIDD_CTRL` register specifies if the interrupt signal is delivered to the system interrupt controller, which in turn may or may not generate an interrupt to CPU[cite: 238]. [cite_start]The `EOF1`, `EOF0`, and `DONE` bits in the `IRQSTATUS_RAW` register reflect the interrupt signal, regardless of being delivered to the system interrupt controller or not[cite: 239].

##### 13.3.4.1.2 Raster Mode
[cite_start]When operating in Raster mode, the DMA engine can generate the interrupts in the following scenarios: [cite: 241]

1.  [cite_start]**Output FIFO under-run:** This occurs when the DMA engine cannot keep up with the data rate consumed by the LCD (which is determined by the `LCD_PCLK`.)[cite: 242, 243]. [cite_start]This is likely due to a system memory throughput issue or an incorrect `LCD_PCLK` setting[cite: 243]. The `FUF` bit in `IRQSTATUS_RAW` is set when this error occurs[cite: 244]. [cite_start]This bit is cleared by writing a 1 to the `FUF` bit in the `IRQSTATUS` register[cite: 245].
2.  [cite_start]**Frame synchronization lost:** This error happens when the DMA engine attempts to read what it believes to be the first word of the video buffer but it cannot be recognized as such[cite: 246]. [cite_start]This could be caused by an invalid frame buffer address or an invalid BPP value (for more details, see Section 13.3.6.2)[cite: 247]. [cite_start]The `SYNC` bit in the `IRQSTATUS_RAW` register is set when such an error is detected[cite: 248]. This bit is cleared by writing a 1 to the `SYNC` bit in the `IRQSTATUS` register[cite: 249].
3.  [cite_start]**Palette loaded:** When using palette-only or palette+data modes, the `PL` bit in the `IRQSTATUS_RAW` register will be set when the palette portion of a DMA transfer has been loaded into palette RAM[cite: 250]. This interrupt can be cleared by writing a '1' to the `PL` bit in the `IRQSTATUS` register[cite: 251].
4.  [cite_start]**AC bias transition:** If the `ACB_I` bit in the `RASTER_TIMING_2` register is programmed with a non-zero value, an internal counter will be loaded with this value and starts to decrement each time `LCD_AC_BIAS_EN` (AC-bias signal) switches its state[cite: 252]. [cite_start]When the counter reaches zero, the `ACB` bit in the `IRQSTATUS_RAW` register is set, which will deliver an interrupt signal to the system interrupt controller (if the interrupt is enabled.)[cite: 253]. [cite_start]The counter reloads the value in field `ACB_I`, but does not start to decrement until the `ACB` bit is cleared by writing 1 to this bit in the `IRQSTATUS` register[cite: 253].
5.  [cite_start]**Frame transfer completed:** When one frame of data is transferred completely, the `DONE` bit in the `IRQSTATUS_RAW` register is set[cite: 254]. Note that the `EOF0` and `EOF1` bits in the `IRQSTATUS_RAW` register will be set accordingly[cite: 255]. [cite_start]This bit is cleared by writing a 1 to the corresponding interrupt in the `IRQSTATUS` register[cite: 256].

[cite_start]Note that the interrupt enable bits are in the `IRQENABLE_SET` register[cite: 257]. The corresponding enable bit must be set in order to generate an interrupt to the CPU[cite: 258]. However, the `IRQSTATUS_RAW` register reflects the interrupt signal regardless of the interrupt enable bits settings[cite: 259].

##### 13.3.4.1.3 Interrupt Handling
[cite_start]See Chapter 6, Interrupts, for information about LCD interrupt number to CPU[cite: 261]. [cite_start]The interrupt service routine needs to determine the interrupt source by examining the `IRQSTATUS_RAW` register and clearing the interrupt properly[cite: 262].

---

### 13.3.5 LIDD Controller

[cite_start]The LIDD Controller is designed to support LCD panels with a memory-mapped interface[cite: 264]. [cite_start]The types of displays range from low-end character monochrome LCD panels to high-end TFT smart LCD panels[cite: 265].

[cite_start]LIDD mode (and the use of this logic) is enabled by clearing the `MODESEL` bit in the LCD control register (`LCD_CTRL`)[cite: 266]. [cite_start]LIDD Controller operation is summarized as follows: [cite: 267]

* [cite_start]During initialization, the LCD LIDD CS0/CS1 configuration registers (`LIDD_CS0_CONF` and `LIDD_CS1_CONF`) are configured to match the requirements of the LCD panel being used[cite: 268].
* [cite_start]During normal operation, the CPU writes display data to the LCD data registers (`LIDD_CS0_DATA` and `LIDD_CS1_DATA`)[cite: 275]. [cite_start]The LIDD interface converts the CPU write into the proper signal transition sequence for the display, as programmed earlier[cite: 276].
* [cite_start]Note that the first CPU write should send the beginning address of the update to the LCD panel and the subsequent writes update data at display locations starting from the first address and continuing sequentially[cite: 277]. [cite_start]Note that DMA may be used instead of CPU[cite: 278].

[cite_start]The LIDD Controller is also capable of reading back status or data from the LCD panel, if the latter has this capability[cite: 279]. [cite_start]This is set up and activated in a similar manner to the write function described above[cite: 280].

[cite_start]**NOTE:** If an LCD panel is not used, this interface can be used to control any MCU-like peripheral[cite: 281]. [cite_start]See your device-specific data manual to check the LIDD features supported by the LCD controller[cite: 282].

[cite_start]Table 13-6 describes how the signals are used to interface external LCD modules, which are configured by the `LIDD_CTRL` register[cite: 283].

**Table 13-6. LIDD I/O Name Map**

| Interface Type / Display Type | `LIDD_CTRL[2:0]` | I/O Name | Data Bits | Display I/O Name | Comment |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **Character Display**<br>HD44780 Type | `100` | `LCD_DATA[7:4]`<br>`LCD_AC_BIAS_EN`<br>`LCD_HSYNC`<br>`LCD_VSYNC`<br>`LCD_MCLK` | 4 | DATA[7:4]<br>E (or E0)<br>R/W<br>RS<br>E1 | [cite_start]Data Bus (length defined by Instruction)<br>Enable Strobe (first display)<br>ReadWrite/<br>Register Select (Data/not Instruction)<br>Enable Strobe (second display optional) [cite: 285] |
| **Character Display**<br>HD44780 Type | `100` | `LCD_DATA[7:0]`<br>`LCD_AC_BIAS_EN`<br>`LCD_HSYNC`<br>`LCD_VSYNC`<br>`LCD_MCLK` | 8 | DATA[7:0]<br>E (or E0)<br>R/W<br>RS<br>E1 | [cite_start]Data Bus (length defined by Instruction)<br>Enable Strobe (first display)<br>ReadWrite/<br>Register Select (Data/not Instruction)<br>Enable Strobe (second display optional) [cite: 285] |
| **Micro Interface Graphic Display**<br>6800 Family | `001`<br>`000` | `LCD_DATA[15:0]`<br>`LCD_PCLK`<br>`LCD_HSYNC`<br>`LCD_VSYNC`<br>`LCD_AC_BIAS_EN`<br>`LCD_MCLK`<br>`LCD_MCLK` | Up to 16 | DATA[15:0]<br>E<br>R/W<br>A0<br>CS (or CS0)<br>CS1<br>None | [cite_start]Data Bus (16 bits always available)<br>Enable Clock<br>ReadWrite/<br>Address/Data Select<br>Chip Select (first display)<br>Chip Select (second display optional)<br>Synchronous Clock (optional) [cite: 285] |
| **Micro Interface Graphic Display**<br>8080 Family | `011`<br>`010` | `LCD_DATA[15:0]`<br>`LCD_PCLK`<br>`LCD_HSYNC`<br>`LCD_VSYNC`<br>`LCD_AC_BIAS_EN`<br>`LCD_MCLK`<br>`LCD_MCLK` | Up to 16 | DATA[15:0]<br>RD<br>WR<br>A0<br>CS (or CS0)<br>CS1<br>None | [cite_start]Data Bus (16 bits always available)<br>Read Strobe<br>Write Strobe<br>Address/Data Select<br>Chip Select (first display)<br>Chip Select (second display optional)<br>Synchronous Clock (optional) [cite: 285] |

---

#### 13.3.5.1 LIDD Controller Timing

[cite_start]The timing parameters are defined by the `LIDD_CS0_CONF` and `LIDD_CS1_CONF` registers, which are described in and[cite: 292]. [cite_start]The timing configuration is based on an internal reference clock, MCLK[cite: 293]. [cite_start]The MCLK is generated out of `LCD_CLK`, which is determined by the `CLKDIV` bit in the `LCD_CTRL` register[cite: 294].

[cite_start]$MCLK = LCD\_CLK$ when $CLKDIV = 0$. [cite: 296]
[cite_start]$MCLK = \frac{LCD\_CLK}{CLKDIV}$ when $CLKDIV \neq 0$. [cite: 299]

[cite_start]See your device-specific data manual for the timing configurations supported by the LCD controller[cite: 300].

[Figure: Figure 13-4. LIDD Mode HD44780 Write Timing Diagram. [cite_start]Displays waveforms for MCLK, LCD_DATA, LCD_PCLK, LCD_VSYNC, LCD_HSYNC, and LCD_AC_BIAS_EN during a write sequence[cite: 302].]

[Figure: Figure 13-5. LIDD Mode HD44780 Read Timing Diagram. [cite_start]Displays waveforms for MCLK, LCD_DATA, LCD_PCLK, LCD_VSYNC, LCD_HSYNC, and LCD_AC_BIAS_EN during a read sequence[cite: 331].]

[Figure: Figure 13-6. LIDD Mode 6800 Write Timing Diagram. [cite_start]Details write timing states including W_SU, W_STROBE, W_HOLD, and CS_DELAY[cite: 359].]

[Figure: Figure 13-7. LIDD Mode 6800 Read Timing Diagram. [cite_start]Details read timing states including R_SU, R_STROBE, R_HOLD, and CS_DELAY[cite: 407].]

[Figure: Figure 13-8. LIDD Mode 6800 Status Timing Diagram. [cite_start]Details status read timings[cite: 422].]

[Figure: Figure 13-9. LIDD Mode 8080 Write Timing Diagram. [cite_start]Details 8080-specific write timing sequences[cite: 430].]

[Figure: Figure 13-10. LIDD Mode 8080 Read Timing Diagram. [cite_start]Details 8080-specific read timing sequences[cite: 465].]

[Figure: Figure 13-11. LIDD Mode 8080 Status Timing Diagram. [cite_start]Details 8080-specific status timings[cite: 489].]

[cite_start]**NOTE:** The `CS_DELAY` in above figures is same as bit field `TA` in `LCDLIDDCSOCONFIG`[cite: 501].

### 13.3.6 Raster Controller

[cite_start]Raster mode (and the use of this logic) is enabled by setting the `MODESEL` bit in the LCD control register (`LCD_CTRL`)[cite: 515]. [cite_start]Table 13-7 shows the active external signals when this mode is active[cite: 516].

**Table 13-7. [cite_start]Operation Modes Supported by Raster Controller** [cite: 517, 518]

| Interface | Data Bus Width | Register Bits (`RASTER_CTRL[9, 7, 1]`) | Signal Name | Description |
| :--- | :--- | :--- | :--- | :--- |
| **Passive (STN) Mono 4-bit** | 4 | `001` | `LCD_DATA[3:0]`<br>`LCD_PCLK`<br>`LCD_HSYNC`<br>`LCD_VSYNC`<br>`LCD_AC_BIAS_EN`<br>`LCD_MCLK` | Data bus<br>Pixel clock<br>Horizontal clock(Line Clock)<br>Vertical clock (Frame Clock)<br>AC Bias<br>Not used |
| **Passive (STN) Mono 8-bit** | 8 | `101` | `LCD_DATA[7:0]`<br>`LCD_PCLK`<br>`LCD_HSYNC`<br>`LCD_VSYNC`<br>`LCD_AC_BIAS_EN`<br>`LCD_MCLK` | Data bus<br>Pixel clock<br>Horizontal clock(Line Clock)<br>Vertical clock (Frame Clock)<br>AC Bias<br>Not used |
| **Passive (STN) Color** | 8 | `100` | `LCD_DATA[7:0]`<br>`LCD_PCLK`<br>`LCD_HSYNC`<br>`LCD_VSYNC`<br>`LCD_AC_BIAS_EN`<br>`LCD_MCLK` | Data bus<br>Pixel clock<br>Horizontal clock(Line Clock)<br>Vertical clock (Frame Clock)<br>AC Bias<br>Not used |
| **Active (TFT) Color** | 16 | `x10` | `LCD_DATA[15:0]`<br>`LCD_PCLK`<br>`LCD_HSYNC`<br>`LCD_VSYNC`<br>`LCD_AC_BIAS_EN`<br>`LCD_MCLK` | Data bus<br>Pixel clock<br>Horizontal clock(Line Clock)<br>Vertical clock (Frame Clock)<br>Output enable<br>Not used |

---

#### 13.3.6.1 Logical Data Path

[cite_start]The block diagram of the Raster Controller is shown in Figure 13-1[cite: 526]. [cite_start]Figure 13-12 illustrates its logical data path for various operation modes (passive (STN) versus active (TFT), various BPP size)[cite: 527]. 

[Figure: Figure 13-12. Logical Data Path for Raster Controller. [cite_start]Displays data flow from Data source (frame buffers) to Input FIFO, branching into STN (passive) and TFT (active) paths, passing through Palette, Gray-scaler/serializer, and Output FIFO to the Output pins] [cite: 531-546].

Figure 13-12 shows that:
* The gray-scaler/serializer and output FIFO blocks are bypassed in active (TFT) modes[cite: 529].
* [cite_start]The palette is bypassed in both 12- and 16-BPP modes[cite: 530].

**In summary:**
* [cite_start]The display image is stored in frame buffers[cite: 548].
* The built-in DMA engine constantly transfers the data stored in the frame buffers to the Input FIFO[cite: 549].
* [cite_start]The Raster Controller relays data to the external pins according to the specified format[cite: 550].

[cite_start]The remainder of this section describes the functioning blocks in Figure 13-12, including frame buffers, palette, and gray-scaler/serializer[cite: 551]. [cite_start]Their operation and programming techniques are covered in detail[cite: 551]. [cite_start]The output format is also described in Section 13.3.6.5[cite: 552].

---

#### 13.3.6.2 Frame Buffer

[cite_start]A frame buffer is a contiguous memory block, storing enough data to fill a full LCD screen[cite: 561]. [cite_start]For this device, external memory needs to be used for the frame buffer[cite: 562]. [cite_start]For specific details on which external memory interface (EMIF) controller can be accessed by the LCD controller, see your device-specific data manual[cite: 563]. 

[cite_start]The data in the frame buffer consists of pixel values as well as a look-up palette[cite: 564]. 

[Figure: Figure 13-13. Frame Buffer Structure. For 1, 2, 4, 12, 16, 24 BPP Modes: Palette is 32 bytes followed by Pixel Data. [cite_start]For 8 BPP Mode: Palette is 512 bytes followed by Pixel Data] [cite: 566-576].

**NOTE:**
* [cite_start]8-BPP mode uses the first 512 bytes in the frame buffer as the palette while the other modes use 32 bytes[cite: 577].
* 12-, 16-, and 24-BPP modes do not need a palette; i.e., the pixel data is the desired RGB value[cite: 578]. However, the first 32 bytes are still considered a palette[cite: 579]. The first entry should be `4000h` (bit 14 is 1) while the remaining entries must be filled with 0[cite: 580]. 
* [cite_start]Each entry in a palette occupies 2 bytes[cite: 581]. [cite_start]As a result, 8-BPP mode palette has 256 color entries while the other palettes have up to 16 color entries[cite: 581].
* 4-BPP mode uses up all the 16 entries in a palette[cite: 582].
* [cite_start]1-BPP mode uses the first 2 entries in a palette while 2-BPP mode uses the first 4 entries[cite: 583]. [cite_start]The remaining entries are not used and must be filled with 0[cite: 584].
* In 12- and 16-BPP modes, pixel data is RGB data[cite: 585]. For all the other modes, pixel data is actually an index of the palette entry[cite: 586].

**Table 13-8. [cite_start]Bits-Per-Pixel Encoding for Palette Entry 0 Buffer** [cite: 592, 593]

| Bit (14-12) Name: `BPP` | Value | Description |
| :--- | :--- | :--- |
| `000` | 1 BPP | Eight 1-bit pixels. |
| `001` | 2 BPP | Four 2-bit pixels. |
| `010` | 4 BPP | Two 4-bit pixels. |
| `011` | 8 BPP | Packed into each byte. |
| `1xx` | 12 BPP | 12 BPP in passive mode (`TFT_STN = 0` and `STN_565 = 0` in `RASTER_CTRL`). |
| `1xx` | 16 BPP | 16 BPP in passive mode (`TFT_STN = 0` and `STN_565 = 1` in `RASTER_CTRL`). |
| `1xx` | 16 BPP | 16 BPP in active mode (`LCDTFT = 1` and `TFT24 = 0` in `RASTER_CTRL`). |
| `1xx` | 24 BPP | 24 BPP in active mode (`LCDTFT = 1` and `TFT24 = 1` in `RASTER_CTRL`). |

[cite_start]*Note 1: Eight 1-bit pixels, four 2-bit pixels, and two 4-bit pixels are packed into each byte, and 12-bit pixels are right justified on (16-bit) word boundaries (in the same format as palette entry)[cite: 596].*
[cite_start]*Note 2: For Raw Data (12/16/24 bpp) framebuffers, no Palette lookup is employed therefore `PALMODE = 0x10` in `RASTER_CTRL`[cite: 600].*

[cite_start]The equations shown in Table 13-9 are used to calculate the total frame buffer size (in bytes) based on varying pixel size encoding and screen sizes[cite: 601].

**Table 13-9. [cite_start]Frame Buffer Size According to BPP** [cite: 603, 604]

| BPP | Frame Buffer Size (Formula) |
| :--- | :--- |
| 1 | $32 + (Lines \times Columns) / 8$ |
| 2 | $32 + (Lines \times Columns) / 4$ |
| 4 | $32 + (Lines \times Columns) / 2$ |
| 8 | $512 + (Lines \times Columns)$ |
| 12 / 16 | $32 + 2 \times (Lines \times Columns)$ |

##### Memory Organization within Frame Buffer

[cite_start]Figure 13-14 and Figure 13-15 show more detail of the palette entry organization[cite: 602]. [cite_start]Figure 13-16 through Figure 13-21 show the memory organization within the frame buffer for each pixel encoding size[cite: 714].

[Figure: Figure 13-14. 16-Entry Palette/Buffer Format (1, 2, 4, 12, 16 BPP) and Figure 13-15. 256-Entry Palette/Buffer Format (8 BPP). [cite_start]Shows bit breakdowns for Red, Green, Blue, and Mono values, along with Base address offsets.] [cite: 611-711].

* **Figure 13-16. [cite_start]16-BPP Data Memory Organization (TFT Mode Only)-Little Endian:** Base = Pixel 0, Base + 2 = Pixel 1 [cite: 715, 736-738].
* **Figure 13-17. [cite_start]12-BPP Data Memory Organization-Little Endian:** Unused [15-12] bits are filled with zeroes in TFT mode[cite: 745, 771].
* **Figure 13-18. [cite_start]8-BPP Data Memory Organization:** Base = Pixel 0, Base + 1 = Pixel 1, Base + 2 = Pixel 2 [cite: 772, 786-791].
* **Figure 13-19. 4-BPP Data Memory Organization:** Base = Pixel 0, Pixel 1; Base + 1 = Pixel 2, Pixel 3 [cite: 792, 804-809].

**Table: Figure 13-20. [cite_start]2-BPP Data Memory Organization** [cite: 818, 819]

| Memory Address | Bits 7-6 | Bits 5-4 | Bits 3-2 | Bits 1-0 |
| :--- | :--- | :--- | :--- | :--- |
| **Base** | Pixel 0 | Pixel 1 | Pixel 2 | Pixel 3 |
| **Base + 1** | Pixel 4 | Pixel 5 | Pixel 6 | Pixel 7 |
| **Base + 2** | Pixel 8 | Pixel 9 | Pixel 10 | Pixel 11 |

**Table: Figure 13-21. [cite_start]1-BPP Data Memory Organization** [cite: 820, 821]

| Memory Address | Bit 7 | Bit 6 | Bit 5 | Bit 4 | Bit 3 | Bit 2 | Bit 1 | Bit 0 |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| **Base** | P0 | P1 | P2 | P3 | P4 | P5 | P6 | P7 |
| **Base + 1** | P8 | P9 | P10 | P11 | P12 | P13 | P14 | P15 |

---

#### 13.3.6.3 Palette

As explained in the previous section, the pixel data is an index of palette entry (when palette is used)[cite: 825]. The number of colors supported is given by $2^{BPP}$[cite: 826]. However, due to a limitation of the gray-scaler/serializer block, fewer grayscales or colors may be supported[cite: 826, 827].

The `PLM` field (in `RASTER_CTRL`) affects the palette loading[cite: 828]:
* [cite_start]If `PLM` is `00b` (palette-plus-data mode) or `01b` (palette-only mode), the palette is loaded by the DMA engine at the very beginning, which is followed by the loading of pixel data[cite: 829].
* If `PLM` is `10b` (data-only mode), the palette is not loaded. [cite_start]Instead, the DMA engine loads the pixel data immediately[cite: 830].

---

#### 13.3.6.4 Gray-Scaler/Serializer

##### 13.3.6.4.1 Passive (STN) Mode
[cite_start]Once a palette entry is selected from the look-up palette by the pixel data, its content is sent to the gray-scaler/serializer[cite: 833]. 
* [cite_start]If it is monochrome data, it is encoded as 4 bits[cite: 834]. 
* If it is color data, it is encoded as 4 bits (Red), 4 bits (Green), and 4 bits (Blue)[cite: 835]. 

These 4-bit values are used to select one of the 16 intensity levels, as shown in Table 13-10[cite: 836]. A patented algorithm is used during this processing to provide an optimized intensity value that matches the eye's visual perception of color/gray gradations[cite: 837].

##### 13.3.6.4.2 Active (TFT) Mode
The gray-scaler/serializer is bypassed[cite: 839].

**Table 13-10. [cite_start]Color/Grayscale Intensities and Modulation Rates** [cite: 846, 847]

| Dither Value (4-Bit Value from Palette) | Intensity (0% is White) | Modulation Rate (Ratio of ON to ON+OFF Pixels) |
| :--- | :--- | :--- |
| `0000` | 0.0% | 0 |
| `0001` | 14.3% | $1/7$ |
| `0010` | 20.0% | $1/5$ |
| `0011` | 25% | $1/4$ |
| `0100` | 33.3% | $3/9$ |
| `0101` | 40.0% | $2/5$ |
| `0110` | 44.4% | $4/9$ |
| `0111` | 50.0% | $1/2$ |
| `1000` | 55.6% | $5/9$ |
| `1001` | 60.0% | $3/5$ |
| `1010` | 66.6% | $6/9$ |
| `1011` | 75% | $3/4$ |
| `1100` | 80.0% | $4/5$ |
| `1101` | 85.7% | $6/7$ |
| `1110` | 93.3% | $14/15$ |
| `1111` | 100.0% | 1 |

---

#### 13.3.6.4.3 Summary of Color Depth

**Table 13-11. [cite_start]Number of Colors/Shades of Gray Available on Screen** [cite: 848, 850]

| Number of BPP | Passive Mode (`LCDTFT = 0`) Monochrome (`LCDBW = 1`) / Color (`LCDBW = 0`) | Active Mode (`LCDTFT = 1`) Color Only (`LCDBW = 0`) |
| :--- | :--- | :--- |
| **1** | 2 palette entries to select within 15 grayscales / 2 palette entries to select within 3375 possible colors | 2 palette entries to select within 4096 possible colors |
| **2** | 4 palette entries to select within 15 grayscales / 4 palette entries to select within 3375 possible colors | 4 palette entries to select within 4096 possible colors |
| **4** | 16 palette entries to select within 15 grayscales / 16 palette entries to select within 3375 possible colors | 16 palette entries to select within 4096 possible colors |
| **8** | Not relevant since it would consist in 256 palette entries to select within 15 grayscales, but exists anyway / 256 palette entries to select 3375 possible colors | 256 palette entries to select within 4096 possible colors |
| **12** | 3375 possible colors / X | 4096 possible colors |
| **16** | 3375 possible colors (`STN_565 = 1`) / X | Up to 65536 possible colors |
| **24** | X / X | Up to 16.7 million colors |

#### 13.3.6.5 Output Format

##### 13.3.6.5.1 Passive (STN) Mode
[cite_start]As shown in Figure 13-12, the pixel data stored in frame buffers go through palette (if applicable) and gray-scaler/serializer before reaching the Output FIFO[cite: 859]. [cite_start]As a result, it is likely that the data fed to the Output FIFO is numerically different from the data in the frame buffers[cite: 860]. (However, they represent the same color or grayscale.) [cite_start][cite: 861]

[cite_start]The output FIFO formats the received data according to display modes (see Table 13-7)[cite: 862]. [cite_start]Figure 13-22 shows the actual data output on the external pins[cite: 863].

##### 13.3.6.5.2 Active (TFT) Mode
[cite_start]As shown in Figure 13-12, the gray-scaler/serializer and output FIFO are bypassed in active (TFT) mode[cite: 865]. [cite_start]Namely, at each pixel clock, one pixel data (16 bits) is output to the external LCD[cite: 866].

[Figure: Figure 13-22. Monochrome and Color Output. [cite_start]Shows how Pixel clock synchronizes with LCD controller output pins for Monochrome (MONO8B-0, MONO8B-1 with Pixel data [3:0] and [7:0]) and Color (Pixel data [7:0] mapping Red, Green, and Blue pixels to pins 0-7)] [cite: 870-940].

---

#### 13.3.6.6 Subpicture Feature

[cite_start]A feature exists in the LCD to cover either the top or lower portion of the display with a default color[cite: 950]. [cite_start]This feature is called a subpicture and is illustrated in Figure 13-23[cite: 951]. [cite_start]Subpictures are only allowed for Active Matrix mode (`cfg_lcdtft = '1'`)[cite: 952].

[cite_start]Subpictures reduce the bandwith to the DDR since lines containing default pixel data are not read from memory[cite: 953]. [cite_start]For example, suppose the panel has 100 lines of which 50 are default pixel data lines[cite: 954]. [cite_start]Then, only 50 lines of data are DMAed from DDR for this subpicture setup[cite: 955]. [cite_start]That is, the `cfg_fbx_base` and `cfg_fbx_ceiling` registers only encompass 50 lines of data instead of 100[cite: 956].

[Figure: Figure 13-23. Example of Subpicture. [cite_start]Shows a news broadcast image with the bottom half covered by a solid teal block, representing the default color area.] [cite: 957-958].

[cite_start]The subpicture feature is enabled when the `spen` MMR control bit is set to '1'[cite: 959]. 
* [cite_start]The `hols` bit, when set to '0,' puts the Default Pixel Data lines at the top of the screen and the active video lines at the bottom of the screen[cite: 960]. 
* [cite_start]When the `hols` bit is set to '1,' Active video lines are at the top of the screen and Default Pixel Data lines are at the bottom of the screen[cite: 961]. 

[cite_start]The `hols` bit behavior is shown in Figure 13-24[cite: 962].

[Figure: Figure 13-24. Subpicture HOLS Bit. Illustrates the screen split with "hols = '1'" (video top, solid bottom) and "hols = '0'" (solid top, video bottom). [cite_start]Also indicates the 'lppt' dimension.] [cite: 963-968].

[cite_start]The lines per panel threshold (LPPT) bitfield defines the number of lines at the bottom of the picture for both `hols = '1'` or `'0'`[cite: 969]. [cite_start]LPPT is an encoded value in the range `{0:2047}` used to represent the number of lines in the range `{1:2048}`[cite: 970].

---

[Figure: Figure 13-25. Raster Mode Display Format. [cite_start]A grid showing "Data lines (from 1 to L)" on the Y-axis and "Data pixels (from 1 to P)" on the X-axis, mapping coordinates like (1,1), (2,1), down to (P, L) onto the LCD area.] [cite: 983-998].

#### 13.3.6.7 Raster Controller Timing

[Figure: Figure 13-26. Raster Mode Passive (STN) Timing Diagram. Shows waveform timings for LCD_HSYNC, LCD_VSYNC, LCD_DATA[15:0], LCD_AC_BIAS_EN, and LCD_PCLK. [cite_start]Highlights parameters like VBP, VFP, VSW, Line time, Lines per panel (LPP), AC bias frequency (ACB), Pixels per line (PPL), Horizontal front porch (HFP), Horizontal sync pulse width (HSW), and Horizontal back porch (HBP).] [cite: 1006-1059].

[Figure: Figure 13-27. Raster Mode Active (TFT) Timing Diagram. Shows waveform timings for LCD_HSYNC, LCD_VSYNC, LCD_DATA[15:0], LCD_AC_BIAS_EN, and LCD_PCLK. [cite_start]Highlights parameters like Vertical sync pulse width (VSW), Vertical back porch (VBP), Lines per panel (LPP), Vertical front porch (VFP), Line time, AC bias frequency (ACB), Pixels per line (PPL), Horizontal front porch (HFP), Horizontal sync pulse width (HSW), and Horizontal back porch (HBP).] [cite: 1069-1109].
### 13.3.7 Interrupt Conditions

#### 13.3.7.1 Highlander 0.8 Interrupts

##### 13.3.7.1.1 Highlander Interrupt Basics
[cite_start]The interrupt mechanism is Highlander 0.8-compliant and relies on the `ipgvmodirq` IP Generic[cite: 1120]. [cite_start]The `ipgvmodirq` module supports hardware-initiated interrupts, each of which can also be individually triggered by software[cite: 1121]. [cite_start]An interrupt mask function allows each interrupt to be masked or enabled[cite: 1122]. [cite_start]The software can read all of the raw interrupts or only those that are unmasked[cite: 1123]. [cite_start]All pending interrupts in the LCD module must be serviced by the Host's Interrupt Service Routine before it exits[cite: 1124]. [cite_start]The Interrupt Module registers are described in the following table[cite: 1125].

**Table 13-12. Highlander 0.8 Interrupt Module Control Registers**

| Address Offset | Name | Description |
| :--- | :--- | :--- |
| `0x58` | `Reg22` | Interrupt Raw Status Register |
| `0x5C` | `Reg23` | Interrupt Masked Status Register |
| `0x60` | `Reg24` | Interrupt Enable Set (Unmask) |
| `0x64` | `Reg25` | Interrupt Enable Clear (Mask) |
| `0x68` | `Reg26` | End of Interrupt Indicator |

##### 13.3.7.1.2 Raw Status Register
[cite_start]Interrupts are associated with a bit position[cite: 1129]. [cite_start]For instance, Hardware Interrupt 0 is physically connected to bit 0 of the interrupt controller and all Sets, Clears, and Masks to this interrupt will reference the Bit 0 location of the interrupt vector[cite: 1129]. [cite_start]Likewise, Hardware Interrupt 1 is referenced by bit 1 of the interrupt vector, and so on[cite: 1130].

[cite_start]The Host CPU can see all the interrupts that have been set, regardless of the interrupt mask, by reading `Reg22`, the Raw Status Register[cite: 1131]. [cite_start]If the Host CPU writes a '1' to a bit position in Reg 22, it will do a software set for the interrupt associated with that bit position[cite: 1132].

##### 13.3.7.1.3 Masked Status Register
[cite_start]The Masked Status Register contains all the pending interrupts that are unmasked (enabled)[cite: 1134]. [cite_start]The Interrupt Service Routine should read this register to determine which interrupts must be serviced[cite: 1135].

##### 13.3.7.1.4 Interrupt Enable Set Register
[cite_start]To unmask an interrupt, the Host CPU writes a '1' to the appropriate bit position of the Enable Set (Unmask) register[cite: 1137].

##### 13.3.7.1.5 Interrupt Enable Clear Register
[cite_start]To mask an interrupt, the Host CPU writes a '1' to the appropriate bit position of the Enable Clear (Mask) register[cite: 1139].

##### 13.3.7.1.6 End of Interrupt Register
[cite_start]The `ipgvmodirq` module supports level or pulse interrupts to the CPU[cite: 1141]. [cite_start]For pulse interrupts, the Host must write to an end-of-interrupt (EOI), memory-mapped address to indicate that the Interrupt Service Routine has completed and is exiting[cite: 1142]. [cite_start]Any pending interrupts that have not been serviced will trigger another interrupt pulse to the Host CPU[cite: 1143].

---

#### 13.3.7.2 Interrupt Sources

##### 13.3.7.2.1 Overview of Interrupt Sources
The interrupt sources include:
* [cite_start]DMA End of Frame 0 [cite: 1152]
* [cite_start]DMA End of Frame 1 [cite: 1153]
* [cite_start]Palette Loaded [cite: 1154]
* [cite_start]FIFO Underflow [cite: 1155]
* [cite_start]AC Bias Count [cite: 1156]
* [cite_start]Sync Lost [cite: 1157]
* [cite_start]Recurrent Frame Done [cite: 1158]
* [cite_start]LIDD or Raster Frame Done [cite: 1159]

###### 13.3.7.2.1.1 DMA End of Frame 0 and End of Frame 1 Interrupt
[cite_start]The DMA End of Frame 0 and End of Frame 1 interrupts are triggered when the DMA module has completed transferring the contents of a frame buffer bounded by `cfg_fb0_base`/`cfg_bf0_ceil` or `cfg_fb1_base`/`cfg_fb1_ceil`[cite: 1162].

###### 13.3.7.2.1.2 Palette Loaded Interrupt
[cite_start]When `cfg_palmode` is set to Palette-only or Palette+data, the Palette Loaded interrupt is triggered when the palette portion of the DMA transfer has been stored in the Palette RAM[cite: 1164].

###### 13.3.7.2.1.3 FIFO Underflow Interrupt
[cite_start]The FIFO Underflow interrupt is triggered when the real-time output needs to send a value for pixel data but one cannot be found in the FIFO[cite: 1166].

###### 13.3.7.2.1.4 AC Bias Count Interrupt
[cite_start]For Passive Matrix displays, a count can be kept of the number of times the AC Bias line toggles[cite: 1168]. [cite_start]Once the specified number of transitions has been seen, the AC Bias Count interrupt is triggered[cite: 1169]. [cite_start]The module will not post any further interrupts or keep counting AC Bias transitions until the interrupt has been cleared[cite: 1170].

###### 13.3.7.2.1.5 Sync Lost Interrupt
[cite_start]When the DMA module reads a frame buffer and stores it in the FIFO, it sets a start frame and an end frame indicator embedded with the data[cite: 1172]. [cite_start]On retrieving the data from the FIFO in the `lcd_clk` domain, the Sync Lost interrupt is triggered if the start indicator is not found at the first pixel of a new frame[cite: 1173].

###### 13.3.7.2.1.6 Recurrent Frame Done Interrupt
[cite_start]In raster mode, the Recurrent Frame Done interrupt is triggered each time a complete frame has been sent to the interface pins[cite: 1175].

###### 13.3.7.2.1.7 LIDD or Raster Frame Done Interrupt
[cite_start]In LIDD DMA mode, a frame buffer of data is sent[cite: 1177]. [cite_start]When the frame buffer has completed, the LIDD Frame Done interrupt is triggered[cite: 1178]. [cite_start]In order to do another LIDD DMA, the DMA engine must be disabled and then re-enabled[cite: 1179]. [cite_start]In Raster mode, the interrupt is triggered after `cfg_lcden` is set to '0' and after the last frame is sent to the pins[cite: 1180]. [cite_start]After the Raster mode DMA is running, the interrupt occurs only once after the module is disabled[cite: 1181].

---

### 13.3.8 DMA

[cite_start]DDR access is handled internally by the DMA module[cite: 1189]. [cite_start]For Character Displays, the DMA module can transfer a sequence of data transactions from the DDR to LCD panel[cite: 1189]. [cite_start]By using the DMA instead of the Host CPU, the Host will not be stalled waiting for the slow external peripheral to complete[cite: 1190]. [cite_start]For Passive and Active Matrix displays, the DMA is used to read frame buffers with associated palette information from DDR[cite: 1191]. [cite_start]The DMA parses the frame buffer according to the frame buffer type and supplies the raster processing chain with Palette information and pixel data as needed[cite: 1192].

---

### 13.3.9 Power Management

Power management within the DSS can be accomplished in several ways:
1. [cite_start]L4 OCP MConnect/SConnect can disable the internal L4 clock network[cite: 1194].
2. [cite_start]L3 OCP MConnect/Sconnect can disable the internal L3 clock network[cite: 1195].
3. [cite_start]Within the Clock Control register, there are clock enable registers to disable the clock networks to all major internal functional paths[cite: 1196].
4. [cite_start]Power Compiler clock gates are automatically instantiated within datapaths to minimize active power[cite: 1197].

[cite_start]Items 1 and 2 are accomplished using the standard IDLE (for L4) and STANDBY (for L3) IPGeneric modules[cite: 1198]. [cite_start]When these modules are instructed to disable clocks for the internal L3 or L4 (MMR) clock domains, the internal clock networks will be shut down[cite: 1199]. [cite_start]This shutdown applies to the external clock pins `l3_clk` and `l4_clk`[cite: 1200].

[cite_start]All other internal clock domains (Item 3) can only be shut down by writing the appropriate register bit within the Clock Enable register[cite: 1201]. [cite_start]This software clock control applies to all other clock inputs[cite: 1202].

[cite_start]Power Compiler clock gating is done automatically as a function of the design[cite: 1203]. [cite_start]There is no special control required for this operation[cite: 1204].

[cite_start]Because the LCD normally drives displays, and because all video is sourced from the L3 clock domain, shutting down the L3 domain using the IPGenerics can cause undesirable display effects[cite: 1205]. [cite_start]In most circumstances, it will be necessary to hardware/software reset the LCD module after such an event has occurred[cite: 1206].

## 13.4 Programming Model

### 13.4.1 LCD Character Displays

#### 13.4.1.1 Configuration Registers, Setup, and Settings

##### 13.4.1.1.1 Configuration Registers
Set the following to appropriate values for the target LCD character panel:
* [cite_start]`cfg_cs1_e1_pol` [cite: 1219]
* [cite_start]`cfg_cs0_e0_pol` [cite: 1220]
* [cite_start]`ws_dir_pol` [cite: 1221]
* [cite_start]`cfg_rs_en_pol` [cite: 1222]
* [cite_start]`cfg_alepol` [cite: 1223]

[cite_start]`cfg_lidd_mode_sel[2:0]` defines the type of CPU bus that will be used in interfacing with the LCD character panel[cite: 1225]. [cite_start]Note that the clocked bus styles only support a single panel using CS0 since the clock pin takes a device pin that is otherwise used for CS1[cite: 1226, 1227].

Set the following to appropriate bus timing parameters for the target LCD character panel:
* [cite_start]`cfg_w_su` [cite: 1228]
* [cite_start]`cfg_w_strobe` [cite: 1229]
* [cite_start]`cfg_w_hold` [cite: 1230]
* [cite_start]`cfg_r_su` [cite: 1230]
* [cite_start]`cfg_r_strobe` [cite: 1231]
* [cite_start]`cfg_r_hold` [cite: 1232]
* [cite_start]`cfg_ta` [cite: 1233]

[cite_start]A set of bus timing parameters are individually available for CS0 and CS1 such that the bus transactions can be customized for each of the two supported LCD character displays[cite: 1234].

##### 13.4.1.1.2 Defining Panel Commands and Panel Data
[cite_start]In the Hitachi interface mode used for the example panel, whether the Character Panel understands a data transfer as Command or Data depends on the state of the REGSEL input pin[cite: 1236]. 
* [cite_start]Writing to the `cfg_adr_indx` register will output a Command transfer[cite: 1237]. 
* [cite_start]Writing to the `cfg_data` register will result in a Data transfer[cite: 1238]. 

[cite_start]Functionally, the ALE (`lcd_fp` pin) from the LCD controller is tied to the REGSEL input of the character panel[cite: 1239]. For example, to send byte 0xAB as a command to the previously described character panel, the CPU would write 0x00AB to the `adr_indx` register[cite: 1240]. [cite_start]To send byte 0xAB as data, the CPU would write 0x00AB to the `data` register[cite: 1241].

#### 13.4.1.2 CPU Initiated Data Bus Transactions

##### 13.4.1.2.1 Initiating Data Bus Transactions
* [cite_start]Writing to `cfg_cs0_data` will initiate a write transfer to the CS0 panel[cite: 1250]. 
* [cite_start]Reading from `cfg_cs0_data` will initiate a read transfer from the CS0 panel[cite: 1251].
* [cite_start]Writing to `cfg_cs1_data` will initiate a write transfer to the CS1 panel[cite: 1252]. 
* [cite_start]Reading from `cfg_cs1_data` will initiate a read transfer from the CS0 panel[cite: 1253].

[cite_start]**NOTE:** Writes to CS1 translate to valid bus transactions only if `cfg_lidd_mode_sel[2:0]` is configured for an asynchronous mode[cite: 1254].

#### 13.4.1.3 DMA Initiated Data Bus Transactions for LIDD

##### 13.4.1.3.1 DMA Overview for MPU Bus Output
[cite_start]Writing a long sequence of data to the Character Display Panel will ensure that the CPU will be occupied for a long time[cite: 1257]. [cite_start]However, the DMA module supports a mode in which this sequence of data elements can first be written in DRAM by the CPU[cite: 1258]. [cite_start]The DMA can read this sequence of commands or data from the DRAM and send it to the LCD Interface Display Driver (LIDD) module such that each data element becomes a write bus transaction to the external Character Panel/MPU Bus[cite: 1259]. [cite_start]The data bus write transaction can target either CS0 or CS1 and use the appropriate bus timing parameters[cite: 1260].

[cite_start]Functionally, in this DMA LIDD mode, the DMA module sends the sequence of data to the LIDD module by acting as another CPU[cite: 1261]. [cite_start]The DMA can only perform write bus transactions[cite: 1262]. [cite_start]It cannot read from the external character panel a series of data elements and store them in the DRAM[cite: 1262].

[cite_start]When the LIDD module is controlled by the DMA module by setting `cfg_lidd_dma_en = '1'`, CPU reads or writes to `cfg_adr_index` and `cfg_data` are not allowed[cite: 1263]. [cite_start]The `fb0_base` and `fb0_ceil` registers define the address boundary of data elements to be sent out the character display by the DMA engine[cite: 1264]. [cite_start]Setting `cfg_lidd_dma_en` from '0' to '1' will initiate the DMA as if a virtual CPU is reading data from the DDR and writing the values to Reg6 or Reg9[cite: 1265]. [cite_start]`cfg_dma_cs0_cs1` determines whether the virtual CPU writes to Reg6 (CS0) or Reg7 (CS1)[cite: 1265].

[cite_start]**NOTE:** Writes to CS1 translate to valid bus transactions only if `cfg_lidd_mode_sel[2:0]` is configured for an asynchronous mode[cite: 1266].

[cite_start]The DMA module requires the start and end DDR addresses to be on word-aligned byte addresses[cite: 1267]. [cite_start]The MPU/LIDD bus is a halfword (16 bit) output, so both the upper and lower halfwords of the DDR memory will be sent out[cite: 1268]. [cite_start]Thus, the number of data elements sent to the LIDD by the DMA must always result in an even number of bus MPU bus transactions[cite: 1269]. [cite_start]In other words, a transfer of three 32-bit words from DDR will result in six 16-bit bus transactions[cite: 1270].

##### 13.4.1.3.2 MCU/LIDD DMA Setup: Example Pseudo Code
[cite_start]Suppose we want to send by DMA a section of DDR memory from byte address 0x4 to byte address 0x3C to the MCU bus using chip select 0. The pseudo code for such an operation is listed below[cite: 1278].

## 13.4 Programming Model

### 13.4.1 LCD Character Displays

#### 13.4.1.1 Configuration Registers, Setup, and Settings

##### 13.4.1.1.1 Configuration Registers
Set the following to appropriate values for the target LCD character panel:
* [cite_start]`cfg_cs1_e1_pol` [cite: 1219]
* [cite_start]`cfg_cs0_e0_pol` [cite: 1220]
* [cite_start]`ws_dir_pol` [cite: 1221]
* [cite_start]`cfg_rs_en_pol` [cite: 1222]
* [cite_start]`cfg_alepol` [cite: 1223]

[cite_start]`cfg_lidd_mode_sel[2:0]` defines the type of CPU bus that will be used in interfacing with the LCD character panel[cite: 1225]. [cite_start]Note that the clocked bus styles only support a single panel using CS0 since the clock pin takes a device pin that is otherwise used for CS1[cite: 1226, 1227].

Set the following to appropriate bus timing parameters for the target LCD character panel:
* [cite_start]`cfg_w_su` [cite: 1228]
* [cite_start]`cfg_w_strobe` [cite: 1229]
* [cite_start]`cfg_w_hold` [cite: 1230]
* [cite_start]`cfg_r_su` [cite: 1230]
* [cite_start]`cfg_r_strobe` [cite: 1231]
* [cite_start]`cfg_r_hold` [cite: 1232]
* [cite_start]`cfg_ta` [cite: 1233]

[cite_start]A set of bus timing parameters are individually available for CS0 and CS1 such that the bus transactions can be customized for each of the two supported LCD character displays[cite: 1234].

##### 13.4.1.1.2 Defining Panel Commands and Panel Data
[cite_start]In the Hitachi interface mode used for the example panel, whether the Character Panel understands a data transfer as Command or Data depends on the state of the REGSEL input pin[cite: 1236]. 
* [cite_start]Writing to the `cfg_adr_indx` register will output a Command transfer[cite: 1237]. 
* [cite_start]Writing to the `cfg_data` register will result in a Data transfer[cite: 1238]. 

[cite_start]Functionally, the ALE (`lcd_fp` pin) from the LCD controller is tied to the REGSEL input of the character panel[cite: 1239]. For example, to send byte 0xAB as a command to the previously described character panel, the CPU would write 0x00AB to the `adr_indx` register[cite: 1240]. [cite_start]To send byte 0xAB as data, the CPU would write 0x00AB to the `data` register[cite: 1241].

#### 13.4.1.2 CPU Initiated Data Bus Transactions

##### 13.4.1.2.1 Initiating Data Bus Transactions
* [cite_start]Writing to `cfg_cs0_data` will initiate a write transfer to the CS0 panel[cite: 1250]. 
* [cite_start]Reading from `cfg_cs0_data` will initiate a read transfer from the CS0 panel[cite: 1251].
* [cite_start]Writing to `cfg_cs1_data` will initiate a write transfer to the CS1 panel[cite: 1252]. 
* [cite_start]Reading from `cfg_cs1_data` will initiate a read transfer from the CS0 panel[cite: 1253].

[cite_start]**NOTE:** Writes to CS1 translate to valid bus transactions only if `cfg_lidd_mode_sel[2:0]` is configured for an asynchronous mode[cite: 1254].

#### 13.4.1.3 DMA Initiated Data Bus Transactions for LIDD

##### 13.4.1.3.1 DMA Overview for MPU Bus Output
[cite_start]Writing a long sequence of data to the Character Display Panel will ensure that the CPU will be occupied for a long time[cite: 1257]. [cite_start]However, the DMA module supports a mode in which this sequence of data elements can first be written in DRAM by the CPU[cite: 1258]. [cite_start]The DMA can read this sequence of commands or data from the DRAM and send it to the LCD Interface Display Driver (LIDD) module such that each data element becomes a write bus transaction to the external Character Panel/MPU Bus[cite: 1259]. [cite_start]The data bus write transaction can target either CS0 or CS1 and use the appropriate bus timing parameters[cite: 1260].

[cite_start]Functionally, in this DMA LIDD mode, the DMA module sends the sequence of data to the LIDD module by acting as another CPU[cite: 1261]. [cite_start]The DMA can only perform write bus transactions[cite: 1262]. [cite_start]It cannot read from the external character panel a series of data elements and store them in the DRAM[cite: 1262].

[cite_start]When the LIDD module is controlled by the DMA module by setting `cfg_lidd_dma_en = '1'`, CPU reads or writes to `cfg_adr_index` and `cfg_data` are not allowed[cite: 1263]. [cite_start]The `fb0_base` and `fb0_ceil` registers define the address boundary of data elements to be sent out the character display by the DMA engine[cite: 1264]. [cite_start]Setting `cfg_lidd_dma_en` from '0' to '1' will initiate the DMA as if a virtual CPU is reading data from the DDR and writing the values to Reg6 or Reg9[cite: 1265]. [cite_start]`cfg_dma_cs0_cs1` determines whether the virtual CPU writes to Reg6 (CS0) or Reg7 (CS1)[cite: 1265].

[cite_start]**NOTE:** Writes to CS1 translate to valid bus transactions only if `cfg_lidd_mode_sel[2:0]` is configured for an asynchronous mode[cite: 1266].

[cite_start]The DMA module requires the start and end DDR addresses to be on word-aligned byte addresses[cite: 1267]. [cite_start]The MPU/LIDD bus is a halfword (16 bit) output, so both the upper and lower halfwords of the DDR memory will be sent out[cite: 1268]. [cite_start]Thus, the number of data elements sent to the LIDD by the DMA must always result in an even number of bus MPU bus transactions[cite: 1269]. [cite_start]In other words, a transfer of three 32-bit words from DDR will result in six 16-bit bus transactions[cite: 1270].

##### 13.4.1.3.2 MCU/LIDD DMA Setup: Example Pseudo Code
[cite_start]Suppose we want to send by DMA a section of DDR memory from byte address 0x4 to byte address 0x3C to the MCU bus using chip select 0. The pseudo code for such an operation is listed below[cite: 1278].
// Enable Clocks
wr 0060_0000 0007

// LCD Control Register
wr 0004_0000 8000 // set clock divisor

// LIDD Control Register
wr 0000_0000 000C // set output bus polarities and lidd_mode_sel

// LIDD CS0 Register
wr 0010_0822 1044 // set bus timing parameters for CS0

// DMA Control Register
wr 0040_0000 0030 // set DMA parameters like burst size, memory layout

// DMA FB0 Base Register
wr 0044_0000 0004 // DMA start byte address

// DMA FB0 Ceiling Register
wr 0048_0000 003C // DMA end byte address

// LIDD Control Register enable DMA
wr 0000_0000 010C // Flip LIDD DMA enable bit

[cite: 1279-1296]

Once the DMA completes sending data to the Async FIFO, the Eof0 interrupt will occur[cite: 1297]. The Done interrupt will occur when the last word is written out the MPU bus[cite: 1298]. The CPU must bring `cfg_lidd_dma_en` low before the CPU can directly initiate MPU bus transactions or for the DMA module to start again[cite: 1299].

#### 13.4.1.4 Passive Matrix

##### 13.4.1.4.1 Monochrome Bitrate Awareness
In a mostly testbench related note, care must be taken when configuring the module for Passive Matrix (`cfg.lcdtft='0'`) monochrome (`cfg.lcdbw='1'`) modes[cite: 1302]. In passive matrix mode, the Blue component of the Grayscaler output is used as the quantized value for each scan order pixel[cite: 1303]. 

* When `cfg_mono8b='1'`, eight pixel values must be sent through the grayscaler before one 8-bit output is ready[cite: 1304]. This output data represents the passive matrix output states for eight pixels[cite: 1305]. 
* Likewise, when `cfg_mono8b='0'`, four pixel values must be sent through the grayscaler before one 4-bit output is ready[cite: 1306]. This output data represents the passive matrix output states for four pixels[cite: 1307].

The problem arises when the output clock is fast (`cfg_clkdiv=0x2`)[cite: 1308]

### [cite_start]13.4.2 Active Matrix Displays [cite: 1319]

#### [cite_start]13.4.2.1 Interfacing to Dual LVDS Transmitters [cite: 1320]
[cite_start]The pixel clock rate for HD-sized pictures is approximately 148.5 MHz. [cite: 1322] [cite_start]At this speed, the LVDS link requires a double-wide data bus for transferring the even and odd pixels at half the pixel rate. [cite: 1323] [cite_start]The LCD Controller outputs one pixel per pixel clock cycle. [cite: 1324] [cite_start]Some LVDS transmitters accept a high speed, single pixel input and output to dual LVDS drivers, in which case external glue logic is unnecessary. [cite: 1325] [cite_start]For those LVDS transmitters that require the even and odd pixel to enter the LVDS transmitter at half the pixel clock rate, external logic is required. [cite: 1326]

---

### [cite_start]13.4.3 System Interaction [cite: 1327]

#### [cite_start]13.4.3.1 DMA End of Frame Interrupts [cite: 1328]
[cite_start]The LCD module works with the DMA such that data is fetched from DDR and sent to a FIFO memory. [cite: 1329] [cite_start]The DMA module does this fetching independently of the logic on the output side of the FIFO. [cite: 1330]

* [cite_start]**LIDD Mode DMA:** For LIDD mode DMA, the module fetches frame buffer 0. [cite: 1331] [cite_start]When the last word of frame buffer 0 is stored in the FIFO memory, the Eof0 interrupt is triggered (if `cfg_eof_inten="1"`) and the DMA stops. [cite: 1331] [cite_start]The CPU has to set `cfg_lidd_dma_en` to `'0'`, followed by a `cfg_lidd_dma_en='1'`, before the next burst from frame buffer 0 is read from DDR. [cite: 1332]
* [cite_start]**Raster Mode DMA:** For Raster mode DMA, the module fetches frame buffer 0. [cite: 1333] [cite_start]When the last word of frame buffer 0 is stored in the FIFO memory, the Eof0 interrupt is triggered (if `cfg_eof_inten="1"`) but the DMA does not stop. [cite: 1333] [cite_start]The DMA module ping pongs immediately to frame buffer 1 if `cfg_frame_mode='1'`. [cite: 1334] [cite_start]Otherwise, the DMA fetches the frame buffer 0 address range from DDR. [cite: 1335] [cite_start]When the DMA module fetches frame buffer 1, and the last word of frame buffer 1 is stored in the FIFO memory, the Eof1 interrupt is triggered (if `cfg_eof_inten='1'`). [cite: 1336] [cite_start]This pattern would repeat. [cite: 1337]

---

### [cite_start]13.4.4 Palette Lookup [cite: 1338]
[cite_start]For Active Matrix and Passive Matrix modes, the 12-bit Palette RAM Lookup can be used. [cite: 1339]

* [cite_start]**Active Matrix (`cfg.lcdtft='1'`):** Palette lookup is enabled when `cfg.tft24='0'` and the bpp field in the Palette RAM is set to "000," "001," "010," or "011" (1, 2, 4, or 8 bpp). [cite: 1340] [cite_start]Palette lookup cannot be used when the bpp field is set to "100" (12/16 bpp). [cite: 1341]
* [cite_start]**Passive Matrix (`cfg_lcdtft='0'`):** Palette lookup is enabled when the bpp field in the Palette RAM is set to "000," "001," "010," or "011" (1, 2, 4, or 8 bpp). [cite: 1342] [cite_start]Palette lookup cannot be used when the bpp field is set to "100" (12/16 bpp). [cite: 1343]

[Figure: Figure 13-28. Palette Lookup Examples. [cite_start]Illustrates the memory organization for 1 BPP, 2 BPP, 4 BPP, and 8 BPP mapping to the 256 entries in the 12-bit Palette RAM.] [cite: 1361-1382]

* **1 BPP:** When the bpp encoding is set to 1 bpp, each bit in a 16-bit frame buffer halfword is used to index the two bottom locations of the palette RAM. [cite: 1345] Suppose the frame buffer bit value is '0', this '0' indicates that the address 0 entry in the Palette RAM should be read. [cite: 1346] If the frame buffer bit value is '1,' the address 1 entry in the Palette RAM is used. [cite: 1347] The resulting 12-bit output from the Palette RAM is the quantized pixel value of a 4-bit per color component quantized pixel value. [cite: 1348]
* [cite_start]**2 BPP:** When the bpp encoding is set to 2 bpp, every two bits in a 16 bit frame buffer halfword is used to index the bottom 4 locations of the palette RAM. [cite: 1349] Suppose the frame buffer bit value is "00." [cite_start]This "00" indicates that the address 0 entry in the Palette RAM should be read. [cite: 1350] [cite_start]If the frame buffer bit value is "01," the address 1 entry in the Palette RAM is used. [cite: 1351] [cite_start]When the frame buffer bit value is "10," the address 2 entry in the Palette RAM is read. [cite: 1352] [cite_start]Finally, if the frame buffer bit value is "11," the address 3 entry in the Palette RAM is read. [cite: 1353] [cite_start]The resulting 12 bit output from the Palette RAM is the quantized pixel value of the 4 bit per color component. [cite: 1354]
* **4 BPP:** The 4 bpp encoding allows every four bits from a frame buffer halfword to address 16 entries in the Palette RAM. [cite: 1355]
* [cite_start]**8 BPP:** The 8 bpp encoding enables every byte from a frame buffer halfword to address one of the 256 entries in the Palette RAM. [cite: 1356]

#### Frame Buffer Read Ordering
[cite_start]A 16-bit halfword is read from the DDR frame buffer. [cite: 1385] [cite_start]This halfword can be byte lane and halfword swapped using the DMA configuration values `cfg_byte_swap` and `cfg_bigendian`. [cite: 1386] [cite_start]This section will deal with the frame buffer data as it is returned post swapped from the DMA module. [cite: 1387]

[cite_start]The DMA module actually outputs a 32-bit word. [cite: 1388] [cite_start]The Palette Lookup logic uses the lower halfword first, followed by the upper halfword. [cite: 1388] [cite_start]The `cfg_rdorder` and `cfg_nibmode` registers determine the raster read ordering of the frame buffer data to be sent to the palette lookup table. [cite: 1389] 

[cite_start]There are precedence rules for the hardware as it parses each 16-bit word from the frame buffer: [cite: 1390]
1.  [cite_start]If `cfg_rdorder = '0'`, the data halfword is parsed from the least significant bit to the most significant bit. [cite: 1391]
2.  [cite_start]Else, if `cfg_nibmode = '1'`, the data halfword is parsed byte swapped with the scan order going from the most significant bit of each byte to the least significant bit of each byte. [cite: 1392]
3.  [cite_start]Otherwise, the data halfword is parsed from the most significant bit to the least significant bit. [cite: 1393]

[cite_start]The bitwise scan order for each halfword fetched from the frame buffer is shown below. [cite: 1394] [cite_start]The bitfields returned are used to determine the addressing of the Palette RAM. [cite: 1395]

[cite_start]**Frame buffer halfword scan order for 1 bpp** [cite: 1402]
1. [cite_start]If `cfg_rdorder = 0`, scan order is `[0] [1] [2] [3] [4] [5] [6] [7] [8] [9] [10] [11] [12] [13] [14] [15]` [cite: 1404]
2. [cite_start]Else if `cfg_nibmode = 1`, scan order is `[7] [6] [5] [4] [3] [2] [1] [0] [15] [14] [13] [12] [11] [10] [9] [8]` [cite: 1405]
3. [cite_start]Otherwise, scan order is `[15] [14] [13] [12] [11] [10] [9] [8] [7] [6] [5] [4] [3] [2] [1] [0]` [cite: 1405]

[cite_start]**Frame buffer halfword scan order for 2 bpp** [cite: 1406]
1. [cite_start]If `cfg_rdorder = 0`, scan order is `[1:0] [3:2] [5:4] [7:6] [9:8] [11:10] [13:12] [15:14]` [cite: 1407]
2. [cite_start]Else if `cfg_nibmode = 1`, scan order is `[7:6] [5:4] [3:2] [1:0] [15:14] [13:12] [11:10] [9:8]` [cite: 1408]
3. [cite_start]Otherwise, scan order is `[15:14] [13:12] [11:10] [9:8] [7:6] [5:4] [3:2] [1:0]` [cite: 1409]

[cite_start]**Frame buffer halfword scan order for 4 bpp** [cite: 1410]
1. [cite_start]If `cfg_rdorder = 0`, scan order is `[3:0] [7:4] [11:8] [15:12]` [cite: 1411]
2. [cite_start]Else if `cfg_nibmode = 1`, scan order is `[7:4] [3:0] [15:12] [11:8]` [cite: 1412]
3. [cite_start]Otherwise, scan order is `[15:12] [11:8] [7:4] [3:0]` [cite: 1413]

[cite_start]**Frame buffer halfword scan order for 8 bpp** [cite: 1414]
1. [cite_start]If `cfg_rdorder = 0`, scan order is `[7:0] [15:8]` [cite: 1415]
2. [cite_start]Else if `cfg_nibmode = 1`, scan order is `[7:0] [15:8]` [cite: 1416]
3. [cite_start]Otherwise, scan order is `[15:8] [7:0]` [cite: 1417]

---

### [cite_start]13.4.5 Test Logic [cite: 1418]
*(Section left intentionally blank per documentation)*

---

### [cite_start]13.4.6 Disable and Software Reset Sequence [cite: 1419]
[cite_start]In Raster Modes, the module must be disabled before applying a software reset. [cite: 1420] [cite_start]When `cfg_lcden` is set to `0` to disable the module, the output continues to the end of the current frame. [cite: 1421] [cite_start]The Done interrupt will trigger once the frame is complete. [cite: 1422] [cite_start]The software reset can then be applied to the module. [cite: 1422] [cite_start]The software reset will clear all the frame information in the FIFO. [cite: 1423] [cite_start]Upon a restart, the L3 DMA will fetch from the `fb0_base` address. [cite: 1424]

[cite_start]To summarize: [cite: 1425]
1.  [cite_start]Set `cfg_lcden = '0'`. [cite: 1426]
2.  [cite_start]Wait for the Done interrupt. [cite: 1427]
3.  [cite_start]Set the software reset bits high (`cfg_main_rst = '1'` or [`cfg_dma_rst = '1'` and `cfg_core_rst = '1'`]) for several cycles. [cite: 1428]
4.  [cite_start]Set the resets back low. [cite: 1429]
5.  [cite_start]Set `cfg_lcden = '1'`. [cite: 1430]

[cite_start]The disable and reset sequence must be done in this order to properly operate the LCD module and the EMIF. [cite: 1431]

---

### [cite_start]13.4.7 Precedence Order for Determining Frame Buffer Type [cite: 1436]
[cite_start]The precedence order for determining frame buffer type is specified as follows: [cite: 1437]

```c
[cite_start]If (cfg_lcdtft == 1) // active matrix [cite: 1437]
    [cite_start]If (cfg_tft24 == 1) // 24 bpp [cite: 1438]
        [cite_start]If (cfg_tft24_unpacked == 1) [cite: 1439]
            [cite_start]4 pixels in 4 words [cite: 1441]
        [cite_start]else [cite: 1440]
            [cite_start]4 pixels in 3 words [cite: 1442]
    [cite_start]else // 1/2/4/8/12/16 bpp [cite: 1443]
        [cite_start]if (bpp[2] == 1) [cite: 1444]
            [cite_start]12/16 bpp data [cite: 1446]
        [cite_start]else [cite: 1445]
            [cite_start]if (bpp == 0) [cite: 1447, 1448]
                [cite_start]1 bpp data [cite: 1449]
            [cite_start]else if (bpp == 1) [cite: 1450]
                [cite_start]2 bpp data [cite: 1451]
            [cite_start]else if (bpp == 2) [cite: 1452]
                [cite_start]4 bpp data [cite: 1453]
            [cite_start]else // if (bpp == 3) [cite: 1454]
                [cite_start]8 bpp data [cite: 1455]
[cite_start]else // passive matrix [cite: 1456]
    [cite_start]if (bpp[2] == 1) [cite: 1457]
        [cite_start]12/16 bpp data [cite: 1459]
    [cite_start]else [cite: 1458]
        [cite_start]if (bpp == 0) [cite: 1460]
            [cite_start]1 bpp data [cite: 1461]
        [cite_start]else if (bpp == 1) [cite: 1462]
            [cite_start]2 bpp data [cite: 1463]
        [cite_start]else if (bpp == 2) [cite: 1464]
            [cite_start]4 bpp data [cite: 1465]
        [cite_start]else // if (bpp == 3) [cite: 1466, 1467, 1469]
            [cite_start]8 bpp data [cite: 1468]

## 13.5 Registers

### 13.5.1 LCD Registers

[cite_start]Table 13-13 lists the memory-mapped registers for the LCD[cite: 3097]. [cite_start]All register offset addresses not listed in Table 13-13 should be considered as reserved locations and the register contents should not be modified[cite: 3097].

**Table 13-13. LCD Registers**

| Offset | Acronym | Register Name | Section |
| :--- | :--- | :--- | :--- |
| `0h` | PID | | Section 13.5.1.1 |
| `4h` | CTRL | | Section 13.5.1.2 |
| `Ch` | LIDD_CTRL | | Section 13.5.1.3 |
| `10h` | LIDD_CS0_CONF | | Section 13.5.1.4 |
| `14h` | LIDD_CS0_ADDR | | Section 13.5.1.5 |
| `18h` | LIDD_CS0_DATA | | Section 13.5.1.6 |
| `1Ch` | LIDD_CS1_CONF | | Section 13.5.1.7 |
| `20h` | LIDD_CS1_ADDR | | Section 13.5.1.8 |
| `24h` | LIDD_CS1_DATA | | Section 13.5.1.9 |
| `28h` | RASTER_CTRL | | Section 13.5.1.10 |
| `2Ch` | RASTER_TIMING_0 | | Section 13.5.1.11 |
| `30h` | RASTER_TIMING_1 | | Section 13.5.1.12 |
| `34h` | RASTER_TIMING_2 | | Section 13.5.1.13 |
| `38h` | RASTER_SUBPANEL | | Section 13.5.1.14 |
| `3Ch` | RASTER_SUBPANEL2 | | Section 13.5.1.15 |
| `40h` | LCDDMA_CTRL | | Section 13.5.1.16 |
| `44h` | LCDDMA_FB0_BASE | | Section 13.5.1.17 |
| `48h` | LCDDMA_FB0_CEILING | | Section 13.5.1.18 |
| `4Ch` | LCDDMA_FB1_BASE | | Section 13.5.1.19 |
| `50h` | LCDDMA_FB1_CEILING | | Section 13.5.1.20 |
| `54h` | SYSCONFIG | | Section 13.5.1.21 |
| `58h` | IRQSTATUS_RAW | | Section 13.5.1.22 |
| `5Ch` | IRQSTATUS | | Section 13.5.1.23 |
| `60h` | IRQENABLE_SET | | Section 13.5.1.24 |
| `64h` | IRQENABLE_CLEAR | | Section 13.5.1.25 |
| `6Ch` | CLKC_ENABLE | | Section 13.5.1.26 |
| `70h` | CLKC_RESET | | Section 13.5.1.27 |

[cite_start][cite: 3099]

---

#### 13.5.1.1 PID Register (offset = 0h) [reset = 0h]

[cite_start]PID is shown in Figure 13-29 and described in Table 13-14[cite: 3105].

[Figure: Figure 13-29. PID Register. [cite_start]Shows bits 31-0 split into fields: scheme, RESERVED, func, rtl, major, custom, and minor.] [cite: 3109-3110]

**Table 13-14. PID Register Field Descriptions**

| Bit | Field | Type | Reset | Description |
| :--- | :--- | :--- | :--- | :--- |
| 31-30 | `scheme` | R | 0h | The scheme of the register used. This field indicates the 3.5 Method. |
| 29-28 | `RESERVED` | R | 0h | |
| 27-16 | `func` | R | 0h | The function of the module being used. |
| 15-11 | `rtl` | R | 0h | The Release number for this IP. |
| 10-8 | `major` | R | 0h | Major Release Number |
| 7-6 | `custom` | R | 0h | Custom IP |
| 5-0 | `minor` | R | 0h | Minor Release Number |

[cite_start][cite: 3113]

---

#### 13.5.1.2 CTRL Register (offset = 4h) [reset = 0h]

[cite_start]CTRL is shown in Figure 13-30 and described in Table 13-15[cite: 3121].

[Figure: Figure 13-30. CTRL Register. [cite_start]Shows bits 31-0 split into fields: RESERVED, clkdiv, auto_uflow_restart, and modesel.] [cite: 3123-3124]

**Table 13-15. CTRL Register Field Descriptions**

| Bit | Field | Type | Reset | Description |
| :--- | :--- | :--- | :--- | :--- |
| 31-16 | `RESERVED` | R | 0h | |
| 15-8 | `clkdiv` | R/W | 0h | **Clock divisor.**<br>Raster mode: Values of 2 through 255 are permitted and resulting pixel clock is lcd_clk/2 through lcd_clk/255.<br>LIDD mode: Values of 0 through 255 are permitted with resulting MCLK of lcd_clk/1 through lcd_clk/255 where both 0 and 1 result in lcd_clk/1. |
| 7-2 | `RESERVED` | R/W | 0h | |
| 1 | `auto_uflow_restart` | R/W | 0h | On an underflow, the software has to restart the module.<br>`0` = On an underflow, the software has to restart the module.<br>`1` = On an underflow, the hardware will restart on the next frame. |
| 0 | `modesel` | R/W | 0h | **LCD Mode select.**<br>`0` = Controller in LIDD Mode.<br>`1` = LCD Controller in Raster Mode. |

[cite_start][cite: 3127]

---

#### 13.5.1.3 LIDD_CTRL Register (offset = Ch) [reset = 0h]

[cite_start]LIDD_CTRL is shown in Figure 13-31 and described in Table 13-16[cite: 3133].

[Figure: Figure 13-31. LIDD_CTRL Register. [cite_start]Shows bits 31-0 split into fields: RESERVED, dma_cs0_cs1, lidd_dma_en, cs1_e1_pol, cs0_e0_pol, ws_dir_pol, rs_en_pol, alepol, and lidd_mode_sel.] [cite: 3136-3137]

**Table 13-16. LIDD_CTRL Register Field Descriptions**

| Bit | Field | Type | Reset | Description |
| :--- | :--- | :--- | :--- | :--- |
| 31-10 | `RESERVED` | R | 0h | |
| 9 | `dma_cs0_cs1` | R/W | 0h | **CS0/CS1 Select for LIDD DMA writes.**<br>`0` = writes to LIDD CS0.<br>`1` = DMA writes for LIDD CS1. |
| 8 | `lidd_dma_en` | R/W | 0h | **LIDD DMA Enable.**<br>`0` = Deactivate DMA control of LIDD interface. DMA control is released upon completion of transfer of the current frame of data (LIDD Frame Done) after this bit is cleared. The MPU has direct read/write access to the panel in this mode.<br>`1` = Activate DMA to drive LIDD interface to support streaming data to smart panels. The MPU cannot access the panel directly in this mode. |
| 7 | `cs1_e1_pol` | R/W | 0h | **Chip Select 1/Enable 1 (Secondary) Polarity Control.**<br>`0` = Do Not Invert Chip Select 1/Enable 1. Chip Select 1 is active low by default. Enable 1 is active high by default.<br>`1` = Invert Chip Select 1/Enable 1. |
| 6 | `cs0_e0_pol` | R/W | 0h | **Chip Select 0/Enable 0 (Secondary) Polarity Control.**<br>`0` = Do Not Invert Chip Select 0/Enable 0. Chip Select 0 is active low by default. Enable 0 is active high by default.<br>`1` = Invert Chip Select 0/Enable 0. |
| 5 | `ws_dir_pol` | R/W | 0h | **Write Strobe/Direction Polarity Control.**<br>`0` = Do Not Invert Write Strobe/Direction. Write Strobe/Direction is active low/write low by default.<br>`1` = Invert Write Strobe/Direction. |
| 4 | `rs_en_pol` | R/W | 0h | **Read Strobe/Direction Polarity Control.**<br>`0` = Do Not Invert Read Strobe/Direction. Read Strobe/Direction is active low/write low by default.<br>`1` = Invert Read Strobe/Direction. |
| 3 | `alepol` | R/W | 0h | **Address Latch Enable (ALE) Polarity Control.**<br>`0` = Do Not Invert ALE. ALE is active low by default.<br>`1` = Invert. |
| 2-0 | `lidd_mode_sel` | R/W | 0h | **LIDD Mode Select.**<br>Selects type of LCD display interface for the LIDD to drive.<br>`000b` = Sync MPU68.<br>`001b` = Async MPU68.<br>`010b` = Sync MPU80.<br>`011b` = Async MPU80.<br>`100b` = Hitachi (Async).<br>`101b` = N/A.<br>`110b` = N/A.<br>`111b` = N/A. |

[cite_start][cite: 3140, 3148]

---

#### 13.5.1.4 LIDD_CS0_CONF Register (offset = 10h) [reset = 0h]

[cite_start]LIDD_CS0_CONF is shown in Figure 13-32 and described in Table 13-17[cite: 3156].

[Figure: Figure 13-32. LIDD_CS0_CONF Register. [cite_start]Shows bits 31-0 split into fields: w_su, w_strobe, w_hold, r_su, r_strobe, r_hold, and ta.] [cite: 3157-3158]

**Table 13-17. LIDD_CS0_CONF Register Field Descriptions**

| Bit | Field | Type | Reset | Description |
| :--- | :--- | :--- | :--- | :--- |
| 31-27 | `w_su` | R/W | 0h | **Write Strobe Set-Up cycles.**<br>When performing a write access, this field defines the number of memclk cycles that Data Bus/Pad Output Enable, the Direction bit, and Chip Select 0 have to be ready before the Write Strobe is asserted. The minimum value is 0x0. |
| 26-21 | `w_strobe` | R/W | 0h | **Write Strobe Duration cycles.**<br>Field value defines the number of memclk cycles for which the Write Strobe is held active when performing a write access. The minimum value is 0x1. |
| 20-17 | `w_hold` | R/W | 0h | **Write Strobe Hold cycles.**<br>Field value defines the number of memclk cycles for which Data Bus/Pas Output Enable, ALE, the Direction bit, and Chip Select 0 are held after the Write Strobe is de-asserted when performing write access. The minimum value is 0x1. |
| 16-12 | `r_su` | R/W | 0h | **Read Strobe Set-Up cycles.**<br>When performing a read access, this field defines the number of memclk cycles that Data Bus/Pad Output Enable, the Direction bit, and Chip Select 0 have to be ready before the Read Strobe is asserted. |
| 11-6 | `r_strobe` | R/W | 0h | **Read Strobe Duration cycles.**<br>Field value defines the number of memclk cycles for which the Read Strobe is held active when performing a read access. The minimum value is 0x1. |
| 5-2 | `r_hold` | R/W | 0h | **Read Strobe Hold cycles.**<br>Field value defines the number of memclk cycles for which Data Bus/Pad Output Enable, the Direction bit, and Chip Select 0 are held after the Read Strobe is deasserted when performing a read access. The minimum value is 0x1. |
| 1-0 | `ta` | R/W | 0h | Field value defines the number of memclk (ta+1) cycles between the end of one CS0 device access and the start of another CS0 device access unless the two accesses are both Reads. In this case, this delay is not incurred. The minimum value is 0x0. |

[cite_start][cite: 3162]

---

#### 13.5.1.5 LIDD_CS0_ADDR Register (offset = 14h) [reset = 0h]

[cite_start]LIDD_CS0_ADDR is shown in Figure 13-33 and described in Table 13-18[cite: 3170].

[Figure: Figure 13-33. LIDD_CS0_ADDR Register. [cite_start]Shows bits 31-0 split into fields: RESERVED and adr_indx.] [cite: 3171-3172]

**Table 13-18. LIDD_CS0_ADDR Register Field Descriptions**

| Bit | Field | Type | Reset | Description |
| :--- | :--- | :--- | :--- | :--- |
| 31-16 | `RESERVED` | R | 0h | |
| 15-0 | `adr_indx` | R/W | 0h | The LCD Controller supports a shared Address/Data output bus. A write to this register would initiate a bus write transaction. A read from this register would initiate a bus read transaction. CPU reads and writes to this register are not permitted if the LIDD module is in DMA mode (`cfg_lidd_dma_en = 1`). If the LIDD is being used as a generic bus interface, writing to this register can store adr_indx to an external transparent latch holding a 16-bit address. If the LIDD is being used to interface with a character based LCD panel in configuration mode, reading and writing to this register can be used to access the command instruction area of the panel. |

[cite_start][cite: 3175]

---

#### 13.5.1.6 LIDD_CS0_DATA Register (offset = 18h) [reset = 0h]

[cite_start]LIDD_CS0_DATA is shown in Figure 13-34 and described in Table 13-19[cite: 3183].

[Figure: Figure 13-34. LIDD_CS0_DATA Register. [cite_start]Shows bits 31-0 split into fields: RESERVED and data.] [cite: 3184-3185]

**Table 13-19. LIDD_CS0_DATA Register Field Descriptions**

| Bit | Field | Type | Reset | Description |
| :--- | :--- | :--- | :--- | :--- |
| 31-16 | `RESERVED` | R | 0h | |
| 15-0 | `data` | R/W | 0h | The LCD Controller supports a shared Address/Data output bus. A write to this register would initiate a bus write transaction. A read from this register would initiate a bus read transaction. CPU reads and writes to this register are not permitted if the LIDD module is in DMA mode (`cfg_lidd_dma_en = 1`). If the LIDD is being used as a generic bus interface, writing to this register can store adr_indx to an external transparent latch holding a 16-bit address. If the LIDD is being used to interface with a character based LCD panel in configuration mode, reading and writing to this register can be used to access the command instruction area of the panel. |

[cite_start][cite: 3189]

---

#### 13.5.1.7 LIDD_CS1_CONF Register (offset = 1Ch) [reset = 0h]

[cite_start]LIDD_CS1_CONF is shown in Figure 13-35 and described in Table 13-20[cite: 3197].

[Figure: Figure 13-35. LIDD_CS1_CONF Register. [cite_start]Shows bits 31-0 split into fields: w_su, w_strobe, w_hold, r_su, r_strobe, r_hold, and ta.] [cite: 3198-3199]

**Table 13-20. LIDD_CS1_CONF Register Field Descriptions**

| Bit | Field | Type | Reset | Description |
| :--- | :--- | :--- | :--- | :--- |
| 31-27 | `w_su` | R/W | 0h | **Write Strobe Set-Up cycles.**<br>When performing a write access, this field defines the number of memclk cycles that Data Bus/Pad Output Enable, the Direction bit, and Chip Select 1 have to be ready before the Write Strobe is asserted. The minimum value is 0x0. |
| 26-21 | `w_strobe` | R/W | 0h | **Write Strobe Duration cycles.**<br>Field value defines the number of memclk cycles for which the Write Strobe is held active when performing a write access. The minimum value is 0x1. |
| 20-17 | `w_hold` | R/W | 0h | **Write Strobe Hold cycles.**<br>Field value defines the number of memclk cycles for which Data Bus/Pad Output Enable, ALE, the Direction bit, and Chip Select 1 are held after the Write Strobe is deasserted when performing a write access. The minimum value is 0x1. |
| 16-12 | `r_su` | R/W | 0h | **Read Strobe Set-Up cycles.**<br>When performing a read access, this field defines the number of memclk cycles that Data Bus/Pad Output Enable, the Direction bit, and Chip Select 1 have to be ready before the Read Strobe is asserted. The minimum value is 0x0. |
| 11-6 | `r_strobe` | R/W | 0h | **Read Strobe Duration cycles.**<br>Field value defines the number of memclk cycles for which the Read Strobe is held active when performing a read access. The minimum value is 0x1. |
| 5-2 | `r_hold` | R/W | 0h | **Read Strobe Hold cycles.**<br>Field value defines the number of memclk cycles for which Data Bus/Pad Output Enable, the Direction bit, and Chip Select 1 are held after the Read Strobe is deasserted when performing a read access. The minimum value is 0x1. |
| 1-0 | `ta` | R/W | 0h | Field value defines the number of memclk (ta+1) cycles between the end of one CS1 device access and the start of another CS1 device access unless the two accesses are both Reads. In this case, this delay is not incurred. The minimum value is 0x0. |

[cite_start][cite: 3203]

---

#### 13.5.1.8 LIDD_CS1_ADDR Register (offset = 20h) [reset = 0h]

[cite_start]LIDD_CS1_ADDR is shown in Figure 13-36 and described in Table 13-21[cite: 3209].

[Figure: Figure 13-36. LIDD_CS1_ADDR Register. [cite_start]Shows bits 31-0 split into fields: RESERVED and adr_indx.] [cite: 3210-3211]

**Table 13-21. LIDD_CS1_ADDR Register Field Descriptions**

| Bit | Field | Type | Reset | Description |
| :--- | :--- | :--- | :--- | :--- |
| 31-16 | `RESERVED` | R | 0h | |
| 15-0 | `adr_indx` | R/W | 0h | The LCD Controller supports a shared Address/Data output bus. A write to this register would initiate a bus write transaction. A read from this register would initiate a bus read transaction. CPU reads and writes to this register are not permitted if the LIDD module is in DMA mode (`cfg_lidd_dma_en = 1`). If the LIDD is being used as a generic bus interface, writing to this register can store adr_indx to an external transparent latch holding a 16-bit address. If the LIDD is being used to interface with a character based LCD panel in configuration mode, reading and writing to this register can be used to access the command instruction area of the panel. |

[cite_start][cite: 3214]

---

#### 13.5.1.9 LIDD_CS1_DATA Register (offset = 24h) [reset = 0h]

[cite_start]LIDD_CS1_DATA is shown in Figure 13-37 and described in Table 13-22[cite: 3223].

[Figure: Figure 13-37. LIDD_CS1_DATA Register. [cite_start]Shows bits 31-0 split into fields: RESERVED and data.] [cite: 3225-3226]

**Table 13-22. LIDD_CS1_DATA Register Field Descriptions**

| Bit | Field | Type | Reset | Description |
| :--- | :--- | :--- | :--- | :--- |
| 31-16 | `RESERVED` | R | 0h | |
| 15-0 | `data` | R/W | 0h | The LCD Controller supports a shared Address/Data output bus. A write to this register would initiate a bus write transaction. A read from this register would initiate a bus read transaction. CPU reads and writes to this register are not permitted if the LIDD module is in DMA mode (`cfg_lidd_dma_en = 1`). If the LIDD is being used as a generic bus interface, writing to this register can store adr_indx to an external transparent latch holding a 16-bit address. If the LIDD is being used to interface with a character based LCD panel in configuration mode, reading and writing to this register can be used to access the command instruction area of the panel. |

[cite_start][cite: 3229]
#### 13.5.1.10 RASTER_CTRL Register (offset = 28h) [reset = 0h]

RASTER_CTRL is shown in Figure 13-38 and described in Table 13-23.

[Figure: Figure 13-38. RASTER_CTRL Register. [cite_start]Shows bits 31-0 split into fields: RESERVED, tft24unpacked, tft24, stn565, tftmap, nibmode, palmode, reqdly, mono8b, rdorder, lcdtft, lcdbw, and lcden.] [cite: 3364-3365]

**Table 13-23. RASTER_CTRL Register Field Descriptions**

| Bit | Field | Type | Reset | Description |
| :--- | :--- | :--- | :--- | :--- |
| 31-27 | `RESERVED` | R/W | 0h | |
| 26 | `tft24unpacked` | R/W | 0h | [cite_start]**24 bit Mode Packing.**<br>Only used when `cfg_tft24=1` and `cfg_lcdtft=1`.<br>`0` = 24-bit pixels are packed into 32 bit boundaries, which means 4 pixels are saved in every three words.<br>Word 0: pix1[0], pix0[23:0].<br>Word 1: pix2[15:0], pix1[23:8].<br>Word 2: pix3[23:0], pix2[23:16].<br>`1` = 24-bit pixels are stored unpacked with the uppermost byte unused.<br>Word 0: Unused[7:0], pix0[23:0].<br>Word 1: Unused[7:0], pix1[23:0].<br>Word 2: Unused[7:0], pix2[23:0].<br>Word 3: Unused[7:0], pix3[23:0]. [cite: 3368] |
| 25 | `tft24` | R/W | 0h | **24 bit mode.**<br>`0` = off.<br>`1` = on (24-bit data in active mode). [cite_start]The format of the framebuffer data depends on `cfg_tft24unpacked`. [cite: 3368] |
| 24 | `stn565` | R/W | 0h | **Passive Matrix Mode only** (`cfg_lcdtft='0'`) and 16 bpp raw data framebuffers (`bpp='00'`).<br>If the bpp field in the framebuffer palette header is '00' (12/16/24 bpp source), then the DDR contains raw data and the palette lookup is bypassed. Only for this case, this bit selects whether the framebuffer format is 16 bpp 565 or 12 bpp. The Grayscaler can only take 12 bits per pixel. [cite_start]The framebuffer data is 16 bits per pixel 565 when `stn565` is set to '1' and only the 4 most significant bits of each color component are sent to the Grayscaler input.<br>`0` = Framebuffer is 12 bpp packed in bits [11:0].<br>`1` = Framebuffer is 16 bpp 565. [cite: 3376] |
| 23 | `tftmap` | R/W | 0h | **TFT Mode Alternate Signal Mapping for Palettized framebuffer.**<br>Must be '0' for all 12/16/24 bpp raw data formats. Can only be '1' for 1/2/4/8 bpp Palette Lookup data. [cite_start]Valid only in Active Matrix mode when `cfg_lcdtft='1'`.<br>`0` = 4 bits per component output data for 1, 2, 4, and 8 bpp modes will be right aligned on lcd_pins(11:0).<br>`1` = 4 bits per component output data for 1, 2, 4, and 8 bpp will be converted to 5,6,5 format and use pins(15:0) R3 R2 R1 R0 R3 G3 G2 G1 G0 G3 G2 B3 B2 B1 B0 B3. [cite: 3376] |
| 22 | `nibmode` | R/W | 0h | [cite_start]**Nibble Mode.**<br>This bit is used to determine palette indexing and is used in conjunction with `cfg_rdorder`.<br>`0` = Nibble mode is disabled.<br>`1` = Nibble mode is enabled. [cite: 3376] |
| 21-20 | `palmode` | R/W | 0h | **Palette Loading Mode.**<br>`00` = Palette and data loading, reset value.<br>`01` = Palette loading only.<br>`10` = Data loading only. For Raw Data (12/16/24 bpp) framebuffers, no palette lookup is employed. [cite_start]Thus, these framebuffers should use the data-only loading mode. [cite: 3376] |
| 19-12 | `reqdly` | R/W | 0h | **Palette Loading Delay**<br>When loading the Palette from DDR, palette data is burst into the internal Palette SRAM from the Async FIFO. 1-, 2-, and 4-bit per pixel framebuffer encodings use a fixed 16-word entry palette residing above the video data. The 8 bit per pixel framebuffer encoding uses a 256-word entry palette residing above the video data. Likewise, 12, 16, and 24 bit per pixel framebuffer encodings also define a 256-word entry palette even though these encodings will not do a full bit-depth palette lookup. However, the 256-word palette entry must still be read from DDR as a framebuffer is fetched. Bursting in 256 words in sequential lcd_clk cycles may cause the Async FIFO to underflow depending on the SOC DDR burst bandwidth. This 8-bit `reqdly` parameter pauses reading of the Palette data from the Async FIFO between each burst of 16 words. The delay is in terms of lcd_clk (system clock) cycles.<br>Value (0-255) used to specify the number of system clock cycles that should be paused between bursts of 16 word reads from the Async FIFO while loading the Palette SRAM. [cite_start]Programming `reqdly=00h` disables this pause when loading the palette table. [cite: 3376] |
| 11-10 | `RESERVED` | R/W | [cite_start]0h | [cite: 3376] |
| 9 | `mono8b` | R/W | 0h | [cite_start]**Mono 8 bit.**<br>`0` = lcd_pixel_o[3:0] is used to output four pixel values to the panel each pixel clock transition.<br>`1` = lcd_pixel_o[7:0] is used to output eight pixel values to the panel each pixel clock transition.<br>This bit is ignored in all other modes. [cite: 3382] |
| 8 | `rdorder` | R/W | 0h | **Raster Data Order Select.**<br>`0` = The frame buffer parsing for Palette Data lookup is from Bit 0 to Bit 31 of the input word from the DMA output.<br>`1` = The frame buffer parsing for Palette Data lookup is from Bit 31 to Bit 0 of the input word from the DMA output.<br>This bit has no effect on raw data framebuffers (12/16/24 bpp). [cite_start]This bit is used to determine palette indexing and is used in conjunction with `cfg_nibmode`. [cite: 3382] |
| 7 | `lcdtft` | R/W | 0h | [cite_start]`0` = Passive or display operation enabled, dither logic is enabled.<br>`1` = Active or display operation enabled, external palette and DAC required, dither logic bypassed, pin timing changes to support continuous pixel clock, output enable, vsync, and hsync. [cite: 3382] |
| 6-2 | `RESERVED` | R/W | [cite_start]0h | [cite: 3382] |
| 1 | `lcdbw` | R/W | 0h | [cite_start]**Only Applies for Passive Matrix Panels LCD Monochrome.**<br>`0` = Color operation enabled.<br>`1` = Monochrome operation enabled. [cite: 3382] |
| 0 | `lcden` | R/W | 0h | [cite_start]**LCD Controller Enable.**<br>`0` = LCD controller disabled.<br>`1` = LCD controller enabled. [cite: 3382] |

---

#### 13.5.1.11 RASTER_TIMING_0 Register (offset = 2Ch) [reset = 0h]

RASTER_TIMING_0 is shown in Figure 13-39 and described in Table 13-24.

[Figure: Figure 13-39. RASTER_TIMING_0 Register. [cite_start]Shows bits 31-0 split into fields: hbp, hfp, hsw, pplmsb, ppllsb, and RESERVED.] [cite: 3394-3395]

**Table 13-24. RASTER_TIMING_0 Register Field Descriptions**

| Bit | Field | Type | Reset | Description |
| :--- | :--- | :--- | :--- | :--- |
| 31-24 | `hbp` | R/W | 0h | **Horizontal Back Porch Lowbits.**<br>Bits 7 to 0 of the horizontal back porch field. Encoded value (from 1-1024) used to specify the number of pixel clock periods to add to the beginning of a line transmission before the first set of pixels is output to the display (programmed value plus 1). [cite_start]Note that pixel clock is held in its inactive state during the beginning of the line wait period in passive display mode, and is permitted to transition in active display mode. [cite: 3397] |
| 23-16 | `hfp` | R/W | 0h | **Horizontal Front Porch Lowbits.**<br>Encoded value (from 1 to 1024) used to specify the number of pixel clock periods to add to the end of a line transmission before line clock is asserted (programmed value plus 1). [cite_start]Note that pixel clock is held in its inactive state during the end of line wait period in passive display mode, and is permitted to transition in active display mode. [cite: 3397] |
| 15-10 | `hsw` | R/W | 0h | **Horizontal Sync Pulse Width Lowbits.**<br>Bits 5 to 0 of the horizontal sync pulse width field. Encoded value (from 1-1024) used to specify the number of pixel clock periods to pulse the line clock at the end of each line (programmed value plus 1). [cite_start]Note that pixel clock is held in its inactive state during the generation of line clock in passive display mode, and is permitted to transition in active display mode. [cite: 3397] |
| 9-4 | `ppllsb` | R/W | 0h | **Pixels-per-line LSB [9:4].**<br>Encoded LSB value (from 1-1024) used to specify the number of pixels contained within each line on the LCD display (programmed to value minus one).<br>`PPL = 11'b{pplmsb, ppllsb, 4'b1111} + 1`<br>ex: `pplmsb=1'b1`, `pppllsb=6'b000100`<br>`PPL = 11'b{1, 000100, 1111} + 1 = 1104` (decimal) pixels per line. [cite_start]In other words, `PPL = 16*({pplmsb, ppllsb}+1)`. [cite: 3397] |
| 3 | `pplmsb` | R/W | 0h | [cite_start]**Pixels-per-line MSB[10].**<br>Needed in order to support up to 2048 ppl. [cite: 3397] |
| 2-0 | `RESERVED` | R | [cite_start]0h | [cite: 3397] |

---

#### 13.5.1.12 RASTER_TIMING_1 Register (offset = 30h) [reset = 0h]

RASTER_TIMING_1 is shown in Figure 13-40 and described in Table 13-25.

[Figure: Figure 13-40. RASTER_TIMING_1 Register. [cite_start]Shows bits 31-0 split into fields: vbp, vfp, vsw, and lpp.] [cite: 3405-3406]

**Table 13-25. RASTER_TIMING_1 Register Field Descriptions**

| Bit | Field | Type | Reset | Description |
| :--- | :--- | :--- | :--- | :--- |
| 31-24 | `vbp` | R/W | 0h | **Vertical Back Porch.**<br>Value (from 0-255) used to specify the number of line clock periods to add to the beginning of a frame before the first set of pixels is output to the display. [cite_start]Note that line clock transitions during the insertion of the extra line clock periods. [cite: 3410] |
| 23-16 | `vfp` | R/W | 0h | **Vertical Front Porch.**<br>Value (from 0-255) used to specify the number of line clock periods to add to the end of each frame. [cite_start]Note that the line clock transitions during the insertion of the extra line clock periods. [cite: 3410] |
| 15-10 | `vsw` | R/W | 0h | **Vertical Sync Width Pulse.**<br>In active mode (`lcdtft=1`), encoded value (from 1-64) used to specify the number of line clock periods to set the lcd_fp pin active at the end of each frame after the (vfp) period elapses. The number of clock cycles is programmed value plus one. The frame clock is used as the VSYNC signal in active mode. In passive mode (`lcdtft=0`), encoded value (from 1-64) used to specify the number of extra line clock periods to insert after the vertical front porch (vfp) period has elapsed. [cite_start]Note that the width of lcd_fp is not affected by `vsw` in passive mode and line clock transitions during the insertion of the extra line clock periods (programmed value plus one). [cite: 3410] |
| 9-0 | `lpp` | R/W | 0h | **Lines Per Panel.**<br>Encoded value (programmed value range of {0:2047} represents an actual range of {1:2048}) used to specify the number of lines per panel. It represents the total number of lines on the LCD (programmed value plus one). [cite_start]Bit 10 of this field is in RASTER_TIMING_2. [cite: 3410] |

---

#### 13.5.1.13 RASTER_TIMING_2 Register (offset = 34h) [reset = 0h]

RASTER_TIMING_2 is shown in Figure 13-41 and described in Table 13-26.

[Figure: Figure 13-41. RASTER_TIMING_2 Register. [cite_start]Shows bits 31-0 split into fields: RESERVED, hsw_highbits, lpp_b10, phsvs_on_off, phsvs_rf, ieo, ipc, ihs, ivs, acbi, acb, hbp_highbits, and hfp_highbits.] [cite: 3421-3422]

**Table 13-26. RASTER_TIMING_2 Register Field Descriptions**

| Bit | Field | Type | Reset | Description |
| :--- | :--- | :--- | :--- | :--- |
| 31 | `RESERVED` | R | [cite_start]0h | [cite: 3425] |
| 30-27 | `hsw_highbits` | R/W | 0h | [cite_start]Bits 9 to 6 of the horizontal sync width field. [cite: 3425] |
| 26 | `lpp_b10` | R/W | 0h | [cite_start]**Lines Per Panel Bit 10.**<br>Bit 10 of the `lpp` field in RASTER_TIMING_1. [cite: 3425] |
| 25 | `phsvs_on_off` | R/W | 0h | **Hsync/Vsync Pixel Clock Control On/Off.**<br>`0` = lcd_lp and lcd_fp are driven on opposite edges of pixel clock than the lcd_pixel_o.<br>`1` = lcd_lp and lcd_fp are driven according to bit 24.<br>Note that this bit MUST be set to '0' for Passive Matrix displays. [cite_start]The edge timing is fixed. [cite: 3425] |
| 24 | `phsvs_rf` | R/W | 0h | [cite_start]**Program HSYNC/VSYNC Rise or Fall.**<br>`0` = lcd_lp and lcd_fp are driven on the rising edge of pixel clock (bit 25 must be set to 1).<br>`1` = lcd_lp and lcd_fp are driven on the falling edge of pixel clock (bit 25 must be set to 1). [cite: 3425] |
| 23 | `ieo` | R/W | 0h | **Invert Output Enable.**<br>`0` = lcd_ac pin is active high in active display mode.<br>`1` = lcd_ac pin is active low in active display mode.<br>Active display mode: data driven out of the LCD's data lines on programmed pixel clock edge where AC-bias is active. [cite_start]Note that `ieo` is ignored in passive display mode. [cite: 3425] |
| 22 | `ipc` | R/W | 0h | **Invert Pixel Clock.**<br>`0` = Data is driven on the LCD's data lines on the rising edge of lcd_cp.<br>`1` = Data is driven on the LCD's data lines on the falling edge of lcd_cp.<br>For Active Matrix output (`cfg_lcdtft='1'`), the Output Pixel Clock is a free running clock in that it transitions in horizontal blanking (including horizontal front porch, horizontal back porch) areas and all vertical blanking times. For Passive Matrix output (`cfg_lcdtft='0'`), the Output Pixel Clock only occurs when an output data value is written. [cite_start]It is in a return-to-zero state when `cfg_ipc='0'` and a return-to-one state when `cfg_ipc='1'`. [cite: 3425] |
| 21 | `ihs` | R/W | 0h | [cite_start]**Invert Hsync.**<br>`0` = lcd_lp pin is active high and inactive low.<br>`1` = lcd_lp pin is active low and inactive high.<br>Active and passive mode: horizontal sync pulse/line clock active between lines, after the end of line wait period. [cite: 3425] |
| 20 | `ivs` | R/W | 0h | **Invert Vsync.**<br>`0` = lcd_fp pin is active high and inactive low.<br>`1` = lcd_fp pin is active low and inactive high.<br>Active mode: vertical sync pulse active between frames, after end of frame wait period. [cite_start]Passive mode: frame clock active during first line of each frame. [cite: 3432] |
| 19-16 | `acbi` | R/W | 0h | **AC Bias Pins Transitions per Interrupt.**<br>Value (from 0 to 15) used to specify the number of AC Bias pin transitions to count before setting the line count status (`lcs`) bit, signaling an interrupt request. Counter frozen when lcd is set, and is restarted when `lcs` is cleared by software. [cite_start]This function is disabled when `acbi=b'0000'`. [cite: 3432] |
| 15-8 | `acb` | R/W | 0h | **AC Bias Pin Frequency.**<br>Value (from 0-255) used to specify the number of line clocks to count before transitioning the AC Bias pin. This pin is used to periodically invert the polarity of the power supply to prevent DC charge build-up within the display. [cite_start]`acb` = Number of line clocks/toggle of the lcd_ac pin. [cite: 3432] |
| 7-6 | `RESERVED` | R | [cite_start]0h | [cite: 3432] |
| 5-4 | `hbp_highbits` | R/W | 0h | [cite_start]Bits 9:8 of the horizontal back porch field. [cite: 3432] |
| 3-2 | `RESERVED` | R | [cite_start]0h | [cite: 3432] |
| 1-0 | `hfp_highbits` | R/W | 0h | [cite_start]Bits 9:8 of the horizontal front porch field. [cite: 3432] |

---

#### 13.5.1.14 RASTER_SUBPANEL Register (offset = 38h) [reset = 0h]

RASTER_SUBPANEL is shown in Figure 13-42 and described in Table 13-27.

[Figure: Figure 13-42. RASTER_SUBPANEL Register. [cite_start]Shows bits 31-0 split into fields: spen, RESERVED, hols, lppt, and dpdlsb.] [cite: 3443-3444]

**Table 13-27. RASTER_SUBPANEL Register Field Descriptions**

| Bit | Field | Type | Reset | Description |
| :--- | :--- | :--- | :--- | :--- |
| 31 | `spen` | R/W | 0h | [cite_start]**Sub Panel Enable.**<br>`0` = function disabled.<br>`1` = sub-panel function mode enabled. [cite: 3447] |
| 30 | `RESERVED` | R | [cite_start]0h | [cite: 3447] |
| 29 | `hols` | R/W | 0h | [cite_start]**High or Low Signal.**<br>This field indicates the position of the sub-panel based on the LPPT value.<br>`0` = Default Pixel Data lines are at the top of the screen and the active video lines are at the bottom of the screen.<br>`1` = Active video lines are at the top of the screen and Default Pixel Data lines are at the bottom of the screen. [cite: 3447] |
| 28-26 | `RESERVED` | R/W | [cite_start]0h | [cite: 3447] |
| 25-16 | `lppt` | R/W | 0h | **Line Per Panel Threshold.**<br>Encoded value (programmed value range of {0:2047} represents an actual range of {1:2048}) used to specify the number of lines on the bottom part of the panel. Bit10 of this field is in RASTER_SUBPANEL2. Hols determines whether Default Pixel Data is on the top (`hols="0"`) or on the bottom (`hols='1'`). [cite_start]Lppt defines the number of lines on the bottom part of the output. [cite: 3447] |
| 15-0 | `dpdlsb` | R/W | 0h | [cite_start]**Default Pixel Data LSB [15:0].**<br>DPD defines the default value of the pixel data sent to the panel for the lines until LPPT is reach or after passing LPPT. [cite: 3447] |

---

#### 13.5.1.15 RASTER_SUBPANEL2 Register (offset = 3Ch) [reset = 0h]

RASTER_SUBPANEL2 is shown in Figure 13-43 and described in Table 13-28.

[Figure: Figure 13-43. RASTER_SUBPANEL2 Register. [cite_start]Shows bits 31-0 split into fields: RESERVED, lppt_b10, and dpdmsb.] [cite: 3456-3457]

**Table 13-28. RASTER_SUBPANEL2 Register Field Descriptions**

| Bit | Field | Type | Reset | Description |
| :--- | :--- | :--- | :--- | :--- |
| 31-9 | `RESERVED` | R | [cite_start]0h | [cite: 3460] |
| 8 | `lppt_b10` | R/W | 0h | [cite_start]**Lines Per Panel Threshold Bit 10.**<br>This register is Bit 10 of the `lppt` field in RASTER_SUBPANEL. [cite: 3460] |
| 7-0 | `dpdmsb` | R/W | 0h | [cite_start]**Default Pixel Data MSB [23:16].**<br>DPD defines the default value of the pixel data sent to the panel for the lines until LPPT is reached or after passing the LPPT. [cite: 3460] |

---

#### 13.5.1.16 LCDDMA_CTRL Register (offset = 40h) [reset = 0h]

LCDDMA_CTRL is shown in Figure 13-44 and described in Table 13-29.

[Figure: Figure 13-44. LCDDMA_CTRL Register. [cite_start]Shows bits 31-0 split into fields: RESERVED, dma_master_prio, th_fifo_ready, burst_size, byte_swap, bigendian, and frame_mode.] [cite: 3469-3470]

**Table 13-29. LCDDMA_CTRL Register Field Descriptions**

| Bit | Field | Type | Reset | Description |
| :--- | :--- | :--- | :--- | :--- |
| 31-19 | `RESERVED` | R/W | [cite_start]0h | [cite: 3473] |
| 18-16 | `dma_master_prio` | R/W | 0h | [cite_start]**Priority for the L3 OCP Master Bus.**<br>`000b` = Highest priority.<br>`111b` = Lowest priority. [cite: 3473] |
| 15-8 | `th_fifo_ready` | R/W | 0h | [cite_start]**DMA FIFO threshold.**<br>The DMA FIFO becomes ready when the number of words specified by this register from the frame buffer have been loaded.<br>`000b` = 8.<br>`001b` = 16.<br>`010b` = 32.<br>`011b` = 64.<br>`100b` = 128.<br>`101b` = 256.<br>`110b` = 512.<br>`111b` = Reserved. [cite: 3473] |
| 7 | `RESERVED` | R/W | [cite_start]0h | [cite: 3473] |
| 6-4 | `burst_size` | R/W | 0h | **Burst Size setting for DMA transfers** (all DMA transfers are 32 bits wide).<br>`000b` = burst size of 1.<br>`001b` = burst size of 2.<br>`010b` = burst size of 4.<br>`011b` = burst size of 8.<br>`100b` = burst size of 16.<br>`101b` = N/A.<br>`110b` = N/A.<br>`111b` = N/A.<br>`burst_size` cannot be changed once the DMA is enabled in LIDD or Raster modes. [cite_start]In this case, the DMA must be disabled and the Done Interrupt must occur before the value in this register can be changed. [cite: 3473] |
| 3 | `byte_swap` | R/W | 0h | This bit controls the bytelane ordering of the data on the output of the DMA module. It works in conjunction with the `bigendian` bit. [cite_start]See the `bigendian` description for configuration guidelines. [cite: 3473] |
| 2 | `RESERVED` | R | [cite_start]0h | [cite: 3473] |
| 1 | `bigendian` | R/W | 0h | **Big Endian Enable.**<br>Use this bit when the processor is operating in Big Endian mode AND writes to the frame buffer(s) are less than 32 bits wide. [cite_start]Only in this scenario do we need to change the byte alignment for data coming into the FIFO from the frame buffer(s).<br>`0` = Big Endian data reordering disabled.<br>`1` = Big Endian data reordering enabled. [cite: 3479] |
| 0 | `frame_mode` | R/W | 0h | [cite_start]**Frame Mode.**<br>`0` = one frame buffer (FB0 only) used.<br>`1` = two frame buffers used DMA ping-pongs between FB0 and FB1 in this mode. [cite: 3479] |

#### 13.5.1.17 LCDDMA_FB0_BASE Register (offset = 44h) [reset = 0h]

LCDDMA_FB0_BASE is shown in Figure 13-45 and described in Table 13-30.

[Figure: Figure 13-45. LCDDMA_FB0_BASE Register. [cite_start]Shows bits 31-0 split into fields: fb0_base and RESERVED.] [cite: 3708-3709]

**Table 13-30. LCDDMA_FB0_BASE Register Field Descriptions**

| Bit | Field | Type | Reset | Description |
| :--- | :--- | :--- | :--- | :--- |
| 31-2 | `fb0_base` | R/W | 0h | [cite_start]**Frame Buffer 0 Base Address pointer.** [cite: 3712] |
| 1-0 | `RESERVED` | R | [cite_start]0h | [cite: 3712] |

---

#### 13.5.1.18 LCDDMA_FB0_CEILING Register (offset = 48h) [reset = 0h]

LCDDMA_FB0_CEILING is shown in Figure 13-46 and described in Table 13-31.

[Figure: Figure 13-46. LCDDMA_FB0_CEILING Register. [cite_start]Shows bits 31-0 split into fields: fb0_ceil and RESERVED.] [cite: 3722-3723]

**Table 13-31. LCDDMA_FB0_CEILING Register Field Descriptions**

| Bit | Field | Type | Reset | Description |
| :--- | :--- | :--- | :--- | :--- |
| 31-2 | `fb0_ceil` | R/W | 0h | [cite_start]**Frame Buffer 0 Ceiling Address pointer.** [cite: 3726] |
| 1-0 | `RESERVED` | R | [cite_start]0h | [cite: 3726] |

---

#### 13.5.1.19 LCDDMA_FB1_BASE Register (offset = 4Ch) [reset = 0h]

LCDDMA_FB1_BASE is shown in Figure 13-47 and described in Table 13-32.

[Figure: Figure 13-47. LCDDMA_FB1_BASE Register. [cite_start]Shows bits 31-0 split into fields: fb1_base and RESERVED.] [cite: 3735-3736]

**Table 13-32. LCDDMA_FB1_BASE Register Field Descriptions**

| Bit | Field | Type | Reset | Description |
| :--- | :--- | :--- | :--- | :--- |
| 31-2 | `fb1_base` | R/W | 0h | [cite_start]**Frame Buffer 1 Base Address pointer.** [cite: 3739] |
| 1-0 | `RESERVED` | R | [cite_start]0h | [cite: 3739] |

---

#### 13.5.1.20 LCDDMA_FB1_CEILING Register (offset = 50h) [reset = 0h]

LCDDMA_FB1_CEILING is shown in Figure 13-48 and described in Table 13-33.

[Figure: Figure 13-48. LCDDMA_FB1_CEILING Register. [cite_start]Shows bits 31-0 split into fields: fb1_ceil and RESERVED.] [cite: 3748-3749]

**Table 13-33. LCDDMA_FB1_CEILING Register Field Descriptions**

| Bit | Field | Type | Reset | Description |
| :--- | :--- | :--- | :--- | :--- |
| 31-2 | `fb1_ceil` | R/W | 0h | [cite_start]**Frame Buffer 1 Ceiling Address pointer.** [cite: 3752] |
| 1-0 | `RESERVED` | R | [cite_start]0h | [cite: 3752] |

---

#### 13.5.1.21 SYSCONFIG Register (offset = 54h) [reset = 0h]

SYSCONFIG is shown in Figure 13-49 and described in Table 13-34.

[Figure: Figure 13-49. SYSCONFIG Register. [cite_start]Shows bits 31-0 split into fields: RESERVED, standbymode, idlemode, and RESERVED.] [cite: 3761-3762]

**Table 13-34. SYSCONFIG Register Field Descriptions**

| Bit | Field | Type | Reset | Description |
| :--- | :--- | :--- | :--- | :--- |
| 31-6 | `RESERVED` | R | [cite_start]0h | [cite: 3765] |
| 5-4 | `standbymode` | R/W | 0h | **Configuration of the local initiator state management mode.**<br>By definition, initiator may generate read/write transaction as long as it is out of STANDBY state.<br>`0` = Force-standby mode: local initiator is unconditionally placed in standby state. Backup mode, for debug only.<br>`1` = No-standby mode: local initiator is unconditionally placed out of standby state. Backup mode, for debug only.<br>`2` = Smart-standby mode: local initiator standby status depends on local conditions, that is, the module's functional requirement from the initiator. [cite_start]IP module shall not generate (initiator-related) wakeup events.<br>`3` = Reserved. [cite: 3765] |
| 3-2 | `idlemode` | R/W | 0h | **Configuration of the local target state management mode.**<br>By definition, target can handle read/write transaction as long as it is out of IDLE state.<br>`0` = Force-idle mode: local target's idle state follows (acknowledges) the system's idle requests unconditionally, i.e. regardless of the IP module's internal requirements. Backup mode, for debug only.<br>`1` = No-idle mode: local target never enters idle state. Backup mode, for debug only.<br>`2` = Smart-idle mode: local target's idle state eventually follows (acknowledges) the system's idle requests, depending on the IP module's internal requirements. [cite_start]IP module shall not generate (IRQ- or DMA-request-related) wakeup events.<br>`3` = Reserved. [cite: 3765] |
| 1-0 | `RESERVED` | R/W | [cite_start]0h | [cite: 3765] |

---

#### 13.5.1.22 IRQSTATUS_RAW Register (offset = 58h) [reset = 0h]

IRQSTATUS_RAW is shown in Figure 13-50 and described in Table 13-35.

[Figure: Figure 13-50. IRQSTATUS_RAW Register. [cite_start]Shows bits 31-0 split into various interrupt raw set fields.] [cite: 3774-3775]

**Table 13-35. IRQSTATUS_RAW Register Field Descriptions**

| Bit | Field | Type | Reset | Description |
| :--- | :--- | :--- | :--- | :--- |
| 31-10 | `RESERVED` | R/W | [cite_start]0h | [cite: 3778] |
| 9 | `eof1_raw_set` | R/W | 0h | **DMA End-of-Frame 1 Raw Interrupt Status and Set.**<br>Read indicates raw status.<br>`0` = Inactive.<br>`1` = Active.<br>Writing 1 will set status. [cite_start]Writing 0 has no effect. [cite: 3778] |
| 8 | `eof0_raw_set` | R/W | 0h | **DMA End-of-Frame 0 Raw Interrupt Status and Set.**<br>Read indicates raw status.<br>`0` = inactive.<br>`1` = active.<br>Writing 1 will set status. [cite_start]Writing 0 has no effect. [cite: 3778] |
| 7 | `RESERVED` | R/W | [cite_start]0h | [cite: 3778] |
| 6 | `pl_raw_set` | R/W | 0h | **DMA Palette Loaded Raw Interrupt Status and Set.**<br>Read indicates raw status.<br>`0` = inactive.<br>`1` = active.<br>Writing 1 will set status. [cite_start]Writing 0 has no effect. [cite: 3778] |
| 5 | `fuf_raw_set` | R/W | 0h | **DMA FIFO Underflow Raw Interrupt Status and Set.**<br>LCD dithering logic not supplying data to FIFO at a sufficient rate, FIFO has completely emptied and data pin driver logic has attempted to take added data from FIFO. Read indicates raw status.<br>`0` = Inactive.<br>`1` = Active.<br>Writing 1 will set status. [cite_start]Writing 0 has no effect. [cite: 3778] |
| 4 | `RESERVED` | R | [cite_start]0h | [cite: 3778] |
| 3 | `acb_raw_set` | R/W | 0h | **For Passive Matrix Panels Only AC Bias Count Raw Interrupt Status and Set.**<br>AC bias transition counter has decremented to zero, indicating that the lcd_ac_o line has transitioned the number of times which is specified by the `acbi` control bit-field. The counter is reloaded with the value in `acbi` but it is disabled until the user clears ABC. Read indicates raw status.<br>`0` = inactive.<br>`1` = active.<br>Writing 1 will set status. [cite_start]Writing 0 has no effect. [cite: 3786] |
| 2 | `sync_raw_set` | R/W | 0h | **Frame Synchronization Lost Raw Interrupt Status and Set.**<br>Read indicates raw status.<br>`0` = inactive.<br>`1` = active.<br>Writing 1 will set status. [cite_start]Writing 0 has no effect. [cite: 3786] |
| 1 | `recurrent_raster_done_raw_set` | R/W | 0h | **Raster Mode Frame Done Interrupt.**<br>Read indicates raw status.<br>`0` = Inactive.<br>`1` = Active.<br>Writing 1 will set status. [cite_start]Writing 0 has no effect. [cite: 3786] |
| 0 | `done_raw_set` | R/W | 0h | **Raster or LIDD Frame Done (shared, depends on whether Raster or LIDD mode enabled).**<br>Raw Interrupt Status and Set Read indicates raw status.<br>`0` = inactive.<br>`1` = active.<br>Writing 1 will set status. [cite_start]Writing 0 has no effect. [cite: 3786] |

---

#### 13.5.1.23 IRQSTATUS Register (offset = 5Ch) [reset = 0h]

IRQSTATUS is shown in Figure 13-51 and described in Table 13-36.

[Figure: Figure 13-51. IRQSTATUS Register. [cite_start]Shows bits 31-0 split into various interrupt enable and clear fields.] [cite: 3797]

**Table 13-36. IRQSTATUS Register Field Descriptions**

| Bit | Field | Type | Reset | Description |
| :--- | :--- | :--- | :--- | :--- |
| 31-10 | `RESERVED` | R/W | [cite_start]0h | [cite: 3800] |
| 9 | `eof1_en_clr` | R/W | 0h | **DMA End-of-Frame 1 Enabled Interrupt and Clear.**<br>Read indicates enabled (masked) status.<br>`0` = inactive.<br>`1` = active.<br>Writing 1 will clear interrupt enable. [cite_start]Writing 0 has no effect. [cite: 3800] |
| 8 | `eof0_en_clr` | R/W | 0h | **DMA End-of-Frame 0 Enabled Interrupt and Clear.**<br>Read indicates enabled (masked) status.<br>`0` = inactive.<br>`1` = active.<br>Writing 1 will clear interrupt enable. [cite_start]Writing 0 has no effect. [cite: 3800] |
| 7 | `RESERVED` | R/W | [cite_start]0h | [cite: 3800] |
| 6 | `pl_en_clr` | R/W | 0h | **DMA Palette Loaded Enabled Interrupt and Clear.**<br>Read indicates enabled (masked) status.<br>`0` = inactive.<br>`1` = active.<br>Writing 1 will clear interrupt enable. [cite_start]Writing 0 has no effect. [cite: 3800] |
| 5 | `fuf_en_clr` | R/W | 0h | **DMA FIFO Underflow Enabled Interrupt and Clear.**<br>LCD dithering logic not supplying data to FIFO at a sufficient rate, FIFO has completely emptied and data pin driver logic has attempted to take added data from FIFO. Read indicates enabled (masked) status.<br>`0` = inactive.<br>`1` = active.<br>Writing 1 will clear interrupt enable. [cite_start]Writing 0 has no effect. [cite: 3800] |
| 4 | `RESERVED` | R/W | [cite_start]0h | [cite: 3800] |
| 3 | `acb_en_clr` | R/W | 0h | **For Passive Matrix Panels Only. AC Bias Count Enabled Interrupt and Clear.**<br>AC bias transition counter has decremented to zero, indicating that the lcd_ac_o line has transitioned the number of times which is specified by the `acbi` control bit-field. The counter is reloaded with the value in `acbi` but it is disabled until the user clears ABC. Read indicates enabled (masked) status.<br>`0` = inactive.<br>`1` = active.<br>Writing 1 will clear interrupt enable. [cite_start]Writing 0 has no effect. [cite: 3807] |
| 2 | `sync_en_clr` | R/W | 0h | **Frame Synchronization Lost Enabled Interrupt and Clear.**<br>Read indicates enabled status.<br>`0` = inactive.<br>`1` = active.<br>Writing 1 will clear interrupt enable. [cite_start]Writing 0 has no effect. [cite: 3807] |
| 1 | `recurrent_raster_done_en_clr` | R/W | 0h | **Raster Frame Done Interrupt.**<br>Read indicates enabled status.<br>`0` = Inactive.<br>`1` = Active.<br>Writing 1 will clear interrupt enable. [cite_start]Writing 0 has no effect. [cite: 3807] |
| 0 | `done_en_clr` | R/W | 0h | **Raster or LIDD Frame Done (shared, depends on whether Raster or LIDD mode enabled) Enabled Interrupt and Clear.**<br>Read indicates enabled status.<br>`0` = inactive.<br>`1` = active.<br>Writing 1 will clear interrupt enable. [cite_start]Writing 0 has no effect. [cite: 3807] |

---

#### 13.5.1.24 IRQENABLE_SET Register (offset = 60h) [reset = 0h]

IRQENABLE_SET is shown in Figure 13-52 and described in Table 13-37.

[Figure: Figure 13-52. IRQENABLE_SET Register. [cite_start]Shows bits 31-0 split into various interrupt enable set fields.] [cite: 3818-3819]

**Table 13-37. IRQENABLE_SET Register Field Descriptions**

| Bit | Field | Type | Reset | Description |
| :--- | :--- | :--- | :--- | :--- |
| 31-10 | `RESERVED` | R/W | [cite_start]0h | [cite: 3822] |
| 9 | `eof1_en_set` | R/W | 0h | **DMA End-of-Frame 1 Interrupt Enable Set.**<br>Read indicates enabled (mask) status.<br>`0` = disabled.<br>`1` = enabled.<br>Writing 1 will set interrupt enable. [cite_start]Writing 0 has no effect. [cite: 3822] |
| 8 | `eof0_en_set` | R/W | 0h | **DMA End-of-Frame 0 Interrupt Enable Set.**<br>Read indicates enabled (mask) status.<br>`0` = disabled.<br>`1` = enabled.<br>Writing 1 will set interrupt enable. [cite_start]Writing 0 has no effect. [cite: 3822] |
| 7 | `RESERVED` | R/W | [cite_start]0h | [cite: 3822] |
| 6 | `pl_en_set` | R/W | 0h | **DMA Palette Loaded Interrupt Enable Set.**<br>Read indicates enabled (mask) status.<br>`0` = disabled.<br>`1` = enabled.<br>Writing 1 will set interrupt enable. [cite_start]Writing 0 has no effect. [cite: 3822] |
| 5 | `fuf_en_set` | R/W | 0h | **DMA FIFO Underflow Interrupt Enable Set.**<br>LCD dithering logic not supplying data to FIFO at a sufficient rate, FIFO has completely emptied and data pin driver logic has attempted to take added data from FIFO. Read indicates enabled (mask) status.<br>`0` = disabled.<br>`1` = enabled.<br>Writing 1 will set interrupt enable. [cite_start]Writing 0 has no effect. [cite: 3822] |
| 4 | `RESERVED` | R | [cite_start]0h | [cite: 3822] |
| 3 | `acb_en_set` | R/W | 0h | **For Passive Matrix Panels Only AC Bias Count Interrupt Enable Set.**<br>AC bias transition counter has decremented to zero, indicating that the lcd_ac_o line has transitioned the number of times which is specified by the `acbi` control bit-field. The counter is reloaded with the value in `acbi` but it is disabled until the user clears ABC. Read indicates enabled (mask) status.<br>`0` = disabled.<br>`1` = enabled.<br>Writing 1 will set interrupt enable. [cite_start]Writing 0 has no effect. [cite: 3831] |
| 2 | `sync_en_set` | R/W | 0h | **Frame Synchronization Lost Interrupt Enable Set.**<br>Read indicates enabled (mask) status.<br>`0` = disabled.<br>`1` = enabled.<br>Writing 1 will set interrupt enable. [cite_start]Writing 0 has no effect. [cite: 3831] |
| 1 | `recurrent_raster_done_en_set` | R/W | 0h | **Raster Done Interrupt Enable Set.**<br>Read indicates enabled (mask) status.<br>`0` = Disabled.<br>`1` = Enabled.<br>Writing 1 will set interrupt enable. [cite_start]Writing 0 has no effect. [cite: 3831] |
| 0 | `done_en_set` | R/W | 0h | [cite_start]**Raster or LIDD Frame Done (shared, depends on whether Raster or LIDD mode enabled) Interrupt Enable Set.**<br>Read indicates enabled (mask) status.<br>`0` = disabled.<br>`1` = enabled.<br>Writing 1 will set interrupt enable. [cite: 3831] |

#### 13.5.1.25 IRQENABLE_CLEAR Register (offset = 64h) [reset = 0h]

IRQENABLE_CLEAR is shown in Figure 13-53 and described in Table 13-38.

[Figure: Figure 13-53. IRQENABLE_CLEAR Register. [cite_start]Shows bits 31-0 split into various interrupt enable clear fields.] [cite: 5669-5674]

**Table 13-38. IRQENABLE_CLEAR Register Field Descriptions**

| Bit | Field | Type | Reset | Description |
| :--- | :--- | :--- | :--- | :--- |
| 31-10 | `RESERVED` | R/W | 0h | |
| 9 | `eof1_en_clr` | R/W | 0h | **DMA End-of-Frame 1 Interrupt Enable Clear.**<br>Read indicates enabled status.<br>`0` = disabled.<br>`1` = enabled.<br>Writing 1 will clear interrupt enable. Writing 0 has no effect. |
| 8 | `eof0_en_clr` | R/W | 0h | **DMA End-of-Frame 0 Interrupt Enable Clear.**<br>Read indicates enabled status.<br>`0` = disabled.<br>`1` = enabled.<br>Writing 1 will clear interrupt enable. Writing 0 has no effect. |
| 7 | `RESERVED` | R/W | 0h | |
| 6 | `pl_en_clr` | R/W | 0h | **DMA Palette Loaded Interrupt Enable Clear.**<br>Read indicates enabled status.<br>`0` = disabled.<br>`1` = enabled.<br>Writing 1 will clear interrupt enable. Writing 0 has no effect. |
| 5 | `fuf_en_clr` | R/W | 0h | **DMA FIFO Underflow Interrupt Enable Clear.**<br>LCD dithering logic not supplying data to FIFO at a sufficient rate, FIFO has completely emptied and data pin driver logic has attempted to take added data from FIFO.<br>Read indicates enabled status.<br>`0` = disabled.<br>`1` = enabled.<br>Writing 1 will clear interrupt enable. Writing 0 has no effect. |
| 4 | `RESERVED` | R/W | 0h | |
| 3 | `acb_en_clr` | R/W | 0h | **For Passive Matrix Panels Only. AC Bias Count Interrupt Enable Clear.**<br>AC bias transition counter has decremented to zero, indicating that the lcd_ac_o line has transitioned the number of times which is specified by the `acbi` control bit-field. The counter is reloaded with the value in `acbi` but it is disabled until the user clears ABC. Read indicates enabled status.<br>`0` = disabled.<br>`1` = enabled.<br>Writing 1 will clear interrupt enable. Writing 0 has no effect. |
| 2 | `sync_en_clr` | R/W | 0h | **Frame Synchronization Lost Interrupt Enable Clear.**<br>Read indicates enabled status.<br>`0` = disabled.<br>`1` = enabled.<br>Writing 1 will clear interrupt enable. Writing 0 has no effect. |
| 1 | `recurrent_raster_done_en_clr` | R/W | 0h | **Raster Done Interrupt Enable Clear.**<br>Read indicates enabled status.<br>`0` = Disabled.<br>`1` = Enabled.<br>Writing 1 will clear interrupt enable. Writing 0 has no effect. |
| 0 | `done_en_clr` | R/W | 0h | **Raster or LIDD Frame Done (shared, depends on whether Raster or LIDD mode enabled) Interrupt Enable Clear.**<br>Read indicates enabled status.<br>`0` = disabled.<br>`1` = enabled.<br>Writing 1 will clear interrupt enable. Writing 0 has no effect. |

---

#### 13.5.1.26 CLKC_ENABLE Register (offset = 6Ch) [reset = 0h]

CLKC_ENABLE is shown in Figure 13-54 and described in Table 13-39.

[Figure: Figure 13-54. CLKC_ENABLE Register. [cite_start]Shows bits 31-0 split into fields: RESERVED, dma_clk_en, lidd_clk_en, and core_clk_en.] [cite: 5691-5696]

**Table 13-39. CLKC_ENABLE Register Field Descriptions**

| Bit | Field | Type | Reset | Description |
| :--- | :--- | :--- | :--- | :--- |
| 31-3 | `RESERVED` | R | 0h | |
| 2 | `dma_clk_en` | R/W | 0h | **Software Clock Enable for the DMA submodule.**<br>The DMA submodule runs on the L3 Clock domain. |
| 1 | `lidd_clk_en` | R/W | 0h | **Software Clock Enable for the LIDD submodule (character displays).**<br>The LIDD submodule runs on the System Clock (`lcd_clk`) domain. |
| 0 | `core_clk_en` | R/W | 0h | **Software Clock Enable for the Core, which encompasses the Raster Active Matrix and Passive Matrix logic.**<br>The Core runs on the System Clock (`lcd_clk`) domain. |

---

#### 13.5.1.27 CLKC_RESET Register (offset = 70h) [reset = 0h]

CLKC_RESET is shown in Figure 13-55 and described in Table 13-40.

[Figure: Figure 13-55. CLKC_RESET Register. [cite_start]Shows bits 31-0 split into fields: RESERVED, main_rst, dma_rst, lidd_rst, and core_rst.] [cite: 5705-5709]

**Table 13-40. CLKC_RESET Register Field Descriptions**

| Bit | Field | Type | Reset | Description |
| :--- | :--- | :--- | :--- | :--- |
| 31-4 | `RESERVED` | R/W | 0h | |
| 3 | `main_rst` | R/W | 0h | **Software Reset for the entire LCD module.**<br>`1` = Reset Enable.<br>`0` = Reset Disable. |
| 2 | `dma_rst` | R/W | 0h | **Software Reset for the DMA submodule.**<br>`1` = Reset Enable.<br>`0` = Reset Disable. |
| 1 | `lidd_rst` | R/W | 0h | **Software Reset for the LIDD submodule (character displays).**<br>`1` = Reset Enable.<br>`0` = Reset Disable. |
| 0 | `core_rst` | R/W | 0h | **Software Reset for the Core, which encompasses the Raster Active Matrix and Passive Matrix logic.**<br>`1` = Reset Enable.<br>`0` = Reset Disable. |
