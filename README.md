# 20V Peak-to-Peak Input Portable Oscilloscope (ECE342-S25 Group 06)

Andrew Gondoputro - 934509117  
Gavin Le - 934436827  
Luka Radovic - 934514337

---

## Video Link

https://youtu.be/6TVrHASN3Tg

https://drive.google.com/file/d/1aEVAO4HJbRVvt9BJfBhxNW7Rm_KZIpJn/view?usp=sharing


## Team Member Work Distribution

| Team Member | ID | Contributions | Hours Worked |
|-------------|----|----------------|---------------|
| **Andrew Gondoputro** | 934509117 | **Enclosure:** Implementing a case to house the system that is durable using Fusion360 and printed in PLA. Designed around all other parts in the project with portholes. Incorporated wire management and worked with the interior of the case. | (26 hours) |
| | | **Code Integration and Processing Logic:** Scaling ADC output, managing waveform buffers, UI state logic, and graphical data formatting for LCD | (20 hours) |
| | | **Total:** | **46 hours** |
| **Gavin Le** | 934436827 | **Power Block:** Researching buck converters, parts for the negative voltage circuit, buying parts, soldering, simulation | (9.5 hours) |
| | | **User Interface and Display:** Researching encoder, microcontroller, and LCD screen functionality. Understanding I2C and SPI interfaces | (21 hours) |
| | | **Total:** | **30.5 hours** |
| **Luka Radovic** | 934514337 | **Microcontroller block understanding and verifying functional capability** | (16 hours) |
| | | **Analog Front End design and development including:** circuit conceptualization, oscilloscope AFE research, relearning Op-Amp functionality, current, voltage and gain calculations, schematic design, PCB design, PCB order, component selection, simulation, and verification | (52 hours) |
| | | **Total:** | **68 hours** |

---

## Engineering Requirements

- **Customer Requirement:** The oscilloscope must have multiple channels.  
  - **Engineering Requirement:** The system will have at least two channels that can function simultaneously and independently.

- **Customer Requirement:** The system must be modular.  
  - **Engineering Requirement:** The system must connect and disconnect from the oscilloscope probes using robust connectors.

- **Customer Requirement:** The system must have a capable user interface.  
  - **Engineering Requirement:** The system must include a configurable trigger, adjustable time, and adjustable voltage axis.

- **Customer Requirement:** The oscilloscope must be responsive.  
  - **Engineering Requirement:** The system must respond to user input in under 100 milliseconds.

- **Customer Requirement:** The oscilloscope must have good fidelity.  
  - **Engineering Requirement:** The system must sample at a rate of at least 200 kHz independently on all channels.

- **Customer Requirement:** The system must be easy to transport.  
  - **Engineering Requirement:** The system will be less than 7.5 lbs in weight.

- **Customer Requirement:** The system enclosure must be sturdy.  
  - **Engineering Requirement:** The system will support a weight of 10 lbs placed on top.

---

## System Level Block Diagram

Oscilloscope  
ext_pwr_dcpwr  
extprobe_sigint_asig  
UI_user_output  
ext_UI_userin  
ext_case_envin  

*Figure 1. System boundary diagram of the oscilloscope and its external interfaces.*

The system boundary diagram depicts the oscilloscope as a black-box system with its four system-level interfaces: ext_pwr_dcpwr, extprobe_sigint_asig, extinput_UI_userin, and UI_user_output. These labels correspond to the power input, analog input, user control input, and display output, respectively. This figure summarizes the primary external interactions relevant to system-level validation and engineering requirements.

---

## System Description

The 20V Peak-to-Peak Input Portable Oscilloscope is a compact, dual-channel digital signal measurement system designed to capture and visualize analog input signals up to ±10 V (20 Vpp) with per-channel bandwidths of 100 kHz. The system features simultaneous two-channel operation, waveform plotting, user-adjustable voltage and time scaling, and trigger/pause functionality. A 3D-printed PLA enclosure houses all components and is sized at approximately 3.5 x 4.5 x 7.5 inches, providing structural integrity sufficient to support over 10 lbs of external load while remaining lightweight for portability.

At the center of the system is the Teensy 4.1 microcontroller, selected for its high-speed 12-bit ADCs and processing capabilities exceeding 1 million samples per second. The Teensy receives conditioned signals from a custom-built analog front end (AFE), which scales the ±10 V input range to a safe 0-3.3 V window compatible with the Teensy's ADCs. This signal interpreter stage includes AC/DC coupling via a selectable switch, a unity gain buffer, and an inverting amplifier with a gain of −1.65, biased using a 0.623 V precision reference. The output is then passed through a low-pass filter and a diode clamping network to remove high-frequency noise and protect against overvoltage.

