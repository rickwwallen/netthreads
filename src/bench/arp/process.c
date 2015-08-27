#include "common.h"
#include "pktbuff.h"
#include "dev.h"
#include "support.h"

#define PKT_SIZE (sizeof(struct ioq_header)  + sizeof(struct ether_header) + sizeof(struct ether_arp))

int process_arp(struct net_iface *iface, struct ioq_header *ioq, struct ether_header *eth, struct ether_arp *etharp, t_addr *pkt)
{
	unsigned short int my_hrd;
	unsigned short int my_pro;
	unsigned short int my_op;
	t_addr *reply;
	struct ether_header *reth;
	struct ether_arp *rarp;

	my_hrd = 6;		// set to mac(6)
	my_pro = 4;		// set to ipv4(4)
	my_op = ARPOP_REPLY;	// set to reply

	// if this isn't an ARP reply or request we don't care
	if(ntohs(etharp->ea_hdr.ar_hrd) != ARPHRD_ETHER || 
		ntohs(etharp->ea_hdr.ar_pro) !=  ETHERTYPE_IP ||
		etharp->ea_hdr.ar_hln != my_hrd ||
		etharp->ea_hdr.ar_pln != my_pro ||
		(
			ntohs(etharp->ea_hdr.ar_op) != ARPOP_REPLY &&
			ntohs(etharp->ea_hdr.ar_op) != ARPOP_REQUEST
		)
	)
	{
		return 1;
	}
	if(memcmp(htonl(etharp->arp_tpa), iface->ip, 4) == 0)
	{
		if(ntohs(etharp->ea_hdr.ar_op) == ARPOP_REQUEST)
		{
			// allocate reply size
			reply = nf_pktout_alloc(PKT_SIZE);

			// setup the ioq_header
			fill_ioq((struct ioq_header*) reply, 2, PKT_SIZE);

			// setup the ethernet header
			reth = (struct ether_header*) (reply + sizeof(struct ioq_header));

			// setup the ethernet arp
			rarp = (struct ether_arp*) (reply + sizeof(struct ioq_header) + sizeof(struct ether_header));
		
			// start putting things into the packet
			// ethernet
			memcpy(reth->ether_shost, iface->mac, ETH_ALEN);
			memcpy(reth->ether_dhost, eth->ether_shost, ETH_ALEN);
			reth->ether_type = ETHERTYPE_ARP;

			// arp header
			rarp->ea_hdr.ar_hrd = htons(ARPHRD_ETHER);
			rarp->ea_hdr.ar_pro = htons(ETHERTYPE_IP);
			rarp->ea_hdr.ar_hln = 6;
			rarp->ea_hdr.ar_pln = 4;
			rarp->ea_hdr.ar_op = htons(ARPOP_REPLY);

			// arp ethernet
				// source
			memcpy(rarp->arp_sha, iface->mac, ETH_ALEN);
			memcpy(rarp->arp_spa, iface->ip, 4);
				// target
			memcpy(rarp->arp_tha, etharp->arp_sha, ETH_ALEN);
			memcpy(rarp->arp_tpa, etharp->arp_spa, 4);

			// send it
			nf_pktout_send(reply, reply + PKT_SIZE); 
			//nf_pktout_send(reply, reply + PKT_SIZE); 
			//nf_pktout_send(reply, reply + PKT_SIZE); 
			//nf_pktout_send(reply, reply + PKT_SIZE); 
		}
	}

} 

int process_eth(struct net_iface *iface, t_addr *data)
{
	int result;
	struct ioq_header *ioq;
	unsigned int size;
	struct ether_header *eth;
	struct ether_arp *etharp;

	result = 0;

	ioq = data;
	size = ntohs(ioq->byte_length);

	eth = (struct ether_header*) (data + sizeof(struct ioq_header));
	etharp = (struct ether_arp*) (data + (sizeof(struct ioq_header) + sizeof(struct ether_header)));

	switch (ntohs(eth->ether_type))
	{
		case ETHERTYPE_ARP:
			//result = process_arp(iface, ioq, eth, &pkt);
			result = process_arp(iface, ioq, eth, etharp, data);
			break;
		default:
			result = 1;
			break;
	}
	return result;
}

