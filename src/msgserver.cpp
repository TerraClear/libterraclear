#include "msgserver.hpp"

namespace terraclear
{

    msgserver:: msgserver(server_type svr_type, std::string network_interface, uint16_t ip_port, std:: string ip_address)
    {
        _server_type = svr_type;
        _network_interface = network_interface;
        _ip_port = ip_port;
        _ip_address = ip_address;

        init_server();
    }

    msgserver::~msgserver() 
    {
        if (_socket_fd != 0)
            close(_socket_fd);
    }
    
    void msgserver::init_server()
    {
        //clear out socket
        memset(&_socket_address, 0, sizeof(_socket_address)); //clear our socket

        //Set IP V4 address and specific port.
        _socket_address.sin_family = AF_INET;
        _socket_address.sin_port = htons(_ip_port); 
        //_socket_address.sin_addr.s_addr = inet_at  INADDR_ANY; //Bind to ALL IP's    
        
        //bind to specific address
         inet_aton(_ip_address.c_str(), &_socket_address.sin_addr); 
        
         //UDP or TCP
         int socket_type = (_server_type == server_type::TCP_DIRECT) ? SOCK_STREAM : SOCK_DGRAM;

        //Open Socket File Descriptor..
        _socket_fd = socket(AF_INET, socket_type, 0);
        if (_socket_fd == -1)   
            throw msg_error("Socket Descriptor Failed", errno);
              
        //Set Socket Options
        int enable = 1;
        int retval = 0;

        //bind to specific NIC
        retval = setsockopt(_socket_fd, SOL_SOCKET, SO_BINDTODEVICE, _network_interface.c_str(), _network_interface.length());

        //allow re-use of adddress
        retval = setsockopt(_socket_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
        if ( retval == -1 )   
            throw msg_error("setsockopt(SO_REUSEADDR) failed", errno);
        
        #ifdef SO_REUSEPORT
        retval = setsockopt(_socket_fd, SOL_SOCKET, SO_REUSEPORT, &enable, sizeof(int));
        if (retval == -1)
            throw msg_error("setsockopt(SO_REUSEPORT) failed", errno);
        #endif        
    
        //Set broadcast option
        if (_server_type == server_type::UDP_BROADCAST)
        {
            retval = setsockopt(_socket_fd, SOL_SOCKET, SO_BROADCAST, (void *) &enable, sizeof(int));    
            if ( retval == -1 )   
                throw msg_error("setsockopt(SO_BROADCAST) failed", errno);
        }
    
         
    }   
    
    uint32_t msgserver::write_string(std::string message_string)
    {
        return sendto(_socket_fd, message_string.c_str(), message_string.length(), 0, (struct sockaddr *) &_socket_address, sizeof(_socket_address));
    }
    
}