The microcontroller firmware handles three critical tasks: signal acquisition, user input processing, and graphical display. In the sampling loop, the Teensy reads from two ADC channels and scales the digital values to reconstruct the original ±10 V waveform. These samples are stored in arrays, which are converted into pixel positions for rendering on the display. The firmware also continuously monitors a rotary encoder for user input; turning the encoder adjusts the X or Y scaling or changes the active channel, while pressing the encoder toggles modes or activates the trigger. This decision matrix polling logic ensures smooth and responsive user interaction. On the output side, the Teensy uses SPI to update a Hosyond IPS capacitive LCD display with mode information, scaling values, and waveform plots, while also using I2C to communicate with the Adafruit rotary encoder.

Power is supplied via a 9V external battery, which is stepped down through a buck converter to deliver 5 V to the Teensy and display. The analog front end is powered directly from ±9 V, which is provided through both the 9V battery and voltage inverter circuit, supporting full-range swing and accurate signal conditioning. Internally, the enclosure is organized with mounting slots for all components, with precise cutouts for the display and encoder to ensure alignment and durability. This oscilloscope system meets all engineering requirements: it supports multi-channel operation, modular design, responsive UI control under 100 ms, high sampling fidelity, low weight under 7.5 lbs, and mechanical robustness for portable use.

---

## System Design Details and Validation

### Top Level Architecture




*Figure 2. Top-level architecture diagram showing internal module interactions and interface structure.*

The top-level architecture includes six major subsystems: Power, Signal Interpreter, Microcontroller, Code, User Interface & Display, and Enclosure. Internal and system-level interfaces are labeled, with system-level interfaces shown in bold. The architecture reflects how signals are conditioned, digitized, processed, and displayed throughout the whole system, and how user and environmental interactions affect system behavior.

---

### System Design Synthesis

The dual-channel portable oscilloscope system integrates several key subsystems that work together to capture, process, and display analog waveforms. Its primary goal is to allow users to visualize signals up to ±10 V at frequencies exceeding 100 kHz, with adjustable scaling, triggering, and channel selection, all within a compact and portable form factor. The oscilloscope is intended for use in educational labs, sensor testing environments, and lightweight diagnostics for embedded systems, offering an affordable and approachable alternative to commercial benchtop scopes.

Analog input signals enter the system through standard BNC connectors and are routed into the analog front end. This stage includes selectable AC/DC coupling, a unity gain buffer, and an inverting amplifier with a gain of -1.65. The amplifier is level-shifted using a reference voltage of approximately 0.623 V, ensuring that the output remains within the safe 0-3.3 V range of the Teensy’s analog-to-digital converter. A low-pass filter removes high-frequency noise, while diode clamps prevent voltage spikes from reaching the ADC input. This signal conditioning stage ensures safe and accurate conversion of ±10 V analog inputs into digitizable signals.

Digitized acquisition and system control are handled by the Teensy 4.1 microcontroller. Using its high-speed ADCs, the Teensy samples both channels at ≥200 kHz per channel. Captured samples are scaled in firmware and organized into arrays. The firmware then translates these digital values into pixel coordinates for rendering on the oscilloscope’s LCD display. Simultaneously, the Teensy polls a rotary encoder over I2C to monitor user input. Turning the encoder allows the user to adjust the horizontal (time) or vertical (voltage) scaling, depending on the selected mode, while pressing the encoder toggles modes, switches channels, or enables trigger and pause functionality.

The graphical user interface is implemented on a 3.5" Hosyond capacitive touch LCD connected via SPI to the Teensy. It displays voltage and time axes, trigger status, selected channel, and other user feedback such as scaling factors. Waveforms are plotted with display updates occurring as fast as 60 frames per second. User input is captured through an Adafruit Stemma QT I2C rotary encoder, which includes an integrated pushbutton for intuitive mode control. All communications between the microcontroller, display, and encoder use standardized protocols (SPI and I2C), and the firmware includes logic branches and debounce handling to ensure reliable and responsive input reading within 100 ms, meeting the system’s responsiveness requirement.

