#include <cstdio>

#include <getdns/getdns_extra.h>

#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32")

constexpr char domain[] = "ctf.nefu.edu.cn";

int main() {
    getdns_return_t r;
    getdns_context *ctxt = nullptr;
    getdns_dict    *resp = nullptr;
    getdns_bindata *address;
    char            address_str[1024];

    if ((r = getdns_context_create(&ctxt, 1)))
        fprintf(stderr, "Could not create context: %s\n", getdns_get_errorstr_by_id(r));

    else if ((r = getdns_address_sync(ctxt, domain, nullptr, &resp)))
        fprintf(stderr, "Unable to do an address lookup: %s\n", getdns_get_errorstr_by_id(r));

    else if ((r = getdns_dict_get_bindata(resp, "/just_address_answers/0/address_data", &address)))
        fprintf(stderr, "Unable to get an address from the response: %s\n", getdns_get_errorstr_by_id(r));

    else if (address->size != 4 && address->size != 16)
        fprintf(stderr, "Unable to determine type of this address\n");

    else if (!inet_ntop(address->size == 4 ? AF_INET : AF_INET6, address->data, address_str, sizeof(address_str)))
        fprintf(stderr, "Could not convert address to string\n");

    else
        printf("An address of %s is: %s\n", domain, address_str);

    if (resp)
        getdns_dict_destroy(resp);
    if (ctxt)
        getdns_context_destroy(ctxt);

    return r ? EXIT_FAILURE : EXIT_SUCCESS;
}