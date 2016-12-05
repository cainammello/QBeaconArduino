#ifndef BLUETOOTH_LE
#define BLUETOOTH_LE

#include <SoftwareSerial.h>
#include "Debug.h"

class BluetoothLE {
    
    private:
        int RX, TX;
        SoftwareSerial * serial;        

        bool streaming;
        String buffer;
    
    public:
        BluetoothLE(int rx, int tx) {
            this->RX = rx;
            this->TX = tx;
            this->serial = new SoftwareSerial(rx, tx);
            
            this->streaming = false;
            this->buffer = "";
        }

        ~BluetoothLE() {
            delete serial;
        }

        void begin(int baudRate) {
            serial->begin(baudRate);
            delay(100);
        }

        void write(char c) {
            serial->write(c);
        }

        void write(char * c) {
            serial->write(c);
        }

        void write(String message) {
            char buff[message.length() + 1];
            message.toCharArray(buff, message.length() + 1);
            buff[message.length()] = '\0';
            serial->write(buff);
        }

        bool hasMessage() {
            return !streaming && buffer.length() > 0;
        }

        String getMessage() {
            return buffer;
        }

        void setUUID(int index, String uuid) {
            uuid.toUpperCase();
            write(String("AT+IBE") + String(index) + uuid);
            delay(100);
        }

        void loop() {
            if(!streaming) {
                buffer = "";
            }
            if(serial->available()) {
                char c = serial->read();
                if(c >= 32 && c <= 126) {
                    buffer += c;
                    streaming = true;
                } else {
                    streaming = false;
                }
            } else {
                streaming = false;
            }
        }
};

#endif // BLUETOOTH_LE
