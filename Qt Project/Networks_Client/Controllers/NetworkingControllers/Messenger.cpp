#include "Messenger.h"
#include "../../Factories/SocketFactory.h"

#include "../../Constants/ProtocolConstants.h"
#include "../../Models/MessageModels/Message.h"
#include "../../Models/MessageModels/Messages/LoginMessage.h"
#include "../../Models/MessageModels/Messages/RegisterMessage.h"
#include "../../Models/MessageModels/Messages/PrivateMessage.h"

#include <iostream>

using namespace std;
using namespace std::placeholders;

Messenger::Messenger(std::string serverAddress, unsigned short port)
        : Messenger(serverAddress, port, SocketFactory::createSocket(serverAddress, port))
{
}

Messenger::Messenger(std::string serverAddress, unsigned short port, SOCKET socket)
        : _sender(socket),
        _receiver(socket, bind(&Messenger::messageReceived, this, _1)),
        _receivingThread(bind(&MessageReceiver::startReceiving, &_receiver))
{
}


Messenger::~Messenger() {
    _receiver.stopReceiving();
    _receivingThread.join();
}



void Messenger::sendLogin(std::string username, std::string password, MessengerCallback callback) const {
    LoginMessage loginMessage(_messageIndex, MessageTag::New, username, password);
    sendMessage(&loginMessage, callback);
}


void Messenger::sendSignup(std::string username, std::string password, MessengerCallback callback) const {
    RegisterMessage registerMessage(_messageIndex, MessageTag::New, username, password);
    sendMessage(&registerMessage, callback);
}



void Messenger::sendPrivateMessage(std::string message, int recipientId, MessengerCallback callback) const {
    PrivateMessage privateMessage(_messageIndex, MessageTag::New, recipientId, message);
    sendMessage(&privateMessage, callback);
}


void Messenger::sendGroupMessage(std::string message, std::unordered_set<int> recipientIds, MessengerCallback callback) const {
    //TODO: Update GroupMessage and implement group message sending
}

void Messenger::registerCallbackForMessageType(MessageType type, MessengerCallback callback) {
    auto pair = _callbacksByType.find(type);
    if (pair == _callbacksByType.end()) {
        //TODO: Fill in the details
    }
}

void Messenger::unregisterCallbackForMessageType(MessageType type, MessengerCallback callback) {

}


void Messenger::sendMessage(Message* message, MessengerCallback callback) const {
    _callbacksByIndex.emplace(_messageIndex, callback);
    if (_messageIndex < kMaximumMessageIndex - 1) {
        _messageIndex++;
    }
    else {
        _messageIndex = 0;
    }
    //TODO: Add error handling
    _sender.sendMessage(message->serialize());
}


void Messenger::messageReceived(shared_ptr<Message> message) {
    int32_t index = message->index();
    auto pair = _callbacksByIndex.find(index);
    if (pair == _callbacksByIndex.end()) {
        qDebug() << "Callback not found";
    }
    else {
        MessengerCallback callback = pair->second;
        callback(true);
        //_callbacksByIndex.erase(index);
    }

    shared_ptr<LoginMessage> login = dynamic_pointer_cast<LoginMessage>(message);

}