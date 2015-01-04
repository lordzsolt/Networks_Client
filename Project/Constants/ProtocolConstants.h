#pragma once

#include <string>

extern std::string kServerAddress;
extern unsigned short kServerPort;

extern std::string presharedKey;


enum class MessageType : int32_t {
    Register,
    Log_In,
    Log_Out,
    Status_Update,
    Private_Message,
    Group_Message,
    Send_File
};

enum class MessageTag : int{
    New,
    Acknowledge,
    Corrupt,
    Missing
};


extern std::string kMessageDelimiter;
extern std::string kMessageKeyUsername;
extern std::string kMessageKeyPassword;
extern std::string kMessageKeyRecipientId;
extern std::string kMessageKeyMessage;


extern unsigned int kMaximumMessageLength;

extern unsigned int kMaximumMessageIndex;