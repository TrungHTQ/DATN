#include "queuehandler.h"

// ===== QUEUE HANDLER IMPLEMENTATION =====
QueueHandler::QueueHandler(QueueHandler_t handler, data_type_t type, UBaseType_t length) 
    : handlerType(handler), dataType(type), queueLength(length), initialized(false) {
}

QueueHandler::~QueueHandler() {
    // Destructor - queue cleanup is handled by FreeRTOS
}

esp_err_t QueueHandler::init() {
    esp_err_t result = Sys_Queue_Init(handlerType, dataType, queueLength);
    if (result == ESP_OK) {
        initialized = true;
    }
    return result;
}

bool QueueHandler::isInitialized() const {
    return initialized;
}

QueueHandler_t QueueHandler::getHandlerType() const {
    return handlerType;
}

const char* QueueHandler::getHandlerName() const {
    return getQueueHandlerName(handlerType);
}

// Send message methods
esp_err_t QueueHandler::sendMessage(queue_command_t* msg, TickType_t timeout) {
    if (!initialized || msg == nullptr) return ESP_FAIL;
    return Sys_Queue_Send(handlerType, (void*)msg, timeout);
}


esp_err_t QueueHandler::sendMessage(string_command_t* msg, TickType_t timeout) {
    if (!initialized || msg == nullptr) return ESP_FAIL;
    return Sys_Queue_Send(handlerType, (void*)msg, timeout);
}

esp_err_t QueueHandler::sendMessage(pwm_command_t* msg, TickType_t timeout) {
    if (!initialized || msg == nullptr) return ESP_FAIL;
    return Sys_Queue_Send(handlerType, (void*)msg, timeout);
}

esp_err_t QueueHandler::sendMessage(indicator_command_t* msg, TickType_t timeout) {
    if (!initialized || msg == nullptr) return ESP_FAIL;
    return Sys_Queue_Send(handlerType, (void*)msg, timeout);
}

esp_err_t QueueHandler::sendMessage(sync_command_t* msg, TickType_t timeout) {
    if (!initialized || msg == nullptr) return ESP_FAIL;
    return Sys_Queue_Send(handlerType, (void*)msg, timeout);
}

esp_err_t QueueHandler::sendMessage(i2c_command_t* msg, TickType_t timeout) {
    if (!initialized || msg == nullptr) return ESP_FAIL;
    return Sys_Queue_Send(handlerType, (void*)msg, timeout);
}

// Receive message methods
esp_err_t QueueHandler::receiveMessage(queue_command_t* msg, TickType_t timeout) {
    if (!initialized || msg == nullptr) return ESP_FAIL;
    return Sys_Queue_Receive(handlerType, (void*)msg, timeout);
}


esp_err_t QueueHandler::receiveMessage(string_command_t* msg, TickType_t timeout) {
    if (!initialized || msg == nullptr) return ESP_FAIL;
    return Sys_Queue_Receive(handlerType, (void*)msg, timeout);
}

esp_err_t QueueHandler::receiveMessage(pwm_command_t* msg, TickType_t timeout) {
    if (!initialized || msg == nullptr) return ESP_FAIL;
    return Sys_Queue_Receive(handlerType, (void*)msg, timeout);
}

esp_err_t QueueHandler::receiveMessage(indicator_command_t* msg, TickType_t timeout) {
    if (!initialized || msg == nullptr) return ESP_FAIL;
    return Sys_Queue_Receive(handlerType, (void*)msg, timeout);
}

esp_err_t QueueHandler::receiveMessage(sync_command_t* msg, TickType_t timeout) {
    if (!initialized || msg == nullptr) return ESP_FAIL;
    return Sys_Queue_Receive(handlerType, (void*)msg, timeout);
}

esp_err_t QueueHandler::receiveMessage(i2c_command_t* msg, TickType_t timeout) {
    if (!initialized || msg == nullptr) return ESP_FAIL;
    return Sys_Queue_Receive(handlerType, (void*)msg, timeout);
}

// Queue management methods
esp_err_t QueueHandler::reset() {
    if (!initialized) return ESP_FAIL;
    return Sys_Queue_Reset(handlerType);
}

UBaseType_t QueueHandler::getWaitingMessages() {
    if (!initialized || queue_system[handlerType] == NULL) return 0;
    return uxQueueMessagesWaiting(queue_system[handlerType]);
}

UBaseType_t QueueHandler::getAvailableSpaces() {
    if (!initialized || queue_system[handlerType] == NULL) return 0;
    return uxQueueSpacesAvailable(queue_system[handlerType]);
}

// ===== SPECIALIZED SENSOR QUEUE HANDLER =====
SensorQueueHandler::SensorQueueHandler(QueueHandler_t handler, data_type_t type, UBaseType_t length) 
    : QueueHandler(handler, type, length) {
}