int main(void)
{
	t_addr *pkt;
	struct net_iface iface;
	struct ether_header *reth;
	struct ether_arp *rarp;
	unsigned char dest_mac[6];
	unsigned char dest_ip[4];

	// iface is not shared, it's on the stack
	arp_init(&iface.arp);

	iface.mac[0] = 0x00;
	iface.mac[1] = 0x43;
	iface.mac[2] = 0x32;
	iface.mac[3] = 0x46;
	iface.mac[4] = 0x4e;
	iface.mac[5] = 0x00;

	iface.ip[0] = 192;
	iface.ip[1] = 168;
	iface.ip[2] = 0;
	iface.ip[3] = 100;

	dest_mac[0] = 0xff;
	dest_mac[1] = 0xff;
	dest_mac[2] = 0xff;
	dest_mac[3] = 0xff;
	dest_mac[4] = 0xff;
	dest_mac[5] = 0xff;

	dest_ip[0] = 192;
	dest_ip[1] = 168;
	dest_ip[2] = 0;
	dest_ip[3] = 185;

	//only run this program on thread 0
	if (nf_tid() != 0) 
	{
	   while (1) {}
	}
	
	// initialize
	nf_pktout_init();
	nf_pktin_init();

	// This is to just send an ARP request start uncommenting to play
	// allocate an output buffer
	//t_addr *pkt = nf_pktout_alloc(PKT_SIZE);
	pkt = nf_pktout_alloc(PKT_SIZE);

	// setup the ioq_header
	fill_ioq((struct ioq_header*) pkt, 2, PKT_SIZE);

	// setup the ethernet header
	//struct ether_header *reth = (struct ether_header*) (pkt + sizeof(struct ioq_header));
	reth = (struct ether_header*) (pkt + sizeof(struct ioq_header));
 
	// setup the ethernet arp
	//struct ether_arp *rarp = (struct ether_arp*) (pkt + sizeof(struct ioq_header) + sizeof(struct ether_header));
	rarp = (struct ether_arp*) (pkt + sizeof(struct ioq_header) + sizeof(struct ether_header));

	// start putting things into the packet
	// ethernet
	memcpy(reth->ether_shost, &iface.mac, ETH_ALEN);
	memcpy(reth->ether_dhost, &dest_mac, ETH_ALEN);
	//memcpy(reth->ether_type, htons(ethtype), sizeof(u_int16_t));
	//memcpy(reth->ether_type, &ethtype, 2);
	//memcpy(reth->ether_type, &mytype, 2);
	reth->ether_type = ETHERTYPE_ARP;

	// arp header
	rarp->ea_hdr.ar_hrd = htons(ARPHRD_ETHER);
	rarp->ea_hdr.ar_pro = htons(ETHERTYPE_IP);
	//rarp->ea_hdr.ar_pro = htons(0x0800);
	rarp->ea_hdr.ar_hln = 6;
	rarp->ea_hdr.ar_pln = 4;
	rarp->ea_hdr.ar_op = htons(ARPOP_REQUEST);

	// arp ethernet
		// source
	memcpy(rarp->arp_sha, &iface.mac, ETH_ALEN);
	memcpy(rarp->arp_spa, &iface.ip, 4);
		// target
	memcpy(rarp->arp_tha, dest_mac, ETH_ALEN);
	memcpy(rarp->arp_tpa, dest_ip, 4);

	// send it
	nf_pktout_send(pkt, pkt + PKT_SIZE); 
	nf_pktout_send(pkt, pkt + PKT_SIZE); 
	nf_pktout_send(pkt, pkt + PKT_SIZE); 
	nf_pktout_send(pkt, pkt + PKT_SIZE);
	//end uncomment

	// start in on replying
	while(1)
	{
		pkt = nf_pktin_pop();  // test for next_packet

		if(!nf_pktin_is_valid(pkt))
			continue;

		process_eth(&iface, pkt);

		nf_pktin_free(pkt);
	} 

  return 0;
}
