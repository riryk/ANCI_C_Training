
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>

//int WinsockSelectServer(int argc, char **argv);

void main_multicast(BOOL bReceive);
void main_multicast_receive();
void main_multicast_send();
int main_address_list();
int main_addr_list_query(int argc, char **argv);
int main_get_atm_address(int argc, char **argv);
int main_sio_get_broadcast_address(int argc, char **argv);
int main_sio_keepalive_vals(int argc, char **argv);
int main_sio_routing_interface_query(int argc, char **argv);
int main_so_acceptconn(int argc, char **argv);
int main_so_broadcast(int argc, char **argv, BOOL bSender);
int main_protocol_info(int argc, char **argv);
int main_rcvtimeo(int argc, char **argv);
int main_so_type(int argc, char **argv);