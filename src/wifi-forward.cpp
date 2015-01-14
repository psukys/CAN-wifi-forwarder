#include "wifi-forward.hpp"

void WifiForward::query_ips()
{
    //queries ips
}


void WifiForward::print_string_vector(std::vector<std::string> sv)
{
    for (std::vector<std::string>::iterator it = sv.begin();
        it != sv.end();
        ++it)
    {
        std::cout << *it << std::endl;
    }
}

void WifiForward::print_dhcp_ips()
{
    std::cout << "DHCP leases IPS" << std::endl;
    print_string_vector(dhcp_ips);
}

std::vector<std::string> WifiForward::get_leased_ips()
{
    regex_t regex;
    regmatch_t match[1];
    std::ifstream dhcp_file;
    std::string line;
    std::vector<std::string> ips;

    if (regcomp(&regex, IP_REGEX, REG_EXTENDED))
    {
        fprintf(stderr, "Could not compile regex for leased ips\n");
        return ips;
    }

    dhcp_file.open(DHCP_LEASES_FILE);

    while (!dhcp_file.eof())
    {
        dhcp_file >> line; //reads word by word
        if (!regexec(&regex, line.c_str(), 1, match, 0))
        {
            ips.push_back(line);
        }
    }
    return ips;
}

std::vector<std::string> WifiForward::get_client_ips()
{
    std::vector<std::string> ips;

    ips = get_leased_ips();

    /* remove duplicates:
        1. Sort
        2. Check if next value is equal and act*/
    std::sort(ips.begin(), ips.end());

    for (std::vector<std::string>::iterator it = ips.begin(); it != ips.end()- 1;)
    {
        if (*it == *(it + 1))
        {
            std::cout << *it << " = " << *(it + 1) << std::endl;
            it = ips.erase(it);
        }
        else
        {
            ++it;
        }
    }

    return ips;
}

WifiForward::WifiForward()
{
    struct sockaddr_in addr;
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_fd < 0)
    {
        fprintf(stderr, "Cannot create socket");
    }

    memset(&addr, 0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(PORT);

    if (bind(socket_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        fprintf(stderr, "Bind failed");
    }

    /* set the "linger" timeout to zero, to close the listen socket
        immediatelly at program termination */
    struct linger linger_opt = {1, 0}; /* Linger active, timeout 0 */
    setsockopt(socket_fd, SOL_SOCKET, SO_LINGER, &linger_opt, sizeof(linger_opt));

    if (listen(socket_fd, BUFSIZE) < 0)
    {
        fprintf(stderr, "Cannot listen");
    }

    dhcp_ips = get_client_ips();
}

int main()
{
    WifiForward wf;
    wf.query_ips();
    return 0;
}