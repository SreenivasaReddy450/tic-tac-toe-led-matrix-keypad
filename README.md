# Tic-Tac-Toe on STM32 Nucleo-F446RE

A classic **Tic-Tac-Toe game** built entirely from scratch on the **STM32 Nucleo-F446RE** development board.
The project combines **embedded hardware control** with **game logic**, using a **4x4 keypad** for player input, a **custom 3x3 LED matrix** for visual display, and the **Serial Wire Output (SWO)** interface for real-time console feedback.

---

## ✨ Features

-   **Interactive Gameplay:** Two players can play against each other.
-   **Physical Input:** Uses a standard 4x4 matrix keypad for selecting cells (keys 1-9).
-   **Synchronized Display:** The game state is displayed simultaneously on the physical **3x3 LED matrix** for a visual overview and on the **console output** as a classic text-based grid, offering two ways to follow the game.
-   **Player Indicators:** Player 1 ('X') is represented by a flickering LED, and Player 2 ('O') is represented by a steady LED.
-   **Win Detection:** The game automatically detects and indicates a win by highlighting the winning line.

---

## 🛠️ Hardware Requirements

-   1 × **Nucleo-F446RE** development board
-   1 × **4x4 Matrix Keypad**
-   3 × **Green LEDs**
-   3 × **Yellow LEDs**
-   3 × **Blue LEDs**
-   2 × **240 Ω resistors** (for Yellow & Green columns)
-   1 × **20 Ω resistor** (for Blue column)
-   Breadboard and jumper wires

> **Note:** The LED matrix uses 3×Green, 3×Yellow, and 3×Blue LEDs simply because 9 identical LEDs were not available at the time.
> The logic and wiring remain valid for any 9 LEDs of the same color.

---

## 🔌 Hardware Connections

### Keypad → Nucleo-F446RE

-   **Rows (Outputs):** PC0, PC1, PC2, PC3
-   **Columns (Inputs with Pull-up):** PC4, PC5, PC6, PC7

### LED Matrix → Nucleo-F446RE

Configured as **common-anode multiplexed display**. Rows source current, columns sink with PWM.

-   **Rows (Anodes):**
    -   Row 1 → PB1
    -   Row 2 → PB2
    -   Row 3 → PB6
-   **Columns (Cathodes with resistors):**
    -   Column 1 (Yellow LEDs) → 240 Ω → PB4 (TIM3_CH1)
    -   Column 2 (Green LEDs)  → 240 Ω → PB5 (TIM3_CH2)
    -   Column 3 (Blue LEDs)   → 20 Ω → PB0 (TIM3_CH3)

---

## ⚙️ Technical Implementation

### 🔹 Keypad Scanning

The keypad is scanned using a row-column scanning technique. The program drives one row pin LOW at a time and reads the column pins. If a key is pressed, it connects a row to a column, pulling the corresponding column pin LOW and allowing the microcontroller to identify the keypress. A software delay provides debouncing.

### 🔹 LED Matrix Multiplexing

The matrix is driven using multiplexing. The `pwm_led()` function rapidly cycles through the three rows, activating one at a time by driving its anode pin HIGH. While a row is active, the PWM signals on the column cathode pins determine the brightness of the three LEDs in that row. This cycle repeats fast enough (~222 Hz) for the human eye to perceive a stable, complete image.

### 🔹 Current Limiting

Different colored LEDs have different forward voltages ($V_{forward}$). To protect the GPIOs and achieve a balanced brightness, different current-limiting resistors are used for each column based on Ohm's Law ($R = (V_{source} - V_{forward}) / I_{target}$).

-   **Yellow/Green LEDs (240 Ω):** With a $V_{forward}$ of ~2.1V, the current is limited to a safe **5 mA**.
-   **Blue LEDs (20 Ω):** With a higher $V_{forward}$ of ~3.1V, a smaller resistor is needed to achieve a target current of **10 mA**.

### 🔹 Console via SWO

To display the game's text, this project uses the Serial Wire Output (SWO) line instead of a separate UART connection. The `printf` function is retargeted to the microcontroller's ITM (Instrumentation Trace Macrocell), which sends the data through the SWO pin to the ST-Link debugger. This allows the player to see the game grid and prompts in the "SWV ITM Data Console" within an IDE like STM32CubeIDE, requiring no extra hardware.

---

## ▶️ How to Play

1.  **Connect & Flash:** Connect the Nucleo board to your PC and flash the program using your preferred IDE (e.g., STM32CubeIDE).
2.  **Open Console:** Open the SWV ITM Data Console in your IDE and start the trace.
3.  **Start Game:** The console will prompt "Player 1 Enter position of X".
4.  **Enter Move:** Use the 4x4 keypad to press a number from **1 to 9** corresponding to a cell.
5.  **Observe the Board:** Check the game state on either the physical **3x3 LED matrix** or the updated grid in the **console output**. Player 1's move ('X') will be a flickering LED, and Player 2's ('O') will be a steady LED.
6.  **Win/Draw:** The game ends when a player gets three in a row or when the board is full. The winning line is highlighted on the matrix.

---

## 📜 License

This project is licensed under the **MIT License**.
See the [LICENSE](./LICENSE) file for details.
