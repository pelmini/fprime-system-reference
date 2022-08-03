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
    : ImuComponentBase(compName) {
  this->m_i2cDevAddress = I2C_DEV0_ADDR;

}

void Imu ::init(const NATIVE_INT_TYPE instance) {
  ImuComponentBase::init(instance);
}

Imu ::~Imu() {}

// ----------------------------------------------------------------------
// Handler implementations for user-defined typed input ports
// ----------------------------------------------------------------------

void Imu ::Run_handler(const NATIVE_INT_TYPE portNum,
                       NATIVE_UINT_TYPE context) {
    printf("WE ARE IN RUN HANDLER\n");
    updateAccel();
    updateGyro();
}

Gnc::ImuData Imu ::getAcceleration_handler(const NATIVE_INT_TYPE portNum) {
   ImuData accelData = m_accel;
   printf("WE ARE IN ACCEL HANDLER\n");
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
  printf("WE ARE IN READ REGISTER BLOCK\n");
  Drv::I2cStatus status;
  status = this->write(m_i2cDevAddress, registerAdd);

  printf("STATUS AFTER WRITE %d\n", status.e);

  if (status == Drv::I2cStatus::I2C_OK) {
    status = this->read(m_i2cDevAddress, buffer);
  }
  return status;
}

void Imu::updateAccel(){
  printf("WE ARE IN UPDATE\n");
  Fw::Buffer buffer;
  Fw::Time t_time;
  U8 data[IMU_MAX_DATA_SIZE];
  Gnc::Vector vector;

  buffer.setData(data);
  buffer.setSize(IMU_MAX_DATA_SIZE);
  t_time = this->getTime();

  Drv::I2cStatus statusAccelerate = readRegisterBlock(IMU_RAW_ACCEL_ADDR, buffer);
  printf("BACK IN UPDATE\n");
  printf("STATUS-ACCEL AFTER READ %d\n", statusAccelerate.e);

  if (statusAccelerate == Drv::I2cStatus::I2C_OK) {
    FW_ASSERT(IMU_MAX_DATA_SIZE >= 6);
    printf("IN ACCEL IF, I2CSTATUS -OK \n");
    m_accel.setstatus(Svc::MeasurementStatus::OK);

    printf("DATA 1: %d\n", data[0]);
    printf("DATA 2: %d\n", data[1]);
    printf("DATA 3: %d\n", data[2]);
    vector[0] = static_cast<F32>((((U16)data[0]) << 8) | ((U16)data[1]));
    vector[1] = static_cast<F32>((((U16)data[2]) << 8) | ((U16)data[3]));
    vector[2] = static_cast<F32>((((U16)data[4]) << 8) | ((U16)data[5]));
    m_accel.setvector(vector);

    m_accel.settime(this->getTime());
    this->tlmWrite_accelerometer(m_accel.getvector());
  } else {
    this->log_WARNING_HI_TelemetryError(statusAccelerate);
    m_accel.setstatus(Svc::MeasurementStatus::FAILURE);
  }
  printf("TIME ACCEL %d %d %d \n", static_cast<U32>(t_time.getTimeBase()), t_time.getSeconds(), t_time.getUSeconds());
  printf("VECTOR-ACCEL %f, %f, %f\n", vector[0], vector[1], vector[2]);
}

//OK = 0, //!< Measurement was good
//FAILURE = 1, //!< Failure to retrieve measurement
//STALE = 2, //!< Measurement is stale


//I2C_OK = 0, //!< Transaction okay
//    I2C_ADDRESS_ERR = 1, //!< I2C address invalid
//    I2C_WRITE_ERR = 2, //!< I2C write failed
//    I2C_READ_ERR = 3, //!< I2C read failed
//    I2C_OTHER_ERR = 4, //!< Other errors that don't fit
void Imu::updateGyro(){
  Fw::Buffer buffer;
  U8 data[IMU_MAX_DATA_SIZE];
  Vector vector;

  buffer.setData(data);
  buffer.setSize(IMU_MAX_DATA_SIZE);

  Drv::I2cStatus statusGyro = readRegisterBlock(IMU_RAW_GYRO_ADDR, buffer);

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
//  printf("VECTOR-GYRO %f, %f, %f", vector[0], vector[1], vector[2]);
}

Drv::I2cStatus Imu ::write(U8 dev_addr, U8 reg) {
  Fw::Buffer buffer;
  U8 data[IMU_REG_SIZE];
  buffer.setData(data);
  buffer.setSize(IMU_REG_SIZE);

  FW_ASSERT(sizeof(data) > 0);
  data[0] = reg;

  return this->write_out(0, dev_addr, buffer);
}
} // end namespace Gnc
