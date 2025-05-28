/*
 * C
 *
 * Copyright 2015-2023 MicroEJ Corp. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be found with this software.
 *
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file
 * @brief lwip_util implementation over LWIP and FreeRTOS.
 * @author MicroEJ Developer Team
 * @version 0.2.1
 * @date 3 August 2020
 */

#include <string.h>
#include "lwip_util.h"
#include "LLNET_Common.h"


#include "ping.h"
#include "netif/ethernet.h"
#include "ethernetif.h"
#include "lwip/dhcp.h"
#include "lwip/init.h"
#include "lwip/ip_addr.h"
#include "lwip/netifapi.h"
#include "lwip/prot/dhcp.h"
#include "lwip/tcpip.h"
#include "lwip/sys.h"
#include "lwip/dns.h"

#include "pin_mux.h"
#include "board.h"
#ifndef configMAC_ADDR
#include "fsl_silicon_id.h"
#endif
#include "fsl_phy.h"

#include "fsl_enet.h"
#include "fsl_phylan8741.h"

static TaskHandle_t	dhcp_task_handle;

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Ethernet configuration. */
phy_lan8741_resource_t g_phy_resource;
#define ENET_PHY_ADDRESS  BOARD_ENET0_PHY_ADDRESS
#define ENET_PHY_OPS      &phylan8741_ops
#define ENET_PHY_RESOURCE &g_phy_resource
#define ENET_CLOCK_FREQ CLOCK_GetCoreSysClkFreq()

#define ETHERNETIF_INIT ethernetif0_init

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*Initialize physic etherner handler */
static phy_handle_t phyHandle;

#define MAX_DHCP_TRIES  4
#define LWIP_DHCP_TASK_PRIORITY 1

/* DHCP process states */
#define DHCP_START                 (uint8_t) 1
#define DHCP_WAIT_ADDRESS          (uint8_t) 2
#define DHCP_ADDRESS_ASSIGNED      (uint8_t) 3
#define DHCP_TIMEOUT               (uint8_t) 4
#define DHCP_LINK_DOWN             (uint8_t) 5

struct netif gnetif;
uint8_t DHCP_state;

static uint8_t dhcp_sleeping = 1;

/* variable used to notify that DNS servers list has changed */
uint8_t dns_servers_list_updated = 1;

static void ethernetif_static_ip_config(void);

/** @brief Array holding all the network interface names at LwIP level. */
static char lwip_netif[NUMB_OF_NETIF_TO_STORE][MAX_SIZE_OF_NETIF_NAME];

/**
  * @brief  Reset the network interface ip, netmask and gateway addresses to zero.
  * @param  netif: the network interface
  * @retval None
  */
static void netif_addr_set_zero_ip4(struct netif* netif){
	ip_addr_set_zero_ip4(&netif->ip_addr);
	ip_addr_set_zero_ip4(&netif->netmask);
	ip_addr_set_zero_ip4(&netif->gw);
}

/**
  * @brief  This function is called when the network interface is disconnected.
  * @param  netif: the network interface
  * @retval None
  */
void netif_not_connected(struct netif *netif){
	netif_addr_set_zero_ip4(netif);
	LLNET_DEBUG_TRACE("[INFO] The network cable is not connected \n");
}

/**
  * @brief  Notify the User about the nework interface config status
  * @param  netif: the network interface
  * @retval None
  */
static void User_notification(struct netif *netif)
{
	int32_t dhcpConfEnabled = true;
	if (netif_is_up(netif))
	{
		if(dhcpConfEnabled)
		{
			/* Update DHCP state machine */
			DHCP_state = DHCP_START;
		}else{
			// launch static Network Interface configuration
			ethernetif_static_ip_config();
		}
	}
	else
	{
		netif_not_connected(netif);
		if(dhcpConfEnabled)
		{
			/* Update DHCP state machine */
			DHCP_state = DHCP_LINK_DOWN;
		}
	}

}

static void MDIO_Init(void)
{
    (void)CLOCK_EnableClock(s_enetClock[ENET_GetInstance(ENET0)]);
    ENET_SetSMI(ENET0, ENET_CLOCK_FREQ);
}

static status_t MDIO_Write(uint8_t phyAddr, uint8_t regAddr, uint16_t data)
{
    return ENET_MDIOWrite(ENET0, phyAddr, regAddr, data);
}

static status_t MDIO_Read(uint8_t phyAddr, uint8_t regAddr, uint16_t *pData)
{
    return ENET_MDIORead(ENET0, phyAddr, regAddr, pData);
}

