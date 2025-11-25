#ifndef SYS_QUEUE_H
#define SYS_QUEUE_H

#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "esp_err.h"
#include <time.h>

// ===== CONFIG =====
#define MAX_QUEUE_TASK 30        // Số lượng queue tối đa theo spec
#define MAX_LENGTH   200         // Độ dài tối đa cho string data theo spec

// ===== ENUM DEFINITIONS =====
typedef enum {
    // Main system queues
    MAIN_QUEUE_HANDLER = 0,
    TIMER_QUEUE_HANDLER,
    
    // Communication queues
    MQTT_QUEUE_HANDLER,
    WIFI_QUEUE_HANDLER,
    COMM_TASK_QUEUE_HANDLER,
    
    // Sensor queues
    DHT20_QUEUE_HANDLER,
    INA266_QUEUE_HANDLER,
    SENSOR_EC_QUEUE_HANDLER,
    SENSOR_PH_QUEUE_HANDLER,
    SENSOR_TASK_QUEUE_HANDLER,
    
    // Control and output queues
    PWM_QUEUE_HANDLER,
    CONTROL_TASK_QUEUE_HANDLER,
    INDICATOR_QUEUE_HANDLER,
    
    // I2C and system queues
    I2C_QUEUE_HANDLER,
    SYNC_QUEUE_HANDLER,
    LOGGER_QUEUE_HANDLER,
    DATABASE_QUEUE_HANDLER,
    
    MAX_QUEUE_HANDLER
} QueueHandler_t;

typedef enum {
    NORMAL_DATA,
    TIME_DATA,
    SERVER_DATA,
    PWM_DATA,
    INDICATOR_DATA,
    SYNC_DATA,
    DATABASE_DATA
} data_type_t;

// ===== ADDITIONAL ENUMS =====
typedef enum {
    GET_NAME_SEND = 0,
    GET_NAME_REVC
} get_queue_name_t;

