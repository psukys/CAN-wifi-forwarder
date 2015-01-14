#ifndef WIFI_FORWARD_H__
#define WIFI_FORWARD_H__

#include <sys/socket.h> /* sockets */
#include <iostream>
#include <fstream>
#include <regex.h>
#include <string.h>
#include <vector>
#include <unistd.h>
#include <netinet/in.h> /* needed for sockaddr */
#include <errno.h> /* for error handling */
#include <algorithm> /* sorting */
#include <pthread.h> /* multithreading */qu


#define BUFSIZE 512 /* max length of buffer */
#define PORT 9753
/* please uncomment the original line, as "res/dhcpd.leases" is only for testing */
#define DHCP_LEASES_FILE "res/dhcpd.leases"
/*#define DHCP_LEASES_FILE "/var/lib/dhcp/dhcpd.leases"*/
/* posix regex requies backslash before curly brackets*/
#define IP_REGEX "[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}"



class WifiForward
{
private:
    std::vector<std::string> dhcp_ips;
    std::vector<std::pthread_t> clients;
    int socket_fd;
    int port;

    int check_socket_error(int fd);

    /// <summary>
    /// Gets DHCP's leased IP addresses
    /// </summary>
    /// <param name="dhcp_ips">ip addresses to be returned</param>
    /// <param name="ips_len">lenght of dhcp_ips array</param>
    /// <returns>
    /// A list of all leased dhcp_ips (that can either be inactive and duplicate)
    /// </returns>
    /// <remarks>
    /// Uses DHCPLEASES definition and prints perrors inside
    /// </remark>
    std::vector<std::string> get_leased_ips();

    /// <summary>
    /// Get client list
    /// </summary>
    /// <param name="dhcp_ips">ip addresses that are active (will be returned)</param
    /// <param name="ips_len">length of dhcp_ips array</param
    /// <returns>
    /// A list of active unique dhcp_ips
    /// </returns>
    /// <remarks>
    /// Prints perrors inside
    /// </remarks>
    std::vector<std::string> get_client_ips();

    /// <summary>
    /// Ping a specific IP
    /// </summary>
    /// <param name="ip">ip address of the form 0.0.0.0-255.255.255.255</param>
    /// <returns>
    /// 0 - the ip pinged back
    /// 1 - the ip did not ping back
    /// 2 - error while trying to ping
    /// </returns>
    int ping_ip(char * ip);

    /// <summary>
    /// Prints a string vector element list
    /// </summary>
    /// <param name="sv"> string vector </param>
    void print_string_vector(std::vector<std::string> sv);

    ///

public:
    /// <summary>
    /// Sets up WifiForward class:
    /// Sets up a socket and parses a list of active dhcp_ips from dhcp
    /// </summary>
    WifiForward();

    /// <summary>
    /// Prints client dhcp_ips (unique found on DHCP)
    /// </summary>
    void print_dhcp_ips();

    /// <summary>
    /// Queries IPs if they support API and adds accordingly adds them.
    /// All added ips are started in an individual thread and managed
    /// individually
    /// </summary>
    void query_ips();
};

#endif