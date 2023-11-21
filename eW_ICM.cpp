#include "eW_ICM.h"  // Include necessary header file(s)

unsigned long lastMillis = 0;  // To store the last time the counter was reset
unsigned long updateCounter = 0;  // Counter for the number of times ICMupdate is called
float sampleRate = 0;  // Variable to store the calculated sample rate

// Function to initialize ICM20948
void ICMinit() {
  //SERIAL_PORT.begin(115200);  // Initialize the serial port
    
  debugPrint("Initializing ICM20948");  // Print initialization message if in debug mode
  SPI_PORT.begin();  // Initialize the SPI port

  myICM.begin(CS_PIN, SPI_PORT);  // Initialize ICM20948

  // Set sample rates for internal accelerometer and gyroscope
  ICM_20948_smplrt_t smplrt;
  smplrt.a = 10;
  smplrt.g = 10;
  Serial.print("setAccSampleRate = "); Serial.println(myICM.setSampleRate(ICM_20948_Internal_Acc, smplrt));
  Serial.print("setGyrSampleRate = "); Serial.println(myICM.setSampleRate(ICM_20948_Internal_Gyr, smplrt));
  
  debugPrint(myICM.statusString());  // Print status message if in debug mode
  if (myICM.status != ICM_20948_Stat_Ok) {
    debugPrint("Trying again...");  // Print message if status is not ok
    delay(500);
  }

}

// Function to update IMU data
bool ICMupdate(ImuData* imuData) {
  debugPrint("ICM_updating...");  // Print updating message if in debug mode
  if (myICM.dataReady()) {
    myICM.getAGMT();  // Update AGMT data

    // Store accelerometer data in g
    imuData->aX = myICM.accX() / 1000;
    imuData->aY = myICM.accY() / 1000;
    imuData->aZ = myICM.accZ() / 1000;

    // Store gyroscope data in DPS
    imuData->gX = myICM.gyrX();
    imuData->gY = myICM.gyrY();
    imuData->gZ = myICM.gyrZ();

    // Store magnetometer data in uT
    imuData->mX = myICM.magX();
    imuData->mY = myICM.magY();
    imuData->mZ = myICM.magZ();
    
    debugPrint("IMU_Data fetched");
    updateCounter++;
    return true;  // Print fetched message if in debug mode
  } else {
    debugPrint("Waiting for data");  // Print waiting message if in debug mode
    return false;
  }
  //printScaledAGMT(&myICM);  // Call the function to print scaled AGMT data
}

void debugPrint(const char* message) {
  if (DEBUG_MODE) {
    SERIAL_PORT.println(message);
  }
}

// Function to print a formatted floating-point number
void printFormattedFloat(float val, uint8_t leading, uint8_t decimals) {
  float aval = abs(val);
  if (val < 0) {
    SERIAL_PORT.print("-");
  } else {
    SERIAL_PORT.print(" ");
  }
  for (uint8_t indi = 0; indi < leading; indi++) {
    uint32_t tenpow = 0;
    if (indi < (leading - 1)) {
      tenpow = 1;
    }
    for (uint8_t c = 0; c < (leading - 1 - indi); c++) {
      tenpow *= 10;
    }
    if (aval < tenpow) {
      SERIAL_PORT.print("0");
    } else {
      break;
    }
  }
  if (val < 0) {
    SERIAL_PORT.print(-val, decimals);
  } else {
    SERIAL_PORT.print(val, decimals);
  }
}

// Function to print scaled AGMT data
void printScaledAGMT(ICM_20948_SPI *sensor) {
  SERIAL_PORT.print("Scaled. Acc (g) [ ");
  printFormattedFloat(sensor->accX() / 1000, 5, 2);
  SERIAL_PORT.print(", ");
  printFormattedFloat(sensor->accY() / 1000, 5, 2);
  SERIAL_PORT.print(", ");
  printFormattedFloat(sensor->accZ() / 1000, 5, 2);
  SERIAL_PORT.print(" ], Gyr (DPS) [ ");
  printFormattedFloat(sensor->gyrX(), 5, 2);
  SERIAL_PORT.print(", ");
  printFormattedFloat(sensor->gyrY(), 5, 2);
  SERIAL_PORT.print(", ");
  printFormattedFloat(sensor->gyrZ(), 5, 2);
  SERIAL_PORT.print(" ], Mag (uT) [ ");
  printFormattedFloat(sensor->magX(), 5, 2);
  SERIAL_PORT.print(", ");
  printFormattedFloat(sensor->magY(), 5, 2);
  SERIAL_PORT.print(", ");
  printFormattedFloat(sensor->magZ(), 5, 2);
  SERIAL_PORT.println();
}


// Function to calculate the sample rate
void calculateSampleRate() {
  unsigned long currentMillis = millis();

  // Calculate the time elapsed since the last counter reset
  unsigned long elapsedTime = currentMillis - lastMillis;

  // Check if one minute has passed (60000 milliseconds)
  if (elapsedTime >= 10000) {
    // Calculate the sample rate in Hz
    sampleRate = static_cast<float>(updateCounter) / (elapsedTime / 1000.0);

    // Reset the counter and update the lastMillis
    updateCounter = 0;
    lastMillis = currentMillis;

    // Print or use the calculated sample rate as needed
    Serial.print("Sample Rate: ");
    Serial.print(sampleRate);
    Serial.println(" Hz");
  }
}