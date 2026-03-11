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
