This lab designs a traffic light controller using the PIC18F4620, system timer, and input sensors. The controller manages four RGB LEDs, pedestrian crossing displays, buzzer signals, and day/night operation modes.  

✅ Lab Tasks  

- Timer Routines  

  - Implement Wait_Half_Second(), Wait_One_Second(), and Wait_N_Seconds() using Timer0.  

  - Toggle a status LED (SEC_LED) for timing verification.  

- Traffic Light Control  

  - Define RGB signals for NS, EW, NSLT, and EWLT directions.  

  - Write Set_NS(), Set_EW(), Set_NSLT(), Set_EWLT() to set RED/GREEN/YELLOW.  

- Pedestrian Countdown  

  - Implement PED_Control(Direction, Num_Sec) to display countdown on dual 7-segment displays.  

  - Activate buzzer during countdown (audible pedestrian aid).  

- Modes of Operation

  - Night Mode – Simplified sequence without pedestrian counters; MODE LED OFF.  

  - Day Mode – Full sequence with pedestrian crossing logic; MODE LED ON.  

- Sensors & Inputs  

  - Car left-turn switches (NSLT_SW, EWLT_SW).  

  - Pedestrian switches (NSPED_SW, EWPED_SW).  

  - Photoresistor input determines Day/Night mode (2.5V threshold).  

🔑 Key Skills  

- Hardware timer delays with prescaler calculation.  

- Multi-LED traffic signal control with RGB states.  

- Pedestrian countdown display and buzzer synchronization.  

- Mode switching based on sensor inputs.  

- Structured sequencing for real-world traffic controller simulation.  
