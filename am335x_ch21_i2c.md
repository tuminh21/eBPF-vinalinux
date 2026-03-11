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
