#ifndef QUEUE_HANDLER_H
#define QUEUE_HANDLER_H

#include "queue.h"

// ===== QUEUE HANDLER CLASS =====
class QueueHandler {
private:
    QueueHandler_t handlerType;
    data_type_t dataType;
    UBaseType_t queueLength;
    bool initialized;

public:
    // Constructor
    QueueHandler(QueueHandler_t handler, data_type_t type, UBaseType_t length);
    
    // Destructor
    ~QueueHandler();
    
    // Initialization
    esp_err_t init();
    
    // Check if initialized
    bool isInitialized() const;
    
    // Get handler type
    QueueHandler_t getHandlerType() const;
    
    // Get handler name
    const char* getHandlerName() const;
    
    // Send methods for different message types
    esp_err_t sendMessage(queue_command_t* msg, TickType_t timeout = pdMS_TO_TICKS(100));
    esp_err_t sendMessage(string_command_t* msg, TickType_t timeout = pdMS_TO_TICKS(100));
    esp_err_t sendMessage(pwm_command_t* msg, TickType_t timeout = pdMS_TO_TICKS(100));
    esp_err_t sendMessage(indicator_command_t* msg, TickType_t timeout = pdMS_TO_TICKS(100));
    esp_err_t sendMessage(sync_command_t* msg, TickType_t timeout = pdMS_TO_TICKS(100));
    esp_err_t sendMessage(i2c_command_t* msg, TickType_t timeout = pdMS_TO_TICKS(100));
    
    // Receive methods for different message types
    esp_err_t receiveMessage(queue_command_t* msg, TickType_t timeout = portMAX_DELAY);
    esp_err_t receiveMessage(string_command_t* msg, TickType_t timeout = portMAX_DELAY);
    esp_err_t receiveMessage(pwm_command_t* msg, TickType_t timeout = portMAX_DELAY);
    esp_err_t receiveMessage(indicator_command_t* msg, TickType_t timeout = portMAX_DELAY);
    esp_err_t receiveMessage(sync_command_t* msg, TickType_t timeout = portMAX_DELAY);
    esp_err_t receiveMessage(i2c_command_t* msg, TickType_t timeout = portMAX_DELAY);
    
    // Queue management
    esp_err_t reset();
    UBaseType_t getWaitingMessages();
    UBaseType_t getAvailableSpaces();
};

// ===== SPECIALIZED QUEUE HANDLERS =====
class SensorQueueHandler : public QueueHandler {
public:
    SensorQueueHandler(QueueHandler_t handler, data_type_t type, UBaseType_t length);
    
    // Specialized sensor methods
    esp_err_t sendSensorReading(event_type_t event, float value, uint32_t timestamp, uint8_t errorCode = 0);
    esp_err_t requestSensorReading(event_type_t event);
};

class CommunicationQueueHandler : public QueueHandler {
public:
    CommunicationQueueHandler(QueueHandler_t handler, data_type_t type, UBaseType_t length);
    
    // Specialized communication methods
    esp_err_t sendConnectionStatus(event_type_t event, uint8_t status, const char* info = nullptr);
    esp_err_t sendDataPacket(const char* data, QueueHandler_t sender);
};

class ControlQueueHandler : public QueueHandler {
public:
    ControlQueueHandler(QueueHandler_t handler, data_type_t type, UBaseType_t length);
    
    // Specialized control methods
    esp_err_t sendControlCommand(uint8_t deviceId, uint8_t action, float setValue, uint32_t duration = 0);
    esp_err_t sendSafetyAlert(uint8_t deviceId, const char* alertMessage);
};

// ===== QUEUE MANAGER =====
class QueueManager {
private:
    static QueueManager* instance;
    QueueHandler* handlers[MAX_QUEUE_HANDLER];
    bool systemInitialized;
    
    // Private constructor for singleton
    QueueManager();

public:
    // Singleton access
    static QueueManager* getInstance();
    
    // System management
    esp_err_t initializeSystem();
    bool isSystemInitialized() const;
    
    // Handler management
    esp_err_t registerHandler(QueueHandler_t handlerType, data_type_t dataType, UBaseType_t queueLength);
    QueueHandler* getHandler(QueueHandler_t handlerType);
    
    // System-wide operations
    esp_err_t resetAllQueues();
    void printSystemStatus();
    
    // Destructor
    ~QueueManager();
};

#endif // QUEUE_HANDLER_H