The power system’s purpose is to step down and convert the input voltage of 9 V into the different voltage levels required by the individual components of the oscilloscope. This includes supplying appropriate input voltages for the signal interpreter, the microcontroller, and the user interface and display. The power system is composed of two separate subcircuits: a buck converter and an inverting voltage generator. The buck converter is an Eplzon LM2596 module that steps 9 V down to 5 V for powering the Teensy 4.1 and LCD. The inverting voltage generator uses an ICL7660S IC to produce a −9 V rail, which, together with the original +9 V input, forms the ±9 V supply needed by the analog front end. Each subcircuit serves a different purpose and provides clean, stable voltages to support reliable oscilloscope operation.

All components are mounted in a custom 3D-printed PLA enclosure measuring approximately 3.5 x 4.5 x 7.5 inches. The housing includes internal mounting features for PCBs, external cutouts for connectors and controls, and structural support to prevent mechanical failure under loads of at least 10 lbs. The enclosure was modeled to ensure all parts are properly aligned and thermally isolated where needed.

System-level interfaces were verified to meet compliance with engineering requirements. The analog front end was tested to confirm that its output consistently remains within the 0-3.3 V range under all valid input conditions. Sampling rates were confirmed to exceed 200 kHz per channel. User inputs were tested for responsiveness and correct behavior during scaling and triggering operations. SPI and I2C communications between the display, encoder, and microcontroller were verified via both visual output and debug Serial printing. Power delivery was measured under load using multimeters and simulated loads to ensure all voltage rails remained stable.

In closing, the system’s subsystems work together to satisfy design criteria: modularity, sampling, dual-channel support, clear visualization, responsive user control, mechanical durability, and low-cost manufacturability. This oscilloscope is an ideal fit for many simple and practical applications where a full lab grade oscilloscope is not necessary and would be cumbersome.

---

### Block Design Details List

- Microcontroller and Signal Interpreter  
- Power and User Interface/Display  
- Code and Enclosure

---

## Future Recommendations

This term project focused on building a portable, dual-channel oscilloscope with custom analog input conditioning, firmware, and a user interface. This was the first project of such scope for all of our team. A major turning point came when we redesigned the analog front end to use an inverting amplifier with level-shift biasing, enabling clean scaling of ±10 V signals. Another key moment was optimizing the display and thus entire system module by shifting to an LCD over a monitor.

Some technical challenges we faced were analog signal scaling and code design. We overcame these by refining our op-amp circuit and simplifying our firmware loop. If a new team took over, they’d need to understand how tightly the ADC, encoder, and display are linked, timing across these systems is critical. Additional non technical challenges we faced were tighter then expected deadlines and shipping delays.

Our advice to the next team is to prototype early and test subsystems independently. Use known libraries and validate signal paths before integrating. If we could restart the term, we’d begin firmware development earlier, and front-load analog testing to avoid end-of-term integration stress. We recognize though that the first couple of weeks are almost a data gathering or understanding phase that is difficult to avoid.

---

## References

[1] A. Gondoputro, Block 1 and 2 Design Document (Individual submission), 2025.  
[2] L. Radovic, Block 2 and 3 Design Document (Individual submission), 2025.  
[3] PJRC, “Teensy 4.1 Technical Specifications,” [Online]. Available: https://www.pjrc.com/store/teensy41.html.  
[4] Texas Instruments, “TLV2372 Dual Low-Noise Rail-to-Rail Out Op Amp,” [Online]. Available: https://www.ti.com/lit/ds/symlink/tlv2372.pdf.  
[5] Digital Caliper Measurement Log, Enclosure Dimensions vs. Fusion360 CAD Design, April 2025. Measurement by A. Gondoputro.  
[6] G. Le, Block 1 and 5 Design Document (Individual submission), 2025.  
[7] Adafruit, “Rotary Encoder Breakout with Seesaw,” [Online]. Available: https://learn.adafruit.com/rotary-encoder-seesaw.  
[8] Adafruit, “Seesaw I2C Encoder Library,” GitHub Repository, [Online]. Available: https://github.com/adafruit/Adafruit_Seesaw.  
[9] Hosyond, “3.5 Inch TFT LCD Display Module ILI9486 SPI Interface,” [Online]. Available: https://www.amazon.com/dp/B07Q2T5KYG.  
[10] Texas Instruments, “LM358 Low Power Dual Op-Amp,” [Online]. Available: https://www.ti.com/lit/ds/symlink/lm358.pdf.  
[11] Texas Instruments, “OPA2131: Precision, Low-Noise, Low-Bias Op Amp,” [Online]. Available: https://www.ti.com/lit/ds/symlink/opa2131.pdf.

---

*Got feedback on this document? Email us!*

