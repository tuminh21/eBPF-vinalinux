# 21.1 Introduction

[cite_start]The multi-master I2C peripheral provides an interface between a CPU and any I2C-bus-compatible device that connects via the I2C serial bus[cite: 5]. [cite_start]External components attached to the I2C bus can serially transmit/receive up to 8-bit data to/from the CPU device through the two-wire I2C interface[cite: 6].

[cite_start]The I2C bus is a multi-master bus[cite: 7]. [cite_start]The I2C controller supports the multi-master mode that allows more than one device capable of controlling the bus to be connected to it[cite: 7]. 

### Device Roles and Communication
* [cite_start]**Addressing:** Each I2C device is recognized by a unique address and can operate as either transmitter or receiver, according to the function of the device[cite: 8].
* [cite_start]**Master/Slave Dynamics:** In addition to being a transmitter or receiver, a device connected to the I2C bus can also be considered as master or slave when performing data transfers[cite: 9].
* [cite_start]**Master Definition:** A master device is the device which initiates a data transfer on the bus and generates the clock signals to permit that transfer[cite: 10].
* [cite_start]**Slave Definition:** During a transfer, any device addressed by this master is considered a slave[cite: 11].

## 21.1.1 I2C Features

The general features of the I2C controller are:
* [cite_start]**Specification:** Compliant with Philips I2C specification version 2.1[cite: 14].
* [cite_start]**Speed:** Supports standard mode (up to 100K bits/s) and fast mode (up to 400K bits/s)[cite: 15].
* **Operational Modes:**
    * [cite_start]Multimaster transmitter/slave receiver mode[cite: 16].
    * [cite_start]Multimaster receiver/slave transmitter mode[cite: 17].
    * [cite_start]Combined master transmit/receive and receive/transmit modes[cite: 18].
* [cite_start]**Addressing Modes:** 7-bit and 10-bit device addressing modes[cite: 19].
* [cite_start]**Buffering:** Built-in 32-byte FIFO for buffered read or writes in each module[cite: 20].
* [cite_start]**Generation & I/O:** Programmable clock generation, two DMA channels, and one interrupt line[cite: 21, 22].

---

## 21.1.2 Unsupported I2C Features

[cite_start]The I2C module features not supported in this device are shown in Table 21-1[cite: 25].

| Feature | Reason |
| :--- | :--- |
| SCCB Protocol | [cite_start]SCCB signal not pinned out [cite: 26] |
| High Speed (3.4 MBPS) operation | [cite_start]Not supported [cite: 26] |

**Table 21-1. [cite_start]Unsupported I2C Features** [cite: 27]

---
[cite_start]*Source: Texas Instruments Incorporated, SPRUH73Q (Revised December 2019)* [cite: 1, 2, 29, 30]

# 21.2 Integration

[cite_start]This device includes three instantiations of the I2C module[cite: 36]. [cite_start]This peripheral implements the multi-master I2C bus which allows serial transfer of 8-bit data to/from other I2C master/slave devices through a two-wire interface[cite: 36].

The three I2C module instantiations are:
* [cite_start]**I2C0:** Located in the Wake-up power domain[cite: 37].
* [cite_start]**I2C1 and I2C2:** General peripheral instantiations[cite: 37, 109].

## 21.2.1 I2C Connectivity Attributes

The following tables detail the connectivity and resource allocation for the I2C modules.

### Table 21-2. I2C0 Connectivity Attributes
| Attributes | Type |
| :--- | :--- |
| Power Domain | [cite_start]Wakeup Domain [cite: 98] |
| Clock Domain | [cite_start]PD WKUP L4 WKUP GCLK (Interface/OCP) [cite: 98][cite_start]<br>PD WKUP I2C0_GFCLK (Func) [cite: 98] |
| Reset Signals | [cite_start]WKUP_DOM_RST_N [cite: 98] |
| Idle/Wakeup Signals | [cite_start]Smart Idle / Wakeup [cite: 98] |
| Interrupt Requests | [cite_start]1 interrupt to MPU Subsystem (I2C0INT), PRU-ICSS, and WakeM3 [cite: 98] |
| DMA Requests | [cite_start]2 DMA requests to EDMA (I2CTXEVT0, I2CRXEVT0) [cite: 98] |
| Physical Address | [cite_start]L4 Wakeup slave port [cite: 98] |

### Table 21-3. I2C(1-2) Connectivity Attributes
| Attributes | Type |
| :--- | :--- |
| Power Domain | [cite_start]Peripheral Domain [cite: 108] |
| Clock Domain | [cite_start]PD_PER_L4LS_GCLK (Interface/OCP) [cite: 108][cite_start]<br>PD_PER_I2C_FCLK (Func) [cite: 108] |
| Reset Signals | [cite_start]PER_DOM_RST_N [cite: 108] |
| Idle/Wakeup Signals | [cite_start]Smart Idle [cite: 108] |
| Interrupt Requests | [cite_start]1 interrupt per instance to MPU Subsystem (I2C1INT, I2C2INT) [cite: 108] |
| DMA Requests | [cite_start]2 DMA requests per instance to EDMA (I2CTXEVTX, I2CRXEVTX) [cite: 108] |
| Physical Address | [cite_start]L4 Peripheral slave port [cite: 108] |

---

## 21.2.2 I2C Clock and Reset Management

[cite_start]The I2C controllers utilize separate bus interface and functional clocks[cite: 111]. [cite_start]During power-down mode, the `I2Cx_SCL` and `I2Cx_SDA` pins are configured as inputs[cite: 112].

### Table 21-4. I2C Clock Signals
| Clock Signal | Max Freq | Reference / Source | Comments |
| :--- | :--- | :--- | :--- |
| **I2C0 Interface (PIOCPCLK)** | - | pd_wkup_l4_wkup_gclk | [cite_start]From PRCM [cite: 114] |
| **I2C0 Functional (PISYSCLK)** | 48 MHz | [cite_start]PER CLKOUTM2/4 | pd_wkup_i2c0_gfclk from PRCM [cite: 114] |
| **I2C(1-2) Interface (PIOCPCLK)** | 100 MHz | [cite_start]CORE_CLKOUTM4/2 | pd_per_l4ls_gclk from PRCM [cite: 114] |
| **I2C(1-2) Functional (PISYSCLK)** | 48 MHz | [cite_start]PER CLKOUTM2/4 | pd_per_i2c_fclk from PRCM [cite: 114] |

---

## 21.2.3 I2C Pin List

[cite_start]The external signals use standard LVCMOS I/Os[cite: 116]. [cite_start]**Note:** These may not meet full I2C specification compliance for Fast-mode devices regarding slope control and input filtering[cite: 116].

| Pin | Type | Description |
| :--- | :--- | :--- |
| **I2Cx_SCL** | I/OD | [cite_start]I2C serial clock (open drain) [cite: 117] |
| **I2Cx_SDA** | I/OD | [cite_start]I2C serial data (open drain) [cite: 117] |

### Hardware Implementation Recommendations
* [cite_start]**Input Enabling:** The associated `CONF_<module>_<pin>_RXACTIVE` bit must be set to 1 to enable the inputs back to the module[cite: 121].
* [cite_start]**Signal Integrity:** It is recommended to place a 33-ohm re

# [cite_start]21.3 Functional Description [cite: 131]

## [cite_start]21.3.1 Functional Block Diagram [cite: 132]
[cite_start]The I2C peripheral consists of the following primary blocks: [cite: 159]
* [cite_start]**Serial Interface:** Includes one data pin (`I2C_SDA`) and one clock pin (`I2C_SCL`). [cite: 160]
* [cite_start]**Data Registers:** Temporarily hold receive and transmit data traveling between the `I2C_SDA` pin and the CPU or DMA controller. [cite: 161]
* [cite_start]**Control and Status Registers:** Manage module operation and report state. [cite: 162]
* [cite_start]**Peripheral Data Bus Interface:** Enables the CPU and DMA controller to access the I2C peripheral registers. [cite: 163]
* [cite_start]**Clock Synchronizer:** Synchronizes the I2C input clock (from the processor clock generator) with the `I2C_SCL` pin, and synchronizes data transfers with masters of different clock speeds. [cite: 164]
* [cite_start]**Prescaler:** Divides down the input clock driven to the I2C peripheral. [cite: 165]
* [cite_start]**Noise Filter:** Located on each of the two pins, `I2C_SDA` and `I2C_SCL`. [cite: 166]
* [cite_start]**Arbitrator and Interrupt Logic:** Handles arbitration between the I2C peripheral (when acting as a master) and other masters, and generates interrupts for the CPU. [cite: 167]
* [cite_start]**DMA Event Logic:** Sends an interrupt to the CPU upon data reception and transmission. [cite: 168]

## [cite_start]21.3.2 I2C Master/Slave Controller Signals [cite: 169]
[cite_start]Data is communicated to devices interfacing with the I2C via the serial data line (SDA) and the serial clock line (SCL). [cite: 170]
* [cite_start]**Bi-directional Pins:** Both SDA and SCL are bi-directional pins. [cite: 172] [cite_start]They must be connected to a positive supply voltage via a pull-up resistor. [cite: 172]
* [cite_start]**Bus State:** When the bus is free, both pins are high. [cite: 173]
* [cite_start]**Driver Type:** The driver of these two pins has an open drain to perform the required wired-AND function. [cite: 173]

### Table 21-6. [cite_start]Signal Pads [cite: 194]
| Name | Default Operating Mode | Description |
| :--- | :--- | :--- |
| **I2C_SCL** | In/Out | I2C serial CLK line. Open-drain output buffer. [cite_start]Requires external pull-up resistor (Rp). [cite: 191] |
| **I2C_SDA** | In/Out | I2C serial data line. Open-drain output buffer. [cite_start]Requires external pull-up resistor (Rp). [cite: 191] |

---

## [cite_start]21.3.3 I2C Reset [cite: 193]
[cite_start]The I2C module can be reset in the following three ways: [cite: 195]
1.  [cite_start]**System Reset (`PIRSTNA = 0`):** A device reset causes the system reset. [cite: 196] [cite_start]All registers are reset to power up reset values. [cite: 197]
2.  [cite_start]**Software Reset:** Triggered by setting the `SRST` bit in the `I2C_SYSC` register. [cite: 198] [cite_start]This has exactly the same action on the module logic as the system bus reset. [cite: 199] [cite_start]All registers are reset to power up reset values. [cite: 200]
3.  [cite_start]**Module Reset Control (`I2C_EN` bit):** The `I2C_EN` bit in the `I2C_CON` register can be used to hold the I2C module in reset. [cite: 200] [cite_start]When the system bus reset is removed (`PIRSTNA=1`), `I2C_EN=0` keeps the functional part of the I2C module in reset state and all configuration registers can be accessed. [cite: 201] [cite_start]Setting `I2C_EN=0` does not reset the registers to power up reset values. [cite: 202]

### Table 21-7. [cite_start]Reset State of I2C Signals [cite: 204]
| Pin | I/O/Z | System Reset | (`I2C_EN = 0`) |
| :--- | :--- | :--- | :--- |
| **SDA** | I/O/Z | High impedance | [cite_start]High impedance [cite: 203] |
| **SCL** | I/O/Z | High impedance | [cite_start]High impedance [cite: 203] |
[cite_start]*(Note: I = Input, O = Output, Z = High impedance)* [cite: 206]

---

## [cite_start]21.3.4 Data Validity [cite: 213]
* [cite_start]**Stability:** The data on the SDA line must be stable during the high period of the clock. [cite: 215]
* **State Changes:** The high and low states of the data line can only change when the clock signal on the SCL line is LOW. [cite: 216]

## 21.3.5 START & STOP Conditions
[cite_start]The I2C module generates START and STOP conditions when it is configured as a master[cite: 234].
* [cite_start]**START condition:** A high-to-low transition on the SDA line while SCL is high[cite: 235].
* [cite_start]**STOP condition:** A low-to-high transition on the SDA line while SCL is high[cite: 236].
* [cite_start]**Bus State:** The bus is considered to be busy after the START condition (BB=1) and free after the STOP condition (BB=0)[cite: 237].

---

## 21.3.6 I2C Operation

### 21.3.6.1 Serial Data Formats
* [cite_start]**Word Size:** The I2C controller operates in an 8-bit word data format (byte write access is supported for the last access)[cite: 246]. [cite_start]Each byte put on the SDA line is 8 bits long[cite: 247].
* [cite_start]**Transmission Length:** The number of bytes that can be transmitted or received is restricted by the value programmed in the DCOUNT register[cite: 248].
* [cite_start]**Transfer Order:** The data is transferred with the most significant bit (MSB) first[cite: 249].
* [cite_start]**Acknowledgment:** Each byte is followed by an acknowledge bit from the I2C module if it is in receiver mode[cite: 250]. [cite_start]In the acknowledge mode, an extra bit dedicated for acknowledgment is inserted after each byte[cite: 271].

[cite_start]The I2C module supports two data formats[cite: 268]:
1. [cite_start]7-bit/10-bit addressing format[cite: 268].
2. [cite_start]7-bit/10-bit addressing format with repeated start condition[cite: 269].

**Addressing Structure**
* [cite_start]The first byte after a start condition (S) always consists of 8 bits[cite: 270].
* [cite_start]**7-bit Addresses:** The first byte is composed of 7 MSB slave address bits and 1 LSB R/nW bit[cite: 272].
* [cite_start]**10-bit Addresses:** The first byte is composed of 7 MSB slave address bits, such as 11110XX (where XX is the two MSB of the 10-bit addresses), and 1 LSB R/nW bit, which is 0 in this case[cite: 273].
* [cite_start]**Transmission Direction:** The least significant R/nW of the address byte indicates the direction of transmission of the following data bytes[cite: 282]. [cite_start]If R/nW is 0, the master writes data into the selected slave [cite: 283][cite_start]; if it is 1, the master reads data out of the slave[cite: 284].

### 21.3.6.2 Master Transmitter
[cite_start]In this mode, data assembled in one of the previously described data formats is shifted out on the serial data line SDA in synch with the self-generated clock pulses on the serial clock line SCL[cite: 337]. [cite_start]The clock pulses are inhibited and SCL held low when the intervention of the processor is required (XUDF) after a byte has been transmitted[cite: 338].

### 21.3.6.3 Master Receiver
[cite_start]This mode can only be entered from the master transmitter mode[cite: 340]. [cite_start]With either of the address formats, the master receiver is entered after the slave address byte and bit R/W has been transmitted, if R/W is high[cite: 341].
* [cite_start]Serial data bits received on bus line SDA are shifted in synch with the self-generated clock pulses on SCL[cite: 342].
* [cite_start]The clock pulses are inhibited and SCL held low when the intervention of the processor is required (ROVR) after a byte has been transmitted[cite: 343].
* [cite_start]At the end of a transfer, it generates the stop condition[cite: 344].

### 21.3.6.4 Slave Transmitter
[cite_start]This mode can only be entered from the slave receiver mode[cite: 346]. [cite_start]With either of the address formats, the slave transmitter is entered if the slave address byte is the same as its own address and bit R/W has been transmitted, if R/W is high[cite: 347].
* [cite_start]The slave transmitter shifts the serial data out on the data line SDA in synch with the clock pulses that are generated by the master device[cite: 348].
* [cite_start]It does not generate the clock but it can hold clock line SCL low while intervention of the CPU is required (XUDF)[cite: 349].

### 21.3.6.5 Slave Receiver
[cite_start]In this mode, serial data bits received on the bus line SDA are shifted-in in synch with the clock pulses on SCL that are generated by the master device[cite: 351]. [cite_start]It does not generate the clock but it can hold clock line SCL low while intervention of the CPU is required (ROVR) following the reception of a byte[cite: 352].

