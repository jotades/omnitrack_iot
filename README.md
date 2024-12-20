## Omnitrack

The system consists of distributed and coordinated units placed on both generic objects and the user. Each unit can provide assistive feedback, such as light, sound, or vibration patterns, useful to the patient for space orientation. These feedback are generated real-time, based on how the user navigates through the space and their proximity to target objects. To achieve this goal, the system embeds functionalities to recognize the spatial positions and orientation of both the user and the objects, how the objects are manipulated, including actuators, lights, and speakers to deliver the feedback.

**Control Unit:** 
      The control unit coordinates the real-time functioning of the system, receives and analyzes data from input devices, and processes the data to generate the appropriate feedback. 
    
**Input:** 
    - Positional sensor: Detect the spatial position of the user and the object within the space. 
    - Orientation sensor: Data collected from IMU sensor (Inertial Measurement Unit) which include accelerometer and gyroscope are used for a comprehensive understanding of the device's orientation. 

**Output:**
    - Haptic: A vibrator motor is included to provide haptic feedback. This motor can be used in various applications, such as guiding users through vibration in scenarios where auditory or visual signals might not be effective. 
    - Lights: NeoPixel Ring lights are used for visual feedback. These lights are programmable and can display a wide range of colors and patterns, making them suitable for signaling and user interface purposes. 
    Speaker Amplifier: The speaker amplifier is experimentally integrated to generate sound signal stimulating sound spatial orientation. 

**Concept:**
![image](https://github.com/user-attachments/assets/961e26b9-bf1f-4304-b19f-f70473c14408)
