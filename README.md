# 20V Peak-to-Peak Input Portable Oscilloscope (ECE342-S25 Group 06)

Andrew Gondoputro - 934509117  
Gavin Le - 934436827  
Luka Radovic - 934514337

---

## Video Link

https://drive.google.com/file/d/1aEVAO4HJbRVvt9BJfBhxNW7Rm_KZIpJn/view?usp=sharing

---

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