## 21.3.7 Arbitration
[cite_start]If two or more master transmitters start a transmission on the same bus almost simultaneously, an arbitration procedure is invoked[cite: 360]. [cite_start]The arbitration procedure uses the data presented on the serial bus by the competing transmitters[cite: 361]. 
* [cite_start]When a transmitter senses that a high signal it has presented on the bus has been overruled by a low signal, it switches to the slave receiver mode, sets the arbitration lost (AL) flag, and generates the arbitration lost interrupt[cite: 362]. 
* [cite_start]The arbitration procedure gives priority to the device that transmits the serial data stream with the lowest binary value[cite: 363]. 
* [cite_start]Should two or more devices send identical first bytes, arbitration continues on the subsequent bytes[cite: 364].


## 21.3.8 I2C Clock Generation and I2C Clock Synchronization
[cite_start]Under normal conditions, only one master device generates the clock signal, SCL[cite: 388]. [cite_start]During the arbitration procedure, however, there are two or more master devices and the clock must be synchronized so that the data output can be compared[cite: 389].
* [cite_start]The wired-AND property of the clock line means that a device that first generates a low period of the clock line overrules the other devices[cite: 390].
* [cite_start]At this high/low transition, the clock generators of the other devices are forced to start generation of their own low period[cite: 391].
* [cite_start]The clock line is then held low by the device with the longest low period, while the other devices that finish their low periods must wait for the clock line to be released before starting their high periods[cite: 392].
* [cite_start]A synchronized signal on the clock line is thus obtained, where the slowest device determines the length of the low period and the fastest the length of the high period[cite: 393].


**Clock Stretching & Bus Clear Operations**
* [cite_start]If a device pulls down the clock line for a longer time, the result is that all clock generators must enter the WAIT-state[cite: 394]. [cite_start]In this way a slave can slow down a fast master and the slow device can create enough time to store a received byte or to prepare a byte to be transmitted (Clock Stretching)[cite: 395].
* [cite_start]If the clock line (SCL) is stuck low, the preferred procedure is to reset the bus using the hardware reset signal if your I2C devices have hardware reset inputs[cite: 398]. [cite_start]If the I2C devices do not have hardware reset inputs, cycle power to the devices to activate the mandatory internal power-on reset (POR) circuit[cite: 399].
* [cite_start]If the data line (SDA) is stuck low, the master should send nine clock pulses[cite: 400]. [cite_start]The device that held the bus low should release it sometime within those nine clocks[cite: 401]. [cite_start]If not, use the hardware reset or cycle power to clear the bus[cite: 402].

## 21.3.9 Prescaler (SCLK/ICLK)
* [cite_start]The I2C module is operated with a functional clock (SCLK) frequency that can be in a range of 12-100 MHz, according to I2C mode that must be used (an internal ~24 MHz clock (ICLK) is recommended in case of F/S operation mode)[cite: 419].
* [cite_start]Note that the frequency of the functional clock influences directly the I2C bus performance and timings[cite: 420].
* [cite_start]The internal clock used for I2C logic - ICLK - is generated via the I2C prescaler block[cite: 421].
* [cite_start]The prescaler consists of a 4-bit register - `I2C_PSC`, and is used to divide the system clock (SCLK) to obtain the internal required clock for the I2C module[cite: 422].

## 21.3.10 Noise Filter
* [cite_start]The noise filter is used to suppress any noise that is 50 ns or less, in the case of F/S mode of operation[cite: 424].
* It is designed to suppress noise with one ICLK. [cite_start]The noise filter is always one ICLK cycle, regardless of the bus speed[cite: 425].
* [cite_start]For FS mode (prescaler = 4, ICLK = 24 MHz), the maximum width of the suppressed spikes is 41.6 ns[cite: 426]. [cite_start]To ensure a correct filtering, the prescaler must be programmed accordingly[cite: 427].

## 21.3.11 I2C Interrupts
[cite_start]The I2C module generates 12 types of interrupt: addressed as slave, bus free (stop condition detected), access error, start condition, arbitration-lost, noacknowledge, general call, registers-ready-for-access, receive and transmit data, receive and transmit draining[cite: 429].
* [cite_start]These 12 interrupts are accompanied with 12 interrupt masks and flags defined in the `I2C_IRQENABLE_SET` and respectively `I2C_IRQSTATUS_RAW` registers[cite: 430]. [cite_start]Note that all these 12 interrupt events are sharing the same hardware interrupt line[cite: 431].
* [cite_start]**Addressed As Slave interrupt (AAS):** Generated to inform the Local Host that an external master addressed the module as a slave[cite: 432]. [cite_start]When this interrupt occurs, the CPU can check the `I2C_ACTOA` status register to check which of the 4 own addresses was used[cite: 433].
* [cite_start]**Bus Free interrupt (BF):** Generated to inform the Local Host that the I2C bus became free (when a Stop Condition is detected on the bus) and the module can initiate his own I2C transaction[cite: 434].
* [cite_start]**Start Condition interrupt (STC):** Generated after the module being in idle mode have detected a possible Start Condition on the bus (signalized with WakeUp)[cite: 435].
* [cite_start]**Access Error interrupt (AERR):** Generated if a Data read access is performed while RX FIFO is empty or a Data write access is performed while TX FIFO is full[cite: 436].
* [cite_start]**Arbitration lost interrupt (AL):** Generated when the I2C arbitration procedure is lost[cite: 437].
* [cite_start]**No-acknowledge interrupt (NACK):** Generated when the master I2C does not receive acknowledge from the receiver[cite: 438].
* [cite_start]**General call interrupt (GC):** Generated when the device detects the address of all zeros (8 bits)[cite: 439].
* [cite_start]**Registers-ready-for-access interrupt (ARDY):** Generated by the I2C when the previously programmed address, data, and command have been performed and the status bits have been updated[cite: 440].
* [cite_start]**Receive interrupt/status (RRDY):** Generated when there is received data ready to be read by the CPU from the `I2C_DATA` register[cite: 451].
* [cite_start]**Transmit interrupt/status (XRDY):** Generated when the CPU needs to put more data in the `I2C_DATA` register after the transmitted data has been shifted out on the SDA pin[cite: 453].
* [cite_start]**Receive/Transmit draining interrupts (RDR/XDR):** Generated when the transfer length is not a multiple of threshold value, to inform the CPU that it can read/write the amount of data left and to enable the draining mechanism[cite: 455, 456].

## 21.3.12 DMA Events
[cite_start]The I2C module can generate two DMA requests events, read (`I2C_DMA_RX`) and write (`I2C_DMA_TX`) that can be used by the DMA controller to synchronously read received data from the `I2C_DATA` or write transmitted data to the `I2C_DATA` register[cite: 459].

## 21.3.13 Interrupt and DMA Events
* [cite_start]I2C has two DMA channels (Tx and Rx)[cite: 464].
* [cite_start]I2C has one interrupt line for all the interrupt requests[cite: 465].

## 21.3.14 FIFO Management
[cite_start]The I2C module implements two internal 32-bytes FIFOs with dual clock for RX and TX modes[cite: 468]. [cite_start]The depth of the FIFOs can be configured at integration via a generic parameter which will also be reflected in `I2C_IRQSTATUS_RAW.FIFODEPTH` register[cite: 469].


### 21.3.14.1 FIFO Interrupt Mode Operation
[cite_start]In FIFO interrupt mode, the processor is informed of the status of the receiver and transmitter by an interrupt signal[cite: 471]. [cite_start]These interrupts are raised when receive/transmit FIFO threshold (defined by `I2C_BUF.TXTRSH` or `I2C_BUF.RXTRSH`) are reached[cite: 472]. 
* [cite_start]**Receive Mode:** RRDY interrupt is not generated until the FIFO reaches its receive threshold[cite: 493]. [cite_start]Once low, the interrupt can only be de-asserted when the Local Host has handled enough bytes to make the FIFO level below threshold[cite: 494].
* [cite_start]**Transmit Mode:** The interrupt request is generated when TX FIFO is empty, or the TX FIFO threshold is not reached and there are still data bytes to be transferred in the TX FIFO[cite: 505].

### 21.3.14.2 FIFO Polling Mode Operation
[cite_start]In FIFO polled mode, the status of the module (receiver or transmitter) can be checked by polling the XRDY and RRDY status registers (`I2C_IRQSTATUS_RAW`)[cite: 523]. [cite_start]This mode is an alternative to the FIFO interrupt mode of operation[cite: 525].

### 21.3.14.3 FIFO DMA Mode Operation
* [cite_start]**Receive DMA:** Generated as soon as the receive FIFO exceeds its threshold level (`I2C_BUF.RXTRSH + 1`)[cite: 527]. [cite_start]This request should be de-asserted when the number of bytes defined by the threshold level has been read by the DMA, by setting `I2C_DMARXENABLE_CLR.DMARX_ENABLE_CLEAR`[cite: 528].
* [cite_start]**Transmit DMA:** Automatically asserted when the transmit FIFO is empty[cite: 537]. [cite_start]De-asserted when the number of bytes in the threshold register (`I2C_BUF.TXTHRS+1`) has been written by the DMA, by setting `I2C_DMATXENABLE_CLR.DMATX_ENABLE_CLEAR`[cite: 538].
* [cite_start]**Thresholds & Clearing:** * In I2C Slave TX Mode, the TX FIFO threshold should be set to 1 (`I2C_BUF.TXTRSH = 0`), since the length of the transfer may not be known at configuration time[cite: 576]. 
    * [cite_start]The I2C module offers the possibility to the user to clear the RX or TX FIFO using `I2C_BUF.RXFIFO_CLR` and `I2C_BUF.TXFIFO_CLR` registers, which act like software reset for the FIFOs[cite: 581, 582].

## 21.3.14.4 Draining Feature
[cite_start]The Draining Feature is implemented by the I2C core for handling the end of transfers whose length is not a multiple of the FIFO threshold value, and offers the possibility to transfer the remaining amount of bytes (since the threshold is not reached)[cite: 598]. [cite_start]Note that this feature prevents the CPU or the DMA controller from attempting more FIFO accesses than necessary (for example, to generate at the end of a transfer a DMA RX request having in the FIFO fewer bytes than the configured DMA transfer length)[cite: 599]. [cite_start]Otherwise, an Access Error interrupt will be generated (see `I2C_IRQSTATUS_RAW.AERR` interrupt)[cite: 600]. 

[cite_start]The Draining mechanism will generate an interrupt (`I2C_IRQSTATUS_RAW.RDR` or `I2C_IRQSTATUS_RAW.XDR`) at the end of the transfer informing the CPU that it needs to check the amount of data left to be transferred (`I2C_BUFSTAT.TXSTAT` or `RXSTAT`) and to enable the Draining Feature of the DMA controller if DMA mode is enabled (by re-configuring the DMA transfer length according to this value), or perform only the required number of data accesses if DMA mode is disabled[cite: 601].

* [cite_start]**Receiving Mode (Master or Slave):** If the RX FIFO threshold is not reached but the transfer was ended on the I2C bus and there is still data left in the FIFO (less than the threshold), the receive draining interrupt (`I2C_IRQSTATUS_RAW.RDR`) will be asserted to inform the local host that it can read the amount of data in the FIFO (`I2C_BUFSTAT.RXSTAT`)[cite: 602]. The CPU will perform a number of data read accesses equal to the `RXSTAT` value (if in interrupt or polling mode) or re-configure the DMA controller with the required value in order to drain the FIFO[cite: 603].
* [cite_start]**Master Transmit Mode:** If the TX FIFO threshold is not reached but the amount of data remaining to be written in the FIFO is less than `TXTRSH`, the transmit draining interrupt (`I2C_IRQSTATUS_RAW.XDR`) will be asserted to inform the local host that it can read the amount of data remained to be written in the TX FIFO (`I2C_BUFSTAT.TXSTAT`)[cite: 604]. The CPU will need to write the required number of data bytes (specified by `TXSTAT` value) or re-configure the DMA controller with the required value in order to transfer the last bytes to the FIFO[cite: 605].

> [cite_start]**Note:** In master mode, the CPU can alternatively skip the checking of `TXSTAT` and `RXSTAT` values since it can obtain this information internally (by computing `DATACOUNT` modulo `TX/RXTHRSH`)[cite: 606].

[cite_start]The draining feature is disabled by default, and it can be enabled using `I2C_IRQENABLE_SET.XDR_IE` or `I2C_IRQENABLE_SET.RDR_IE` registers (default disabled) only for transfers with lengths not equal to the threshold value[cite: 607].

---

## 21.3.15 How to Program I2C

### 21.3.15.1 Module Configuration Before Enabling the Module
1. [cite_start]Program the prescaler to obtain an approximately 12-MHz I2C module clock (`I2C_PSC = x`; this value is to be calculated and is dependent on the System clock frequency)[cite: 610].
2. [cite_start]Program the I2C clock to obtain 100 Kbps or 400 Kbps (`SCLL = x` and `SCLH = x`: these values are to be calculated and are dependent on the System clock frequency)[cite: 611, 612].
3. [cite_start]Configure its own address (`I2C_OA = x`) - only in case of I2C operating mode (F/S mode)[cite: 613].
4. [cite_start]Take the I2C module out of reset (`I2C_CON:I2C_EN = 1`)[cite: 614].

### 21.3.15.2 Initialization Procedure
1. [cite_start]Configure the I2C mode register (`I2C_CON`) bits[cite: 616].
2. [cite_start]Enable interrupt masks (`I2C_IRQENABLE_SET`), if using interrupt for transmit/receive data[cite: 617].
3. [cite_start]Enable the DMA (`I2C_BUF` and `I2C_DMA/RX/TX/ENABLE_SET` and program the DMA controller) - only in case of I2C operating mode (F/S mode), if using DMA for transmit/receive data[cite: 618, 619].

### 21.3.15.3 Configure Slave Address and DATA Counter Registers
[cite_start]In master mode, configure the slave address (`I2C_SA = x`) and the number of bytes associated with the transfer (`I2C_CNT = x`)[cite: 621].

### 21.3.15.4 Initiate a Transfer
[cite_start]Poll the bus busy (BB) bit in the I2C status register (`I2C_IRQSTATUS_RAW`)[cite: 631]. [cite_start]If it is cleared to 0 (bus not busy), configure START/STOP (`I2C_CON: STT/I2C_CON: STP` condition to initiate a transfer) - only in case of I2C operating mode (F/S mode)[cite: 632].

### 21.3.15.5 Receive Data
[cite_start]Poll the receive data ready interrupt flag bit (RRDY) in the I2C status register (`I2C_IRQSTATUS_RAW`), use the RRDY interrupt (`I2C_IRQENABLE_SET.RRDY_IE` set) or use the DMA RX (`I2C_BUF.RDMA_EN` set together with `I2C_DMARXENABLE_SET`) to read the received data in the data receive register (`I2C_DATA`)[cite: 634]. [cite_start]Use the draining feature (`I2C_IRQSTATUS_RAW.RDR` enabled by `I2C_IRQENABLE_SET.RDR_IE`) if the transfer length is not equal to the FIFO threshold[cite: 635].

### 21.3.15.6 Transmit Data
[cite_start]Poll the transmit data ready interrupt flag bit (XRDY) in the I2C status register (`I2C_IRQSTATUS_RAW`), use the XRDY interrupt (`I2C_IRQENABLE_SET.XRDY_IE` set) or use the DMA TX (`I2C_BUF.XDMA_EN` set together with `I2C_DMATXENABLE_SET`) to write data into the data transmit register (`I2C_DATA`)[cite: 638]. [cite_start]Use the draining feature (`I2C_IRQSTATUS_RAW.XDR` enabled by `I2C_IRQENABLE_SET.XDR_IE`) if the transfer length is not equal to the FIFO threshold[cite: 639].

---

## 21.3.16 I2C Behavior During Emulation
[cite_start]To configure the I2C to stop during emulation suspend events (for example, debugger breakpoints), set up the I2C and the Debug Subsystem[cite: 641]:
1. Set `I2C_SYSTEST.FREE = 0`. [cite_start]This will allow the Suspend_Control signal from the Debug Subsystem to stop and start the I2C[cite: 642]. [cite_start]Note that if `FREE = 1`, the Suspend_Control signal is ignored and the I2C is free running regardless of any debug suspend event[cite: 643]. [cite_start]This FREE bit gives local control from a module perspective to gate the suspend signal coming from the Debug Subsystem[cite: 644].
2. [cite_start]Set the appropriate `xxx_Suspend_Control` register = `0x9` as described in Section 27.1.1.1, Debug Suspend Support for Peripherals[cite: 645]. [cite_start]Choose the register appropriate to the peripheral you want to suspend during a suspend event[cite: 646].

