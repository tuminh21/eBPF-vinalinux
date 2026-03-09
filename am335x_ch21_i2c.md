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
