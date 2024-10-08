//----------------------------------------------------------------------------
//
//  Workfile: mqtt.h$
//
//  Copyright: Jim Wright 2024
//
//  Notes:
//     This is a singleton publishing and subscribing to mqtt topics
//
//----------------------------------------------------------------------------
#ifndef MQTT_H
#define MQTT_H

//----------------------------------------------------------------------------
//  Includes
//----------------------------------------------------------------------------
#include "queue"
#include "MQTTAsync.h"
#include "connection.h"
#include "mqttMessage.h"

const int QOS = 1;
const int TIMEOUT = 10000L;
const int VERBOSE_MESSAGE_DISPLAY_MOD = 2000;

//----------------------------------------------------------------------------
//  Class Declarations
//----------------------------------------------------------------------------
//
// Class Name: MQTT
// 
// Purpose:
//      
//
//----------------------------------------------------------------------------
class MQTT
{
  public:
    //----------------------------------------------------------------------------
    //  Class Methods
    //----------------------------------------------------------------------------
    MQTT();
    ~MQTT();

    void loadConfiguration(std::string fileName);
    void loadConfiguration(std::string address, std::string port, std::string user, std::string password);
    void openMQTT();
//    void connect(Connection connect);
    void send(const char* topic, const char* payload, int payloadLength);
    void send(const char* topic, std::string payload);
    void send(std::string topic, std::string payload);
    void subscribe(char* topic, bool addTopic = true);
    void subscribe(std::string topic, bool addTopic = true);
    void subscribe(std::shared_ptr<std::string> topic, bool addTopic = true);
    void setMessageArrivedFunction(void (*messageArrivedFunction)(char *topicName, int topicLen, MQTTAsync_message *message));
    std::shared_ptr<std::queue<std::shared_ptr<MQTTMessage>>> getMessageQueue();
    bool isConnected();
    bool getAutoConnectOnSend();
    void setAutoConnectOnSend(bool value);
    void reSubscribe();

  private:
    //----------------------------------------------------------------------------
    //  Class Atributes
    //----------------------------------------------------------------------------
    MQTTAsync mClient;
    Connection mConnection;
    volatile MQTTAsync_token mDeliveredtoken;
    static void (*mMessageArrivedFunction)(char *topicName, int topicLen, MQTTAsync_message *message);
    static std::shared_ptr<std::queue<std::shared_ptr<MQTTMessage>>> mMesageQueue;
    static int mVerboseCount;
    static bool mUseQueue;
    static bool mConnected;
    static bool mAutoConnectOnSend;
    static int mConnectFailedLog;
    static std::shared_ptr<std::vector<std::shared_ptr<std::string>>> mSubscribeList;

    std::string mUser;
    std::string mPassword;
    std::string mMQttAddress;
    std::string mClientID;

    //----------------------------------------------------------------------------
    //  Class Methods
    //----------------------------------------------------------------------------
    static void onConnect(void* context, MQTTAsync_successData* response);
    static void onConnectFailure(void* context, MQTTAsync_failureData* response);
    static void onSend(void* context, MQTTAsync_successData* response);
    static void onDisconnect(void* context, MQTTAsync_successData* response);
    static void connlost(void *context, char *cause);
    static void onSubscribe(void* context, MQTTAsync_successData* response);
    static void onSubscribeFailure(void* context, MQTTAsync_failureData* response);
    static int messageArrived(void *context, char *topicName, int topicLen, MQTTAsync_message *message);
};
#endif
