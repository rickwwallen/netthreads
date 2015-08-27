Trie *load_zone()
{
	char domNme[DNM_SZ];
	RR *rrs;
	uint32_t dTtl = 0; //default ttl gets redefined by SOA
	uint16_t dClass = 0; //default class gets redefined by SOA
	char rR2[LNE_SZ];
	Trie *root;

	root = createNode('*', NULL);

	memset(&domNme, '\0', DNM_SZ);
	memset(&rR2, '\0', LNE_SZ);
	memcpy(&domNme, ".", 1);
	memcpy(&rR2, "IN,SOA,SRI-NIC.ARPA,HOSTMASTER.SRI-NIC.ARPA,870611,1800,300,604800,86400,", 73);
	rrs = createResRec(rR2, &dTtl, &dClass);
	if(rrs != NULL)
	{
		addTrie(root, domNme, rrs);
	}
	rrs = NULL;

	memset(&domNme, '\0', DNM_SZ);
	memset(&rR2, '\0', LNE_SZ);
	memcpy(&domNme, ".", 1);
	memcpy(&rR2, "NS,A.ISI.EDU.,", 14);
	rrs = createResRec(rR2, &dTtl, &dClass);
	if(rrs != NULL)
	{
		addTrie(root, domNme, rrs);
	}
	rrs = NULL;

	memset(&domNme, '\0', DNM_SZ);
	memset(&rR2, '\0', LNE_SZ);
	memcpy(&domNme, ".", 1);
	memcpy(&rR2, "NS,C.ISI.EDU.,", 14);
	rrs = createResRec(rR2, &dTtl, &dClass);
	if(rrs != NULL)
	{
		addTrie(root, domNme, rrs);
	}
	rrs = NULL;

	memset(&domNme, '\0', DNM_SZ);
	memset(&rR2, '\0', LNE_SZ);
	memcpy(&domNme, ".", 1);
	memcpy(&rR2, "NS,SRI-NIC.ARPA.,", 17);
	rrs = createResRec(rR2, &dTtl, &dClass);
	if(rrs != NULL)
	{
		addTrie(root, domNme, rrs);
	}
	rrs = NULL;

	memset(&domNme, '\0', DNM_SZ);
	memset(&rR2, '\0', LNE_SZ);
	memcpy(&domNme, ".MIL", 4);
	memcpy(&rR2, "86400,CH,NS,SRI-NIC.ARPA.,", 26);
	rrs = createResRec(rR2, &dTtl, &dClass);
	if(rrs != NULL)
	{
		addTrie(root, domNme, rrs);
	}
	rrs = NULL;

	memset(&domNme, '\0', DNM_SZ);
	memset(&rR2, '\0', LNE_SZ);
	memcpy(&domNme, ".MIL", 4);
	memcpy(&rR2, "86400,NS,A.ISI.EDU.,", 20);
	rrs = createResRec(rR2, &dTtl, &dClass);
	if(rrs != NULL)
	{
		addTrie(root, domNme, rrs);
	}
	rrs = NULL;

	memset(&domNme, '\0', DNM_SZ);
	memset(&rR2, '\0', LNE_SZ);
	memcpy(&domNme, ".EDU", 4);
	memcpy(&rR2, "CH,NS,SRI-NIC.ARPA.,", 20);
	rrs = createResRec(rR2, &dTtl, &dClass);
	if(rrs != NULL)
	{
		addTrie(root, domNme, rrs);
	}
	rrs = NULL;

	memset(&domNme, '\0', DNM_SZ);
	memset(&rR2, '\0', LNE_SZ);
	memcpy(&domNme, ".EDU", 4);
	memcpy(&rR2, "86400,NS,C.ISI.EDU.,", 20);
	rrs = createResRec(rR2, &dTtl, &dClass);
	if(rrs != NULL)
	{
		addTrie(root, domNme, rrs);
	}
	rrs = NULL;

	memset(&domNme, '\0', DNM_SZ);
	memset(&rR2, '\0', LNE_SZ);
	memcpy(&domNme, ".ARPA.SRI-NIC", 13);
	memcpy(&rR2, "CH,A,26.0.0.73,", 15);
	rrs = createResRec(rR2, &dTtl, &dClass);
	if(rrs != NULL)
	{
		addTrie(root, domNme, rrs);
	}
	rrs = NULL;

	memset(&domNme, '\0', DNM_SZ);
	memset(&rR2, '\0', LNE_SZ);
	memcpy(&domNme, ".ARPA.SRI-NIC", 13);
	memcpy(&rR2, "A,10.0.0.51,", 12);
	rrs = createResRec(rR2, &dTtl, &dClass);
	if(rrs != NULL)
	{
		addTrie(root, domNme, rrs);
	}
	rrs = NULL;

	memset(&domNme, '\0', DNM_SZ);
	memset(&rR2, '\0', LNE_SZ);
	memcpy(&domNme, ".ARPA.SRI-NIC", 13);
	memcpy(&rR2, "MX,0,SRI-NIC.ARPA.,", 19);
	rrs = createResRec(rR2, &dTtl, &dClass);
	if(rrs != NULL)
	{
		addTrie(root, domNme, rrs);
	}
	rrs = NULL;

	memset(&domNme, '\0', DNM_SZ);
	memset(&rR2, '\0', LNE_SZ);
	memcpy(&domNme, ".ARPA.SRI-NIC", 13);
	memcpy(&rR2, "HINFO,DEC-2060,TOPS20,", 22);
	rrs = createResRec(rR2, &dTtl, &dClass);
	if(rrs != NULL)
	{
		addTrie(root, domNme, rrs);
	}
	rrs = NULL;

	memset(&domNme, '\0', DNM_SZ);
	memset(&rR2, '\0', LNE_SZ);
	memcpy(&domNme, ".ARPA.ACC", 9);
	memcpy(&rR2, "A,26.6.0.65,", 12);
	rrs = createResRec(rR2, &dTtl, &dClass);
	if(rrs != NULL)
	{
		addTrie(root, domNme, rrs);
	}
	rrs = NULL;

	memset(&domNme, '\0', DNM_SZ);
	memset(&rR2, '\0', LNE_SZ);
	memcpy(&domNme, ".ARPA.ACC", 9);
	memcpy(&rR2, "HINFO,PDP-11/70,UNIX,", 21);
	rrs = createResRec(rR2, &dTtl, &dClass);
	if(rrs != NULL)
	{
		addTrie(root, domNme, rrs);
	}
	rrs = NULL;

	memset(&domNme, '\0', DNM_SZ);
	memset(&rR2, '\0', LNE_SZ);
	memcpy(&domNme, ".ARPA.ACC", 9);
	memcpy(&rR2, "MX,10,ACC.ARPA.,", 16);
	rrs = createResRec(rR2, &dTtl, &dClass);
	if(rrs != NULL)
	{
		addTrie(root, domNme, rrs);
	}
	rrs = NULL;

	memset(&domNme, '\0', DNM_SZ);
	memset(&rR2, '\0', LNE_SZ);
	memcpy(&domNme, ".ARPA.USC-ISIC", 14);
	memcpy(&rR2, "CNAME,C.ISI.EDU.,", 17);
	rrs = createResRec(rR2, &dTtl, &dClass);
	if(rrs != NULL)
	{
		addTrie(root, domNme, rrs);
	}
	rrs = NULL;

	memset(&domNme, '\0', DNM_SZ);
	memset(&rR2, '\0', LNE_SZ);
	memcpy(&domNme, ".MIL.ASDF", 9);
	memcpy(&rR2, "CNAME,ARMY.MIL.,", 16);
	rrs = createResRec(rR2, &dTtl, &dClass);
	if(rrs != NULL)
	{
		addTrie(root, domNme, rrs);
	}
	rrs = NULL;

	memset(&domNme, '\0', DNM_SZ);
	memset(&rR2, '\0', LNE_SZ);
	memcpy(&domNme, ".EDU.ISI.A", 10);
	memcpy(&rR2, "86400,A,26.3.0.103,", 19);
	rrs = createResRec(rR2, &dTtl, &dClass);
	if(rrs != NULL)
	{
		addTrie(root, domNme, rrs);
	}
	rrs = NULL;

	memset(&domNme, '\0', DNM_SZ);
	memset(&rR2, '\0', LNE_SZ);
	memcpy(&domNme, ".EDU.ISI.C", 10);
	memcpy(&rR2, "86400,A,10.0.0.52,", 18);
	rrs = createResRec(rR2, &dTtl, &dClass);
	if(rrs != NULL)
	{
		addTrie(root, domNme, rrs);
	}
	rrs = NULL;

	return root;
}
