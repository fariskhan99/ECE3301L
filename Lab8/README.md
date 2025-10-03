This lab extends the Lab 7 traffic controller by replacing 7-segment outputs with a TFT LCD (ST7735 over SPI) to display mode, sensor states, countdowns, and light colors.  

✅ Lab Tasks  

- Set up project with provided files (main.h, ST7735_TFT.c, etc.) and update pin mappings.  

- Replace hardcoded delays with named constants (NS_WAIT, PEDESTRIAN_NS_WAIT, etc.).  

- Implement LCD update routines:  

  - update_LCD_color – draw traffic light state.  

  - update_LCD_count – show countdown timers.  

  - update_LCD_PED_Count – show pedestrian timers.  

  - update_LCD_misc – show mode, sensor states, and light sensor voltage.  

- Integrate LCD updates into Lab 7 routines (Set_NS, PED_Control, Wait_*).  

- Run Day/Night Mode sequences with full LCD feedback.  

🔑 Key Skills  

- SPI-based TFT display control with ST7735 library.  

- Real-time UI updates for sensors, timers, and traffic lights.  

- Hardware abstraction with pin definitions and constants.  
