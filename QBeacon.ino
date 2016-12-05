#include "Updater.h"
#include "BluetoothLE.h"
#include "Debug.h"

#define DEF_BAUD_RATE 9600

void callback(char*, byte*, unsigned int);

BluetoothLE ble(5, 6); // RX, TX
Updater updater(callback);

void setup() {
    Debug::disable();
    Debug::begin(DEF_BAUD_RATE);
    Debug::i("Serial iniciado!");

    Debug::i("Iniciando Bluetooth...");
    ble.begin(DEF_BAUD_RATE);
    Debug::i("Bluetooth iniciado!");

    Debug::i("Iniciando Ethernet...");
    updater.begin(DEF_BAUD_RATE);
    Debug::i("Ethernet iniciado!");
}

void loop() {
    
    Debug::loop();

    if(Debug::hasMessage()) {
        Debug::i(Debug::getMessage());
        ble.write(Debug::getMessage());
    }


    ble.loop();

    if(ble.hasMessage()) {
        Debug::i(ble.getMessage());
    }

    updater.loop();

    delay(10);
    
}

void callback(char* topic, byte* payload, unsigned int length) {
    String message = "";
    for (int i = 0; i < length; i++) {
        message += (char)payload[i];
    }
    
    Debug::i(String("Mensagem recebida [") + topic + "]: " + message);
    
    ble.setUUID(0, message.substring(0, 8));
    ble.setUUID(1, message.substring(8, 16));
    ble.setUUID(2, message.substring(16, 24));
    ble.setUUID(3, message.substring(24, 32));
}
