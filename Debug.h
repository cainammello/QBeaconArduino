#ifndef DEBUG
#define DEBUG

class Debug {

    public:

        static bool streaming;
        static String buffer;

        static boolean enabled;

        static void enable() {
            Debug::enabled = true;
        }

        static void disable() {
            Debug::enabled = false;
        }
    
        static void begin(int baudRate) {
            if(enabled) {
                Serial.begin(baudRate);
                delay(1000);
            }
        }

        static void i(String message) {
            char buff[message.length() + 1];
            message.toCharArray(buff, message.length() + 1);
            Serial.println(buff);
        }

        static void i(char * message) {
            if(enabled) {
                Serial.println(message);
            }
        }

        static void i(char c) {
            if(enabled) {
                Serial.println(c);
            }
        }

        static bool hasMessage() {
            if(enabled)
                return !streaming && buffer.length() > 0;
            return false;
        }

        static String getMessage() {
            return buffer;
        }

        static void loop() {
            if(enabled) {
                if(!streaming) {
                    buffer = "";
                }
                if(Serial.available()) {
                    char c = Serial.read();
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
        }

};

bool Debug::enabled = true;
bool Debug::streaming = false;
String Debug::buffer = "";

#endif // DEBUG
