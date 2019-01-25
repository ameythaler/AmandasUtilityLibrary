#include "stdafx.h"
#include "aul/connection.h"
#include "aul/vector.h"
#include "aul/exception.h"

#if AUL_WIN
#include <WS2tcpip.h>
#endif // AUL_WIN

#define __AUL_SAFETY_CHECK (AUL_DEBUG & (AUL_DEBUG_SAFETY_CHECKS | AUL_DEBUG_OVERRIDE_CONNECTION))

#define __AUL_STATE_CHECK(CONDITION) if (((_socket_state & CONDITION) != CONDITION) || (_socket_state == _state::UNINITIALIZED))

namespace
{
    enum class net_error_mode : aul::uint8
    {
        SHUTDOWN,
        LOST,
        BUSY,
        OTHER,
    };

#if AUL_WIN
    struct windows_sockets
    {
    public:
        static void safety_check()
        {
            static windows_sockets inst;
            if (!inst.is_initialized()) AUL_THROW("Unable to initialize Windows Sockets.");
        }

        bool is_initialized() const { return _is_initialized; }

    private:
        WSADATA _wsa;
        bool _is_initialized;

        windows_sockets()
        {
            if (WSAStartup(MAKEWORD(2, 2), &_wsa) != 0)
            {
                _is_initialized = false;
            }
            _is_initialized = true;
        }

        ~windows_sockets()
        {
            if (_is_initialized)
            {
                WSACleanup();
            }
        }

        AUL_DISABLECOPYING(windows_sockets);
    };

#define __WINSOCK_SAFETY_CHECK() windows_sockets::safety_check()

    net_error_mode get_net_error_mode(aul::int32 err)
    {
        switch (err)
        {
        case WSAESHUTDOWN:
            return net_error_mode::SHUTDOWN;
        case WSAETIMEDOUT:
        case WSAECONNRESET:
        case WSAECONNABORTED:
            return net_error_mode::LOST;
        case WSAEINPROGRESS:
        case WSAEHOSTUNREACH:
            return net_error_mode::BUSY;
        default:
            return net_error_mode::OTHER;
        }
    }

#elif AUL_POSIX

    // #todo use Berkely sockets error codes instead of WinSock
    net_error_mode get_net_error_mode(aul::int32 err)
    {
        switch (err)
        {
        case WSAESHUTDOWN:
            return net_error_mode::SHUTDOWN;
        case WSAETIMEDOUT:
        case WSAECONNRESET:
        case WSAECONNABORTED:
            return net_error_mode::LOST;
        case WSAEINPROGRESS:
        case WSAEHOSTUNREACH:
            return net_error_mode::BUSY;
        default:
            return net_error_mode::OTHER;
        }
    }

#define __WINSOCK_SAFETY_CHECK()

#endif // AUL_WIN

    struct delete_addrinfo
    {
        addrinfo* _ptr;

        delete_addrinfo() : _ptr(nullptr) { }
        ~delete_addrinfo()
        {
            if (_ptr) freeaddrinfo(_ptr);
        }

        addrinfo* operator->() { return _ptr; }
        addrinfo** operator&() { return &_ptr; }
        operator addrinfo*() { return _ptr; }
    };

    void fill_hints(addrinfo& hints, aul::connection_family family, aul::connection_protocol protocol)
    {
        memset(&hints, 0, sizeof(addrinfo));
        hints.ai_family = (aul::int32)family;
        hints.ai_socktype = (protocol == aul::connection_protocol::TCP) ? SOCK_STREAM : SOCK_DGRAM;
    }
}

namespace aul
{
    vector<mb_string> connection::get_local_addresses(connection_family family, const char* port)
    {
#if __AUL_SAFETY_CHECK
        __WINSOCK_SAFETY_CHECK();
#endif // __AUL_SAFETY_CHECK
        vector<mb_string> addresses;
        addrinfo hints;
        delete_addrinfo addr_res;
        char ipstr[INET6_ADDRSTRLEN];

        fill_hints(hints, family, connection_protocol::TCP); // protocol is irrelevant here
        hints.ai_flags = AI_PASSIVE;

        int result = getaddrinfo(nullptr, port, &hints, &addr_res);
        if (result == 0) // success
        {
            for (addrinfo* res = addr_res; res; res = res->ai_next)
            {
                void* addr = nullptr;
                if (res->ai_family == AF_INET)
                {
                    sockaddr_in* ipv4 = (sockaddr_in*)res->ai_addr;
                    addr = &ipv4->sin_addr;
                }
                else
                {
                    sockaddr_in6* ipv6 = (sockaddr_in6*)res->ai_addr;
                    addr = &ipv6->sin6_addr;
                }

                if (inet_ntop(res->ai_family, addr, ipstr, sizeof(ipstr)))
                {
                    addresses.emplace_back(ipstr);
                }
            }
        }

        return addresses;
    }

