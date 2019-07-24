/*
 * AdapterInfo.h
 *
 *      Author: jgu
 */


#ifndef TESTSRC_MIKEJYG_ADAPTERINFO_H_
#define TESTSRC_MIKEJYG_ADAPTERINFO_H_

#ifdef _WIN32

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <iphlpapi.h>

#include <cstdio>

namespace mikejyg {

class AdapterInfo {
public:
	AdapterInfo() {}

	virtual ~AdapterInfo() {}

	/******************************************************************************\
	*       This is a part of the Microsoft Source Code Samples.
	*       Copyright 1996 - 2000 Microsoft Corporation.
	*       All rights reserved.
	*       This source code is only intended as a supplement to
	*       Microsoft Development Tools and/or WinHelp documentation.
	*       See these sources for detailed information regarding the
	*       Microsoft samples programs.
	\******************************************************************************/

	/*
	Module Name:
	    Ipconfig.cpp
	Abstract:
	    This module illustrates how to programmatically retrieve IP configuration
	    information similar to the IPCONFIG.EXE utility.  It demonstrates how to use
	    the IP Helper APIs GetNetworkParams() and GetAdaptersInfo().
	    To execute this application, simply build the application using the Microsoft Visual C++
	    nmake.exe program generation utility to make an executable ipconfig.exe.  After the
	    build is complete, simply execute the resulting ipconfig.exe program.
	Author:
	    Jim Ohlund 21-Apr-98
	Revision History:
	    stbaker 13-Dec-2006
	*/
	static void ipconfig(void) {

	    DWORD Err;

	    PFIXED_INFO pFixedInfo;
	    DWORD FixedInfoSize = 0;

	    PIP_ADAPTER_INFO pAdapterInfo, pAdapt;
	    DWORD AdapterInfoSize;
	    PIP_ADDR_STRING pAddrStr;

	    UINT i;

	    struct tm newtime;
	    char buffer[32];
	    errno_t error;

	    //
	    // Get the main IP configuration information for this machine using a FIXED_INFO structure
	    //
	    if ((Err = GetNetworkParams(NULL, &FixedInfoSize)) != 0)
	    {
	        if (Err != ERROR_BUFFER_OVERFLOW)
	        {
	            printf("GetNetworkParams sizing failed with error %d\n", Err);
	            return;
	        }
	    }

	    // Allocate memory from sizing information
	    if ((pFixedInfo = (PFIXED_INFO) GlobalAlloc(GPTR, FixedInfoSize)) == NULL)
	    {
	        printf("Memory allocation error\n");
	        return;
	    }

	    if ((Err = GetNetworkParams(pFixedInfo, &FixedInfoSize)) == 0)
	    {
	        printf("\tHost Name . . . . . . . . . : %s\n", pFixedInfo->HostName);
	        printf("\tDNS Servers . . . . . . . . : %s\n", pFixedInfo->DnsServerList.IpAddress.String);
	        pAddrStr = pFixedInfo->DnsServerList.Next;
	        while(pAddrStr)
	        {
	            printf("%51s\n", pAddrStr->IpAddress.String);
	            pAddrStr = pAddrStr->Next;
	        }

	        printf("\tNode Type . . . . . . . . . : ");
	        switch (pFixedInfo->NodeType)
	        {
	            case 1:
	                printf("%s\n", "Broadcast");
	                break;
	            case 2:
	                printf("%s\n", "Peer to peer");
	                break;
	            case 4:
	                printf("%s\n", "Mixed");
	                break;
	            case 8:
	                printf("%s\n", "Hybrid");
	                break;
	            default:
	                printf("\n");
	        }

	        printf("\tNetBIOS Scope ID. . . . . . : %s\n", pFixedInfo->ScopeId);
	        printf("\tIP Routing Enabled. . . . . : %s\n", (pFixedInfo->EnableRouting ? "yes" : "no"));
	        printf("\tWINS Proxy Enabled. . . . . : %s\n", (pFixedInfo->EnableProxy ? "yes" : "no"));
	        printf("\tNetBIOS Resolution Uses DNS : %s\n", (pFixedInfo->EnableDns ? "yes" : "no"));
	    } else
	    {
	        printf("GetNetworkParams failed with error %d\n", Err);
	        return;
	    }

	    //
	    // Enumerate all of the adapter specific information using the IP_ADAPTER_INFO structure.
	    // Note:  IP_ADAPTER_INFO contains a linked list of adapter entries.
	    //
	    AdapterInfoSize = 0;
	    if ((Err = GetAdaptersInfo(NULL, &AdapterInfoSize)) != 0)
	    {
	        if (Err != ERROR_BUFFER_OVERFLOW)
	        {
	            printf("GetAdaptersInfo sizing failed with error %d\n", Err);
	            return;
	        }
	    }

	    // Allocate memory from sizing information
	    if ((pAdapterInfo = (PIP_ADAPTER_INFO) GlobalAlloc(GPTR, AdapterInfoSize)) == NULL)
	    {
	        printf("Memory allocation error\n");
	        return;
	    }

	    // Get actual adapter information
	    if ((Err = GetAdaptersInfo(pAdapterInfo, &AdapterInfoSize)) != 0)
	    {
	        printf("GetAdaptersInfo failed with error %d\n", Err);
	        return;
	    }

	    pAdapt = pAdapterInfo;

	    while (pAdapt)
	    {
	        switch (pAdapt->Type)
	        {
	            case MIB_IF_TYPE_ETHERNET:
	                printf("\nEthernet adapter ");
	                break;
	            case MIB_IF_TYPE_TOKENRING:
	                printf("\nToken Ring adapter ");
	                break;
	            case MIB_IF_TYPE_FDDI:
	                printf("\nFDDI adapter ");
	                break;
	            case MIB_IF_TYPE_PPP:
	                printf("\nPPP adapter ");
	                break;
	            case MIB_IF_TYPE_LOOPBACK:
	                printf("\nLoopback adapter ");
	                break;
	            case MIB_IF_TYPE_SLIP:
	                printf("\nSlip adapter ");
	                break;
	            case MIB_IF_TYPE_OTHER:
	            default:
	                printf("\nOther adapter ");
	        }
	        printf("%s:\n\n", pAdapt->AdapterName);

	        printf("\tDescription . . . . . . . . : %s\n", pAdapt->Description);

	        printf("\tPhysical Address. . . . . . : ");

	        for (i=0; i<pAdapt->AddressLength; i++)
	        {
	            if (i == (pAdapt->AddressLength - 1))
	                printf("%.2X\n",(int)pAdapt->Address[i]);
	            else
	                printf("%.2X-",(int)pAdapt->Address[i]);
	        }

	        printf("\tDHCP Enabled. . . . . . . . : %s\n", (pAdapt->DhcpEnabled ? "yes" : "no"));

	        pAddrStr = &(pAdapt->IpAddressList);
	        while(pAddrStr)
	        {
	            printf("\tIP Address. . . . . . . . . : %s\n", pAddrStr->IpAddress.String);
	            printf("\tSubnet Mask . . . . . . . . : %s\n", pAddrStr->IpMask.String);
	            pAddrStr = pAddrStr->Next;
	        }

	        printf("\tDefault Gateway . . . . . . : %s\n", pAdapt->GatewayList.IpAddress.String);
	        pAddrStr = pAdapt->GatewayList.Next;
	        while(pAddrStr)
	        {
	            printf("%51s\n", pAddrStr->IpAddress.String);
	            pAddrStr = pAddrStr->Next;
	        }

	        printf("\tDHCP Server . . . . . . . . : %s\n", pAdapt->DhcpServer.IpAddress.String);
	        printf("\tPrimary WINS Server . . . . : %s\n", pAdapt->PrimaryWinsServer.IpAddress.String);
	        printf("\tSecondary WINS Server . . . : %s\n", pAdapt->SecondaryWinsServer.IpAddress.String);

	        // Display coordinated universal time - GMT
	#ifdef WIN64
	        error = _localtime64_s(&newtime, &pAdapt->LeaseObtained);
	#else
	        error = _localtime32_s(&newtime, &pAdapt->LeaseObtained);
	#endif
	        if (error)
	        {
	            printf("Invalid Argument to _localtime32_s.");
	        } else {
	            // Convert to an ASCII representation
	            error = asctime_s(buffer, 32, &newtime);
	            if (error)
	            {
	                printf("Invalid Argument to asctime_s.");
	            } else {
	                printf( "\tLease Obtained. . . . . . . : %s", buffer);
	            }
	        }

	#ifdef WIN64
	        error = _localtime64_s(&newtime, &pAdapt->LeaseExpires);
	#else
	        error = _localtime32_s(&newtime, &pAdapt->LeaseExpires);
	#endif
	        if (error)
	        {
	            printf("Invalid Argument to _localtime32_s.");
	        } else {
	            // Convert to an ASCII representation
	            error = asctime_s(buffer, 32, &newtime);
	            if (error)
	            {
	                printf("Invalid Argument to asctime_s.");
	            } else {
	                printf( "\tLease Expires . . . . . . . : %s", buffer);
	            }
	        }

	        pAdapt = pAdapt->Next;
	    }
	}

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

#define WORKING_BUFFER_SIZE 15000
#define MAX_TRIES 3

