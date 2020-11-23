#include "IOTboard.h"

static void heartbeat(void);

// MicroOLED Object Declaration
static MicroOLED oled(MODE_I2C, D8, 1);

// Particle soft timer API
// Create heartbeat heartbeat_tmr
Timer heartbeat_tmr(BOARD_HEARTBEAT_MS, heartbeat);

// Sensors
TMP102 temp_sensor;

static void heartbeat(void)
{
    static bool ledState = true;

    if (ledState)
    {
        digitalWrite(BOARD_SYSTEM_LED, LOW);

        ledState = false;
    }
    else
    {
        digitalWrite(BOARD_SYSTEM_LED, HIGH);

        ledState = true;
    }
}

// Class constructor
IOTboard::IOTboard(bool wait_for_serial)
{
    wait_serial_monitor = wait_for_serial;

    memset(iot_ip_addr, 0, sizeof(iot_ip_addr));

    memset(serial_out_buffer, 0, sizeof(serial_out_buffer));
}

void IOTboard::start()
{
    // Setup outputs
    pinMode(BOARD_SYSTEM_LED, OUTPUT);
    digitalWrite(BOARD_SYSTEM_LED, HIGH);

    // Start OLED display
    oled.begin();         // Initialize the OLED
    oled.setFontType(1);  // Set font to type 0
    oled.setCursor(0, 0); // Set cursor to top-left
    oled.clear(PAGE);     // Clear the screen

    delay(100);

    // Start TM102 Sensor
    temp_sensor.begin();

    delay(100);

    // set the Conversion Rate (how quickly the sensor gets a new reading)
    //0-3: 0:0.25Hz, 1:1Hz, 2:4Hz, 3:8Hz
    temp_sensor.setConversionRate(2);

    //set Extended Mode.
    //0:12-bit Temperature(-55C to +128C) 1:13-bit Temperature(-55C to +150C)
    temp_sensor.setExtendedMode(0);

    // start heartbeat timer
    heartbeat_tmr.start();

    // Start serial port
    Serial.begin(115200); // open serial over USB

    if (wait_serial_monitor)
    {
        // Wait until PC is connected to the serial port
        while (!Serial.isConnected())
        {
            Particle.process();
        }
    }
}

void IOTboard::printToDisplay(const char *text)
{

    oled.setCursor(0, 0); // Set cursor to top-left
    oled.clear(PAGE);     // Clear the screen
    oled.print(text);

    oled.display();

    delayMicroseconds(100);
}

float IOTboard::getTempF()
{
    // The core of your code will likely live here.
    float temperature;

    // Turn sensor on to start temperature measurement.
    // Current consumtion typically ~10uA.
    temp_sensor.wakeup();

    // read temperature data
    temperature = temp_sensor.readTempF();
    //temperature = sensor.readTempC();

    // Place sensor in sleep mode to save power.
    // Current consumtion typically <0.5uA.
    temp_sensor.sleep();

    delayMicroseconds(100);

    return (temperature);
}

float IOTboard::getTempC()
{
    // The core of your code will likely live here.
    float temperature;

    // Turn sensor on to start temperature measurement.
    // Current consumtion typically ~10uA.
    temp_sensor.wakeup();

    // read temperature data
    temperature = temp_sensor.readTempC();
    //temperature = sensor.readTempC();

    // Place sensor in sleep mode to save power.
    // Current consumtion typically <0.5uA.
    temp_sensor.sleep();

    delayMicroseconds(100);

    return (temperature);
}

void IOTboard::printToSerialOut(const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);

    memset(serial_out_buffer, 0, sizeof(serial_out_buffer));

    vsnprintf(serial_out_buffer, sizeof(serial_out_buffer), fmt, args);

    va_end(args);

    Serial.printlnf(serial_out_buffer);
}

char IOTboard::getSerialInput()
{
    input_cmd = 0;

    if (Serial.available() > 0)
    {
        Serial.readBytes(&input_cmd, 1);
    }

    return (input_cmd);
}

void IOTboard::connectToWiFi(const char *SSID,
                             const char *user_name,
                             const char *password)
{
    // Ensure that WiFi module is on
    WiFi.on();

    WiFiCredentials credentials(SSID, WPA2_ENTERPRISE);

    credentials.setEapType(WLAN_EAP_TYPE_PEAP);

    credentials.setIdentity(user_name);

    credentials.setPassword(password)
        .setCipher(WLAN_CIPHER_AES);

    WiFi.setCredentials(credentials);

    // Connect if settings were successfully saved
    WiFi.connect();

    delay(1000);
}

void IOTboard::connectToWiFi(const char *SSID,
                             const char *password)
{
    // Ensure that WiFi module is on
    WiFi.on();

    // Set up WPA2 access point "My AP" with password "mypassword" and AES cipher
    WiFiCredentials credentials(SSID, WPA2);

    credentials.setPassword(password)
        .setCipher(WLAN_CIPHER_AES);

    WiFi.setCredentials(credentials);

    // Connect if settings were successfully saved
    WiFi.connect();

    delay(1000);
}

void IOTboard::disconnectWiFi(void)
{
    WiFi.clearCredentials();

    WiFi.off();

    delay(1000);
}

bool IOTboard::isWiFiReady(void)
{
    bool rc;

    rc = WiFi.ready();

    return rc;
}

const char *IOTboard::getWiFiIP()
{
    static IPAddress ip;

    ip = WiFi.localIP();

    snprintf(iot_ip_addr, sizeof(iot_ip_addr), "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);

    return ((const char *)iot_ip_addr);
}