    connection_error connection::initialize_server(connection& new_connection, connection_family family, connection_protocol protocol, const char* port, const char* local_address /* = nullptr */)
    {
        __WINSOCK_SAFETY_CHECK();
        if (!port) AUL_THROW("Cannot initialize client, NULL port specified");
        addrinfo hints;
        delete_addrinfo addr_res;

        fill_hints(hints, family, protocol);
        if (!local_address) hints.ai_flags = AI_PASSIVE;

        int32 result = (int32)getaddrinfo(local_address, port, &hints, &addr_res);
        if (result != 0)
        {
            return connection_error(connection_error_type::NO_VALID_ADDRESS_WITH_SPECIFIED_SETTINGS, result);
        }

        if (protocol == connection_protocol::TCP)
        {
            result = (int32)socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        }
        else if (protocol == connection_protocol::UDP)
        {
            result = (int32)socket(AF_INET, SOCK_DGRAM, 0);
        }

        if (result == -1)
        {
            char buff[256];
            sprintf_s(buff, sizeof(buff), "Unable to create socket for server. Error code %u(%x", result, result);
            AUL_THROW(buff);
        }

        new_connection._socket = (SOCKET)result;
        new_connection._address = *(addr_res->ai_addr);
        new_connection._family = (connection_family)addr_res->ai_family;
        new_connection._protocol = protocol;
        new_connection._socket_state = _state::SERVER_CREATED;
        return connection_error(connection_error_type::NONE);
    }

    connection_error connection::initialize_client(connection& new_connection, connection_family family, connection_protocol protocol, const char* port, const char* address)
    {
        __WINSOCK_SAFETY_CHECK();
        if (!port) AUL_THROW("Cannot initialize client, NULL port specified");
        if (!address) AUL_THROW("Cannot initialize client, NULL address specified");
        addrinfo hints;
        delete_addrinfo addr_res;

        fill_hints(hints, family, protocol);

        int32 result = (int32)getaddrinfo(address, port, &hints, &addr_res);
        if (result != 0)
        {
            return connection_error(connection_error_type::NO_VALID_ADDRESS_WITH_SPECIFIED_SETTINGS, result);
        }

        if (protocol == connection_protocol::TCP)
        {
            result = (int32)socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        }
        else if (protocol == connection_protocol::UDP)
        {
            result = (int32)socket(AF_INET, SOCK_DGRAM, 0);
        }

        if (result == -1)
        {
            char buff[256];
            sprintf_s(buff, sizeof(buff), "Unable to create socket for client. Error code %u(%x", result, result);
            AUL_THROW(buff);
        }

        new_connection._socket = (SOCKET)result;
        new_connection._address = *(addr_res->ai_addr);
        new_connection._family = (connection_family)addr_res->ai_family;
        new_connection._protocol = protocol;
        new_connection._socket_state = _state::CLIENT_CREATED;
        return connection_error(connection_error_type::NONE);
    }

    connection_error connection::bind()
    {
#if __AUL_SAFETY_CHECK
        __WINSOCK_SAFETY_CHECK();
        if (((_socket_state & _state::CREATED) != _socket_state) || (_socket_state == _state::UNINITIALIZED))
        {
            AUL_THROW("Connection not in a bind-able state.");
        }
#endif // __AUL_SAFETY_CHECK
        if (::bind(_socket, &_address, sizeof(_address)) != 0)
        {
            return connection_error(connection_error_type::BIND_RETURNED_ERROR, AUL_NETERR);
        }

        _socket_state = _socket_state | _state::BOUND;
        return connection_error(connection_error_type::NONE);
    }

    connection_error connection::listen(int32 max_pending /* = SOMAXCONN */)
    {
#if __AUL_SAFETY_CHECK
        __WINSOCK_SAFETY_CHECK();
        __AUL_STATE_CHECK(_state::SERVER_BOUND)
        {
            AUL_THROW("Connection not in a listen-able state.");
        }
#endif // __AUL_SAFETY_CHECK
        if (!_is_bound) return connection_error(connection_error_type::LISTEN_CALLED_ON_UNBOUND_SOCKET, -1);

        int result = ::listen(_socket, max_pending);
        if (result < 0) return connection_error(connection_error_type::LISTEN_RETURNED_ERROR, result);

        _socket_state = _socket_state | _state::LISTEN;
        return connection_error(connection_error_type::NONE);
    }

