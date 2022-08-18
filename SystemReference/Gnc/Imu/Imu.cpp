// ======================================================================
// \title  Imu.cpp
// \author vbai
// \brief  cpp file for Imu component implementation class
// ======================================================================

#include "Fw/Types/BasicTypes.hpp"
#include <SystemReference/Gnc/Imu/Imu.hpp>

namespace Gnc {

// ----------------------------------------------------------------------
// Construction, initialization, and destruction
// ----------------------------------------------------------------------

Imu ::Imu(const char *const compName)
    : ImuComponentBase(compName), m_setup(false) {
  this->m_i2cDevAddress = I2C_DEV0_ADDR;

}

void Imu ::init(const NATIVE_INT_TYPE instance) {
  ImuComponentBase::init(instance);
}

void Imu::powerOn(){
    Fw::Buffer buffer;
    U8 data[IMU_REG_SIZE*2];
    buffer.setData(data);
    buffer.setSize(sizeof(data));

    FW_ASSERT(sizeof(data) > 0);
    data[0] = 0x6B;
    data[1] = 0;
    write_out(0, I2C_DEV0_ADDR, buffer);
}

Imu ::~Imu() {}

// ----------------------------------------------------------------------
// Handler implementations for user-defined typed input ports
// ----------------------------------------------------------------------

void Imu ::Run_handler(const NATIVE_INT_TYPE portNum,
                       NATIVE_UINT_TYPE context) {
    if (!m_setup){
      powerOn();
      m_setup = true;
    }
    updateAccel();
    updateGyro();
}

Gnc::ImuData Imu ::getAcceleration_handler(const NATIVE_INT_TYPE portNum) {
   ImuData accelData = m_accel;
   m_accel.setstatus(Svc::MeasurementStatus::STALE);
   return accelData;
}

Gnc::ImuData Imu ::getGyroscope_handler(const NATIVE_INT_TYPE portNum) {
  ImuData gyroData = m_gyro;
  m_gyro.setstatus(Svc::MeasurementStatus::STALE);
  return gyroData;
}

// ----------------------------------------------------------------------
// Helper Functions
// ----------------------------------------------------------------------
// Reads in buffer data
Drv::I2cStatus Imu ::read(U8 dev_addr, Fw::Buffer &buffer) {
  return this->read_out(0, dev_addr, buffer);
}

// Reads the next block of address
Drv::I2cStatus Imu::readRegisterBlock(U8 registerAdd, Fw::Buffer &buffer){
  Drv::I2cStatus status;

  status = this->setupReadRegister(m_i2cDevAddress, registerAdd);
  if (status == Drv::I2cStatus::I2C_OK) {
    status = this->read(m_i2cDevAddress, buffer);
  }
  return status;
}

void Imu::updateAccel(){
  Fw::Buffer buffer;
  Fw::Buffer test;
  U32 value;
  U8 data[IMU_MAX_DATA_SIZE];
  U8 dataTest[IMU_MAX_DATA_SIZE];

  Gnc::Vector vector;

  buffer.setData(data);
  buffer.setSize(IMU_MAX_DATA_SIZE);

  test.setData(dataTest);
  test.setSize(IMU_MAX_DATA_SIZE);

  Drv::I2cStatus statusAccelerate = readRegisterBlock(IMU_RAW_ACCEL_ADDR, buffer);

  Drv::I2cStatus statusRegisterConfig = readRegisterBlock(0x28, buffer);
  if (statusRegisterConfig == Drv::I2cStatus::I2C_OK){
    value = static_cast<F32>((((U16)dataTest[0]) << 8) | ((U16)dataTest[0]));
    printf("DEFAULT ACCEL CONFIG VALUE: %d \n", value);
  }

  if (statusAccelerate == Drv::I2cStatus::I2C_OK) {
    FW_ASSERT(IMU_MAX_DATA_SIZE >= 6);
    m_accel.setstatus(Svc::MeasurementStatus::OK);

    vector[0] = static_cast<F32>((((U16)data[0]) << 8) | ((U16)data[1]));
    vector[1] = static_cast<F32>((((U16)data[2]) << 8) | ((U16)data[3]));
    vector[2] = static_cast<F32>((((U16)data[4]) << 8) | ((U16)data[5]));

    //IMU CONVERSION
    m_accel.setvector(vector);
    m_accel.settime(this->getTime());

    this->tlmWrite_accelerometer(m_accel.getvector(), m_accel.gettime());
  } else {
    this->log_WARNING_HI_TelemetryError(statusAccelerate);
    m_accel.setstatus(Svc::MeasurementStatus::FAILURE);
  }
}

void Imu::updateGyro(){
  Fw::Buffer buffer;
  U8 data[IMU_MAX_DATA_SIZE];
  Vector vector;

  buffer.setData(data);
  buffer.setSize(IMU_MAX_DATA_SIZE);

  Drv::I2cStatus statusGyro = readRegisterBlock(IMU_RAW_GYRO_ADDR, buffer);

  Fw::Buffer test;
  U32 value;
  U8 dataTest[IMU_MAX_DATA_SIZE];
  test.setData(dataTest);
  test.setSize(IMU_MAX_DATA_SIZE);

  Drv::I2cStatus statusRegisterConfig = readRegisterBlock(0x28, buffer);
  if (statusRegisterConfig == Drv::I2cStatus::I2C_OK){
    value = static_cast<F32>((((U16)dataTest[0]) << 8) | ((U16)dataTest[0]));
    printf("DEFAULT ACCEL CONFIG VALUE: %d \n", value);
  }

  if (statusGyro == Drv::I2cStatus::I2C_OK) {
    FW_ASSERT(IMU_MAX_DATA_SIZE >= 6);
    m_gyro.setstatus(Svc::MeasurementStatus::OK);

    vector[0] =  static_cast<F32>((((U16)data[0]) << 8) | ((U16)data[1]));
    vector[1] = static_cast<F32>((((U16)data[2]) << 8) | ((U16)data[3]));
    vector[2] = static_cast<F32>((((U16)data[4]) << 8) | ((U16)data[5]));
    m_gyro.setvector(vector);
    m_gyro.settime(this->getTime());

    this->tlmWrite_gyroscope(m_gyro.getvector(), m_gyro.gettime());
  } else {
    this->log_WARNING_HI_TelemetryError(statusGyro);
    m_gyro.setstatus(Svc::MeasurementStatus::FAILURE);
  }
}

Drv::I2cStatus Imu ::setupReadRegister(U8 dev_addr, U8 reg) {
  Fw::Buffer buffer;
  U8 data[IMU_REG_SIZE];
  buffer.setData(data);
  buffer.setSize(IMU_REG_SIZE);

  FW_ASSERT(sizeof(data) > 0);
  data[0] = reg;

  return this->write_out(0, dev_addr, buffer);
}
} // end namespace Gnc
