
#include "common.h"
#include "pktbuff.h"
#include "dev.h"
#include "support.h"

#define PKT_SIZE (sizeof(struct ioq_header)  + sizeof(struct ether_header) + sizeof(struct ether_arp))
//#define PKT_SIZE (sizeof(struct ether_header) + sizeof(struct ether_arp))

int send_pkt(char* data, unsigned len)
{
  nf_pktout_send(data, &(data[len]));
  return len;
}

int process_arp(struct net_iface *iface, struct ioq_header *ioq, struct ether_header *eth, struct pkt_buff *pkt)
{
	unsigned short int my_hrd;
	unsigned short int my_pro;
	unsigned short int my_op;
	struct ether_arp *etharp;
	//struct pkt_buff reply;
	t_addr *reply;
	struct ether_header *reth;
	struct ether_arp *rarp;


	my_hrd = 6;		// set to mac(6)
	my_pro = 4;		// set to ipv4(4)
	my_op = ARPOP_REPLY;	// set to reply
	

	etharp = pkt_pull(pkt, sizeof(struct ether_arp));

	//if(ntohs(etharp->ea_hdr.ar_hrd) != ARPHRD_ETHER &&
	//	ntohs(etharp->ea_hdr.ar_pro) !=  ETHERTYPE_IP &&
	//	etharp->ea_hdr.ar_hln != 6 &&
	//	etharp->ea_hdr.ar_pln != 4 &&
	//		(
	//			ntohs(etharp->ea_hdr.ar_op) != ARPOP_REPLY ||
	//			ntohs(etharp->ea_hdr.ar_op) != ARPOP_REQUEST
	//		)
	//	)
	//{
	//}
	//if(memcmp(htonl(etharp->arp_tpa), iface->ip, 4) == 0)
	//{
		//if(etharp->ea_hdr.ar_op == ARPOP_REQUEST)
		//{
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
		//}
	//}

} 

int process_eth(struct net_iface *iface, void *data)
{
	int result;
	struct pkt_buff pkt;
	struct ioq_header *ioq;
	unsigned int size;
	struct ether_header *eth;

	result = 0;

	ioq = data;
	size = ntohs(ioq->byte_length);

	pkt_fill(&pkt, data, size + sizeof(struct ioq_header));
	eth = pkt_pull(&pkt, ETHER_HDR_LEN);

			result = process_arp(iface, ioq, eth, &pkt);
//	switch (eth->ether_type)
//	{
//		case ETHERTYPE_ARP:
//			result = process_arp(iface, ioq, eth, &pkt);
//			break;
//		default:
//			result = 1;
//			break;
//	}
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
	//u_int16_t ethtype;

//#ifndef DEBUG
//  t_addr* next_packet;
//  int i=nf_tid();
//  
//  if(i== 0)
//    {
//      nf_lock(LOCK_INIT); // should get it on the first attempt
//      nf_pktout_init();
//      nf_pktin_init();
//    }
//  else 
//    {
//      nf_stall_a_bit();
//      nf_lock(LOCK_INIT); // should not get it
//    }
//   nf_unlock(LOCK_INIT); 
//   
//#endif

	//test_this();
	
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

	//dest_mac[0] = 0x70;
	//dest_mac[1] = 0x56;
	//dest_mac[2] = 0x81;
	//dest_mac[3] = 0xa9;
	//dest_mac[4] = 0x2c;
	//dest_mac[5] = 0x39;

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

	//ethtype = ETHERTYPE_ARP;

	//only run this program on thread 0
	if (nf_tid() != 0) 
	{
	   while (1) {}
	}
	
	// initialize
	nf_pktout_init();
	nf_pktin_init();

	// allocate an output buffer
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

	//free
	//nf_pktin_free(pkt);

	// start in on replying
	//while(1)
	//{
	//	pkt = nf_pktin_pop();  // test for next_packet
	//	if(!nf_pktin_is_valid(pkt))
	//		continue;

	//	process_eth(&iface, pkt);

	//	nf_pktin_free(pkt);
	//} 
//memcpy(reply.data, pkt->head, pkt->len);

//struct ioq_header *reply_ioq = pkt_pull(&reply, sizeof(struct ioq_header));
//fill_ioq(reply_ioq, ioq->src_port, reply.len);

//struct ether_header *reply_ether = pkt_pull(&reply, sizeof(struct ether_header));
//struct ether_arp *reply_arp = pkt_pull(&reply, sizeof(struct ether_arp));
// copy source ip and mac to target
//memcpy(reply_arp->arp_tha, reply_arp->arp_sha, ETH_ALEN);
//memcpy(reply_arp->arp_tpa, reply_arp->arp_spa, 4);
// copy this interface's mac and ip to source
//memcpy(reply_arp->arp_sha, iface->mac, ETH_ALEN);
//memcpy(reply_arp->arp_spa, iface->ip, 4);

//reply_arp->ea_hdr.ar_op = htons(ARPOP_REPLY);
//memcpy(reply_ether->ether_shost, iface->mac, ETH_ALEN);
//memcpy(reply_ether->ether_dhost, reply_arp->arp_tha, ETH_ALEN);

//pkt_push_all(&reply);
//send_pkt(reply.data, reply.len);
	
  return 0;
}
