# lab10_TrafficLight

## Purpose

This lab has these major objectives:

      1) the understanding and implementing of indexed data structures
      2) learning how to create a segmented software system
      3) the study of real-time synchronization by designing a finite state machine controller.

Software skills you will learn include advanced indexed addressing, linked data structures, creating fixed-time delays using the SysTick timer, and debugging real-time systems. Please read the entire lab before starting.


## System Requirements / assignment

Consider a 4-corner intersection as shown in Figure 10.1. There are two one-way streets are labeled South (cars travel South) and West (cars travel West). There are three inputs to your LaunchPad, two are car sensors, and one is a pedestrian sensor. The South car sensor will be true (3.3V) if one or more cars are near the intersection on the South road. Similarly, the West car sensor will be true (3.3V) if one or more cars are near the intersection on the West road. The Walk sensor will be true (3.3V) if a pedestrian is present and he or she wishes to cross in any direction. This walk sensor is different from a walk button on most real intersections. This means when you are testing the system, you must push and hold the walk sensor until the FSM recognizes the presence of the pedestrian. Similarly, you will have to push and hold the car sensor until the FSM recognizes the presence of the car. In this simple system, if the walk sensor is +3.3V, there is pedestrian to service, and if the walk sensor is 0V, there are no people who wish to walk. The walk sensor and walk light will service pedestrians who wish to cross in any direction. This means the roads must both be red before the walk light is activated. In a similar fashion, when a car sensor is 0V, it means no cars are waiting to enter the intersection. The don't walk light should be on while cars have a green or yellow light.

You will interface 6 LEDs that represent the two Red-Yellow-Green traffic lights, and you will use the PF3 green LED for the ???walk??? light and the PF1 red LED for the ???don???t walk??? light. When the ???walk??? condition is signified, pedestrians are allowed to cross in any direction. When the ???don???t walk??? light flashes (and the two traffic signals are red), pedestrians should hurry up and finish crossing. When the ???don???t walk??? condition is on steady, pedestrians should not enter the intersection.

Your solution should not need to be friendly. In other words you should assume the FSM controls all bits in each of the ports it uses.
