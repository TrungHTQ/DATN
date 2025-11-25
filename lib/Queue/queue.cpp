#include "queue.h"

// Global queue system array as specified in queue_sys.txt
QueueHandle_t queue_system[MAX_QUEUE_TASK] = {NULL};

// ===== IMPLEMENTATION =====
esp_err_t Sys_Queue_Init(QueueHandler_t handler, data_type_t type, UBaseType_t uxQueueLength)
{
    // Check if handler is within valid range
    if (handler >= MAX_QUEUE_HANDLER) {
        return ESP_FAIL;
    }

    switch (type) {
        case INDICATOR_DATA:
            queue_system[handler] = xQueueCreate(uxQueueLength, sizeof(indicator_command_t));
            break;
        case NORMAL_DATA:
            queue_system[handler] = xQueueCreate(uxQueueLength, sizeof(queue_command_t));
            break;
        case TIME_DATA:
            queue_system[handler] = xQueueCreate(uxQueueLength, sizeof(i2c_command_t));
            break;
        case SERVER_DATA:
            queue_system[handler] = xQueueCreate(uxQueueLength, sizeof(string_command_t));
            break;
        case PWM_DATA:
            queue_system[handler] = xQueueCreate(uxQueueLength, sizeof(pwm_command_t));
            break;
        case SYNC_DATA:
            queue_system[handler] = xQueueCreate(uxQueueLength, sizeof(sync_command_t));
            break;
        default:
            return ESP_FAIL;
    }

    if (queue_system[handler] == NULL) {
        return ESP_FAIL;
    }
    return ESP_OK;
}

esp_err_t Sys_Queue_Send(QueueHandler_t recv_handler, void *command, TickType_t xTickToWait)
{
    // Logging queue name for debugging (optional)
    Queue_Sys_Get_Name(recv_handler, GET_NAME_SEND, "Queue system");
    
    // Check if handler is within valid range
    if (recv_handler >= MAX_QUEUE_HANDLER) {
        return ESP_FAIL;
    }
    
    // Check if queue is initialized
    if (queue_system[recv_handler] == NULL) {
        return ESP_FAIL;
    }
    
    // Send message to queue
    if (xQueueSend(queue_system[recv_handler], command, xTickToWait) != pdTRUE) {
        return ESP_FAIL;
    }
    
    return ESP_OK;
}

esp_err_t Sys_Queue_Receive(QueueHandler_t handler, void *command, TickType_t xTickToWait)
{
    // Check if handler is within valid range
    if (handler >= MAX_QUEUE_HANDLER) {
        return ESP_FAIL;
    }
    
    // Check if queue is initialized
    if (queue_system[handler] == NULL) {
        return ESP_FAIL;
    }
    
    // Receive message from queue
    if (xQueueReceive(queue_system[handler], command, xTickToWait) != pdTRUE) {
        return ESP_FAIL;
    }
    
    return ESP_OK;
}

esp_err_t Sys_Queue_Reset(QueueHandler_t handler)
{
    // Check if handler is within valid range
    if (handler >= MAX_QUEUE_HANDLER) {
        return ESP_FAIL;
    }
    
    // Check if queue is initialized
    if (queue_system[handler] == NULL) {
        return ESP_FAIL;
    }
    
    return (xQueueReset(queue_system[handler]) == pdPASS) ? ESP_OK : ESP_FAIL;
}

const char *getQueueHandlerName(QueueHandler_t handler)
{
    switch (handler) {
        // Main system queues
        case MAIN_QUEUE_HANDLER:        return "MAIN_QUEUE_HANDLER";
        case TIMER_QUEUE_HANDLER:       return "TIMER_QUEUE_HANDLER";
        
        // Communication queues
        case MQTT_QUEUE_HANDLER:        return "MQTT_QUEUE_HANDLER";
        case WIFI_QUEUE_HANDLER:        return "WIFI_QUEUE_HANDLER";
        case COMM_TASK_QUEUE_HANDLER:   return "COMM_TASK_QUEUE_HANDLER";
        
        // Sensor queues
        case DHT20_QUEUE_HANDLER:       return "DHT20_QUEUE_HANDLER";
        case INA266_QUEUE_HANDLER:      return "INA266_QUEUE_HANDLER";
        case SENSOR_EC_QUEUE_HANDLER:   return "SENSOR_EC_QUEUE_HANDLER";
        case SENSOR_PH_QUEUE_HANDLER:   return "SENSOR_PH_QUEUE_HANDLER";
        case SENSOR_TASK_QUEUE_HANDLER: return "SENSOR_TASK_QUEUE_HANDLER";
        
        // Control and output queues
        case PWM_QUEUE_HANDLER:         return "PWM_QUEUE_HANDLER";
        case CONTROL_TASK_QUEUE_HANDLER:return "CONTROL_TASK_QUEUE_HANDLER";
        case INDICATOR_QUEUE_HANDLER:   return "INDICATOR_QUEUE_HANDLER";
        
        // I2C and system queues
        case I2C_QUEUE_HANDLER:         return "I2C_QUEUE_HANDLER";
        case SYNC_QUEUE_HANDLER:        return "SYNC_QUEUE_HANDLER";
        case LOGGER_QUEUE_HANDLER:      return "LOGGER_QUEUE_HANDLER";
        
        
        default: return "UNKNOWN_HANDLER";
    }
}

// ===== UTILITY FUNCTIONS =====
esp_err_t Queue_Sys_Get_Name(QueueHandler_t handler, get_queue_name_t state, const char *tag)
{
    if (state == GET_NAME_REVC) {
        // ESP_LOGW(tag, "Received command from %s", getQueueHandlerName(handler));
        return ESP_OK;
    } else {
        // ESP_LOGW(tag, "Send command to %s", getQueueHandlerName(handler));
        return ESP_OK;
    }
}