/**
 * @brief  Setup the network interface
 * @param  None
 * @retval None
 */
static void Netif_Config(void)
{
	ip_addr_t ipaddr;
	ip_addr_t netmask;
	ip_addr_t gw;

	ip_addr_set_zero_ip4(&ipaddr);
	ip_addr_set_zero_ip4(&netmask);
	ip_addr_set_zero_ip4(&gw);
    MDIO_Init();
    g_phy_resource.read  = MDIO_Read;
    g_phy_resource.write = MDIO_Write;
    ethernetif_config_t enet_config = {
        .phyHandle   = &phyHandle,
        .phyAddr     = ENET_PHY_ADDRESS,
        .phyOps      = ENET_PHY_OPS,
        .phyResource = ENET_PHY_RESOURCE,
        .srcClockHz  = ENET_CLOCK_FREQ,
#ifdef configMAC_ADDR
        .macAddress = configMAC_ADDR,
#endif
    };
#ifndef configMAC_ADDR
    /* Set special address for each chip. */
    (void)SILICONID_ConvertToMacAddr(&enet_config.macAddress);
#endif

	/* Add the network interface */
#if NO_SYS
	netifapi_netif_add(&gnetif, &ipaddr, &netmask, &gw, &enet_config, ETHERNETIF_INIT, &netif_input);
#else
	netifapi_netif_add(&gnetif, &ipaddr, &netmask, &gw,  &enet_config, ETHERNETIF_INIT, &tcpip_input);

#endif

	/* Registers the default network interface */
	netifapi_netif_set_default(&gnetif);
	netifapi_netif_set_up(netif_default);
	/* Set the link callback function, this function is called on change of link status */
	//netif_set_link_callback(&gnetif, ethernetif_config);
	User_notification(&gnetif);
}

/*
 * Retrieve static IP configuration of the default network interface and set
 * IP parameters (Interface IP address, Netmask and Gateway IP address).
 */
static void ethernetif_static_ip_config()
{
	ip_addr_t ipaddr;
	ip_addr_t netmask;
	ip_addr_t gw;
	ip_addr_t dnsaddr;
	PRINTF("getting static address \n");
	// static IP configuration. Retrieve IP settings from user properties.
	ipaddr.addr = 0;
	netmask.addr = 0;
	gw.addr	= 0;
	netif_set_addr(&gnetif, &ipaddr , &netmask, &gw);
	LLNET_DEBUG_TRACE("[INFO] Static IP address assigned: %s\n", inet_ntoa(ipaddr.addr));

	// set static DNS Host IP address.
	if(DNS_MAX_SERVERS > 0)
	{
		char * static_dns_ip_addr = NULL;
		if(static_dns_ip_addr != NULL)
		{
			dnsaddr.addr = inet_addr(static_dns_ip_addr);
			dns_setserver(0, &dnsaddr);
			// notify DNS servers IP address updated
			dns_servers_list_updated = 1;
		}
	}
}

/**
  * @brief  This function notify user about link status changement.
  * @param  netif: the network interface
  * @retval None
  */
void ethernetif_notify_conn_changed(struct netif *netif)
{
	ip_addr_t ipaddr = {0};
	ip_addr_t netmask = {0};
	ip_addr_t gw = {0};
	int32_t dhcpConfEnabled = true;

	if(netif_is_link_up(netif))
	{
		LLNET_DEBUG_TRACE("[INFO] The network cable is now connected \n");

		if(dhcpConfEnabled)
		{
			/* Update DHCP state machine */
			DHCP_state = DHCP_START;

			netif_set_addr(netif, &ipaddr , &netmask, &gw);

			// resume DHCP thread
			dhcp_sleeping = 0;
			vTaskResume(dhcp_task_handle);
		}else{
			// launch static Network Interface configuration
			ethernetif_static_ip_config();
		}

		/* When the netif is fully configured this function must be called.*/
		netif_set_up(netif);
	}
	else
	{
		if(dhcpConfEnabled){
			/* Update DHCP state machine */
			DHCP_state = DHCP_LINK_DOWN;
		}

		/*  When the netif link is down this function must be called.*/
		netif_set_down(netif);

		netif_not_connected(netif);
	}
}

