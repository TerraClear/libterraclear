#ifndef MSGSERVER_HPP
#define MSGSERVER_HPP

#include <unistd.h>
#include <cerrno>
#include "error_base.hpp"


//socket stuff
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MSGSERVER_DEFAULT_INTERFACE "lo"
#define MSGSERVER_DEFAULT_SERVER_TYPE terraclear::msgserver::UDP_BROADCAST
#define MSGSERVER_DEFAULT_IP_ADDRESS "127.255.255.255"
#define MSGSERVER_DEFAULT_IP_PORT 52525

namespace terraclear
{
    class msgserver 
    {
        public:
            enum server_type
            {
                UDP_BROADCAST,
                UDP_DIRECT,
                TCP_DIRECT
            };

            msgserver(  server_type svr_type = MSGSERVER_DEFAULT_SERVER_TYPE, std::string network_interface = MSGSERVER_DEFAULT_INTERFACE, 
                        uint16_t ip_port = MSGSERVER_DEFAULT_IP_PORT, std:: string ip_address = MSGSERVER_DEFAULT_IP_ADDRESS);
            virtual ~msgserver();

            uint32_t write_string(std::string message_string);
            
        private:
            std::string _network_interface;
            server_type _server_type;
            std::string _ip_address;
            uint16_t _ip_port;
            
            struct sockaddr_in _socket_address;
            int _socket_fd = 0; 
            
            void init_server();

    };

    class msg_error : public error_base     
    {
        public: 
            msg_error(std::string errmsg):error_base(errmsg) {}
            msg_error(std::string errmsg, int error_number):error_base(errmsg, error_number) {}
    };
    
}

#endif /* MSGSERVER_HPP */

