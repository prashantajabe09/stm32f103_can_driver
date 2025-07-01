# STM32F103 CAN Driver (Bare-Metal)

This repository contains a bare-metal CAN protocol driver written in C for the STM32F103 microcontroller, developed without using HAL or Cube libraries.

---

## 🚀 Features

- Supports Standard (11-bit) and Extended (29-bit) IDs
- Polling, Interrupt-based reception (FIFO0)
- Transmit via Available Mailbox 
- Configurable CAN bitrate
- Callback mechanism for RX0 interrupt
- Compatible with STM32F103xx (CAN1)
- Test Cases
- Driver Function Checked with LoopBack Mode.
- Configurable filter parameters. users can config filter either in 16-bit or 32-bit mode. ID list mode or ID Mask Mode.

## 🛠️ Getting Started

### 🧱 Prerequisites

- STM32F103C8T6 Board
- STM32CubeIDE 
- CAN transceiver (e.g., TJA1050, MCP2551)
- Serial terminal (for UART debugging if used)

### 🧩 Integration

1. Add `can.c` and `can.h` to your project.

2. Initialize CAN peripheral using: 
   hal_can_init(&can_configuration);

3. Register callback function
register_rx0_callback(my_callback_function);

4. Transmit a message using hal_can_transmit()

5. You can test using loopback mode.

6. Test cases are provided in /Src/can_test.
Supported message structure: can_rx_frame_t and can_tx_frame_t

** how to run Test Cases ** 
In the Src folder right click on mai.c and exclude it from the build process and include can_test_runner.c for build. 
In the can_test_runner.c file at global scope there are arrays for ID list and Mask list. Modify this as per your test cases. In the main() call test case functions you want to test. 

** Filter Configuration **
In hal_can_init, enter the filter number you want to configure into filter_config structure.
Assign scale, mode, ID, mask etc parameter of that particular filter number to structure. Macros for configurable parameter given in can.h file.
For mask value enter the mask as 32-bit value with bit set where you want to received ID must match with ID register.


