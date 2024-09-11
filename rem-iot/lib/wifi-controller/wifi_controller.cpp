#include "wifi_controller.hpp"
#include "settings_manager.hpp"
#include "terminal.hpp"
#include "WiFi.h"
#include "esp_sntp.h"

WiFiController::WiFiController(WiFiClass *wifi, Terminal * terminal, SettingsManager * settingsManager) {
    this->wifi = wifi;
    this->terminal = terminal;
    this->settingsManager = settingsManager;    
}

void WiFiController::setupSNTP() {
    const char* time_zone = "CET-1CEST,M3.5.0,M10.5.0/3";  // TimeZone rule for Europe/Rome including daylight adjustment rules (optional)

    const char* ntpServer1 = "pool.ntp.org";
    const char* ntpServer2 = "time.nist.gov";
    const long  gmtOffset_sec = 3600;
    const int   daylightOffset_sec = 3600;

    // Setup timing config for NTP. This needs to be done before
    // DHCP address is given to the ESP32C3.
    sntp_servermode_dhcp(1);    // (optional)
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2);
}

bool WiFiController::verifyClockSync() {
    // Verify that we have synced the internal clock with an NTP server
    // Short circuit if alter 10 tries we are not synced.
    sntp_sync_status_t sntp_status = sntp_get_sync_status();
    int count = 0;
    while (sntp_status != SNTP_SYNC_STATUS_COMPLETED) {
        delay(500);
        sntp_status = sntp_get_sync_status();
        if (count > 10) {
            return false;
        }
    }

    return true; 
}

bool WiFiController::setupWiFi() {
    this->wifi->useStaticBuffers(true);
    bool success = this->wifi->mode(WIFI_STA);
    if (!success) {
        this->terminal->debugln("Failed to set wifi mode to WIFI_STA");
        return false;
    }

    // Grab the ssid and password from the settings manager, if they aren't there 
    // that means they haven't been stored yet.  
    const char * wifissid = this->settingsManager->getSetting(SSID_ID);
    if (wifissid == NULL) {
        this->terminal->debugln("Failed to get the SSID from the settings manager");
        return false;
    }

    const char * wifipass = this->settingsManager->getSetting(PASSWORD_ID);
    if (wifipass == NULL) {
        this->terminal->debugln("Failed to get the password from the settings manager");
        return false;
    }

    // Try and connect to the WiFi, to verify a connection happend we have to 
    // poll the WiFi::status().
    wl_status_t wifi_status = this->wifi->begin(wifissid, wifipass);
    int count = 1; 
    do {
        wifi_status = this->wifi->status();
        if (count % 5 == 0) {
            this->terminal->debugf("WiFi connection status: %s\r\n", wifi_status == WL_CONNECTED ? "connected" : "not connected");
        }
        count++;
        delay(100);
    } while (wifi_status != WL_CONNECTED);

    this->terminal->debugln("Connected to WiFi");
    return true;
}
