#include "Messenger.h"
#include "../../Factories/SocketFactory.h"

#include "../../Constants/ProtocolConstants.h"
#include "../../Models/MessageModels/Message.h"
#include "../../Models/MessageModels/Messages/LoginMessage.h"
#include "../../Models/MessageModels/Messages/LoginResponseMessage.h"
#include "../../Models/MessageModels/Messages/RegisterMessage.h"
#include "../../Models/MessageModels/Messages/PrivateMessage.h"

#include <iostream>
#include <sstream>

using namespace std;
using namespace std::placeholders;


Messenger::Messenger(string serverAddress, unsigned short port, MessengerCallback messengerCallback)
    : Messenger(SocketFactory::createSocket(serverAddress, port))
{
    _callbacksByIndex.emplace(0, messengerCallback);
}


Messenger::Messenger(SOCKET socket)
    : _sender(socket),
      _messageReceiver(bind(&Messenger::messageReceived, this, _1, _2)),
      _messageThread(bind(&MessageReceiver::startReceiving, &_messageReceiver))
{
    std::cout << "Messenger started with socket: " << socket << endl;
}


Messenger::~Messenger() {
    _messageReceiver.stopReceiving();
    _messageThread.join();
}


void Messenger::listenToSocket(SOCKET socket)
{
    _messageReceiver.insertSocket(socket);
}


void Messenger::removeSocket(SOCKET socket)
{
    _messageReceiver.removeSocket(socket);
}


void Messenger::sendMessage(Message* message, MessengerCallback callback) const {
    if (_messageIndex < kMaximumMessageIndex - 1) {
        _messageIndex++;
    }
    else {
        _messageIndex = 0;
    }
    _callbacksByIndex.emplace(_messageIndex, callback);

    std::string serializedMessage = message->serialize();
    std::ostringstream stream(std::stringstream::out | std::stringstream::binary);
    stream.write(reinterpret_cast<char*>(&_messageIndex), sizeof(_messageIndex));
    std::string serializedIndex = stream.str();

    std::string messageWithIndex = serializedIndex + serializedMessage;
    _sender.sendMessage(messageWithIndex);
}


void Messenger::messageReceived(int32_t index, shared_ptr<Message> message) {
    MessengerCallback callback = _callbacksByIndex[0];
    callback(message);
}
