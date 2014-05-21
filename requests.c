#include "requests.h"
#include "micro_media_renderer.h"


/*! Device handle supplied by UPnP SDK. */
UpnpDevice_Handle device_handle = -1;

int TvDeviceHandleSubscriptionRequest(struct Upnp_Subscription_Request *sr_event)
{
	unsigned int i = 0;
	int cmp1 = 0;
	int cmp2 = 0;
	const char *l_serviceId = NULL;
	const char *l_udn = NULL;
	const char *l_sid = NULL;

	/* lock state mutex */
	ithread_mutex_lock(&MicroMediaRendererMutex);

	l_serviceId = sr_event->ServiceId;
	l_udn = sr_event->UDN;
	l_sid = sr_event->Sid;
	for (i = 0; i < TV_SERVICE_SERVCOUNT; ++i) {
		cmp1 = strcmp(l_udn, tv_service_table[i].UDN);
		cmp2 = strcmp(l_serviceId, tv_service_table[i].ServiceId);
		if (cmp1 == 0 && cmp2 == 0) {
#if 0
			PropSet = NULL;

			for (j = 0; j < tv_service_table[i].VariableCount; ++j) {
				/* add each variable to the property set */
				/* for initial state dump */
				UpnpAddToPropertySet(&PropSet,
						     tv_service_table[i].
						     VariableName[j],
						     tv_service_table[i].
						     VariableStrVal[j]);
			}

			/* dump initial state  */
			UpnpAcceptSubscriptionExt(device_handle,
						  l_udn,
						  l_serviceId, PropSet, l_sid);
			/* free document */
			Document_free(PropSet);
#endif
			UpnpAcceptSubscription(device_handle,
					       l_udn,
					       l_serviceId,
					       (const char **)
					       tv_service_table[i].VariableName,
					       (const char **)
					       tv_service_table
					       [i].VariableStrVal,
					       tv_service_table[i].
					       VariableCount, l_sid);
		}
	}

	ithread_mutex_unlock(&MicroMediaRendererMutex);

	return 1;
}

int TvDeviceHandleGetVarRequest(struct Upnp_State_Var_Request *cgv_event)
{
	unsigned int i = 0;
	int j = 0;
	int getvar_succeeded = 0;

	cgv_event->CurrentVal = NULL;

	ithread_mutex_lock(&MicroMediaRendererMutex);

	for (i = 0; i < TV_SERVICE_SERVCOUNT; i++) {
		/* check udn and service id */
		const char *devUDN = cgv_event->DevUDN;
		const char *serviceID = cgv_event->ServiceID;
		if (strcmp(devUDN, tv_service_table[i].UDN) == 0 &&
		    strcmp(serviceID, tv_service_table[i].ServiceId) == 0) {
			/* check variable name */
			for (j = 0; j < tv_service_table[i].VariableCount; j++) {
				const char *stateVarName =
					cgv_event->StateVarName;
				if (strcmp(stateVarName,
					   tv_service_table[i].VariableName[j]) == 0) {
					getvar_succeeded = 1;
					cgv_event->CurrentVal = ixmlCloneDOMString(
						tv_service_table[i].VariableStrVal[j]);
					break;
				}
			}
		}
	}
	if (getvar_succeeded) {
		cgv_event->ErrCode = UPNP_E_SUCCESS;
	} else {
		SampleUtil_Print("Error in UPNP_CONTROL_GET_VAR_REQUEST callback:\n"
			"   Unknown variable name = %s\n",
			cgv_event->StateVarName);
		cgv_event->ErrCode = 404;
		strcpy(cgv_event->ErrStr, "Invalid Variable");
	}

	ithread_mutex_unlock(&MicroMediaRendererMutex);

	return cgv_event->ErrCode == UPNP_E_SUCCESS;
}

int TvDeviceHandleActionRequest(struct Upnp_Action_Request *ca_event)
{
	/* Defaults if action not found. */
	int action_found = 0;
	int i = 0;
	int service = -1;
	int retCode = 0;
	const char *errorString = NULL;
	const char *devUDN = NULL;
	const char *serviceID = NULL;
	const char *actionName = NULL;

	ca_event->ErrCode = 0;
	ca_event->ActionResult = NULL;

	devUDN     = ca_event->DevUDN;
	serviceID  = ca_event->ServiceID;
	actionName = ca_event->ActionName;
	if (strcmp(devUDN,    tv_service_table[SERVICE_RENDERING_CONTROL].UDN) == 0 &&
	    strcmp(serviceID, tv_service_table[SERVICE_RENDERING_CONTROL].ServiceId) == 0) {
		/* Request for action in the TvDevice Control Service. */
		service = SERVICE_RENDERING_CONTROL;
	} else if (strcmp(devUDN,       tv_service_table[SERVICE_AV_TRANSPORT].UDN) == 0 &&
		   strcmp(serviceID, tv_service_table[SERVICE_AV_TRANSPORT].ServiceId) == 0) {
		/* Request for action in the TvDevice Picture Service. */
		service = SERVICE_AV_TRANSPORT;
	} else if (strcmp(devUDN,       tv_service_table[SERVICE_CONNECTION_MANAGER].UDN) == 0 &&
		   strcmp(serviceID, tv_service_table[SERVICE_CONNECTION_MANAGER].ServiceId) == 0) {
		/* Request for action in the TvDevice Picture Service. */
		service = SERVICE_CONNECTION_MANAGER;
	}

	/* Find and call appropriate procedure based on action name.
	 * Each action name has an associated procedure stored in the
	 * service table. These are set at initialization. */
	for (i = 0;
	     i < TV_MAXACTIONS && tv_service_table[service].ActionNames[i] != NULL;
	     i++) {
		if (!strcmp(actionName, tv_service_table[service].ActionNames[i])) {
				retCode = tv_service_table[service].actions[i](
					ca_event->ActionRequest,
					&ca_event->ActionResult,
					&errorString);
			action_found = 1;
			break;
		}
	}
  if (action_found != 1)
  {
      printf ("Need to implement : %s\n" , actionName);
  }

	if (!action_found) {
		ca_event->ActionResult = NULL;
		strcpy(ca_event->ErrStr, "Invalid Action");
		ca_event->ErrCode = 401;
	} else {
		if (retCode == UPNP_E_SUCCESS) {
			ca_event->ErrCode = UPNP_E_SUCCESS;
		} else {
			/* copy the error string */
			strcpy(ca_event->ErrStr, errorString);
			switch (retCode) {
			case UPNP_E_INVALID_PARAM:
				ca_event->ErrCode = 402;
				break;
			case UPNP_E_INTERNAL_ERROR:
			default:
				ca_event->ErrCode = 501;
				break;
			}
		}
	}

	return ca_event->ErrCode;
}