static void DHCP_thread(void const * argument)
{
  struct netif *netif = (struct netif *) argument;
  uint32_t IPaddress;

  for (;;)
  {
	// check if DHCP thread has to suspend
	if(dhcp_sleeping == 1){
		vTaskDelete(dhcp_task_handle);
	}

    switch (DHCP_state)
    {
    case DHCP_START:
      {
        netif_addr_set_zero_ip4(netif);
        IPaddress = 0;
	    netifapi_dhcp_start(netif);
        DHCP_state = DHCP_WAIT_ADDRESS;
        LLNET_DEBUG_TRACE("[INFO] DHCP started\n");
      }
      break;

    case DHCP_WAIT_ADDRESS:
      {
        /* Read the new IP address */
        IPaddress = netif->ip_addr.addr;
        if (IPaddress != 0)
        {
			DHCP_state = DHCP_ADDRESS_ASSIGNED;

			/* Stop DHCP */
#if LWIP_VERSION_MAJOR == 1
			dhcp_stop(netif);
#elif LWIP_VERSION_MAJOR == 2
			// LwIP version 2 onward clears existing IP address if dhcp is stopped.
			//dhcp_stop(netif);
#else
			#error "Invalid LWIP version (LWIP_VERSION_MAJOR)."
#endif
			dhcp_sleeping = 1;

			LLNET_DEBUG_TRACE("[INFO] DHCP address assigned: %s\n", inet_ntoa(IPaddress));
			if(1)
			{
				ip_addr_t dnsaddr;
				if(DNS_MAX_SERVERS > 0)
				{
					char * static_dns_ip_addr = NULL;
					if(static_dns_ip_addr != NULL)
					{
						dnsaddr.addr = inet_addr(static_dns_ip_addr);
						dns_setserver(0, &dnsaddr);
					}
				}
			}

			// notify DNS servers IP address updated
			dns_servers_list_updated = 1;
        }
        else
        {
#if LWIP_VERSION_MAJOR == 1

			struct dhcp *dhcp = netif->dhcp;

#elif LWIP_VERSION_MAJOR == 2
			struct dhcp *dhcp = (struct dhcp*)netif_get_client_data(netif, LWIP_NETIF_CLIENT_DATA_INDEX_DHCP);

#else

	#error "Invalid LWIP version (LWIP_VERSION_MAJOR)."

#endif

			/* DHCP timeout */
			if (dhcp->tries > MAX_DHCP_TRIES)
			{
				DHCP_state = DHCP_TIMEOUT;

				/* Stop DHCP */
				dhcp_stop(netif);
				dhcp_sleeping = 1;

				LLNET_DEBUG_TRACE("[INFO] DHCP timeout\n");
			}
        }
      }
      break;

    default: break;
    }
  }
}

/**
 * Network initialization. Start network interfaces and configure it.
 * @return 0 if no error occurred, error code otherwise.
 */
int32_t llnet_lwip_init(void)
{
	PRINTF ("network initialize \n");
	int32_t dhcpConfEnabled =  true;
	/* Initialize the LwIP TCP/IP stack */
	tcpip_init(NULL, NULL);
	/* Configure the Network interface */
	Netif_Config();
	if(dhcpConfEnabled)
	{
		/* Start DHCPClient */
		dhcp_sleeping = 0;
#if defined(__GNUC__)
		xTaskCreate((TaskFunction_t)DHCP_thread, "DHCP", configMINIMAL_STACK_SIZE * 5, &gnetif, LWIP_DHCP_TASK_PRIORITY, &dhcp_task_handle);
#else
		xTaskCreate((TaskFunction_t)DHCP_thread, "DHCP", configMINIMAL_STACK_SIZE*2, &gnetif, LWIP_DHCP_TASK_PRIORITY, &dhcp_task_handle);
#endif
	}
	return 0;
}

void set_lwip_netif_name(int32_t id, char *netif_name)
{
	strcpy(lwip_netif[id], netif_name);
}

char *get_lwip_netif_name(int32_t id)
{
	return lwip_netif[id];
}

struct netif* getNetworkInterface(int8_t* name)
{
	char *llnet_netif[] = LLNET_NETIF_NAMES;
	for (int32_t netif_idx = 0; netif_idx < (sizeof(llnet_netif) / sizeof(llnet_netif[0])); netif_idx++) {
		if (strcmp((char *)name, llnet_netif[netif_idx]) == 0) {
			// the same interface name at LLNET level can have different names at platform level
			// so, keep trying to find the lwip level interface until one is found or until we exhaust the list
			struct netif *ret = netif_find(lwip_netif[netif_idx]);
			if (ret) {
				return ret;
			}
		}
	}
	return NULL;
}
