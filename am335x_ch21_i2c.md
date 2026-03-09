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
