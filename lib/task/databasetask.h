#ifndef DATABASE_TASK_H
#define DATABASE_TASK_H

#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "Database.h"
#include "queue.h"

#define DATABASE_TASK_STACK_SIZE 4096
#define DATABASE_TASK_PRIORITY 2

class DatabaseTask {
private:
    static TaskHandle_t task_handle;
    static bool task_running;
    
    static void databaseTaskFunction(void* parameters);
    static void processDatabaseQueue();
    static void handleSensorData(const database_command_t& cmd);
    
public:
    static esp_err_t init();
    static esp_err_t start();
    static esp_err_t stop();
    static bool isRunning();
    
    // Helper functions to save sensor data
    static esp_err_t saveSensorReading(float temp, float hum, float ph, float ec, float current, float voltage, float power);
    static esp_err_t requestDataExport(QueueHandler_t requester);
    static esp_err_t requestDataClear(QueueHandler_t requester);
};

#endif