# [cite_start]21.4 I2C Registers [cite: 653]

[cite_start]**NOTE:** All bits defined as reserved must be written by software with 0s, for preserving future compatibility. [cite: 654] [cite_start]When read, any reserved bit returns 0. [cite: 655] [cite_start]Also, note that it is good software practice to use complete mask patterns for setting or testing individually bit fields within a register. [cite: 655]

## [cite_start]21.4.1 I2C Registers [cite: 656, 657]

[cite_start]Table 21-8 lists the memory-mapped registers for the I2C. [cite: 658] [cite_start]All register offset addresses not listed in Table 21-8 should be considered as reserved locations and the register contents should not be modified. [cite: 658]


### Table 21-8. [cite_start]I2C Registers [cite: 659]

| Offset | Acronym / Register Name | Section |
| :--- | :--- | :--- |
| **0h** | I2C_REVNB_LO (Module Revision Register - low bytes) | [cite_start]Section 21.4.1.1 [cite: 660] |
| **4h** | I2C_REVNB_HI (Module Revision Register - high bytes) | [cite_start]Section 21.4.1.2 [cite: 660] |
| **10h** | I2C_SYSC (System Configuration Register) | [cite_start]Section 21.4.1.3 [cite: 660] |
| **24h** | I2C_IRQSTATUS_RAW (I2C Status Raw Register) | [cite_start]Section 21.4.1.4 [cite: 660] |
| **28h** | I2C_IRQSTATUS (I2C Status Register) | [cite_start]Section 21.4.1.5 [cite: 660] |
| **2Ch** | I2C_IRQENABLE_SET (I2C Interrupt Enable Set Register) | [cite_start]Section 21.4.1.6 [cite: 660] |
| **30h** | I2C_IRQENABLE_CLR (I2C Interrupt Enable Clear Register) | [cite_start]Section 21.4.1.7 [cite: 660] |
| **34h** | I2C_WE (I2C Wakeup Enable Register) | [cite_start]Section 21.4.1.8 [cite: 660] |
| **38h** | I2C_DMARXENABLE_SET (Receive DMA Enable Set Register) | [cite_start]Section 21.4.1.9 [cite: 660] |
| **3Ch** | I2C_DMATXENABLE_SET (Transmit DMA Enable Set Register) | [cite_start]Section 21.4.1.10 [cite: 660] |
| **40h** | I2C_DMARXENABLE_CLR (Receive DMA Enable Clear Register) | [cite_start]Section 21.4.1.11 [cite: 660] |
| **44h** | I2C_DMATXENABLE_CLR (Transmit DMA Enable Clear Register) | [cite_start]Section 21.4.1.12 [cite: 660] |
| **48h** | I2C_DMARXWAKE_EN (Receive DMA Wakeup Register) | [cite_start]Section 21.4.1.13 [cite: 660] |
| **4Ch** | I2C_DMATXWAKE_EN (Transmit DMA Wakeup Register) | [cite_start]Section 21.4.1.14 [cite: 660] |
| **90h** | I2C_SYSS (System Status Register) | [cite_start]Section 21.4.1.15 [cite: 660] |
| **94h** | I2C_BUF (Buffer Configuration Register) | [cite_start]Section 21.4.1.16 [cite: 660] |
| **98h** | I2C_CNT (Data Counter Register) | [cite_start]Section 21.4.1.17 [cite: 660] |
| **9Ch** | I2C_DATA (Data Access Register) | [cite_start]Section 21.4.1.18 [cite: 660] |
| **A4h** | I2C_CON (I2C Configuration Register) | [cite_start]Section 21.4.1.19 [cite: 660] |
| **A8h** | I2C_OA (I2C Own Address Register) | [cite_start]Section 21.4.1.20 [cite: 660] |
| **ACh** | I2C_SA (I2C Slave Address Register) | [cite_start]Section 21.4.1.21 [cite: 660] |
| **B0h** | I2C_PSC (I2C Clock Prescaler Register) | [cite_start]Section 21.4.1.22 [cite: 660] |
| **B4h** | I2C_SCLL (I2C SCL Low Time Register) | [cite_start]Section 21.4.1.23 [cite: 660] |
| **B8h** | I2C_SCLH (I2C SCL High Time Register) | [cite_start]Section 21.4.1.24 [cite: 660] |
| **BCh** | I2C_SYSTEST (System Test Register) | [cite_start]Section 21.4.1.25 [cite: 660] |
| **C0h** | I2C_BUFSTAT (I2C Buffer Status Register) | [cite_start]Section 21.4.1.26 [cite: 660] |
| **C4h** | I2C_OA1 (I2C Own Address 1 Register) | [cite_start]Section 21.4.1.27 [cite: 660] |
| **C8h** | I2C_OA2 (I2C Own Address 2 Register) | [cite_start]Section 21.4.1.28 [cite: 660] |
| **CCh** | I2C_OA3 (I2C Own Address 3 Register) | [cite_start]Section 21.4.1.29 [cite: 660] |
| **D0h** | I2C_ACTOA (Active Own Address Register) | [cite_start]Section 21.4.1.30 [cite: 660] |
| **D4h** | I2C_SBLOCK (I2C Clock Blocking Enable Register) | [cite_start]Section 21.4.1.31 [cite: 660] |

---
[cite_start]*Source: SPRUH73Q—October 2011—Revised December 2019, Texas Instruments Incorporated.* [cite: 661, 662]
## 21.4.1.1 I2C_REVNB_LO Register (offset = 0h) [reset = 0h]

[cite_start]This read-only register contains the hard-coded revision number of the module[cite: 672]. [cite_start]A write to this register has no effect[cite: 672]. 
* [cite_start]I2C controller with interrupt using interrupt vector register (`I2C_IV`) is revision 1.x[cite: 673]. 
* [cite_start]I2C controller with interrupt using status register bits (`I2C_IRQSTATUS_RAW`) is revision 2.x[cite: 674].


### Table 21-9. I2C_REVNB_LO Register Field Descriptions
| Bit | Field | Type | Reset | Description |
| :--- | :--- | :--- | :--- | :--- |
| **31-16** | RESERVED | R | 0h | [cite_start]Reserved[cite: 679]. |
| **15-11** | RTL | R | 0h | [cite_start]RTL version[cite: 679]. |
| **10-8** | MAJOR | R | 0h | [cite_start]Major Revision[cite: 679]. [cite_start]This field changes when there is a major feature change[cite: 679]. [cite_start]This field does not change due to a bug fix or minor feature change[cite: 679]. |
| **7-6** | CUSTOM | R | 0h | [cite_start]Indicates a special version for a particular device[cite: 679]. [cite_start]Consequence of use may avoid use of standard Chip Support Library (CSL) / Drivers[cite: 679]. [cite_start]0 if non-custom[cite: 679]. |
| **5-0** | MINOR | R | 0h | [cite_start]Minor Revision[cite: 679]. [cite_start]This field changes when features are scaled up or down[cite: 679]. [cite_start]This field does not change due to a bug fix or major feature change[cite: 679]. |
[cite_start]*(LEGEND: R/W = Read/Write; R = Read only; W1toCl = Write 1 to clear bit; -n = value after reset)* [cite: 677]

---

## 21.4.1.2 I2C_REVNB_HI Register (offset = 4h) [reset = 0h]

[cite_start]A reset has no effect on the value returned[cite: 688].

### Table 21-10. I2C_REVNB_HI Register Field Descriptions
| Bit | Field | Type | Reset | Description |
| :--- | :--- | :--- | :--- | :--- |
| **31-16** | RESERVED | R | 0h | [cite_start]Reserved[cite: 694]. |
| **15-14** | SCHEME | R | 0h | [cite_start]Used to distinguish between old Scheme and current[cite: 694]. [cite_start]Spare bit to encode future schemes[cite: 694]. |
| **13-12** | RESERVED | R | 0h | [cite_start]Reserved[cite: 694]. |
| **11-0** | FUNC | R | 0h | [cite_start]Function: Indicates a software compatible module family[cite: 694]. |
[cite_start]*(LEGEND: R/W = Read/Write; R = Read only; W1toCl = Write 1 to clear bit; -n = value after reset)* [cite: 692]

---

## 21.4.1.3 I2C_SYSC Register (offset = 10h) [reset = 0h]

[cite_start]This register allows controlling various parameters of the peripheral interface[cite: 703].


### Table 21-11. I2C_SYSC Register Field Descriptions
| Bit | Field | Type | Reset | Description |
| :--- | :--- | :--- | :--- | :--- |
| **31-10** | RESERVED | R | 0h | [cite_start]Reserved[cite: 709]. |
| **9-8** | CLKACTIVITY | R/W | 0h | [cite_start]Clock Activity selection bits[cite: 709]. [cite_start]Set to 1 to disable external clock gating mechanism in Idle Mode[cite: 709]. [cite_start]Values after reset are low[cite: 709]. [cite_start]<br><br>**Note:** If the System Clock is cut-off, the module will assert a WakeUp event when it asynchronously detects a Start Condition on the I2C Bus[cite: 709]. [cite_start]In this case, the first transfer will not be taken into account (NACK will be detected by the external master) [cite: 709][cite_start].<br><br>`0h` = Both clocks can be cut off [cite: 709][cite_start].<br>`1h` = Only Interface/OCP clock must be kept active; system clock can be cut off[cite: 709].<br>`2h` = Only system clock must be kept active; [cite_start]Interface/OCP clock can be cut off [cite: 709][cite_start].<br>`3h` = Both clocks must be kept active[cite: 709]. |
| **7-5** | RESERVED | R | 0h | [cite_start]Reserved[cite: 709]. |
| **4-3** | IDLEMODE | R/W | 0h | [cite_start]Idle Mode selection bits[cite: 709]. [cite_start]Used to select one of the idle mode operation mechanisms[cite: 709]. [cite_start]Value after reset is 00 (Force Idle) [cite: 709][cite_start].<br>`1h` = No Idle mode [cite: 709][cite_start].<br>`2h` = Smart Idle mode [cite: 709][cite_start].<br>`3h` = Smart-idle wakeup (Available only on I2C0)[cite: 709]. |
| **2** | ENAWAKEUP | R/W | 0h | [cite_start]Enable Wakeup control bit[cite: 709]. [cite_start]When set to 1, the module enables its own wakeup mechanism[cite: 709]. [cite_start]Value after reset is low [cite: 709][cite_start].<br>`0h` = Wakeup mechanism is disabled [cite: 709][cite_start].<br>`1h` = Wakeup mechanism is enabled[cite: 709]. |
| **1** | SRST | R/W | 0h | [cite_start]SoftReset bit[cite: 718]. [cite_start]When set to 1, entire module is reset as for the hardware reset[cite: 718]. [cite_start]Automatically cleared to 0 by the core; only reset by hardware reset[cite: 718]. [cite_start]During reads, it always returns 0 [cite: 718][cite_start].<br>`0h` = Normal mode [cite: 718][cite_start].<br>`1h` = The module is reset[cite: 718]. |
| **0** | AUTOIDLE | R/W | 0h | [cite_start]Autoidle bit[cite: 718]. [cite_start]When set to 1, the module activates its own idle mode mechanism[cite: 718]. [cite_start]By evaluating its internal state, the module can decide to gate part of its internal clock tree to improve power consumption [cite: 718][cite_start].<br>`0h` = Auto Idle mechanism is disabled [cite: 718][cite_start].<br>`1h` = Auto Idle mechanism is enabled[cite: 718]. |
[cite_start]*(LEGEND: R/W = Read/Write; R = Read only; W1toCl = Write 1 to clear bit; -n = value after reset)* [cite: 708]

## [cite_start]21.4.1.4 I2C_IRQSTATUS_RAW Register (offset = 24h) [reset = 0h] [cite: 729]

[cite_start]This register provides core status information for interrupt handling, showing all active events (enabled and not enabled)[cite: 732]. [cite_start]The fields are read-write[cite: 733]. [cite_start]Writing a 1 to a bit will set it to 1, that is, trigger the IRQ (mostly for debug)[cite: 733]. [cite_start]Writing a 0 will have no effect, that is, the register value will not be modified[cite: 734]. [cite_start]Only enabled, active events will trigger an actual interrupt request on the IRQ output line[cite: 735].


### Table 21-12. I2C_IRQSTATUS_RAW Register Field Descriptions

| Bit | Field | Type | Reset | Description |
| :--- | :--- | :--- | :--- | :--- |
| **31-15** | RESERVED | R | 0h | [cite_start]Reserved[cite: 740]. |
| **14** | XDR | R/W | 0h | [cite_start]Transmit draining IRQ status[cite: 740]. [cite_start]I2C Master Transmit mode only[cite: 740]. [cite_start]Set to 1 when the TX FIFO level is below the configured threshold (TXTRSH) and the amount of data still to be transferred is less than TXTRSH[cite: 740]. [cite_start]0h = Transmit draining inactive, 1h = Transmit draining enabled[cite: 740]. |
| **13** | RDR | R/W | 0h | [cite_start]Receive draining IRQ status[cite: 749]. [cite_start]I2C Receive mode only[cite: 749]. [cite_start]Set to 1 when a stop condition was received on the bus and the RX FIFO level is below the configured threshold (RXTRSH)[cite: 749]. [cite_start]0h = Receive draining inactive, 1h = Receive draining enabled[cite: 749]. |
| **12** | BB | R | 0h | [cite_start]Bus busy status[cite: 749]. [cite_start]This read-only bit indicates the state of the serial bus[cite: 749]. [cite_start]0h = Bus is free, 1h = Bus is occupied[cite: 749]. |
| **11** | ROVR | R/W | 0h | [cite_start]Receive overrun status[cite: 749]. [cite_start]I2C receive mode only[cite: 749]. [cite_start]Indicates whether the receiver has experienced overrun[cite: 749]. [cite_start]0h = Normal operation, 1h = Receiver overrun[cite: 749]. |
| **10** | XUDF | R/W | 0h | [cite_start]Transmit underflow status[cite: 757]. [cite_start]I2C transmit mode only[cite: 757]. [cite_start]Indicates whether the transmitter has experienced underflow[cite: 757]. [cite_start]0h = Normal operation, 1h = Transmit underflow[cite: 757]. |
| **9** | AAS | R/W | 0h | [cite_start]Address recognized as slave IRQ status[cite: 757]. [cite_start]I2C mode only[cite: 757]. [cite_start]Set to 1 by the device when it has recognized its own slave address or an address of all zeros (8 bits)[cite: 757]. [cite_start]0h = No action, 1h = Address recognized[cite: 757]. |
| **8** | BF | R/W | 0h | [cite_start]Bus free IRQ status[cite: 757]. [cite_start]I2C mode only[cite: 757]. [cite_start]Set to 1 by the device when the I2C bus became free[cite: 757]. [cite_start]0h = No action, 1h = Bus Free[cite: 757]. |
| **7** | AERR | R/W | 0h | [cite_start]Access Error IRQ status[cite: 757]. [cite_start]I2C mode only[cite: 757]. [cite_start]Set to 1 by the device if an Interface/OCP write access is performed to `I2C_DATA` while the TX FIFO is full or if a read access is performed while the RX FIFO is empty[cite: 757]. [cite_start]0h = No action, 1h = Access Error[cite: 757]. |
| **6** | STC | R/W | 0h | [cite_start]Start Condition IRQ status[cite: 769]. [cite_start]I2C mode only[cite: 769]. [cite_start]Set to 1 by the device if previously the module was in idle mode and a start condition was asynchronously detected on the I2C Bus[cite: 769]. [cite_start]0h = No action, 1h = Start Condition detected[cite: 769]. |
| **5** | GC | R/W | 0h | [cite_start]General call IRQ status[cite: 769]. [cite_start]I2C mode only[cite: 769]. [cite_start]Set to 1 by the device if it detects the address of all zeros (general call)[cite: 769]. [cite_start]0h = No general call detected, 1h = General call address detected[cite: 769]. |
| **4** | XRDY | R/W | 0h | [cite_start]Transmit data ready IRQ status[cite: 780]. [cite_start]Transmit mode only (I2C mode)[cite: 780]. [cite_start]Set to 1 when the I2C peripheral is a transmitter, the CPU needs to send data, and the module requires new data to be served[cite: 780]. [cite_start]0h = Transmission ongoing, 1h = Transmit data ready[cite: 780]. |
| **3** | RRDY | R/W | 0h | [cite_start]Receive data ready IRQ status[cite: 780]. [cite_start]Receive mode only (I2C mode)[cite: 780]. [cite_start]Set to 1 when the RX FIFO level is above the configured threshold (RXTRSH)[cite: 780]. [cite_start]0h = Receive FIFO threshold not reached, 1h = Receive data ready for read (RX FIFO threshold reached)[cite: 780]. |
| **2** | ARDY | R/W | 0h | [cite_start]Register access ready IRQ status[cite: 791]. [cite_start]I2C mode only[cite: 791]. [cite_start]Set to 1 to indicate that the previously programmed data and command has been performed and status bit has been updated[cite: 791]. [cite_start]0h = No action, 1h = Access ready[cite: 791]. |
| **1** | NACK | R/W | 0h | [cite_start]No acknowledgment IRQ status[cite: 791]. [cite_start]I2C mode only[cite: 791]. [cite_start]Set when the hardware detects No Acknowledge has been received[cite: 791]. [cite_start]0h = Normal operation, 1h = Not Acknowledge detected[cite: 791]. |
| **0** | AL | R/W | 0h | [cite_start]Arbitration lost IRQ status[cite: 791]. [cite_start]I2C mode only[cite: 791]. [cite_start]Set to 1 when the device (configured in master mode) detects it has lost an arbitration in Address Phase[cite: 791]. [cite_start]0h = Normal operation, 1h = Arbitration lost detected[cite: 791]. |

