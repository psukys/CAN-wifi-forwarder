#ifndef WIFI_FORWARD_H__
#define WIFI_FORWARD_H__

#include <regex.h>
#include <sys/socket.h> /* sockets */
#include <stdio.h> /* printf */
#include <stdlib.h>
#include <string.h> /* needed for memset */
#include <netinet/in.h> /* needed for sockaddr */
#include <errno.h> /* for error handling */

#define BUFSIZE 512 /* max length of buffer */
#define PORT 3000
#define DHCP_LEASES_FILE "/var/lib/dhcp/dhcpd.leases"
/* posix regex requies backslash before curly brackets*/
#define IP_REGEX "[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}"



/// <summary>
/// Checks for socket's errors upon opening it.
/// </summary>
/// <param name="fd">file description</param>
/// <returns>
/// 1 if there are errors; 0 if there's none
/// </returns>
/// <remarks>
/// It prints out perror inside
/// </remarks>
int check_socket_error(int fd);

/// <summary>
/// Gets DHCP's leased IP addresses
/// </summary>
/// <param name="ips">ip addresses to be returned</param>
/// <param name="ips_len">lenght of ips array</param>
/// <returns>
/// 0 - leased ips were retrieved successfully
/// 1 - dhcp file does not exist
/// 2 - error parsing dhcpd.leases file
/// 3 - regex compile error
/// </returns>
/// <remarks>
/// Uses DHCPLEASES definition and prints perrors inside
/// </remark>
int get_leased_ips(char **ips, unsigned int *ips_len);

/// <summary>
/// Get active client list
/// </summary>
/// <param name="ips">ip addresses that are active (will be returned)</param
/// <param name="ips_len">length of ips array</param
/// <returns>
/// 0 - there were no problems retrieving active ips
/// 1 - no active clients
/// 2 - problem retrieving clients
/// </returns>
/// <remarks>
/// Prints perrors inside
/// </remarks>
int get_client_ips(char **ips, unsigned int *ips_len);


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
/// Removes an element from a string matrix (char**)
/// </summary>
/// <param name="mat">string matrix</param>
/// <param name="i">index of the element to be removed</param>
/// <returns>
/// 0 - element removed successfully
/// 1 - failed to remove element
/// </returns
int string_matrix_remove_elem(char **mat, unsigned int *size, int ind);

void print_strings(char **arr, unsigned int *size)
{
    int i = 0;
    printf("length: %d\n", (*size));
    for ( i = 0; i < (*size); i++)
    {
        printf("%d: %s\n", i, arr[i]);
    }
}
#endif