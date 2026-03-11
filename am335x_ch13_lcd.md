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