*(LEGEND: R/W = Read/Write; R = Read only; W1toCl = Write 1 to clear bit; -n = value after reset)*

## 21.4.1.5 I2C_IRQSTATUS Register (offset = 28h) [reset = 0h]

[cite_start]This register provides core status information for interrupt handling, showing all active and enabled events and masking the others[cite: 803]. [cite_start]The fields are read-write[cite: 804]. [cite_start]Writing a 1 to a bit will clear it to 0, that is, clear the IRQ[cite: 804]. [cite_start]Writing a 0 will have no effect, that is, the register value will not be modified[cite: 805]. [cite_start]Only enabled, active events will trigger an actual interrupt request on the IRQ output line[cite: 806]. [cite_start]For all the internal fields of the `I2C_IRQSTATUS` register, the descriptions given in the `I2C_IRQSTATUS_RAW` subsection are valid[cite: 807].


### Table 21-13. I2C_IRQSTATUS Register Field Descriptions
| Bit | Field | Type | Reset | Description |
| :--- | :--- | :--- | :--- | :--- |
| **31-15** | RESERVED | R | 0h | [cite_start]Reserved. [cite: 812] |
| **14** | XDR | R/W | 0h | Transmit draining IRQ enabled status. [cite_start]0h = Transmit draining inactive, 1h = Transmit draining enabled. [cite: 812] |
| **13** | RDR | R/W | 0h | Receive draining IRQ enabled status. [cite_start]0h = Receive draining inactive, 1h = Receive draining enabled. [cite: 812] |
| **12** | BB | R/W | 0h | Bus busy enabled status. Writing into this bit has no effect. [cite_start]0h = Bus is free, 1h = Bus is occupied. [cite: 812] |
| **11** | ROVR | R/W | 0h | Receive overrun enabled status. Writing into this bit has no effect. [cite_start]0h = Normal operation, 1h = Receiver overrun. [cite: 812] |
| **10** | XUDF | R/W | 0h | Transmit underflow enabled status. Writing into this bit has no effect. [cite_start]0h = Normal operation, 1h = Transmit underflow. [cite: 812] |
| **9** | AAS | R/W | 0h | Address recognized as slave IRQ enabled status. [cite_start]0h = No action, 1h = Address recognized. [cite: 812] |
| **8** | BF | R/W | 0h | Bus Free IRQ enabled status. [cite_start]0h = No action, 1h = Bus free. [cite: 812] |
| **7** | AERR | R/W | 0h | Access Error IRQ enabled status. [cite_start]0h = No action, 1h = Access error. [cite: 812] |
| **6** | STC | R/W | 0h | Start Condition IRQ enabled status. [cite_start]0h = No action, 1h = Condition detected. [cite: 821] |
| **5** | GC | R/W | 0h | General call IRQ enabled status. Set to '1' by core when General call address detected and interrupt signaled to MPUSS. Write '1' to clear. [cite_start]0h = No general call detected, 1h = General call address detected. [cite: 821] |
| **4** | XRDY | R/W | 0h | Transmit data ready IRQ enabled status. Set to '1' by core when transmitter and when new data is requested. When set to '1' by core, an interrupt is signaled to MPUSS. Write '1' to clear. [cite_start]0h = Transmission ongoing, 1h = Transmit data ready. [cite: 821] |
| **3** | RRDY | R/W | 0h | Receive data ready IRQ enabled status. Set to '1' by core when in receiver mode, a new data is able to be read. When set to '1' by core, an interrupt is signaled to MPUSS. Write '1' to clear. [cite_start]0h = No data available, 1h = Receive data available. [cite: 821] |
| **2** | ARDY | R/W | 0h | Register access ready IRQ enabled status. When set to '1' it indicates that previous access has been performed and registers are ready to be accessed again. An interrupt is signaled to MPUSS. Write '1' to clear. [cite_start]0h = Module busy, 1h = Access ready. [cite: 821] |
| **1** | NACK | R/W | 0h | No acknowledgment IRQ enabled status. Bit is set when No Acknowledge has been received, an interrupt is signaled to MPUSS. Write '1' to clear this bit. [cite_start]0h = Normal operation, 1h = Not Acknowledge detected. [cite: 821] |
| **0** | AL | R/W | 0h | Arbitration lost IRQ enabled status. This bit is automatically set by the hardware when it loses the Arbitration in master transmit mode, an interrupt is signaled to MPUSS. [cite_start]During reads, it always returns 0. 0h = Normal operation, 1h = Arbitration lost detected. [cite: 821] |
[cite_start]*(LEGEND: R/W = Read/Write; R = Read only; W1toCl = Write 1 to clear bit; -n = value after reset)* [cite: 810]

---

## 21.4.1.6 I2C_IRQENABLE_SET Register (offset = 2Ch) [reset = 0h]

[cite_start]All 1-bit fields enable a specific interrupt event to trigger an interrupt request[cite: 832]. [cite_start]Writing a 1 to a bit will enable the field[cite: 833]. [cite_start]Writing a 0 will have no effect, that is, the register value will not be modified[cite: 834]. [cite_start]For all the internal fields of the `I2C_IRQENABLE_SET` register, the descriptions given in the `I2C_IRQSTATUS_RAW` subsection are valid[cite: 835].


### Table 21-14. I2C_IRQENABLE_SET Register Field Descriptions
| Bit | Field | Type | Reset | Description |
| :--- | :--- | :--- | :--- | :--- |
| **31-15** | RESERVED | R | 0h | [cite_start]Reserved. [cite: 840] |
| **14** | XDR_IE | R/W | 0h | Transmit draining interrupt enable set. Mask or unmask the interrupt signaled by bit in `I2C_STAT[XDR]`. [cite_start]0h = Transmit draining interrupt disabled, 1h = Transmit draining interrupt enabled. [cite: 840] |
| **13** | RDR_IE | R/W | 0h | Receive draining interrupt enable set. Mask or unmask the interrupt signaled by bit in `I2C_STAT[RDR]`. [cite_start]0h = Receive draining interrupt disabled, 1h = Receive draining interrupt enabled. [cite: 840] |
| **12** | RESERVED | R | 0h | [cite_start]Reserved. [cite: 840] |
| **11** | ROVR | R/W | 0h | Receive overrun enable set. [cite_start]0h = Receive overrun interrupt disabled, 1h = Receive draining interrupt enabled. [cite: 840] |
| **10** | XUDF | R/W | 0h | Transmit underflow enable set. [cite_start]0h = Transmit underflow interrupt disabled, 1h = Transmit underflow interrupt enabled. [cite: 840] |
| **9** | AAS IE | R/W | 0h | Addressed as slave interrupt enable set. Mask or unmask the interrupt signaled by bit in `I2C_STAT[AAS]`. [cite_start]0h = Addressed as slave interrupt disabled, 1h = Addressed as slave interrupt enabled. [cite: 840] |
| **8** | BF_IE | R/W | 0h | Bus free interrupt enable set. Mask or unmask the interrupt signaled by bit in `I2C_STAT[BF]`. [cite_start]0h = Bus free interrupt disabled, 1h = Bus free interrupt enabled. [cite: 840] |
| **7** | AERR IE | R/W | 0h | Access error interrupt enable set. Mask or unmask the interrupt signaled by bit in `I2C_STAT[AERR]`. [cite_start]0h = Access error interrupt disabled, 1h = Access error interrupt enabled. [cite: 840] |
| **6** | STC_IE | R/W | 0h | Start condition interrupt enable set. Mask or unmask the interrupt signaled by bit in `I2C_STAT[STC]`. [cite_start]0h = Start condition interrupt disabled, 1h = Start condition interrupt enabled. [cite: 851] |
| **5** | GC_IE | R/W | 0h | General call interrupt enable set. Mask or unmask the interrupt signaled by bit in `I2C_STAT[GC]`. [cite_start]0h = General call interrupt disabled, 1h = General call interrupt enabled. [cite: 851] |
| **4** | XRDY_IE | R/W | 0h | Transmit data ready interrupt enable set. Mask or unmask the interrupt signaled by bit in `I2C_STAT[XRDY]`. [cite_start]0h = Transmit data ready interrupt disabled, 1h = Transmit data ready interrupt enabled. [cite: 851] |
| **3** | RRDY_IE | R/W | 0h | Receive data ready interrupt enable set. Mask or unmask the interrupt signaled by bit in `I2C_STAT[RRDY]`. [cite_start]0h = Receive data ready interrupt disabled, 1h = Receive data ready interrupt enabled. [cite: 851] |
| **2** | ARDY_IE | R/W | 0h | Register access ready interrupt enable set. Mask or unmask the interrupt signaled by bit in `I2C_[ARDY]`. [cite_start]0h = Register access ready interrupt disabled, 1h = Register access ready interrupt enabled. [cite: 851] |
| **1** | NACK_IE | R/W | 0h | No acknowledgment interrupt enable set. Mask or unmask the interrupt signaled by bit in `I2C_STAT[NACK]`. [cite_start]0h = Not Acknowledge interrupt disabled, 1h = Not Acknowledge interrupt enabled. [cite: 851] |
| **0** | ALIE | R/W | 0h | Arbitration lost interrupt enable set. Mask or unmask the interrupt signaled by bit in `I2C_STAT[AL]`. [cite_start]0h = Arbitration lost interrupt disabled, 1h = Arbitration lost interrupt enabled. [cite: 851] |
[cite_start]*(LEGEND: R/W = Read/Write; R = Read only; W1toCl = Write 1 to clear bit; -n = value after reset)* [cite: 838]

## [cite_start]21.4.1.7 I2C_IRQENABLE_CLR Register (offset = 30h) [reset = 0h] [cite: 858]

[cite_start]All 1-bit fields clear a specific interrupt event[cite: 861]. [cite_start]Writing a 1 to a bit will disable the interrupt field[cite: 861]. [cite_start]Writing a 0 will have no effect, that is, the register value will not be modified[cite: 862]. [cite_start]For all the internal fields of the `I2C_IRQENABLE_CLR` register, the descriptions given in the `I2C_IRQSTATUS_RAW` subsection are valid[cite: 863].


### Table 21-15. I2C_IRQENABLE_CLR Register Field Descriptions
| Bit | Field | Type | Reset | Description |
| :--- | :--- | :--- | :--- | :--- |
| **31-15** | RESERVED | R | 0h | [cite_start]Reserved. [cite: 868] |
| **14** | XDR_IE | R/W | 0h | Transmit draining interrupt enable clear. Mask or unmask the interrupt signaled by bit in `I2C_STAT[XDR]`. [cite_start]0h = Transmit draining interrupt disabled, 1h = Transmit draining interrupt enabled. [cite: 868] |
| **13** | RDR_IE | R/W | 0h | Receive draining interrupt enable clear. Mask or unmask the interrupt signaled by bit in `I2C_STAT[RDR]`. [cite_start]0h = Receive draining interrupt disabled, 1h = Receive draining interrupt enabled. [cite: 868] |
| **12** | RESERVED | R | 0h | [cite_start]Reserved. [cite: 868] |
| **11** | ROVR | R/W | 0h | Receive overrun enable clear. [cite_start]0h = Receive overrun interrupt disabled, 1h = Receive draining interrupt enabled. [cite: 868] |
| **10** | XUDF | R/W | 0h | Transmit underflow enable clear. [cite_start]0h = Transmit underflow interrupt disabled, 1h = Transmit underflow interrupt enabled. [cite: 868] |
| **9** | AAS_IE | R/W | 0h | Addressed as slave interrupt enable clear. Mask or unmask the interrupt signaled by bit in `I2C_STAT[AAS]`. [cite_start]0h = Addressed as slave interrupt disabled, 1h = Addressed as slave interrupt enabled. [cite: 868] |
| **8** | BF_IE | R/W | 0h | Bus Free interrupt enable clear. Mask or unmask the interrupt signaled by bit in `I2C_STAT[BF]`. [cite_start]0h = Bus free interrupt disabled, 1h = Bus free interrupt enabled. [cite: 868] |
| **7** | AERR_IE | R/W | 0h | Access error interrupt enable clear. Mask or unmask the interrupt signaled by bit in `I2C_STAT[AERR]`. [cite_start]0h = Access error interrupt disabled, 1h = Access error interrupt enabled. [cite: 868] |
| **6** | STC_IE | R/W | 0h | Start condition interrupt enable clear. Mask or unmask the interrupt signaled by bit in `I2C_STAT[STC]`. [cite_start]0h = Start condition interrupt disabled, 1h = Start condition interrupt enabled. [cite: 879] |
| **5** | GC_IE | R/W | 0h | General call interrupt enable clear. Mask or unmask the interrupt signaled by bit in `I2C_STAT[GC]`. [cite_start]0h = General call interrupt disabled, 1h = General call interrupt enabled. [cite: 879] |
| **4** | XRDY_IE | R/W | 0h | Transmit data ready interrupt enable clear. Mask or unmask the interrupt signaled by bit in `I2C_STAT[XRDY]`. [cite_start]0h = Transmit data ready interrupt disabled, 1h = Transmit data ready interrupt enabled. [cite: 879] |
| **3** | RRDY_IE | R/W | 0h | Receive data ready interrupt enable clear. Mask or unmask the interrupt signaled by bit in `I2C_STAT[RRDY]`. [cite_start]0h = Receive data ready interrupt disabled, 1h = Receive data ready interrupt enabled. [cite: 879] |
| **2** | ARDY_IE | R/W | 0h | Register access ready interrupt enable clear. Mask or unmask the interrupt signaled by bit in `I2C_[ARDY]`. [cite_start]0h = Register access ready interrupt disabled, 1h = Register access ready interrupt enabled. [cite: 879] |
| **1** | NACK_IE | R/W | 0h | No acknowledgment interrupt enable clear. Mask or unmask the interrupt signaled by bit in `I2C_STAT[NACK]`. [cite_start]0h = Not Acknowledge interrupt disabled, 1h = Not Acknowledge interrupt enabled. [cite: 879] |
| **0** | AL_IE | R/W | 0h | Arbitration lost interrupt enable clear. Mask or unmask the interrupt signaled by bit in `I2C_STAT[AL]`. [cite_start]0h = Arbitration lost interrupt disabled, 1h = Arbitration lost interrupt enabled. [cite: 879] |

