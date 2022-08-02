# How To Integrate a New Gnc Component

F' projects will typically have some sort of Gnc sensor in order to control the movement of the spacecraft. To perform
this function, these projects will need to integrate with Gnc Hardware such as a IMU. This guide will outline
the process to integrate a Gnc component. 

## Who Should Follow This Guide? 

Projects who wish to use the standard F' driver models seen [here](https://github.com/nasa/fprime/tree/0ae2321bb552174ce607075b1283029d6d75a6d6/Drv) but wish to use project specific communications hardware.

### Assumptions

This guide makes the following assumptions about the user following the guide and the project setup that will deploy the
component.

1. User is familiar with standard F´ concepts
2. User understands the [application-manager-drive](https://nasa.github.io/fprime/UsersGuide/best/app-man-drv.html) pattern
3. User is familiar with the F´ development process
4. User understands how to integrate components into an F´ topology

## Example Hardware Description 

Throughout this guide, we will be using the process of integrating an MPU6050 Imu sensor and its I2C interface as an example. 

## Step 1: Define Component Requirements

The first step is to define the type of requirements that the sensor should fulfill. These requirements should 
capture: 

1. Ports and Interfaces
2. Events, telemetry, and commands
3. Component behavior

The example requirements for an Imu component is shown below: 

| Requirement | Description                                                                             | Verification Method |
|-------------|-----------------------------------------------------------------------------------------|---------------------|
| GNC-IMU-001 | The 'Gnc::Imu' component shall produce telemetry of accelerometer data in vector form   | Unit Test           |
| GNC-IMU-002 | The 'Gnc::Imu' component shall produce telemetry of gyroscope data in vector form       | Unit Test           |
| GNC-IMU-003 | The 'Gnc::Imu' component shall be able to communicate with registers using I2C protocol | Inspection          |


## Step 2: Component Design 

The second step to integrating the Imu component is designing the component. This section will go through each design 
and each decision behind the design. Projects should implement design decisions based on component requirements. 

The final FPP model for the MPU6050 Imu component can be found [here](https://github.com/fprime-community/fprime-system-reference/blob/pelmini-branch/SystemReference/Gnc/Imu/Imu.fpp).

### 2.1 Base Component Design 

First, a project should choose the basic component properties. Namely, the component must be `active`, `passive` or
`queued`. Properties of the component will be dependent on the complexity of the required behavior from the hardware. 

For the MPU6050 example, since the device will only be reading and writing accelerometer and gyroscope data through 
by a push approach instead of a pull, having the component be passive is sufficient as there is no need for control 
over the timing of how the data is collected. 

### 2.2 Port Design 

Required ports are typically dependent on the outlined requirements, as well as the underlying driver of the sensor. 
Furthermore, rate group and standard ports are often needed. 

The example component uses ['Drv::LinuxI2cDriver'](https://github.com/nasa/fprime/tree/master/Drv/LinuxI2cDriver),
which implements the `Drv.I2c` interface. The component also contains `Gnc::ImuDataPort`, a rate group port, 
and standard F´ ports for events, channels, and telemetry. An example port chart can be seen below: 

| Kind            | Name              | Port Type         | Usage                                              |
|-----------------|-------------------|-------------------|----------------------------------------------------|
| `output`        | `read`            | `Drv.I2c`         | Port that outputs the read data from sensor        |
| `output`        | `write`           | `Drv.I2c`         | Port that outputs written data from sensor         |
| `guarded input` | `getGyroscope`    | `Gnc.ImuDataPort` | Port that returns gyroscope data                   |
| `guarded input` | `getAcceleration` | `Gnc.ImuDataPort` | Port that returns acceleration data                |
| `guarded input` | `run`             | `Svc.Sched`       | Port that updates accelerometer and gyroscope data |
**Note:** standard event, telemetry, and command ports are not listed above.

### 2.3 Event Design 

Component implementors are free to define whatever events are needed for their project. Typically, Gnc components will 
emit an error event when requesting telemetry. 

In this example, the Imu component will define one event:

| Event          | Severity   | Description                              |
|----------------|------------|------------------------------------------|
| TelemetryError | WARNING_HI | Error occurred when requesting telemetry |

### 2.4 Telemetry Design

Component implementors are free to define whatever telemetry/channels are needed for their project. Typically, Gnc
components emit all sorts of information needed for guidance and navigation.

In this example, the Imu component will define two telemetry channels:

| Telemetry Channel | Type   | Description                             |
|-------------------|--------|-----------------------------------------|
| accelerometer     | Vector | X, Y, Z acceleration from accelerometer |
| gyroscope         | Vector | X, Y, Z degrees from gyroscope          |

### 2.5 Commands Design

Component implementors are free to define whatever commands are needed for their project. Some example of commands to consider 
would include power on, power off. 

In this example, no commands were defined. 

## Implementation and Testing

Projects will need to implement the port handlers and implementation for their communication component on their own.
Specific implementations will diverge based on hardware and design choices.

In order to help in this process, the example component implementation is available for [reference](https://github.com/fprime-community/fprime-system-reference/blob/pelmini-branch/SystemReference/Gnc/Imu/Imu.cpp).

## Topology Integration

Once the design and implementation is done, the component can be added to a projects' topology. 
Project may attach additional support components as needed.

## Conclusion

In this guide, we have covered the design of new communications component and seen how to integrate it into the
topology. At this point, projects should be up and running with whatever communications hardware they deploy.
