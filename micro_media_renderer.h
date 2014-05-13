#ifndef _HOME_GSALUJA_PROJECTS_COOLANT_UPNP_LIBUPNP-1.6.19_UPNP_SAMPLE_COMMON_MICRO_MEDIA_RENDERER.H
#define _HOME_GSALUJA_PROJECTS_COOLANT_UPNP_LIBUPNP-1.6.19_UPNP_SAMPLE_COMMON_MICRO_MEDIA_RENDERER.H

#include <stdio.h>
#include <signal.h>

#include "sample_util.h"
#include "ithread.h"
#include "upnp.h"

#include <stdlib.h>
#include <string.h>

#define DESC_URL_SIZE 200
#define DEFAULT_WEB_DIR "./web"

/*!
 * \brief Function that receives commands from the user at the command prompt
 * during the lifetime of the device, and calls the appropriate
 * functions for those commands. Only one command, exit, is currently
 * defined.
 */
void *MicroMediaRendererCommandLoop(void *args);

/*!
 * \brief Stops the device. Uninitializes the sdk.
 */
int MicroMediaRendererStop(void);

/*!
 * \brief Initializes the UPnP Sdk, registers the device, and sends out
 * advertisements.
 */
int MicroMediaRendererStart(
	/*! [in] ip address to initialize the sdk (may be NULL)
	 * if null, then the first non null loopback address is used. */
	char *ip_address,
	/*! [in] port number to initialize the sdk (may be 0)
	 * if zero, then a random number is used. */
	unsigned short port,
	/*! [in] name of description document.
	 * may be NULL. Default is tvdevicedesc.xml. */
	const char *desc_doc_name,
	/*! [in] path of web directory.
	 * may be NULL. Default is ./web (for Linux) or ../tvdevice/web. */
	const char *web_dir_path,
	/*! [in] print function to use. */
	print_string pfun);

/*!
 * \brief The callback handler registered with the SDK while registering
 * root device.
 *
 * Dispatches the request to the appropriate procedure
 * based on the value of EventType. The four requests handled by the 
 * device are: 
 *	\li 1) Event Subscription requests.  
 *	\li 2) Get Variable requests. 
 *	\li 3) Action requests.
 */
int RendererCallbackEventHandler(
	/*! [in] The type of callback event. */
	Upnp_EventType,
	/*! [in] Data structure containing event data. */
	void *Event,
	/*! [in] Optional data specified during callback registration. */
	void *Cookie);


#endif // _HOME_GSALUJA_PROJECTS_COOLANT_UPNP_LIBUPNP-1.6.19_UPNP_SAMPLE_COMMON_MICRO_MEDIA_RENDERER.H