[cite_start]*(LEGEND: R/W = Read/Write; R = Read only; W1toCl = Write 1 to clear bit; -n = value after reset)* [cite: 866]

---

## [cite_start]21.4.1.8 I2C_WE Register (offset = 34h) [reset = 0h] [cite: 888]

[cite_start]Every 1-bit field in the `I2C_WE` register enables a specific (synchronous) IRQ request source to generate an asynchronous wakeup (on the appropriate swakeup line)[cite: 891]. [cite_start]When a bit location is set to 1 by the local host, a wakeup is signaled to the local host if the corresponding event is captured by the core of the I2C controller[cite: 892]. [cite_start]Value after reset is low (all bits)[cite: 893]. [cite_start]There is no need for an Access Error WakeUp event, since this event occurs only when the module is in Active Mode (for Interface/OCP accesses to FIFO) and is signaled by an interrupt[cite: 893]. [cite_start]With the exception of Start Condition WakeUp, which is asynchronously detected when the Functional clock is turned-off, all the other WakeUp events require the Functional (System) clock to be enabled[cite: 894].


### Table 21-16. I2C_WE Register Field Descriptions
| Bit | Field | Type | Reset | Description |
| :--- | :--- | :--- | :--- | :--- |
| **31-15** | RESERVED | R | 0h | [cite_start]Reserved. [cite: 899] |
| **14** | XDR_WE | R/W | 0h | Transmit draining wakeup enable. Enables or disables wakeup signal generation when I2C module is in idle mode, the TX FIFO level is below the threshold and the amount of data left to be transferred is less than `TXTRSH` value. [cite_start]0h = Transmit draining wakeup disabled, 1h = Transmit draining wakeup enabled. [cite: 899] |
| **13** | RDR_WE | R/W | 0h | Receive draining wakeup enable. Enables or disables wakeup signal generation when I2C is in idle mode, configured as a receiver, and it has detected a stop condition on the bus but the RX FIFO threshold is not reached. [cite_start]0h = Receive draining wakeup disabled, 1h = Receive draining wakeup enabled. [cite: 899] |
| **12** | RESERVED | R | 0h | [cite_start]Reserved. [cite: 899] |
| **11** | ROVR_WE | R/W | 0h | Receive overrun wakeup enable. [cite_start]0h = Receive overrun wakeup disabled, 1h = Receive overrun wakeup enabled. [cite: 899] |
| **10** | XUDF_WE | R/W | 0h | Transmit underflow wakeup enable. [cite_start]0h = Transmit underflow wakeup disabled, 1h = Transmit underflow wakeup enabled. [cite: 899] |
| **9** | AAS_WE | R/W | 0h | Address as slave IRQ wakeup enable. Enables or disables wakeup signal generation when I2C module is in idle mode, and external master addresses the I2C module as a slave. [cite_start]0h = Addressed as slave wakeup disabled, 1h = Addressed as slave wakeup enabled. [cite: 908] |
| **8** | BF_WE | R/W | 0h | Bus free IRQ wakeup enable. Enables or disables wakeup signal generation when I2C module is in idle mode and the I2C bus became free. [cite_start]0h = Bus Free wakeup disabled, 1h = Bus Free wakeup enabled. [cite: 908] |
| **7** | RESERVED | R | 0h | [cite_start]Reserved. [cite: 908] |
| **6** | STC_WE | R/W | 0h | Start condition IRQ wakeup set. Enables or disables wakeup signal generation when I2C module is in idle mode (with the functional clock inactive) and a possible start condition is detected on the I2C line. [cite_start]0h = Start condition wakeup disabled, 1h = Start condition wakeup enabled. [cite: 908] |
| **5** | GC_WE | R/W | 0h | General call IRQ wakeup enable. Enables or disables wakeup signal generation when I2C module is in idle mode and a general call is received on I2C line. [cite_start]0h = General call wakeup disabled, 1h = General call wakeup enabled. [cite: 908] |
| **4** | RESERVED | R | 0h | [cite_start]Reserved. [cite: 908] |
| **3** | DRDY_WE | R/W | 0h | Receive/Transmit data ready IRQ wakeup enable. Enables or disables wakeup signal generation when I2C module is involved into a long transfer and no more registers accesses are performed on the interface. [cite_start]0h = Transmit/receive data ready wakeup disabled, 1h = Transmit/receive data ready wakeup enabled. [cite: 908] |
| **2** | ARDY_WE | R/W | 0h | Register access ready IRQ wakeup enable. Enables or disables wakeup signal generation when I2C module is involved into a long transfer and no more registers accesses are performed on the interface. [cite_start]0h = Register access ready wakeup disabled, 1h = Register access ready wakeup enabled. [cite: 908] |
| **1** | NACK_WE | R/W | 0h | No acknowledgment IRQ wakeup enable. Enables or disables wakeup signal generation when I2C module is involved into a long transfer and no more registers accesses are performed on the interface. [cite_start]0h = Not Acknowledge wakeup disabled, 1h = Not Acknowledge wakeup enabled. [cite: 917] |
| **0** | AL_WE | R/W | 0h | Arbitration lost IRQ wakeup enable. Enables or disables wakeup signal generation when I2C module is configured as a master and it loses the arbitration. [cite_start]0h = Arbitration lost wakeup disabled, 1h = Arbitration lost wakeup enabled. [cite: 917] |

[cite_start]*(LEGEND: R/W = Read/Write; R = Read only; W1toCl = Write 1 to clear bit; -n = value after reset)* [cite: 897]

## 21.4.1.9 I2C_DMARXENABLE_SET Register (offset = 38h) [reset = 0h]

[cite_start]`I2C_DMARXENABLE_SET` is shown in Figure 21-24 and described in Table 21-17. [cite: 927] [cite_start]The 1-bit field enables a receive DMA request. [cite: 929] [cite_start]Writing a 1 to this field will set it to 1. [cite: 929] [cite_start]Writing a 0 will have no effect, that is, the register value is not modified. [cite: 929] [cite_start]Note that the `I2C_BUF.RDMA_EN` field is the global (slave) DMA enabler, and that it is disabled by default. [cite: 930] [cite_start]The `I2C_BUF.RDMA_EN` field should also be set to 1 to enable a receive DMA request. [cite: 931]


### Table 21-17. I2C_DMARXENABLE_SET Register Field Descriptions
| Bit | Field | Type | Reset | Description |
| :--- | :--- | :--- | :--- | :--- |
| **31-1** | RESERVED | R | 0h | [cite_start]Reserved. [cite: 936] |
| **0** | DMARX_ENABLE_SET | R/W | 0h | [cite_start]Receive DMA channel enable set. [cite: 936] |

[cite_start]*(LEGEND: R/W = Read/Write; R = Read only; W1toCl = Write 1 to clear bit; -n = value after reset)* [cite: 934]

---

## 21.4.1.10 I2C_DMATXENABLE_SET Register (offset = 3Ch) [reset = 0h]

[cite_start]`I2C_DMATXENABLE_SET` is shown in Figure 21-25 and described in Table 21-18. [cite: 945] [cite_start]The 1-bit field enables a transmit DMA request. [cite: 947] [cite_start]Writing a 1 to this field will set it to 1. [cite: 947] [cite_start]Writing a 0 will have no effect, that is, the register value is not modified. [cite: 947] [cite_start]Note that the `I2C_BUF.XDMA_EN` field is the global (slave) DMA enabler, and that it is disabled by default. [cite: 948] [cite_start]The `I2C_BUF.XDMA_EN` field should also be set to 1 to enable a transmit DMA request. [cite: 949]

### Table 21-18. I2C_DMATXENABLE_SET Register Field Descriptions
| Bit | Field | Type | Reset | Description |
| :--- | :--- | :--- | :--- | :--- |
| **31-1** | RESERVED | R | 0h | [cite_start]Reserved. [cite: 954] |
| **0** | DMATX_TRANSMIT_SET | R/W | 0h | [cite_start]Transmit DMA channel enable set. [cite: 954] |

[cite_start]*(LEGEND: R/W = Read/Write; R = Read only; W1toCl = Write 1 to clear bit; -n = value after reset)* [cite: 952]

---

## 21.4.1.11 I2C_DMARXENABLE_CLR Register (offset = 40h) [reset = 0h]

[cite_start]`I2C_DMARXENABLE_CLR` is shown in Figure 21-26 and described in Table 21-19. [cite: 963] [cite_start]The 1-bit field disables a receive DMA request. [cite: 965] [cite_start]Writing a 1 to a bit will clear it to 0. [cite: 965] [cite_start]Another result of setting to 1 the `DMARX_ENABLE_CLEAR` field, is the reset of the DMA RX request and wakeup lines. [cite: 965] [cite_start]Writing a 0 will have no effect, that is, the register value is not modified. [cite: 966]

### Table 21-19. I2C_DMARXENABLE_CLR Register Field Descriptions
| Bit | Field | Type | Reset | Description |
| :--- | :--- | :--- | :--- | :--- |
| **31-1** | RESERVED | R | 0h | [cite_start]Reserved. [cite: 971] |
| **0** | DMARX_ENABLE_CLEAR | R/W | 0h | [cite_start]Receive DMA channel enable clear. [cite: 971] |

[cite_start]*(LEGEND: R/W = Read/Write; R = Read only; W1toCl = Write 1 to clear bit; -n = value after reset)* [cite: 969]

---

## 21.4.1.12 I2C_DMATXENABLE_CLR Register (offset = 44h) [reset = 0h]

[cite_start]`I2C_DMATXENABLE_CLR` is shown in Figure 21-27 and described in Table 21-20. [cite: 980] [cite_start]The 1-bit field disables a transmit DMA request. [cite: 982] [cite_start]Writing a 1 to a bit will clear it to 0. [cite: 982] [cite_start]Another result of setting to 1 the `DMATX_ENABLE_CLEAR` field, is the reset of the DMA TX request and wakeup lines. [cite: 982] [cite_start]Writing a 0 will have no effect, that is, the register value is not modified. [cite: 983]

### Table 21-20. I2C_DMATXENABLE_CLR Register Field Descriptions
| Bit | Field | Type | Reset | Description |
| :--- | :--- | :--- | :--- | :--- |
| **31-1** | RESERVED | R | 0h | [cite_start]Reserved. [cite: 988] |
| **0** | DMARX_ENABLE_CLEAR | R/W | 0h | Receive DMA channel enable clear. [cite_start]*(Note: The original documentation labels this field as DMARX_ENABLE_CLEAR, but contextually it controls the TX DMA channel clear).* [cite: 988] |

[cite_start]*(LEGEND: R/W = Read/Write; R = Read only; W1toCl = Write 1 to clear bit; -n = value after reset)* [cite: 986]

---

## 21.4.1.13 I2C_DMARXWAKE_EN Register (offset = 48h) [reset = 0h]

[cite_start]`I2C_DMARXWAKE_EN` is shown in Figure 21-28 and described in Table 21-21. [cite: 998] [cite_start]All 1-bit fields enable a specific (synchronous) DMA request source to generate an asynchronous wakeup (on the appropriate swakeup line). [cite: 1000] [cite_start]Note that the `I2C_SYSC.ENAWAKEUP` field is the global (slave) wakeup enabler, and that it is disabled by default. [cite: 1001]

### Table 21-21. I2C_DMARXWAKE_EN Register Field Descriptions
| Bit | Field | Type | Reset | Description |
| :--- | :--- | :--- | :--- | :--- |
| **31-15** | RESERVED | R | 0h | [cite_start]Reserved. [cite: 1006] |
| **14** | XDR | R/W | 0h | Transmit draining wakeup set. [cite_start]0h = Transmit draining interrupt disabled, 1h = Transmit draining interrupt enabled. [cite: 1006] |
| **13** | RDR | R/W | 0h | Receive draining wakeup set. [cite_start]0h = Receive draining interrupt disabled, 1h = Receive draining interrupt enabled. [cite: 1006] |
| **12** | RESERVED | R | 0h | [cite_start]Reserved. [cite: 1006] |
| **11** | ROVR | R/W | 0h | Receive overrun wakeup set. [cite_start]0h = Receive overrun interrupt disabled, 1h = Receive draining interrupt enabled. [cite: 1006] |
| **10** | XUDF | R/W | 0h | Transmit underflow wakeup set. [cite_start]0h = Transmit underflow interrupt disabled, 1h = Transmit underflow interrupt enabled. [cite: 1006] |
| **9** | AAS | R/W | 0h | Address as slave IRQ wakeup set. [cite_start]0h = Addressed as slave interrupt disabled, 1h = Addressed as slave interrupt enabled. [cite: 1006] |
| **8** | BF | R/W | 0h | Bus free IRQ wakeup set. [cite_start]0h = Bus free wakeup disabled, 1h = Bus free wakeup enabled. [cite: 1006] |
| **7** | RESERVED | R | 0h | [cite_start]Reserved. [cite: 1006] |
| **6** | STC | R/W | 0h | Start condition IRQ wakeup set. [cite_start]0h = Start condition wakeup disabled, 1h = Start condition wakeup enabled. [cite: 1006] |
| **5** | GC | R/W | 0h | General call IRQ wakeup set. [cite_start]0h = General call wakeup disabled, 1h = General call wakeup enabled. [cite: 1006] |
| **4** | RESERVED | R | 0h | [cite_start]Reserved. [cite: 1006] |
| **3** | DRDY | R/W | 0h | Receive/transmit data ready IRQ wakeup set. [cite_start]0h = Transmit/receive data ready wakeup disabled, 1h = Transmit/receive data ready wakeup enabled. [cite: 1013] |
| **2** | ARDY | R/W | 0h | Register access ready IRQ wakeup set. [cite_start]0h = Register access ready wakeup disabled, 1h = Register access ready wakeup enabled. [cite: 1013] |
| **1** | NACK | R/W | 0h | No acknowledgment IRQ wakeup set. [cite_start]0h = Not Acknowledge wakeup disabled, 1h = Not Acknowledge wakeup enabled. [cite: 1013] |
| **0** | AL | R/W | 0h | Arbitration lost IRQ wakeup set. [cite_start]0h = Arbitration lost wakeup disabled, 1h = Arbitration lost wakeup enabled. [cite: 1013] |

[cite_start]*(LEGEND: R/W = Read/Write; R = Read only; W1toCl = Write 1 to clear bit; -n = value after reset)* [cite: 1004]

## 21.4.1.14 I2C_DMATXWAKE_EN Register (offset = 4Ch) [reset = 0h]

[cite_start]`I2C_DMATXWAKE_EN` is shown in Figure 21-29 and described in Table 21-22[cite: 1027]. [cite_start]All 1-bit fields enable a specific (synchronous) DMA request source to generate an asynchronous wakeup (on the appropriate swakeup line)[cite: 1029]. [cite_start]Note that the `I2C_SYSC.ENAWAKEUP` field is the global (slave) wakeup enabler, and that it is disabled by default[cite: 1030].


