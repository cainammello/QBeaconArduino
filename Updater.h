#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

#include "Debug.h"

#define MAC { 0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED }
#define DEF_IP IPAddress(192, 168, 0, 177)
#define SERVER_IP IPAddress(192, 168, 0, 101)
#define PORT 1884
#define CURRENT_CLIENT_ID "BEACON-0"
#define CURRENT_CLIENT_TOPIC "UFC/QUIXADA/BEACON-0"




class Updater {

    EthernetClient ethClient;
    PubSubClient client;

    public:
        Updater(void (*callback)(char*, byte*, unsigned int)) {
            client.setClient(ethClient);
            client.setServer(SERVER_IP, PORT);
            client.setCallback(callback);
        }

        void begin(int baudRate) {
            byte mac[] = MAC;
            if(Ethernet.begin(mac) == 0) {
                Debug::i("Falha ao iniciar o modulo Ethernet! Tentando novamente com ip arbitrario...");
                Ethernet.begin(mac, DEF_IP);
            }
            
            delay(1500);  
        }

        void reconnect() {
            while (!client.connected()) {
                
                Debug::i("Tentando uma conexao MQTT...");
                
                if (client.connect(CURRENT_CLIENT_ID)) {
                    Debug::i("connectado!!!");
                    client.subscribe(CURRENT_CLIENT_TOPIC);
                } else {
                    Debug::i("Falha na conexao!");
                    Debug::i("Tentando novamente em 2s...");
                    delay(2000);
                }
            }
        }

        void loop() {
            if (!client.connected()) {
                reconnect();
            }
            client.loop();
        }
    
};