esp_err_t SensorQueueHandler::requestSensorReading(event_type_t event) {
    if (!isInitialized()) return ESP_FAIL;
    
    queue_command_t cmd = {
        .event_type = event,
        .queue_of_sender_task = getHandlerType()
    };
    return sendMessage(&cmd);
}

// ===== COMMUNICATION QUEUE HANDLER =====


esp_err_t CommunicationQueueHandler::sendDataPacket(const char* data, QueueHandler_t sender) {
    if (!isInitialized() || data == nullptr) return ESP_FAIL;
    
    string_command_t cmd = {
        .event_type = MQTT_PUBLISH_REQUEST,
        .data = {0},
        .queue_of_sender_task = sender
    };
    
    strncpy(cmd.data, data, sizeof(cmd.data) - 1);
    return sendMessage(&cmd);
}

// ===== CONTROL QUEUE HANDLER =====
ControlQueueHandler::ControlQueueHandler(QueueHandler_t handler, data_type_t type, UBaseType_t length)
    : QueueHandler(handler, type, length) {
}


// ===== QUEUE MANAGER IMPLEMENTATION =====
QueueManager* QueueManager::instance = nullptr;

QueueManager::QueueManager() : systemInitialized(false) {
    for (int i = 0; i < MAX_QUEUE_HANDLER; i++) {
        handlers[i] = nullptr;
    }
}

QueueManager* QueueManager::getInstance() {
    if (instance == nullptr) {
        instance = new QueueManager();
    }
    return instance;
}

esp_err_t QueueManager::initializeSystem() {
    if (systemInitialized) {
        return ESP_OK;
    }
    
    // Initialize critical system queues
    esp_err_t result = ESP_OK;
    
    // Initialize main queue
    if (registerHandler(MAIN_QUEUE_HANDLER, NORMAL_DATA, 10) != ESP_OK) {
        result = ESP_FAIL;
    }
    
    systemInitialized = (result == ESP_OK);
    return result;
}

bool QueueManager::isSystemInitialized() const {
    return systemInitialized;
}

esp_err_t QueueManager::registerHandler(QueueHandler_t handlerType, data_type_t dataType, UBaseType_t queueLength) {
    if (handlerType >= MAX_QUEUE_HANDLER) {
        return ESP_FAIL;
    }
    
    if (handlers[handlerType] != nullptr) {
        // Handler already registered
        return ESP_OK;
    }
    
    // Create appropriate handler type
    QueueHandler* handler = nullptr;
    
    switch (handlerType) {
        case DHT20_QUEUE_HANDLER:
        case SENSOR_EC_QUEUE_HANDLER:
        case SENSOR_PH_QUEUE_HANDLER:
        case INA266_QUEUE_HANDLER:
            handler = new SensorQueueHandler(handlerType, dataType, queueLength);
            break;
            
        case WIFI_QUEUE_HANDLER:
        case MQTT_QUEUE_HANDLER:
        case COMM_TASK_QUEUE_HANDLER:
            handler = new CommunicationQueueHandler(handlerType, dataType, queueLength);
            break;
            
        case CONTROL_TASK_QUEUE_HANDLER:
            handler = new ControlQueueHandler(handlerType, dataType, queueLength);
            break;
            
        default:
            handler = new QueueHandler(handlerType, dataType, queueLength);
            break;
    }
    
    if (handler == nullptr) {
        return ESP_FAIL;
    }
    
    esp_err_t result = handler->init();
    if (result == ESP_OK) {
        handlers[handlerType] = handler;
    } else {
        delete handler;
    }
    
    return result;
}

QueueHandler* QueueManager::getHandler(QueueHandler_t handlerType) {
    if (handlerType >= MAX_QUEUE_HANDLER) {
        return nullptr;
    }
    return handlers[handlerType];
}

esp_err_t QueueManager::resetAllQueues() {
    esp_err_t result = ESP_OK;
    
    for (int i = 0; i < MAX_QUEUE_HANDLER; i++) {
        if (handlers[i] != nullptr) {
            if (handlers[i]->reset() != ESP_OK) {
                result = ESP_FAIL;
            }
        }
    }
    
    return result;
}

void QueueManager::printSystemStatus() {
    Serial.println("=== Queue System Status ===");
    Serial.printf("System Initialized: %s\n", systemInitialized ? "YES" : "NO");
    
    for (int i = 0; i < MAX_QUEUE_HANDLER; i++) {
        if (handlers[i] != nullptr) {
            Serial.printf("Handler %d (%s): Initialized=%s, Waiting=%d, Available=%d\n",
                i, handlers[i]->getHandlerName(),
                handlers[i]->isInitialized() ? "YES" : "NO",
                handlers[i]->getWaitingMessages(),
                handlers[i]->getAvailableSpaces());
        }
    }
    Serial.println("===========================");
}

QueueManager::~QueueManager() {
    for (int i = 0; i < MAX_QUEUE_HANDLER; i++) {
        if (handlers[i] != nullptr) {
            delete handlers[i];
        }
    }
}
