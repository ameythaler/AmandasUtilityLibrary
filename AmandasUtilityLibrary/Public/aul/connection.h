#pragma once

#if AUL_WIN

//#include "Windows.h"
#include <winsock2.h>
#define AUL_NETERR WSAGetLastError()
#define AUL_CLOSESOCK(x) closesocket(x)

#elif AUL_POSIX

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
typedef int SOCKET;
typedef timeval TIMEVAL;
#define AUL_NETERR errno
#define AUL_CLOSESOCK(x) close(x)

#endif // AUL_WIN

namespace aul
{
    enum class connection_protocol
    {
        UDP,
        TCP
    };

    enum class connection_family
    {
        IPV4 = AF_INET,
        IPV6 = AF_INET6,
        UNSPECIFIED = AF_UNSPEC
    };

    enum class connection_error_type
    {
        NONE,
        NOMINAL_DISCONNECT,
        CONNECTION_LOST,
        CONNECTION_BUSY,
        UNABLE_TO_INITIALIZE_WINSOCK,
        UNABLE_TO_CREATE_SOCKET,
        NO_VALID_ADDRESS_WITH_SPECIFIED_SETTINGS,
        LISTEN_CALLED_ON_UNBOUND_SOCKET,
        LISTEN_RETURNED_ERROR,
        BIND_RETURNED_ERROR,
        ACCEPT_RETURNED_ERROR,
        NO_ADDRESS_SPECIFIED_ON_CLIENT_INITIALIZATION,
        CONNECT_RETURNED_ERROR,
        SEND_RETURNED_ERROR,
        CLOSE_RETURNED_ERROR,
        SELECT_RETURNED_ERROR
    };

    struct connection_error 
    {
        connection_error_type _error_type;
        int32 _error_code;

        connection_error(connection_error_type error_type, int32 error_code = 0)
            : _error_type(error_type)
            , _error_code(error_code)
        { }
    };

    struct connection_status
    {
        connection_status();

    private:
        FD_SET _status;
        int32 _max_sock;
        void set_sock(SOCKET sock);
        bool is_sock_set(SOCKET sock) const;
        friend class connection;
    };

    class connection
    {
    public:
        connection() : _is_bound(false), _socket_state(_state::UNINITIALIZED) { }
        static connection_error initialize_server(connection& new_connection, connection_family family, connection_protocol protocol, const char* port, const char* local_address = nullptr);
        static connection_error initialize_client(connection& new_connection, connection_family family, connection_protocol protocol, const char* port, const char* address);
        static std::vector<mb_string> get_local_addresses(connection_family family, const char* port = "789");
        connection_error select(int32& num_ready, connection_status& status);
        connection_error bind();
        connection_error listen(int32 max_pending = SOMAXCONN);
        connection_error accept(connection& new_connection);
        connection_error connect();
        connection_error send(const byte* message, int32& message_size);
        connection_error recv(byte* buffer, int32& buffer_size);
        connection_error close();
        bool is_initialized() const { return _is_bound; }
        bool is_ready_to_read();
        bool is_ready_to_read(const connection_status& status) const;
        void initialize_status(connection_status& status) const;

    private:
        enum class _state : uint8
        {
            UNINITIALIZED = 0x00,
            BOUND = 0x01,
            CREATED = 0x06, // only works to check against original value (or be aware of how it behaves)
            SERVER_CREATED = 0x02, // server in the context means set up for listen()
            CLIENT_CREATED = 0x04, // any connection which will tx/rx
            SERVER_BOUND = 0x03,
            CLIENT_BOUND = 0x05,
            LISTEN = 0x08,
            SERVER_LISTEN = 0x0A,
            CONNECTED = 0x10,
            SERVER_CONNECTED = 0x12,
            CLIENT_CONNECTED = 0x14,
            DISCONNECTED = 0x80
        };
        friend AUL_ENUM_FLAG_OP(_state, |, uint8)
        friend AUL_ENUM_FLAG_OP(_state, &, uint8)
        friend AUL_ENUM_FLAG_OP(_state, ^, uint8)

        connection_protocol _protocol;
        sockaddr _address;
        SOCKET _socket;
        bool _is_bound;
        connection_family _family;
        _state _socket_state;

        connection_error handle_txrx_error(aul::int32 err);
    };
}