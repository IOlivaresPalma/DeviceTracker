#include <Arduino.h>
#include "NimBLEDevice.h" // Incluir la biblioteca BLEDevice
#include "NimBLEScan.h"   // Incluir la biblioteca BLEScan
#include "NimBLEUtils.h"  // Incluir la biblioteca BLEUtils
#include <NimBLEBeacon.h>
/*
======================================================================================================================
Define de varibles estaticas y globales
======================================================================================================================

*/

#define LED_PIN 8        // Pin del LED integrado en la placa ESP32-C3
#define ESP_PWR_LVL_P9 9 // Potencia de transmisión máxima (9 dBm) para el ESP32-C3

#define BEACON_UUID "410a81fc-0a3c-45e4-90d5-6e4bb293a104"

enum class Modo : uint8_t { Reposo = 0, Tracker = 1 };
Modo modoActual = Modo::Reposo;

volatile uint32_t tiempo_blink = 1000; //variable que cambiara a medida que se acerca el telefono al beacon


void setup() {
    Serial.begin(115200); // Inicializar la comunicación serial a 115200 baudios
    pinMode(LED_PIN, OUTPUT);   // Configurar el pin del LED como salida
    digitalWrite(LED_PIN, LOW); // Asegurarse de que el LED este apagado al inicio


    const std::string deviceName = "Billetera"; // Nombre del contenedor tracker

    NimBLEDevice::init(deviceName); // Inicializar la pila BLE (obligatorio antes de usar NimBLE)
    //NimBLEDevice::setPower(ESP_PWR_LVL_P9); // Establecer la potencia de transmisión a 9 dBm

    NimBLEBeacon beacon;
    beacon.setManufacturerId(0x4c00); // Establecer el ID del fabricante (Apple)
    beacon.setProximityUUID(NimBLEUUID(BEACON_UUID)); // Establecer el UUID del beacon
    beacon.setMajor(1);
    beacon.setMinor(1);
    beacon.setSignalPower(-59);

    NimBLEAdvertisementData advData;
    advData.setFlags(BLE_HS_ADV_F_BREDR_UNSUP);
    advData.setManufacturerData(reinterpret_cast<const uint8_t*>(&beacon.getData()),
                                sizeof(NimBLEBeacon::BeaconData));

    NimBLEAdvertisementData scanResp;
    scanResp.setName("Billetera");

    NimBLEAdvertising* pAdv = NimBLEDevice::getAdvertising();
    pAdv->setAdvertisementData(advData);
    pAdv->setScanResponseData(scanResp);
    pAdv->enableScanResponse(true);
    pAdv->setConnectableMode(BLE_GAP_CONN_MODE_NON);
    pAdv->setMinInterval(160);
    pAdv->setMaxInterval(160);
    pAdv->start();

    Serial.println("iBeacon emitiendo");


    
}

void loop() {

    digitalWrite(LED_PIN, HIGH); // Encender el LED
    delay(tiempo_blink);         // Esperar el tiempo de parpadeo
    digitalWrite(LED_PIN, LOW);  // Apagar el LED
    delay(tiempo_blink);         // Esperar el tiempo de parpadeo
    Serial.println("LED parpadeando");

}