### Table 21-22. I2C_DMATXWAKE_EN Register Field Descriptions
| Bit | Field | Type | Reset | Description |
| :--- | :--- | :--- | :--- | :--- |
| **31-15** | RESERVED | R | 0h | [cite_start]Reserved[cite: 1035]. |
| **14** | XDR | R/W | 0h | [cite_start]Transmit draining wakeup set[cite: 1035]. [cite_start]0h = Transmit draining interrupt disabled, 1h = Transmit draining interrupt enabled[cite: 1035]. |
| **13** | RDR | R/W | 0h | [cite_start]Receive draining wakeup set[cite: 1035]. [cite_start]0h = Receive draining interrupt disabled, 1h = Receive draining interrupt enabled[cite: 1035]. |
| **12** | RESERVED | R | 0h | [cite_start]Reserved[cite: 1035]. |
| **11** | ROVR | R/W | 0h | [cite_start]Receive overrun wakeup set[cite: 1035]. [cite_start]0h = Receive overrun interrupt disabled, 1h = Receive draining interrupt enabled[cite: 1035]. |
| **10** | XUDF | R/W | 0h | [cite_start]Transmit underflow wakeup set[cite: 1035]. [cite_start]0h = Transmit underflow interrupt disabled, 1h = Transmit underflow interrupt enabled[cite: 1035]. |
| **9** | AAS | R/W | 0h | [cite_start]Address as slave IRQ wakeup set[cite: 1035]. [cite_start]0h = Addressed as slave interrupt disabled, 1h = Addressed as slave interrupt enabled[cite: 1035]. |
| **8** | BF | R/W | 0h | [cite_start]Bus free IRQ wakeup set[cite: 1035]. [cite_start]0h = Bus free wakeup disabled, 1h = Bus free wakeup enabled[cite: 1035]. |
| **7** | RESERVED | R | 0h | [cite_start]Reserved[cite: 1035]. |
| **6** | STC | R/W | 0h | [cite_start]Start condition IRQ wakeup set[cite: 1035]. [cite_start]0h = Start condition wakeup disabled, 1h = Start condition wakeup enabled[cite: 1035]. |
| **5** | GC | R/W | 0h | [cite_start]General call IRQ wakeup set[cite: 1035]. [cite_start]0h = General call wakeup disabled, 1h = General call wakeup enabled[cite: 1035]. |
| **4** | RESERVED | R | 0h | [cite_start]Reserved[cite: 1035]. |
| **3** | DRDY | R/W | 0h | [cite_start]Receive/transmit data ready IRQ wakeup set[cite: 1042]. [cite_start]0h = Transmit/receive data ready wakeup disabled, 1h = Transmit/receive data ready wakeup enabled[cite: 1042]. |
| **2** | ARDY | R/W | 0h | [cite_start]Register access ready IRQ wakeup set[cite: 1042]. [cite_start]0h = Register access ready wakeup disabled, 1h = Register access ready wakeup enabled[cite: 1042]. |
| **1** | NACK | R/W | 0h | [cite_start]No acknowledgment IRQ wakeup set[cite: 1042]. [cite_start]0h = Not Acknowledge wakeup disabled, 1h = Not Acknowledge wakeup enabled[cite: 1042]. |
| **0** | AL | R/W | 0h | [cite_start]Arbitration lost IRQ wakeup set[cite: 1042]. [cite_start]0h = Arbitration lost wakeup disabled, 1h = Arbitration lost wakeup enabled[cite: 1042]. |

*(LEGEND: R/W = Read/Write; R = Read only; W1toCl = Write 1 to clear bit; -n = value after reset)*

---

## 21.4.1.15 I2C_SYSS Register (offset = 90h) [reset = 0h]

[cite_start]`I2C_SYSS` is shown in Figure 21-30 and described in Table 21-23[cite: 1056].

### Table 21-23. I2C_SYSS Register Field Descriptions
| Bit | Field | Type | Reset | Description |
| :--- | :--- | :--- | :--- | :--- |
| **31-1** | RESERVED | R | 0h | [cite_start]Reserved[cite: 1062]. |
| **0** | RDONE | R/W | 0h | [cite_start]Reset done bit[cite: 1062]. [cite_start]This read-only bit indicates the state of the reset in case of hardware reset, global software reset (`I2C_SYSC.SRST`) or partial software reset (`I2C_CON.I2C_EN`)[cite: 1062]. [cite_start]The module must receive all its clocks before it can grant a reset-completed status[cite: 1062]. [cite_start]Value after reset is low[cite: 1062]. [cite_start]0h = Internal module reset in ongoing, 1h = Reset completed[cite: 1062]. |

*(LEGEND: R/W = Read/Write; R = Read only; W1toCl = Write 1 to clear bit; -n = value after reset)*

---

## 21.4.1.16 I2C_BUF Register (offset = 94h) [reset = 0h]

[cite_start]`I2C_BUF` is shown in Figure 21-31 and described in Table 21-24[cite: 1072]. [cite_start]This read/write register enables DMA transfers and allows the configuration of FIFO thresholds for the FIFO management (see the FIFO Management subsection)[cite: 1074].


### Table 21-24. I2C_BUF Register Field Descriptions
| Bit | Field | Type | Reset | Description |
| :--- | :--- | :--- | :--- | :--- |
| **31-16** | RESERVED | R | 0h | [cite_start]Reserved[cite: 1079]. |
| **15** | RDMA_EN | R/W | 0h | [cite_start]Receive DMA channel enable[cite: 1079]. [cite_start]When this bit is set to 1, the receive DMA channel is enabled and the receive data ready status bit (`I2C_IRQSTATUS_RAW: RRDY`) is forced to 0 by the core[cite: 1079]. [cite_start]Value after reset is low[cite: 1079]. [cite_start]0h = Receive DMA channel disabled, 1h = Receive DMA channel enabled[cite: 1079]. |
| **14** | RXFIFO_CLR | R/W | 0h | [cite_start]Receive FIFO clear[cite: 1079]. [cite_start]When set, receive FIFO is cleared (hardware reset for RX FIFO generated)[cite: 1079]. [cite_start]This bit is automatically reset by the hardware[cite: 1079]. [cite_start]During reads, it always returns 0[cite: 1079]. [cite_start]Value after reset is low[cite: 1079]. [cite_start]0h = Normal mode, 1h = Rx FIFO is reset[cite: 1079]. |
| **13-8** | RXTRSH | R/W | 0h | [cite_start]Threshold value for FIFO buffer in RX mode[cite: 1079]. [cite_start]The receive threshold value is used to specify the trigger level for data receive transfers[cite: 1079]. [cite_start]The value is specified from the Interface/OCP point of view[cite: 1079]. [cite_start]Value after reset is 00h[cite: 1079]. [cite_start]For the FIFO management description, see the FIFO Management subsection[cite: 1079]. [cite_start]Note 1: programmed threshold cannot exceed the actual depth of the FIFO[cite: 1079]. [cite_start]Note 2: the threshold must not be changed while a transfer is in progress (after STT was configured or after the module was addressed as a slave)[cite: 1079]. [cite_start]0h = Receive Threshold value = 1, 1h = Receive Threshold value = 2, 3Fh = Receive Threshold value = 64[cite: 1079]. |
| **7** | XDMA_EN | R/W | 0h | [cite_start]Transmit DMA channel enable[cite: 1088]. [cite_start]When this bit is set to 1, the transmit DMA channel is enabled and the transmit data ready status (`I2C_IRQSTATUS_RAW: XRDY`) bit is forced to 0 by the core[cite: 1088]. [cite_start]Value after reset is low[cite: 1088]. [cite_start]0h = Transmit DMA channel disabled, 1h = DMA channel enabled[cite: 1088]. |
| **6** | TXFIFO_CLR | R/W | 0h | [cite_start]Transmit FIFO clear[cite: 1088]. [cite_start]When set, transmit FIFO is cleared (hardware reset for TX FIFO)[cite: 1088]. [cite_start]This bit is automatically reset by the hardware[cite: 1088]. [cite_start]During reads, it always returns 0[cite: 1088]. [cite_start]Value after reset is low[cite: 1088]. [cite_start]0h = Normal mode, 1h = Tx FIFO is reset[cite: 1088]. |
| **5-0** | TXTRSH | R/W | 0h | [cite_start]Threshold value for FIFO buffer in TX mode[cite: 1088]. [cite_start]The Transmit Threshold value is used to specify the trigger level for data transfers[cite: 1088]. [cite_start]The value is specified from the OCP point of view[cite: 1088]. [cite_start]Value after reset is 00h[cite: 1088]. [cite_start]Note 1: programmed threshold cannot exceed the actual depth of the FIFO[cite: 1088]. [cite_start]Note 2: the threshold must not be changed while a transfer is in progress (after STT was configured or after the module was addressed as a slave)[cite: 1088]. [cite_start]0h = Transmit Threshold value = 1, 1h = Threshold value = 2, 3Fh = Transmit Threshold value = 64[cite: 1088]. |

*(LEGEND: R/W = Read/Write; R = Read only; W1toCl = Write 1 to clear bit; -n = value after reset)*
## 21.4.1.17 I2C_CNT Register (offset = 98h) [reset = 0h]

[cite_start]`I2C_CNT` is shown in Figure 21-32 and described in Table 21-25[cite: 2265]. [cite_start]**CAUTION:** During an active transfer phase (between STT having been set to 1 and reception of ARDY), no modification must be done in this register[cite: 2267]. [cite_start]Changing it may result in an unpredictable behavior[cite: 2268]. [cite_start]This read/write register is used to control the numbers of bytes in the I2C data payload[cite: 2268].


### Table 21-25. I2C_CNT Register Field Descriptions
| Bit | Field | Type | Reset | Description |
| :--- | :--- | :--- | :--- | :--- |
| **31-16** | RESERVED | R | 0h | Reserved. |
| **15-0** | DCOUNT | R/W | 0h | Data count. I2C Master Mode only (receive or transmit F/S). [cite_start]This 16-bit countdown counter decrements by 1 for every byte received or sent through the I2C interface[cite: 2273]. [cite_start]A write initializes DCOUNT to a saved initial value[cite: 2273]. [cite_start]A read returns the number of bytes that are yet to be received or sent[cite: 2273]. [cite_start]A read into DCOUNT returns the initial value only before a start condition and after a stop condition[cite: 2273]. [cite_start]When DCOUNT reaches 0, the core generates a stop condition if a stop condition was specified (`I2C_CON.STP = 1`) and the ARDY status flag is set to 1 in the `I2C_IRQSTATUS_RAW` register[cite: 2273]. [cite_start]Note that DCOUNT must not be reconfigured after `I2C_CON.STT` was enabled and before ARDY is received [cite: 2273][cite_start].<br><br>**Note 1:** In case of I2C mode of operation, if `I2C_CON.STP = 0`, then the I2C asserts SCL = 0 when DCOUNT reaches 0[cite: 2273]. [cite_start]The CPU can then reprogram DCOUNT to a new value and resume sending or receiving data with a new start condition (restart)[cite: 2273]. [cite_start]This process repeats until the CPU sets to 1 the `I2C_CON.STP` bit[cite: 2273]. [cite_start]The ARDY flag is set each time DCOUNT reaches 0 and DCOUNT is reloaded to its initial value[cite: 2273]. [cite_start]Values after reset are low (all 16 bits) [cite: 2273][cite_start].<br><br>**Note 2:** Since for DCOUNT = 0, the transfer length is 65536, the module does not allow the possibility to initiate zero data bytes transfers [cite: 2273][cite_start].<br><br>0h = Data counter = 65536 bytes (2^16)<br>1h = Data counter = 1 bytes<br>FFFFh = Data counter = 65535 bytes (2^16-1) [cite: 2273] |

[cite_start]*(LEGEND: R/W = Read/Write; R = Read only; W1toCl = Write 1 to clear bit; -n = value after reset)* [cite: 2271]

---

## 21.4.1.18 I2C_DATA Register (offset = 9Ch) [reset = 0h]

[cite_start]`I2C_DATA` is shown in Figure 21-33 and described in Table 21-26[cite: 2282]. [cite_start]This register is the entry point for the local host to read data from or write data to the FIFO buffer[cite: 2284].


### Table 21-26. I2C_DATA Register Field Descriptions
| Bit | Field | Type | Reset | Description |
| :--- | :--- | :--- | :--- | :--- |
| **31-8** | RESERVED | R | 0h | Reserved. |
| **7-0** | DATA | R/W | 0h | Transmit/Receive data FIFO endpoint. [cite_start]When read, this register contains the received I2C data[cite: 2289]. [cite_start]When written, this register contains the byte value to transmit over the I2C data[cite: 2289]. [cite_start]In SYSTEST loop back mode (`I2C_SYSTEST: TMODE = 11`), this register is also the entry/receive point for the data[cite: 2289]. [cite_start]Values after reset are unknown (all 8-bits) [cite: 2289][cite_start].<br><br>**Note:** A read access, when the buffer is empty, returns the previous read data value[cite: 2289]. [cite_start]A write access, when the buffer is full, is ignored[cite: 2289]. [cite_start]In both events, the FIFO pointers are not updated and an Access Error (AERR) Interrupt is generated[cite: 2289]. |

[cite_start]*(LEGEND: R/W = Read/Write; R = Read only; W1toCl = Write 1 to clear bit; -n = value after reset)* [cite: 2287]

---

## 21.4.1.19 I2C_CON Register (offset = A4h) [reset = 0h]

[cite_start]`I2C_CON` is shown in Figure 21-34 and described in Table 21-27[cite: 2300]. [cite_start]During an active transfer phase (between STT having been set to 1 and reception of ARDY), no modification must be done in this register (except STP enable)[cite: 2302]. [cite_start]Changing it may result in an unpredictable behavior[cite: 2303].


