#include "BluetoothSerial.h"
#include "DHT.h"

#define DHTPIN 23      
#define DHTTYPE DHT11 
#define LED_PIN_RED 13    
#define LED_PIN_YELLOW 14    
#define LED_PIN_GREEN 15

DHT dht(DHTPIN, DHTTYPE);
BluetoothSerial SerialBT;

String device_name = "ESP32-BT";
unsigned long lastDHTReadTime = 0;
const long dhtReadInterval = 2000; // 2 seconds

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

void setup() {
  Serial.begin(115200);
  SerialBT.begin(device_name.c_str()); 
  Serial.printf("The device with name \"%s\" is started.\nNow you can pair it with Bluetooth!\n", device_name.c_str());
  
  dht.begin();
  pinMode(LED_PIN_RED, OUTPUT);
  pinMode(LED_PIN_YELLOW, OUTPUT);
  pinMode(LED_PIN_GREEN, OUTPUT);
}

void loop() {
  char Incoming_value;
  
  // Non-blocking DHT reading
  unsigned long currentMillis = millis();
  if(currentMillis - lastDHTReadTime >= dhtReadInterval) {
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    if (!isnan(h) && !isnan(t)) {
      SerialBT.print("Temperature: ");
      SerialBT.print(t);
      SerialBT.println(" *C");
      SerialBT.print("Humidity: ");
      SerialBT.print(h);
      SerialBT.println(" %");
    } else {
      SerialBT.println("Failed to read from DHT sensor!");
    }

    lastDHTReadTime = currentMillis;
  }

  // Check for Bluetooth data
if (SerialBT.available()) {
    Incoming_value = SerialBT.read();

    //Print the received value
    Serial.print("Received: ");
    Serial.print(Incoming_value);

    // Filter newline and carriage return characters
    if (Incoming_value == '\n' || Incoming_value == '\r') {
        // just skip the rest and start over
    } else {
     switch(Incoming_value) {
      case '1':
        digitalWrite(LED_PIN_RED, HIGH);
        Serial.println("\n RED LED IS TURNED ON");
        break;
      case '2':
        digitalWrite(LED_PIN_YELLOW, HIGH);
        Serial.println("\n YELLOW LED IS TURNED ON");
        break;
      case '3':
        digitalWrite(LED_PIN_GREEN, HIGH);
        Serial.println("\n GREEN LED IS TURNED ON");
        break;
      case '4':
        digitalWrite(LED_PIN_RED, LOW);
        Serial.println("\n RED LED IS TURNED OFF");
        break;
      case '5':
        digitalWrite(LED_PIN_YELLOW, LOW);
        Serial.println("\n YELLOW LED IS TURNED OFF");
        break;
      case '6':
        digitalWrite(LED_PIN_GREEN, LOW);
        Serial.println("\n GREEN LED IS TURNED OFF");
        break;
      case '0':
        digitalWrite(LED_PIN_RED, LOW);
        digitalWrite(LED_PIN_YELLOW, LOW);
        digitalWrite(LED_PIN_GREEN, LOW);
        Serial.println("\n ALL LEDs ARE TURNED OFF");
        break;
      default:
        Serial.println("Received unrecognized command.");
        break;
    }
  } 
}
}
