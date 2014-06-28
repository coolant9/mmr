#ifndef _HOME_GSALUJA_PROJECTS_COOLANT_UPNP_MICRO_MEDIA_RENDERER_REQUESTS_H
#define _HOME_GSALUJA_PROJECTS_COOLANT_UPNP_MICRO_MEDIA_RENDERER_REQUESTS_H

#include "common.h"
#include "upnp.h"

//from common.h
extern ithread_mutex_t MicroMediaRendererMutex;

/*! Device handle supplied by UPnP SDK. */
extern UpnpDevice_Handle device_handle;

/*!
 * \brief Prototype for all actions. For each action that a service 
 * implements, there is a corresponding function with this prototype.
 *
 * Pointers to these functions, along with action names, are stored
 * in the service table. When an action request comes in the action
 * name is matched, and the appropriate function is called.
 * Each function returns UPNP_E_SUCCESS, on success, and a nonzero 
 * error code on failure.
 */
typedef int (*upnp_action)(
	/*! [in] Document of action request. */
	IXML_Document *request,
	/*! [out] Action result. */
	IXML_Document **out,
	/*! [out] Error string in case action was unsuccessful. */
	const char **errorString);

/*! Structure for storing Mmr Service identifiers and state table. */
struct RendererService {
	/*! Universally Unique Device Name. */
	char UDN[NAME_SIZE];
	/*! . */
	char ServiceId[NAME_SIZE];
	/*! . */
	char ServiceType[NAME_SIZE];
	/*! . */
	const char *VariableName[BUFF_MAXVARS];
	/*! . */
	char *VariableStrVal[BUFF_MAXVARS];
	/*! . */
	const char *ActionNames[BUFF_MAXACTIONS];
	/*! . */
	upnp_action actions[BUFF_MAXACTIONS];
	/*! . */
	int VariableCount;
};


/*! Global structure for storing the state table for this device. */
struct RendererService mmr_service_table[3];

/*!
 * \brief Called during a subscription request callback.
 *
 * If the subscription request is for this device and either its
 * AVTransport, ConnectionManager or RenderingControl
 */
int MmrDeviceHandleSubscriptionRequest(
	/*! [in] The subscription request event structure. */
	struct Upnp_Subscription_Request *sr_event);

/*!
 * \brief Called during a get variable request callback.
 *
 * If the request is for this device and either its AVTransport,
 * ConnectionManager or RenderingControl, then respond with the variable value.
 */
int MmrDeviceHandleGetVarRequest(
	/*! [in,out] The control get variable request event structure. */
	struct Upnp_State_Var_Request *cgv_event);

/*!
 * \brief Called during an action request callback.
 *
 * If the request is for this device and either its AVTransport
 * RenderingControl or ConnectionManager, then perform the action and respond.
 */
int MmrDeviceHandleActionRequest(
	/*! [in,out] The control action request event structure. */
	struct Upnp_Action_Request *ca_event);

#endif // _HOME_GSALUJA_PROJECTS_COOLANT_UPNP_MICRO_MEDIA_RENDERER_REQUESTS_H
