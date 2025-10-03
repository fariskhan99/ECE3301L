This lab implements a digital voltmeter and ohmmeter using the PIC18F4620 with ADC measurements, 7-segment display output, UART logging, and RGB LED/buzzer feedback.  

✅ Lab Tasks  

- Voltage Meter  

  - Measure input voltage (AN0) using ADC with external VREF=4.096V.  

  - Display voltage on dual 7-segment (X.Y V format) and send to TeraTerm via UART.  

  - Compare results with DVM readings and calculate % error.  

- Ohm Meter  

  - Measure unknown resistors using 3 reference resistors:  

    - Rref1 = 200 Ω (AN1), Rref2 = 2 kΩ (AN2), Rref3 = 20 kΩ (AN4).  

  - Calculate resistance from divider equation, display on 7-segment, and log to UART.  

  - Record % error vs. DVM for multiple resistors.  

  - Indicators:  

    - D1 (RGB) → resistance range color bands.  

    - D2 (RGB) → finer resistance thresholds.  

    - Buzzer → active when RL < 70 Ω.  

🔑 Key Skills

- ADC channel selection with external reference voltage.  

- Voltage-to-resistance calculations using KVL/KCL.  

- 7-segment display formatting with decimal point control.  

- UART serial output for debugging and measurement logging.

- Multi-channel measurement design with LEDs and buzzer as range/status indicators.