### Table 21-27. I2C_CON Register Field Descriptions
| Bit | Field | Type | Reset | Description |
| :--- | :--- | :--- | :--- | :--- |
| **31-16** | RESERVED | R | 0h | Reserved. |
| **15** | I2C_EN | R/W | 0h | I2C module enable. [cite_start]When this bit is cleared to 0, the I2C controller is not enabled and reset[cite: 2308]. [cite_start]When 0, receive and transmit FIFOs are cleared and all status bits are set to their default values[cite: 2308]. [cite_start]All configuration registers (`I2C_IRQENABLE_SET`, `I2C_IRQWAKE_SET`, `I2C_BUF`, `I2C_CNT`, `I2C_CON`, `I2C_OA`, `I2C_SA`, `I2C_PSC`, `I2C_SCLL` and `I2C_SCLH`) are not reset, they keep their initial values and can be accessed[cite: 2308]. [cite_start]The CPU must set this bit to 1 for normal operation[cite: 2308]. [cite_start]Value after reset is low[cite: 2308].<br>0h = Controller in reset. [cite_start]FIFO are cleared and status bits are set to their default value.<br>1h = Module enabled[cite: 2308]. |
| **14** | RESERVED | R | 0h | Reserved. |
| **13-12** | OPMODE | R/W | 0h | Operation mode selection. [cite_start]These two bits select module operation mode[cite: 2308]. [cite_start]Value after reset is 00.<br>0h = I2C Fast/Standard mode<br>1h = Reserved<br>2h = Reserved<br>3h = Reserved [cite: 2308] |
| **11** | STB | R/W | 0h | Start byte mode (I2C master mode only). [cite_start]The start byte mode bit is set to 1 by the CPU to configure the I2C in start byte mode (`I2C_SA = 0000 0001`)[cite: 2308]. [cite_start]Value after reset is low.<br>0h = Normal mode<br>1h = Start byte mode [cite: 2308] |
| **10** | MST | R/W | 0h | Master/slave mode (I2C mode only). [cite_start]When this bit is cleared, the I2C controller is in the slave mode and the serial clock (SCL) is received from the master device[cite: 2317]. [cite_start]When this bit is set, the I2C controller is in the master mode and generates the serial clock [cite: 2317][cite_start].<br>**Note:** This bit is automatically cleared at the end of the transfer on a detected stop condition, in case of arbitration lost or when the module is configured as a master but addressed as a slave by an external master[cite: 2317]. [cite_start]Value after reset is low.<br>0h = Slave mode<br>1h = Master mode [cite: 2317] |
| **9** | TRX | R/W | 0h | Transmitter/receiver mode (I2C master mode only). [cite_start]When this bit is cleared, the I2C controller is in the receiver mode and data on data line SDA is shifted into the receiver FIFO and can be read from `I2C_DATA` register[cite: 2317]. [cite_start]When this bit is set, the I2C controller is in the transmitter mode and the data written in the transmitter FIFO via `I2C_DATA` is shifted out on data line SDA[cite: 2317]. [cite_start]Value after reset is low [cite: 2317][cite_start].<br>The operating modes are defined as follows:<br>`MST=0`, `TRX=x`, Operating Mode = Slave receiver.<br>`MST=0`, `TRX=x`, Operating Mode = Slave transmitter.<br>`MST=1`, `TRX=0`, Operating Modes = Master receiver.<br>`MST=1`, `TRX=1`, Operating Modes = Master transmitter.<br>0h = Receiver mode<br>1h = Transmitter mode [cite: 2317] |
| **8** | XSA | R/W | 0h | Expand slave address. (I2C mode only). [cite_start]When set, this bit expands the slave address to 10-bit[cite: 2317]. [cite_start]Value after reset is low.<br>0h = 7-bit address mode<br>1h = 10-bit address mode [cite: 2317] |
| **7** | XOA0 | R/W | 0h | Expand own address 0. (I2C mode only). [cite_start]When set, this bit expands the base own address (OA0) to 10-bit[cite: 2317]. [cite_start]Value after reset is low.<br>0h = 7-bit address mode<br>1h = 10-bit address mode [cite: 2317] |
| **6** | XOA1 | R/W | 0h | Expand own address 1. (I2C mode only). [cite_start]When set, this bit expands the first alternative own address (OA1) to 10-bit[cite: 2317]. [cite_start]Value after reset is low.<br>0h = 7-bit address mode<br>1h = 10-bit address mode [cite: 2317] |
| **5** | XOA2 | R/W | 0h | Expand own address 2. (I2C mode only). [cite_start]When set, this bit expands the second alternative own address (OA2) to 10-bit[cite: 2317]. [cite_start]Value after reset is low.<br>0h = 7-bit address mode<br>1h = 10-bit address mode [cite: 2317] |
| **4** | XOA3 | R/W | 0h | Expand own address 3. (I2C mode only). [cite_start]When set, this bit expands the third alternative own address (OA3) to 10-bit[cite: 2317]. [cite_start]Value after reset is low.<br>0h = 7-bit address mode<br>1h = 10-bit address mode [cite: 2317] |
| **3-2** | RESERVED | R | 0h | Reserved. |
| **1** | STP | R/W | 0h | Stop condition (I2C master mode only). [cite_start]This bit can be set to a 1 by the CPU to generate a stop condition[cite: 2324]. [cite_start]It is reset to 0 by the hardware after the stop condition has been generated[cite: 2324]. [cite_start]The stop condition is generated when DCOUNT passes 0[cite: 2324]. [cite_start]When this bit is not set to 1 before the end of the transfer (`DCOUNT = 0`), the stop condition is not generated and the SCL line is hold to 0 by the master, which can re-start a new transfer by setting the STT bit to 1[cite: 2324]. [cite_start]Value after reset is low.<br>0h = No action or stop condition detected<br>1h = Stop condition queried [cite: 2324] |
| **0** | STT | R/W | 0h | Start condition (I2C master mode only). [cite_start]This bit can be set to a 1 by the CPU to generate a start condition[cite: 2324]. [cite_start]It is reset to 0 by the hardware after the start condition has been generated[cite: 2324]. [cite_start]The start/stop bits can be configured to generate different transfer formats[cite: 2324]. [cite_start]Value after reset is low [cite: 2324][cite_start].<br>Note: DCOUNT is data count value in `I2C_CNT` register [cite: 2324].<br>`STT = 1`, `STP = 0` Conditions = Start, Bus Activities = S-A-D [cite: 2324]<br>`STT = 0`, `STP = 1` Conditions = Stop, Bus Activities = P [cite: 2324]<br>`STT = 1`, `STP = 1` Conditions = Start-Stop (`DCOUNT = n`), Bus Activities = S-A-D..(n)..D-P [cite: 2324]<br>`STT = 1`, `STP = 0` Conditions = Start (`DCOUNT = n`), Bus Activities = S-A-D..(n)..D [cite: 2324][cite_start]<br>0h = No action or start condition detected<br>1h = Start condition queried [cite: 2324] |

[cite_start]*(LEGEND: R/W = Read/Write; R = Read only; W1toCl = Write 1 to clear bit; -n = value after reset)* [cite: 2306]
## 21.4.1.20 I2C_OA Register (offset = A8h) [reset = 0h]

[cite_start]`I2C_OA` is shown in Figure 21-35 and described in Table 21-28[cite: 2456, 2457]. [cite_start]**CAUTION:** During an active transfer phase (between STT having been set to 1 and reception of ARDY), no modification must be done in this register[cite: 2459]. [cite_start]Changing it may result in an unpredictable behavior[cite: 2460]. [cite_start]This register is used to specify the module's base I2C 7-bit or 10-bit address (base own address)[cite: 2460].


### Table 21-28. I2C_OA Register Field Descriptions
| Bit | Field | Type | Reset | Description |
| :--- | :--- | :--- | :--- | :--- |
| **31-10** | RESERVED | R | 0h | [cite_start]Reserved[cite: 2464]. |
| **9-0** | OA | R/W | 0h | [cite_start]Own address[cite: 2464]. [cite_start]This field specifies either:<br>• A 10-bit address coded on OA[9:0] when XOA (Expand Own Address, `I2C_CON[7]`) is set to 1 [cite: 2464][cite_start].<br>• A 7-bit address coded on OA[6:0] when XOA (Expand Own Address, `I2C_CON[7]`) is cleared to 0. In this case, OA[9:7] bits must be cleared to 000 by application software [cite: 2464][cite_start].<br>Value after reset is low (all 10 bits)[cite: 2464]. |

[cite_start]*(LEGEND: R/W = Read/Write; R = Read only; W1toCl = Write 1 to clear bit; -n = value after reset)* [cite: 2463]

---

## 21.4.1.21 I2C_SA Register (offset = ACh) [reset = 0h]

[cite_start]`I2C_SA` is shown in Figure 21-36 and described in Table 21-29[cite: 2476, 2477]. [cite_start]**CAUTION:** During an active transfer phase (between STT having been set to 1 and reception of ARDY), no modification must be done in this register[cite: 2479]. [cite_start]Changing it may result in an unpredictable behavior[cite: 2480]. [cite_start]This register is used to specify the addressed I2C module 7-bit or 10-bit address (slave address)[cite: 2480].


### Table 21-29. I2C_SA Register Field Descriptions
| Bit | Field | Type | Reset | Description |
| :--- | :--- | :--- | :--- | :--- |
| **31-10** | RESERVED | R | 0h | [cite_start]Reserved[cite: 2484]. |
| **9-0** | SA | R/W | 0h | [cite_start]Slave address[cite: 2484]. [cite_start]This field specifies either:<br>• A 10-bit address coded on SA[9:0] when XSA (Expand Slave Address, `I2C_CON[8]`) is set to 1 [cite: 2484].<br>• A 7-bit address coded on SA[6:0] when XSA (Expand Slave Address, `I2C_CON[8]`) is cleared to 0. In this case, SA[9:7] bits must be cleared to 000 by application software [cite: 2484][cite_start].<br>Value after reset is low (all 10 bits)[cite: 2484]. |

[cite_start]*(LEGEND: R/W = Read/Write; R = Read only; W1toCl = Write 1 to clear bit; -n = value after reset)* [cite: 2483]

---

## 21.4.1.22 I2C_PSC Register (offset = B0h) [reset = 0h]

[cite_start]`I2C_PSC` is shown in Figure 21-37 and described in Table 21-30[cite: 2496, 2497]. [cite_start]**CAUTION:** During an active mode (`I2C_EN` bit in `I2C_CON` register is set to 1), no modification must be done in this register[cite: 2499]. [cite_start]Changing it may result in an unpredictable behavior[cite: 2500]. [cite_start]This register is used to specify the internal clocking of the I2C peripheral core[cite: 2500].


### Table 21-30. I2C_PSC Register Field Descriptions
| Bit | Field | Type | Reset | Description |
| :--- | :--- | :--- | :--- | :--- |
| **31-8** | RESERVED | R | 0h | [cite_start]Reserved[cite: 2509, 2511, 2514, 2515]. |
| **7-0** | PSC | R/W | 0h | [cite_start]Fast/Standard mode prescale sampling clock divider value[cite: 2510, 2513, 2514, 2515, 2518]. [cite_start]The core uses this 8-bit value to divide the system clock (SCLK) and generates its own internal sampling clock (ICLK) for Fast and Standard operation modes[cite: 2518, 2519, 2520]. [cite_start]The core logic is sampled at the clock rate of the system clock for the module divided by `(PSC + 1)`[cite: 2521]. [cite_start]Value after reset is low (all 8 bits) [cite: 2522][cite_start].<br>0h = Divide by 1 [cite: 2523][cite_start]<br>FFh = Divide by 256 [cite: 2525] |

[cite_start]*(LEGEND: R/W = Read/Write; R = Read only; W1toCl = Write 1 to clear bit; -n = value after reset)* [cite: 2504]

---

## 21.4.1.23 I2C_SCLL Register (offset = B4h) [reset = 0h]

[cite_start]`I2C_SCLL` is shown in Figure 21-38 and described in Table 21-31[cite: 2534, 2536]. **CAUTION:** During an active mode (`I2C_EN` bit in `I2C_CON` register is set to 1), no modification must be done in this register[cite: 2537]. [cite_start]Changing it may result in an unpredictable behavior[cite: 2538]. [cite_start]This register is used to determine the SCL low time value when master[cite: 2538].

### Table 21-31. I2C_SCLL Register Field Descriptions
| Bit | Field | Type | Reset | Description |
| :--- | :--- | :--- | :--- | :--- |
| **31-8** | RESERVED | R | 0h | [cite_start]Reserved[cite: 2543]. |
| **7-0** | SCLL | R/W | 0h | [cite_start]Fast/Standard mode SCL low time[cite: 2543]. [cite_start]I2C master mode only (FS)[cite: 2543]. [cite_start]This 8-bit value is used to generate the SCL low time value (tLOW) when the peripheral is operated in master mode[cite: 2543]. [cite_start]`tLOW = (SCLL + 7) * ICLK` time period[cite: 2543]. [cite_start]Value after reset is low (all 8 bits)[cite: 2543]. |

[cite_start]*(LEGEND: R/W = Read/Write; R = Read only; W1toCl = Write 1 to clear bit; -n = value after reset)* [cite: 2541]

---

## 21.4.1.24 I2C_SCLH Register (offset = B8h) [reset = 0h]

[cite_start]`I2C_SCLH` is shown in Figure 21-39 and described in Table 21-32[cite: 2554, 2555]. [cite_start]**CAUTION:** During an active mode (`I2C_EN` bit in `I2C_CON` register is set to 1), no modification must be done in this register[cite: 2557]. [cite_start]Changing it may result in an unpredictable behavior[cite: 2558]. [cite_start]This register is used to determine the SCL high time value when master[cite: 2558].

### Table 21-32. I2C_SCLH Register Field Descriptions
| Bit | Field | Type | Reset | Description |
| :--- | :--- | :--- | :--- | :--- |
| **31-8** | RESERVED | R | 0h | [cite_start]Reserved[cite: 2566]. |
| **7-0** | SCLH | R/W | 0h | [cite_start]Fast/Standard mode SCL high time[cite: 2566]. [cite_start]I2C master mode only (FS)[cite: 2566]. [cite_start]This 8-bit value is used to generate the SCL high time value (tHIGH) when the peripheral is operated in master mode[cite: 2566]. [cite_start]`tHIGH = (SCLH + 5) * ICLK` time period[cite: 2566]. [cite_start]Value after reset is low (all 8 bits)[cite: 2566]. |

[cite_start]*(LEGEND: R/W = Read/Write; R = Read only; W1toCl = Write 1 to clear bit; -n = value after reset)* [cite: 2562]
## 21.4.1.25 I2C_SYSTEST Register (offset = BCh) [reset = 0h]

[cite_start]`I2C_SYSTEST` is shown in Figure 21-40 and described in Table 21-33[cite: 2614]. [cite_start]**CAUTION:** Never enable this register for normal I2C operation[cite: 2616]. [cite_start]This register is used to facilitate system-level tests by overriding some of the standard functional features of the peripheral[cite: 2616]. [cite_start]It allows testing of SCL counters, controlling the signals that connect to I/O pins, or creating digital loop-back for self-test when the module is configured in system test (SYSTEST) mode[cite: 2617]. [cite_start]It also provides stop/non-stop functionality in the debug mode[cite: 2618].


