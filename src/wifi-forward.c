#include "wifi-forward.h"

int ping_ip(char *ip)
{
    return 0;
}

int string_matrix_remove_elem(char **mat, unsigned int *size, int ind)
{
    /* unfortunately char elements are in CONST_DATA section, ref:
    stackoverflow.com/questions/12290892/how-to-clear-a-char-passed-to-a-function-in-c
    */

    int i;
    if (size > 0)
    {
        --(*size);
    }
    else
    /* Only link exists */
    {
        free(mat);
        return 0;
    }
    char **new_array = (char **) malloc((*size) * sizeof(*new_array));
    for (i = 0; i < (*size); i++)
    {
        if (i >= ind)
        {
            new_array[i] = (char *) malloc(strlen(mat[i + 1]) * sizeof(char*));
            memcpy(new_array[i], mat[i + 1], strlen(mat[i + 1]));
            free(mat[i + 1]);
        }
        else
        {
            new_array[i] = (char *) malloc(strlen(mat[i]) * sizeof(char*));
            memcpy(new_array[i], mat[i], strlen(mat[i]));
            free(mat[i]);
        }
    }
    free(mat);
    mat = new_array;
    return 0;
}

int check_socket_error(int fd)
{
    return 0;
}

int get_leased_ips(char **ips, unsigned int *ips_len)
{
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    /* regex vars */
    regex_t regex;
    regmatch_t match[1];/* in each line we expect only 1 IP address */

    printf("Searching for DHCP leased IPS...\n");
    if (regcomp(&regex, IP_REGEX, REG_EXTENDED))
    {
        perror("ERROR: Could not compile regex\n");
        return 3;
    }

    fp = fopen(DHCP_LEASES_FILE, "r");
    if (!fp)
    {
        perror("Failed to open DHCP leases file\n");
    }

    while (getline(&line, &len, fp) != -1)
    {
        if (!regexec(&regex, line, 1, match, 0))
        {
            ips = (char **) realloc(ips, ++(*ips_len) * sizeof(*ips));
            ips[(*ips_len) - 1] = malloc((match[0].rm_eo - match[0].rm_so) * sizeof(char*));
            memcpy(ips[(*ips_len) - 1], &line[match[0].rm_so], 
                    match[0].rm_eo - match[0].rm_so);
            printf("Found: %s\n", ips[(*ips_len) -1]);
        }
    }

    fclose(fp);
    if (line)
        free(line);

    return 0;
}

int get_client_ips(char **ips, unsigned int *ips_len)
{
    int i = 0;
    /* use the same variable for retrieving dhcp leases */
    if (get_leased_ips(ips, ips_len))
    {
        return 2;
    }

    print_strings(ips, ips_len);
    for ( i = 0; i < (*ips_len); i++)
    {
        if (!ping_ip(ips[i]))
        {
            string_matrix_remove_elem(ips, ips_len, i);
        }
    }

    print_strings(ips, ips_len);
    /* ping each client and see whether they respond.
       On success keep it, else remove */
    return 0;
}

int main()
{
    struct sockaddr_in addr; /* address */
    int fd; /* socket file descriptor */
    char **client_ips;
    unsigned int client_ips_len;

    fd = socket(AF_INET, SOCK_STREAM, 0);

    if (fd < 0)
    {
        fprintf(stderr, "ERROR: Cannot create socket");
        return 1;
    }

    printf("Socket created, fd: %d\n", fd);

    /*bind to a return address */

    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(PORT); /* port defined in header file */
    bind(fd, (struct sockaddr *)&addr, sizeof(addr));

    listen(fd, BUFSIZE);

    client_ips_len = 0;
    client_ips = (char **) malloc(client_ips_len * sizeof(*client_ips));

    if (!get_client_ips(client_ips, &client_ips_len))
    {
        return 1;
    }

    return 0;
}