	static void getAdaptersAddresses() {
		/* Declare and initialize variables */

		DWORD dwSize = 0;
		DWORD dwRetVal = 0;

		unsigned int i = 0;

		// Set the flags to pass to GetAdaptersAddresses
		ULONG flags = GAA_FLAG_INCLUDE_PREFIX;

		// default to unspecified address family (both)
		ULONG family = AF_UNSPEC;

		LPVOID lpMsgBuf = NULL;

		PIP_ADAPTER_ADDRESSES pAddresses = NULL;
		ULONG outBufLen = 0;
		ULONG Iterations = 0;

		PIP_ADAPTER_ADDRESSES pCurrAddresses = NULL;
		PIP_ADAPTER_UNICAST_ADDRESS pUnicast = NULL;
		PIP_ADAPTER_ANYCAST_ADDRESS pAnycast = NULL;
		PIP_ADAPTER_MULTICAST_ADDRESS pMulticast = NULL;
		IP_ADAPTER_DNS_SERVER_ADDRESS *pDnServer = NULL;
		IP_ADAPTER_PREFIX *pPrefix = NULL;

		family = AF_INET;

		printf("Calling GetAdaptersAddresses function with family = ");
		if (family == AF_INET)
			printf("AF_INET\n");
		if (family == AF_INET6)
			printf("AF_INET6\n");
		if (family == AF_UNSPEC)
			printf("AF_UNSPEC\n\n");

		// Allocate a 15 KB buffer to start with.
		outBufLen = WORKING_BUFFER_SIZE;

		do {

			pAddresses = (IP_ADAPTER_ADDRESSES *) MALLOC(outBufLen);
			if (pAddresses == NULL) {
				printf
				("Memory allocation failed for IP_ADAPTER_ADDRESSES struct\n");
				exit(1);
			}

			dwRetVal =
					GetAdaptersAddresses(family, flags, NULL, pAddresses, &outBufLen);

			if (dwRetVal == ERROR_BUFFER_OVERFLOW) {
				FREE(pAddresses);
				pAddresses = NULL;
			} else {
				break;
			}

			Iterations++;

		} while ((dwRetVal == ERROR_BUFFER_OVERFLOW) && (Iterations < MAX_TRIES));

		if (dwRetVal == NO_ERROR) {
			// If successful, output some information from the data we received
			pCurrAddresses = pAddresses;
			while (pCurrAddresses) {
				printf("\tLength of the IP_ADAPTER_ADDRESS struct: %ld\n",
						pCurrAddresses->Length);
				printf("\tIfIndex (IPv4 interface): %u\n", pCurrAddresses->IfIndex);
				printf("\tAdapter name: %s\n", pCurrAddresses->AdapterName);

				pUnicast = pCurrAddresses->FirstUnicastAddress;
				if (pUnicast != NULL) {
					for (i = 0; pUnicast != NULL; i++)
						pUnicast = pUnicast->Next;
					printf("\tNumber of Unicast Addresses: %d\n", i);
				} else
					printf("\tNo Unicast Addresses\n");

				pAnycast = pCurrAddresses->FirstAnycastAddress;
				if (pAnycast) {
					for (i = 0; pAnycast != NULL; i++)
						pAnycast = pAnycast->Next;
					printf("\tNumber of Anycast Addresses: %d\n", i);
				} else
					printf("\tNo Anycast Addresses\n");

				pMulticast = pCurrAddresses->FirstMulticastAddress;
				if (pMulticast) {
					for (i = 0; pMulticast != NULL; i++)
						pMulticast = pMulticast->Next;
					printf("\tNumber of Multicast Addresses: %d\n", i);
				} else
					printf("\tNo Multicast Addresses\n");

				pDnServer = pCurrAddresses->FirstDnsServerAddress;
				if (pDnServer) {
					for (i = 0; pDnServer != NULL; i++)
						pDnServer = pDnServer->Next;
					printf("\tNumber of DNS Server Addresses: %d\n", i);
				} else
					printf("\tNo DNS Server Addresses\n");

				printf("\tDNS Suffix: %wS\n", pCurrAddresses->DnsSuffix);
				printf("\tDescription: %wS\n", pCurrAddresses->Description);
				printf("\tFriendly name: %wS\n", pCurrAddresses->FriendlyName);

				if (pCurrAddresses->PhysicalAddressLength != 0) {
					printf("\tPhysical address: ");
					for (i = 0; i < (int) pCurrAddresses->PhysicalAddressLength;
							i++) {
						if (i == (pCurrAddresses->PhysicalAddressLength - 1))
							printf("%.2X\n",
									(int) pCurrAddresses->PhysicalAddress[i]);
						else
							printf("%.2X-",
									(int) pCurrAddresses->PhysicalAddress[i]);
					}
				}
				printf("\tFlags: %ld\n", pCurrAddresses->Flags);
				printf("\tMtu: %lu\n", pCurrAddresses->Mtu);
				printf("\tIfType: %ld\n", pCurrAddresses->IfType);
				printf("\tOperStatus: %ld\n", pCurrAddresses->OperStatus);
				printf("\tIpv6IfIndex (IPv6 interface): %u\n",
						pCurrAddresses->Ipv6IfIndex);
				printf("\tZoneIndices (hex): ");
				for (i = 0; i < 16; i++)
					printf("%lx ", pCurrAddresses->ZoneIndices[i]);
				printf("\n");

//				printf("\tTransmit link speed: %I64u\n", pCurrAddresses->TransmitLinkSpeed);
//				printf("\tReceive link speed: %I64u\n", pCurrAddresses->ReceiveLinkSpeed);

				pPrefix = pCurrAddresses->FirstPrefix;
				if (pPrefix) {
					for (i = 0; pPrefix != NULL; i++)
						pPrefix = pPrefix->Next;
					printf("\tNumber of IP Adapter Prefix entries: %d\n", i);
				} else
					printf("\tNumber of IP Adapter Prefix entries: 0\n");

				printf("\n");

				pCurrAddresses = pCurrAddresses->Next;
			}
		} else {
			printf("Call to GetAdaptersAddresses failed with error: %d\n",
					dwRetVal);
			if (dwRetVal == ERROR_NO_DATA)
				printf("\tNo addresses were found for the requested parameters\n");
			else {

				if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
						FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
						NULL, dwRetVal, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
						// Default language
						(LPTSTR) & lpMsgBuf, 0, NULL)) {
					printf("\tError: %s", lpMsgBuf);
					LocalFree(lpMsgBuf);
					if (pAddresses)
						FREE(pAddresses);
					exit(1);
				}
			}
		}

		if (pAddresses) {
			FREE(pAddresses);
		}

	}


};

} /* namespace mikejyg */

#endif	// _WIN32

#endif /* TESTSRC_MIKEJYG_ADAPTERINFO_H_ */
