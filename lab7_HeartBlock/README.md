# lab7_HeartBlock

## Purpose

In Lab7 you will walk through the design process to build a very simple pacemaker. The system will read from a switch, makes decisions, and outputs to an LED. You will learn and understand the steps required to initialize parallel ports. You will write subroutines that wait for the switch to be pressed, wait for the switch to be released, and create an output pulse of fixed duration.


## System Requirements / assignment

In this lab you will create seven subroutines. Solving a complex problem by breaking it into simpler subtasks is an effective approach to design. There is one input called the Atrial sensor (AS), and two outputs Ready and Ventricular trigger (VT). AS input is connected to PF4. In a real pacemaker, electrodes (wires) are attached to the heart and electronics are used to sense the activity of the heart. In this lab, a switch will be used to emulate the operation of the atrial sensor. When the switch SW1 is pressed (AS=0) it means the atria has begun to contract. When the switch SW1 is released, it means the atria have completed contraction. The Ready output is connected to PF3. The LED will be green when Ready is high. The Ready signal is used for debugging, and not part of an actual pacemaker. In a real pacemaker, electrodes (wires) are attached to the heart, and electrical pulses are generated that stimulate the heart, causing it to beat. In this lab, the VT output is connected to PF1, such that the LED will be red when VT is high. A 250 ms pulse will cause the ventricules to contract.

The Ready signal is initially high. On the falling edge of the AS, you should make Ready low. You will then wait 10 ms. This wait is necessary to eliminate bouncing in the switch. Next you wait for the rising edge of AS, which means the atria are done contracting. You will then wait 250 ms, and then output a 250ms pulse on VT. In particular, your system should repeat these 9 steps over and over. The steps a-i are illustrated in Figure 7.2.

      a) Make the Ready signal go high
      b) Wait for AS to be 0, switch to be pressed
      c) Make the Ready signal go low
      d) Wait 10ms
      e) Wait for AS to be 1, switch to be released
      f) Wait 250ms
      g) Make the VT signal go high
      h) Wait 250ms
      i) Make the VT signal go low