    connection_error connection::accept(connection& new_connection)
    {
#if __AUL_SAFETY_CHECK
        __WINSOCK_SAFETY_CHECK();
        __AUL_STATE_CHECK(_state::SERVER_LISTEN)
        {
            AUL_THROW("Connection must be a server in listen mode to accept a connection.");
        }
#endif // __AUL_SAFETY_CHECK
        sockaddr addr;
        int addrlen = (int)sizeof(addr);
        int32 result = (int32)::accept(_socket, &addr, &addrlen);
        if (result != 0)
        {
            return connection_error(connection_error_type::ACCEPT_RETURNED_ERROR, AUL_NETERR);
        }

        new_connection = *this;
        new_connection._socket = (SOCKET)result;
        new_connection._address = addr;
        new_connection._socket_state = _state::CLIENT_CONNECTED;
        return connection_error(connection_error_type::NONE);
    }

    connection_error connection::connect()
    {
#if __AUL_SAFETY_CHECK
        __WINSOCK_SAFETY_CHECK();
        __AUL_STATE_CHECK(_state::CLIENT_CREATED)
        {
            AUL_THROW("Connection must be an initialized client in order to connect.");
        }
#endif // _AUL_SAFETY_CHECK
        int addrlen = (int)sizeof(_address);
        int result = ::connect(_socket, &_address, addrlen);

        if (result != 0) return connection_error(connection_error_type::CONNECT_RETURNED_ERROR, AUL_NETERR);

        _socket_state = _socket_state | _state::CONNECTED;
        return connection_error(connection_error_type::NONE, 0);
    }

    connection_error connection::send(const byte* message, int32& message_size)
    {
#if __AUL_SAFETY_CHECK
        __WINSOCK_SAFETY_CHECK();
        __AUL_STATE_CHECK(_state::CONNECTED)
        {
            AUL_THROW("send() called on connection not connected with connect() or accept().");
        }
#endif // __AUL_SAFETY_CHECK
        int bytes_sent = ::send(_socket, (const char*)message, (int32)message_size, 0);
        if (bytes_sent < 0)
        {
            message_size = 0;
            return handle_txrx_error(AUL_NETERR);
        }
        else
        {
            message_size = bytes_sent;
            return connection_error(connection_error_type::NONE);
        }
    }

    connection_error connection::recv(byte* buffer, int32& buffer_size)
    {
#if __AUL_SAFETY_CHECK
        __WINSOCK_SAFETY_CHECK();
        __AUL_STATE_CHECK(_state::CONNECTED)
        {
            AUL_THROW("recv() called on connection not connected with connect() or accept().");
        }
#endif // __AUL_SAFETY_CHECK
        int bytes_recvd = ::recv(_socket, (char*)buffer, buffer_size, 0);
        if (bytes_recvd < 0)
        {
            buffer_size = 0;
            return handle_txrx_error(AUL_NETERR);
        }
        else if (bytes_recvd == 0)
        {
            _socket_state = _state::DISCONNECTED;
            return connection_error(connection_error_type::NOMINAL_DISCONNECT);
        }
        else
        {
            buffer_size = bytes_recvd;
            return connection_error(connection_error_type::NONE);
        }
    }

    connection_error connection::close()
    {
#if __AUL_SAFETY_CHECK
        __WINSOCK_SAFETY_CHECK();
        if(_socket_state == _state::UNINITIALIZED)
        {
            AUL_THROW("close() called on uninitialized connection.");
        }
#endif // __AUL_SAFETY_CHECK

        if (_socket_state != _state::UNINITIALIZED)
        {
#if AUL_WIN
            int result = closesocket(_socket);
#elif AUL_POSIX
            int result = close(_socket);
#endif // AUL_WIN

            if (result != 0) return connection_error(connection_error_type::CLOSE_RETURNED_ERROR, AUL_NETERR);
            _socket_state = _state::UNINITIALIZED;
        }

        return connection_error(connection_error_type::NONE);
    }

    connection_error connection::handle_txrx_error(int32 err)
    {
        switch (get_net_error_mode(err))
        {
        case net_error_mode::SHUTDOWN:
            _socket_state = _state::DISCONNECTED;
            return connection_error(connection_error_type::NOMINAL_DISCONNECT);
        case net_error_mode::LOST:
            _socket_state = _state::DISCONNECTED;
            return connection_error(connection_error_type::CONNECTION_LOST);
        case net_error_mode::BUSY:
            return connection_error(connection_error_type::CONNECTION_BUSY);
        default:
            _socket_state = _state::DISCONNECTED;
            return connection_error(connection_error_type::SEND_RETURNED_ERROR, err);
        }
    }
}

#undef __WINSOCK_SAFETY_CHECK
#undef __AUL_SAFETY_CHECK