typedef enum {
    // System events
    EVENT_NONE = 0,
    EVENT_START,
    EVENT_STOP,
    EVENT_INIT,
    EVENT_RESET,
    EVENT_ERROR,
    EVENT_WARNING,
    EVENT_INFO,
    EVENT_DEBUG,
    EVENT_HEARTBEAT,
    
    // Sensor events - DHT20 (Temperature & Humidity)
    DHT20_READ_REQUEST,
    DHT20_READ_SUCCESS,
    DHT20_READ_ERROR,
    DHT20_TEMP_UPDATE,
    DHT20_HUMIDITY_UPDATE,
    DHT20_CALIBRATE,
    DHT20_INIT_SUCCESS,
    DHT20_INIT_ERROR,
    
    // Sensor events - INA266 (Current & Power)
    INA266_READ_REQUEST,
    INA266_READ_SUCCESS,
    INA266_READ_ERROR,
    INA266_CURRENT_UPDATE,
    INA266_POWER_UPDATE,
    INA266_VOLTAGE_UPDATE,
    INA266_CALIBRATE,
    INA266_OVERCURRENT_ALERT,
    INA266_INIT_SUCCESS,
    INA266_INIT_ERROR,
    
    // Sensor events - EC (Electrical Conductivity)
    SENSOR_EC_READ_REQUEST,
    SENSOR_EC_READ_SUCCESS,
    SENSOR_EC_READ_ERROR,
    SENSOR_EC_VALUE_UPDATE,
    SENSOR_EC_CALIBRATE_LOW,
    SENSOR_EC_CALIBRATE_HIGH,
    SENSOR_EC_CALIBRATE_SUCCESS,
    SENSOR_EC_CALIBRATE_ERROR,
    SENSOR_EC_INIT_SUCCESS,
    SENSOR_EC_INIT_ERROR,
    
    // Sensor events - PH
    SENSOR_PH_READ_REQUEST,
    SENSOR_PH_READ_SUCCESS,
    SENSOR_PH_READ_ERROR,
    SENSOR_PH_VALUE_UPDATE,
    SENSOR_PH_CALIBRATE_4,
    SENSOR_PH_CALIBRATE_7,
    SENSOR_PH_CALIBRATE_10,
    SENSOR_PH_CALIBRATE_SUCCESS,
    SENSOR_PH_CALIBRATE_ERROR,
    SENSOR_PH_INIT_SUCCESS,
    SENSOR_PH_INIT_ERROR,
    
    // Communication events - WiFi
    WIFI_CONNECT_REQUEST,
    WIFI_CONNECT_SUCCESS,
    WIFI_CONNECT_ERROR,
    WIFI_DISCONNECT,
    WIFI_RECONNECT,
    WIFI_SCAN_REQUEST,
    WIFI_SCAN_COMPLETE,
    WIFI_AP_MODE_START,
    WIFI_AP_MODE_STOP,
    WIFI_STATUS_REQUEST,
    
    // Communication events - MQTT
    MQTT_CONNECT_REQUEST,
    MQTT_CONNECT_SUCCESS,
    MQTT_CONNECT_ERROR,
    MQTT_DISCONNECT,
    MQTT_PUBLISH_REQUEST,
    MQTT_PUBLISH_SUCCESS,
    MQTT_PUBLISH_ERROR,
    MQTT_SUBSCRIBE_REQUEST,
    MQTT_SUBSCRIBE_SUCCESS,
    MQTT_MESSAGE_RECEIVED,
    MQTT_STATUS_REQUEST,
    
    // PWM and Control events
    PWM_START_EVENT,
    PWM_STOP_EVENT,
    PWM_SET_DUTY_CYCLE,
    PWM_SET_FREQUENCY,
    PWM_UPDATE_SUCCESS,
    PWM_UPDATE_ERROR,
    
    // Control task events
    CONTROL_AUTO_MODE,
    CONTROL_MANUAL_MODE,
    CONTROL_PUMP_ON,
    CONTROL_PUMP_OFF,
    CONTROL_VALVE_OPEN,
    CONTROL_VALVE_CLOSE,
    CONTROL_HEATER_ON,
    CONTROL_HEATER_OFF,
    CONTROL_FAN_ON,
    CONTROL_FAN_OFF,
    CONTROL_EMERGENCY_STOP,
    CONTROL_SAFETY_CHECK,
    
    // I2C events
    I2C_READ_EVENT,
    I2C_WRITE_EVENT,
    I2C_SCAN_REQUEST,
    I2C_SCAN_COMPLETE,
    I2C_DEVICE_FOUND,
    I2C_DEVICE_ERROR,
    I2C_BUS_ERROR,
    I2C_TIMEOUT_ERROR,
    
    // Timer events
    TIMER_EXPIRED,
    TIMER_START,
    TIMER_STOP,
    TIMER_RESET,
    TIMER_UPDATE_INTERVAL,
    
    // Sync and coordination events
    SYNC_REQUEST_EVENT,
    SYNC_RESPONSE_EVENT,
    SYNC_COMPLETE,
    SYNC_TIMEOUT,
    TASK_SYNC_REQUEST,
    TASK_SYNC_READY,
    
    // Data logging events
    LOG_DATA_REQUEST,
    LOG_DATA_SUCCESS,
    LOG_DATA_ERROR,
    LOG_CLEAR_REQUEST,
    LOG_EXPORT_REQUEST,
    
    // Database events
    DB_INIT_REQUEST,
    DB_INIT_SUCCESS,
    DB_INIT_ERROR,
    DB_SAVE_READING,
    DB_SAVE_SUCCESS,
    DB_SAVE_ERROR,
    DB_GET_LATEST,
    DB_GET_RANGE,
    DB_DELETE_OLD,
    DB_EXPORT_DATA,
    DB_CLEAR_ALL,
    DB_GET_COUNT,
    DB_BACKUP_REQUEST,
    DB_RESTORE_REQUEST,
    
    // Indicator and display events
    INDICATOR_ON,
    INDICATOR_OFF,
    INDICATOR_BLINK,
    INDICATOR_STATUS_UPDATE,
    DISPLAY_UPDATE_REQUEST,
    DISPLAY_CLEAR,
    
    // Configuration events
    CONFIG_SAVE_REQUEST,
    CONFIG_LOAD_REQUEST,
    CONFIG_RESET_REQUEST,
    CONFIG_UPDATE_SUCCESS,
    CONFIG_UPDATE_ERROR,
    
    EVENT_MAX
} event_type_t;

// ===== MESSAGE STRUCTURES =====
typedef struct {
    event_type_t event_type;
    QueueHandler_t queue_of_sender_task;
} queue_command_t;

typedef struct {
    event_type_t event_type;
    struct tm data;
    QueueHandler_t queue_of_sender_task;
    uint16_t result_mm;
} i2c_command_t;

typedef struct {
    event_type_t event_type;
    char data[MAX_LENGTH];
    QueueHandler_t queue_of_sender_task;
} string_command_t;

typedef struct {
    event_type_t event_type;
    float duty;
    QueueHandler_t queue_of_sender_task;
} pwm_command_t;

typedef struct {
    event_type_t event_type;
    QueueHandler_t queue_of_sender_task;
} indicator_command_t;

typedef struct {
    event_type_t event_type;
    QueueHandler_t queue_of_sender_task;
    char message[100];
} sync_command_t;



// ===== GLOBAL VARIABLES =====
extern QueueHandle_t queue_system[MAX_QUEUE_TASK];

// ===== API PROTOTYPES =====
esp_err_t Sys_Queue_Init(QueueHandler_t handler, data_type_t type, UBaseType_t uxQueueLength);
esp_err_t Sys_Queue_Send(QueueHandler_t recv_handler, void *command, TickType_t xTickToWait);
esp_err_t Sys_Queue_Receive(QueueHandler_t handler, void *command, TickType_t xTickToWait);
esp_err_t Sys_Queue_Reset(QueueHandler_t handler);

// ===== UTILITY FUNCTIONS =====
const char *getQueueHandlerName(QueueHandler_t handler);
esp_err_t Queue_Sys_Get_Name(QueueHandler_t handler, get_queue_name_t state, const char *tag);



#endif