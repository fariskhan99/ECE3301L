This lab uses the PIC18F4620 to read analog inputs from a temperature sensor (LMT84) and photoresistor, convert values with the ADC, display results on 7-segment displays, log data over UART, and drive RGB LEDs based on sensor thresholds.  

✅ Lab Tasks  

- Configure ADC (AN0 for temperature, AN1 for light) and compute voltages.  

- Convert LMT84 readings → °C → °F, display on dual 7-segment.  

- Send sensor data to PC via UART (TeraTerm).  

Control RGB LEDs:  

- D1 shows temperature ranges (10°F steps).  

- D2 shows temperature bands.  

- D3 shows light levels from photoresistor.  

🔑 Key Skills  

- ADC setup and channel switching.  

- Sensor data conversion and range mapping.  

- 7-segment BCD decoding.  

- UART communication and debugging.  

- RGB LED control using efficient logic.  
