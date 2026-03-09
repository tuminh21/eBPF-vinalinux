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

# 21.3 Functional Description

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
