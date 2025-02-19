/* RequestCodes.h*/
#ifndef REQUEST_CODES_H
#define REQUEST_CODES_H


    enum class RequestCode : std::uint16_t {
        /* Outgoing requests*/
        Register = 1025,
        CLIENT_PUBLIC_KEY = 1026,
        CLIENT_RECONNECT = 1027,
        CLIENT_SEND_FILE = 1028,
        CLIENT_CRC_OK = 1029,
        CLIENT_CRC_FAILED = 1030,
        CLIENT_CRC_FAILED_FINAL = 1031,

        /* Incoming requests*/
        SERVER_REGISTER_OK = 2100,
        SERVER_REGISTER_FAIL = 2101,
        SERVER_SEND_AES = 2102,
        SERVER_SENT_CRC = 2103,
        SERVER_CONFIRM_RECEIVED = 2104,
        SERVER_RECONNECT_AES = 2105,
        SERVER_RECONNECT_DENIED = 2106,
        SERVER_UNKNOWN_ERROR = 2107
    };

    enum class RequestPayload : std::uint16_t {
        PUBLIC_KEY_REQUEST = 415
    };
#endif 