### Table 21-33. I2C_SYSTEST Register Field Descriptions
| Bit | Field | Type | Reset | Description |
| :--- | :--- | :--- | :--- | :--- |
| **31-16** | RESERVED | R | 0h | [cite_start]Reserved. [cite: 2623] |
| **15** | ST_EN | R/W | 0h | [cite_start]System test enable[cite: 2623]. [cite_start]This bit must be set to 1 to permit other system test registers bits to be set[cite: 2623]. [cite_start]Value after reset is low[cite: 2623]. <br>0h = Normal mode. [cite_start]All others bits in register are read only[cite: 2623]. <br>1h = System test enabled. [cite_start]Permit other system test registers bits to be set[cite: 2623]. |
| **14** | FREE | R/W | 0h | [cite_start]Free running mode (on breakpoint)[cite: 2623]. [cite_start]This bit is used to determine the state of the I2C controller when a breakpoint is encountered in the HLL debugger[cite: 2623]. [cite_start]Note: This bit can be set independently of `ST_EN` value[cite: 2623]. [cite_start]<br>`FREE = 0`: the I2C controller stops immediately after completion of the on-going bit transfer[cite: 2623]. [cite_start]Stopping the transfer is achieved by forcing the SCL line low[cite: 2623]. [cite_start]Note that in this case there will be no status register updates[cite: 2623]. [cite_start]<br>`FREE = 1`: the I2C interface runs free[cite: 2623]. [cite_start]When Suspend indication will be asserted, there will be no accesses on the OCP Interface (the CPU is in debug mode) and consequently the FIFOs will reach full/empty state (according to RX or TX modes) and the I2C SDA line will be kept low[cite: 2623]. [cite_start]Note that the status registers will be updated, but no DMA, IRQ or WakeUp will be generated[cite: 2623]. [cite_start]The status registers likely to be updated in this mode are: `I2C_IRQSTATUS_RAW.XRDY`, `I2C_IRQSTATUS_RAW.RRDY`, `I2C_IRQSTATUS_RAW.XUDF`, `I2C_IRQSTATUS_RAW.ROVR`, `I2C_IRQSTATUS_RAW.ARDY` and `I2C_IRQSTATUS_RAW.NACK`[cite: 2623]. [cite_start]Value after reset is low[cite: 2623]. [cite_start]<br>0h = Stop mode (on breakpoint condition)[cite: 2623]. [cite_start]If Master mode, it stops after completion of the on-going bit transfer[cite: 2623]. [cite_start]In slave mode, it stops during the phase transfer when 1 byte is completely transmitted/received[cite: 2623]. [cite_start]<br>1h = Free running mode[cite: 2623]. |
| **13-12** | TMODE | R/W | 0h | [cite_start]Test mode select[cite: 2631]. [cite_start]In normal functional mode (`ST_EN = 0`), these bits are don't care[cite: 2631]. [cite_start]They are always read as 00 and a write is ignored[cite: 2631]. [cite_start]In system test mode (`ST_EN = 1`), these bits can be set according to the following table to permit various system tests[cite: 2631]. [cite_start]Values after reset are low (2 bits)[cite: 2631]. [cite_start]<br>0h = Functional mode (default)[cite: 2631]. [cite_start]<br>1h = Reserved[cite: 2631]. [cite_start]<br>2h = Test of SCL counters (`SCLL`, `SCLH`, `PSC`)[cite: 2631]. [cite_start]SCL provides a permanent clock with master mode[cite: 2631]. [cite_start]<br>3h = Loop back mode select + SDA/SCL IO mode select[cite: 2631]. |
| **11** | SSB | R/W | 0h | [cite_start]Set status bits[cite: 2631]. [cite_start]Writing 1 into this bit also sets the 6 read/clear-only status bits contained in `I2C_IRQSTATUS_RAW` register (bits 5:0) to 1[cite: 2631]. [cite_start]Writing 0 into this bit doesn't clear status bits that are already set only writing 1 into a set status bit can clear it (see `I2C_IRQSTATUS_RAW` operation)[cite: 2631]. [cite_start]This bit must be cleared prior attempting to clear a status bit[cite: 2631]. [cite_start]Value after reset is low[cite: 2631]. [cite_start]<br>0h = No action[cite: 2631]. [cite_start]<br>1h = Set all interrupt status bits to 1[cite: 2631]. |
| **10-9** | RESERVED | R | 0h | [cite_start]Reserved[cite: 2631]. |
| **8** | SCL_I_FUNC | R | 0h | [cite_start]SCL line input value (functional mode)[cite: 2631]. [cite_start]This read-only bit returns the logical state taken by the SCL line (either 1 or 0)[cite: 2631]. [cite_start]It is active both in functional and test mode[cite: 2631]. [cite_start]Value after reset is low[cite: 2631]. [cite_start]<br>0h (R) = Read 0 from SCL line[cite: 2631]. [cite_start]<br>1h (R) = Read 1 from SCL line[cite: 2631]. |
| **7** | SCL_O_FUNC | R | 0h | [cite_start]SCL line output value (functional mode)[cite: 2631]. [cite_start]This read-only bit returns the value driven by the module on the SCL line (either 1 or 0)[cite: 2631]. [cite_start]It is active both in functional and test mode[cite: 2631]. [cite_start]Value after reset is low[cite: 2631]. [cite_start]<br>0h (R) = Driven 0 on SCL line[cite: 2631]. [cite_start]<br>1h (R) = Driven 1 on SCL line[cite: 2631]. |
| **6** | SDA_I_FUNC | R | 0h | [cite_start]SDA line input value (functional mode)[cite: 2631]. [cite_start]This read-only bit returns the logical state taken by the SDA line (either 1 or 0)[cite: 2631]. [cite_start]It is active both in functional and test mode[cite: 2631]. [cite_start]Value after reset is low[cite: 2631]. [cite_start]<br>0h (R) = Read 0 from SDA line[cite: 2631]. [cite_start]<br>1h (R) = Read 1 from SDA line[cite: 2631]. |
| **5** | SDA_O_FUNC | R | 0h | [cite_start]SDA line output value (functional mode)[cite: 2631]. [cite_start]This read-only bit returns the value driven by the module on the SDA line (either 1 or 0)[cite: 2631]. [cite_start]It is active both in functional and test mode[cite: 2631]. [cite_start]Value after reset is low[cite: 2631]. [cite_start]<br>0h (R) = Driven 0 to SDA line[cite: 2631]. [cite_start]<br>1h (R) = Driven 1 to SDA line[cite: 2631]. |
| **4** | RESERVED | R | 0h | [cite_start]Reserved[cite: 2631]. |
| **3** | SCL_I | R | 0h | [cite_start]SCL line sense input value[cite: 2640]. [cite_start]In normal functional mode (`ST_EN = 0`), this read-only bit always reads 0[cite: 2640]. [cite_start]In system test mode (`ST_EN = 1` and `TMODE = 11`), this read-only bit returns the logical state taken by the SCL line (either 1 or 0)[cite: 2640]. [cite_start]Value after reset is low[cite: 2640]. [cite_start]<br>0h (R) = Read 0 from SCL line[cite: 2640]. [cite_start]<br>1h (R) = Read 1 from SCL line[cite: 2640]. |
| **2** | SCL_O | R | 0h | [cite_start]SCL line drive output value[cite: 2640]. [cite_start]In normal functional mode (`ST_EN = 0`), this bit is don't care[cite: 2640]. [cite_start]It always reads 0 and a write is ignored[cite: 2640]. [cite_start]In system test mode (`ST_EN = 1` and `TMODE = 11`), a 0 forces a low level on the SCL line and a 1 puts the I2C output driver to a high-impedance state[cite: 2640]. [cite_start]Value after reset is low[cite: 2640]. [cite_start]<br>0h (R) = Forces 0 on the SCL data line[cite: 2640]. [cite_start]<br>1h (R) = SCL output driver in high-impedance state[cite: 2640]. |
| **1** | SDA_I | R | 0h | [cite_start]SDA line sense input value[cite: 2640]. [cite_start]In normal functional mode (`ST_EN = 0`), this read-only bit always reads 0[cite: 2640]. [cite_start]In system test mode (`ST_EN = 1` and `TMODE = 11`), this read-only bit returns the logical state taken by the SDA line (either 1 or 0)[cite: 2640]. [cite_start]Value after reset is low[cite: 2640]. [cite_start]<br>0h (R) = Read 0 from SDA line[cite: 2640]. [cite_start]<br>1h (R) = Read 1 from SDA line[cite: 2640]. |
| **0** | SDA_O | R/W | 0h | [cite_start]SDA line drive output value[cite: 2640]. [cite_start]In normal functional mode (`ST_EN = 0`) this bit is don't care[cite: 2640]. [cite_start]It reads as 0 and a write is ignored[cite: 2640]. [cite_start]In system test mode (`ST_EN = 1` and `TMODE = 11`), a 0 forces a low level on the SDA line and a 1 puts the I2C output driver to a high-impedance state[cite: 2640]. [cite_start]Value after reset is low[cite: 2640]. [cite_start]<br>0h = Write 0 to SDA line[cite: 2640]. [cite_start]<br>1h = Write 1 to SDA line[cite: 2640]. |

*(LEGEND: R/W = Read/Write; R = Read only; W1toCl = Write 1 to clear bit; -n = value after reset)*

## 21.4.1.26 I2C_BUFSTAT Register (offset = C0h) [reset = 0h]

`I2C_BUFSTAT` is shown in Figure 21-41 and described in Table 21-34. This read-only register reflects the status of the internal buffers for the FIFO management (see the FIFO Management subsection).


### Table 21-34. I2C_BUFSTAT Register Field Descriptions
| Bit | Field | Type | Reset | Description |
| :--- | :--- | :--- | :--- | :--- |
| **31-16** | RESERVED | R | 0h | Reserved. |
| **15-14** | FIFODEPTH | R | 2h | Internal FIFO buffers depth. This read-only bit indicates the internal FIFO buffer depth. Value after reset is given by the boundary module generic parameter.<br>0h = 8-bytes FIFO<br>1h = 16-bytes FIFO<br>2h = 32-bytes FIFO<br>3h = 64-bytes FIFO |
| **13-8** | RXSTAT | R | 0h | RX buffer status. This read-only field indicates the number of bytes to be transferred from the FIFO at the end of the I2C transfer (when RDR is asserted). It corresponds to the level indication of the RX FIFO (number of written locations). Value after reset is 0. |
| **7-6** | RESERVED | R | 0h | Reserved. |
| **5-0** | TXSTAT | R | 0h | TX buffer status. This read-only field indicates the number of data bytes still left to be written in the TX FIFO (it is equal to the initial value of `I2C_CNT.DCOUNT` minus the number of data bytes already written in the TX FIFO through the OCP Interface). Value after reset is equal to 0. |

*(LEGEND: R/W = Read/Write; R = Read only; W1toCl = Write 1 to clear bit; -n = value after reset)*

---

## 21.4.1.27 I2C_OA1 Register (offset = C4h) [reset = 0h]

`I2C_OA1` is shown in Figure 21-42 and described in Table 21-35. **CAUTION:** During an active transfer phase (between STT has been set to 1 and receiving of ARDY), no modification must be done in this register. Changing it may result in an unpredictable behavior. This register is used to specify the first alternative I2C 7-bit or 10-bit address (own address 1 - OA1).


### Table 21-35. I2C_OA1 Register Field Descriptions
| Bit | Field | Type | Reset | Description |
| :--- | :--- | :--- | :--- | :--- |
| **31-10** | RESERVED | R | 0h | Reserved. |
| **9-0** | OA1 | R/W | 0h | Own address 1. This field specifies either:<br>• A 10-bit address coded on `OA1[9:0]` when XOA1 (Expand Own Address 1 - XOA1, `I2C_CON[6]`) is set to 1.<br>• A 7-bit address coded on `OA1[6:0]` when XOA1 (Expand Own Address 1 - XOA1, `I2C_CON[6]`) is cleared to 0. In this case, `OA1[9:7]` bits must be cleared to 000 by application software.<br>Value after reset is low (all 10 bits). |

*(LEGEND: R/W = Read/Write; R = Read only; W1toCl = Write 1 to clear bit; -n = value after reset)*

---

## 21.4.1.28 I2C_OA2 Register (offset = C8h) [reset = 0h]

`I2C_OA2` is shown in Figure 21-43 and described in Table 21-36. **CAUTION:** During an active transfer phase (between STT has been set to 1 and receiving of ARDY), no modification must be done in this register. Changing it may result in an unpredictable behavior. This register is used to specify the second alternative I2C 7-bit or 10-bit address (own address 2 - OA2).


### Table 21-36. I2C_OA2 Register Field Descriptions
| Bit | Field | Type | Reset | Description |
| :--- | :--- | :--- | :--- | :--- |
| **31-10** | RESERVED | R | 0h | Reserved. |
| **9-0** | OA2 | R/W | 0h | Own address 2. This field specifies either:<br>• A 10-bit address coded on `OA2[9:0]` when XOA2 (Expand Own Address 2 - XOA2, `I2C_CON[5]`) is set to 1.<br>• A 7-bit address coded on `OA2[6:0]` when XOA2 (Expand Own Address 2 - XOA2, `I2C_CON[5]`) is cleared to 0. In this case, `OA2[9:7]` bits must be cleared to 000 by application software.<br>Value after reset is low (all 10 bits). |

*(LEGEND: R/W = Read/Write; R = Read only; W1toCl = Write 1 to clear bit; -n = value after reset)*

---

## 21.4.1.29 I2C_OA3 Register (offset = CCh) [reset = 0h]

`I2C_OA3` is shown in Figure 21-44 and described in Table 21-37. **CAUTION:** During an active transfer phase (between STT has been set to 1 and receiving of ARDY), no modification must be done in this register. Changing it may result in an unpredictable behavior. This register is used to specify the third alternative I2C 7-bit or 10-bit address (own address 3 - OA3).


### Table 21-37. I2C_OA3 Register Field Descriptions
| Bit | Field | Type | Reset | Description |
| :--- | :--- | :--- | :--- | :--- |
| **31-10** | RESERVED | R | 0h | Reserved. |
| **9-0** | OA3 | R/W | 0h | Own address 3. This field specifies either:<br>• A 10-bit address coded on `OA3[9:0]` when XOA3 (Expand Own Address 3 - XOA3, `I2C_CON[4]`) is set to 1.<br>• A 7-bit address coded on `OA3[6:0]` when XOA3 (Expand Own Address 3 - XOA3, `I2C_CON[4]`) is cleared to 0. In this case, `OA3[9:7]` bits must be cleared to 000 by application software.<br>Value after reset is low (all 10 bits). |

*(LEGEND: R/W = Read/Write; R = Read only; W1toCl = Write 1 to clear bit; -n = value after reset)*

## 21.4.1.30 I2C_ACTOA Register (offset = D0h) [reset = 0h]

[cite_start]`I2C_ACTOA` is shown in Figure 21-45 and described in Table 21-38[cite: 2849, 2850]. [cite_start]This read-only register is used to indicate which one of the module's four own addresses the external master used when addressing the module[cite: 2852]. [cite_start]The CPU can read this register when the AAS indication was activated[cite: 2853]. [cite_start]The indication is cleared at the end of the transfer[cite: 2854].


### Table 21-38. I2C_ACTOA Register Field Descriptions
| Bit | Field | Type | Reset | Description |
| :--- | :--- | :--- | :--- | :--- |
| **31-4** | RESERVED | R | 0h | [cite_start]Reserved. [cite: 2859] |
| **3** | OA3_ACT | R | 0h | Own address 3 active. When a bit location is set to 1 by the core, it signalizes to the Local Host that an external master using the corresponding own address addressed the module. Value after reset is low. [cite_start]<br>0h = Own address inactive <br>1h = Own address active [cite: 2859] |
| **2** | OA2_ACT | R | 0h | Own address 2 active. When a bit location is set to 1 by the core, it signalizes to the Local Host that an external master using the corresponding own address addressed the module. Value after reset is low. [cite_start]<br>0h = Own address inactive <br>1h = Own address active [cite: 2859] |
| **1** | OA1_ACT | R | 0h | Own address 1 active. When a bit location is set to 1 by the core, it signalizes to the Local Host that an external master using the corresponding own address addressed the module. Value after reset is low. [cite_start]<br>0h = Own address inactive <br>1h = Own address active [cite: 2859] |
| **0** | OA0_ACT | R | 0h | Own address 0 active. When a bit location is set to 1 by the core, it signalizes to the Local Host that an external master using the corresponding own address addressed the module. Value after reset is low. [cite_start]<br>0h = Own address inactive <br>1h = Own address active [cite: 2859] |

[cite_start]*(LEGEND: R/W = Read/Write; R = Read only; W1toCl = Write 1 to clear bit; -n = value after reset)* [cite: 2857]

---

## 21.4.1.31 I2C_SBLOCK Register (offset = D4h) [reset = 0h]

[cite_start]`I2C_SBLOCK` is shown in Figure 21-46 and described in Table 21-39[cite: 2868, 2869]. [cite_start]This read/write register controls the automatic blocking of I2C clock feature in slave mode[cite: 2871]. [cite_start]It is used for the Local Host to configure for which of the 4 own addresses, the core must block the I2C clock (keep SCL line low) right after the Address Phase, when it is addressed as a slave[cite: 2872].


### Table 21-39. I2C_SBLOCK Register Field Descriptions
| Bit | Field | Type | Reset | Description |
| :--- | :--- | :--- | :--- | :--- |
| **31-4** | RESERVED | R | 0h | [cite_start]Reserved. [cite: 2877] |
| **3** | OA3_EN | R/W | 0h | Enable I2C clock blocking for own address 3. When the CPU sets a bit location to 1, if an external master using the corresponding own address addresses the core, the core will block the I2C clock right after the address phase. For releasing the I2C clock the CPU must write 0 in the corresponding field. Value after reset is low. [cite_start]<br>0h = I2C clock released <br>1h = I2C clock blocked [cite: 2877] |
| **2** | OA2_EN | R/W | 0h | Enable I2C clock blocking for own address 2. When the CPU sets a bit location to 1, if an external master using the corresponding own address addresses the core, the core will block the I2C clock right after the address phase. For releasing the I2C clock the CPU must write 0 in the corresponding field. Value after reset is low. [cite_start]<br>0h = I2C clock released <br>1h = I2C clock blocked [cite: 2877] |
| **1** | OA1_EN | R/W | 0h | Enable I2C clock blocking for own address 1. When the CPU sets a bit location to 1, if an external master using the corresponding own address addresses the core, the core will block the I2C clock right after the address phase. For releasing the I2C clock the CPU must write 0 in the corresponding field. Value after reset is low. [cite_start]<br>0h = I2C clock released <br>1h = I2C clock blocked [cite: 2877] |
| **0** | OA0_EN | R/W | 0h | Enable I2C clock blocking for own address 0. When the CPU sets a bit location to 1, if an external master using the corresponding own address addresses the core, the core will block the I2C clock right after the address phase. For releasing the I2C clock the CPU must write 0 in the corresponding field. Value after reset is low. [cite_start]<br>0h = I2C clock released <br>1h = I2C clock blocked [cite: 2885] |

[cite_start]*(LEGEND: R/W = Read/Write; R = Read only; W1toCl = Write 1 to clear bit; -n = value after reset)* [cite: 2875]
