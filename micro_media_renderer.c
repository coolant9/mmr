#include <assert.h>
#include "micro_media_renderer.h"
#include "common.h"
#include "requests.h"
#include "actions.h"
#include "config_parser.h"

//from common.h
extern ithread_mutex_t MicroMediaRendererMutex;


/*! Global arrays for storing Rendering Control Service variable names, values defaults. */
const char *rc_varname[] = {
  "A_ARG_TYPE_InstanceID",
  "Mute",
  "PresetNameList",
  "Volume",
  "A_ARG_TYPE_Channel",
  "LastChange",
  "A_ARG_TYPE_PresetName" };

char rc_varval[SVC_RENDERING_CONTROL_VARCOUNT][TV_MAX_VAL_LEN];
const char *rc_varval_def[] = { "0", "0", "5", "20", "Master", "LC", "0" };

/*! Global arrays for storing AVTransport Service variable names, values,
 * and defaults. */
const char *avt_varname[] = {
  "CurrentTransportActions",
  "PossibleRecordQualityModes",
  "PlaybackStorageMedium",
  "AbsoluteCounterPosition",
  "A_ARG_TYPE_SeekTarget",
  "CurrentMediaDuration",
  "AVTransportURIMetaData",
  "CurrentTrackMetaData",
  "AbsoluteTimePosition",
  "TransportStatus",
  "A_ARG_TYPE_SeekMode",
  "RelativeTimePosition",
  "CurrentTrackDuration",
  "LastChange",
  "RecordMediumWriteStatus",
  "TransportState",
  "PossibleRecordStorageMedia",
  "A_ARG_TYPE_InstanceID",
  "NextAVTransportURI",
  "NextAVTransportURIMetaData",
  "CurrentPlayMode",
  "NumberOfTracks",
  "CurrentRecordQualityMode",
  "PossiblePlaybackStorageMedia",
  "CurrentTrack",
  "RecordStorageMedium",
  "AVTransportURI",
  "CurrentTrackURI",
  "RelativeCounterPosition",
  "TransportPlaySpeed"
};

char avt_varval[SVC_AV_TRANSPORT_VARCOUNT][TV_MAX_VAL_LEN];
const char *avt_varval_def[] = {
"Play,Stop", "5", "5", "5", "5",
"5", "5", "5", "5", "OK",
"5", "5", "", "5", "5",
"NO_MEDIA_PRESENT", "5", "5", "5", "5",
"NORMAL", "5", "5", "5", "5",
"5", "5", "5", "5", "1"
};


const char *cm_varname[] = {
  "SinkProtocolInfo",
  "A_ARG_TYPE_RcsID",
  "A_ARG_TYPE_ConnectionManager",
  "A_ARG_TYPE_Direction",
  "CurrentConnectionIDs",
  "A_ARG_TYPE_AVTransportID",
  "A_ARG_TYPE_ProtocolInfo",
  "A_ARG_TYPE_ConnectionStatus",
  "A_ARG_TYPE_ConnectionID",
  "SourceProtocolInfo",
};
char cm_varval[SVC_CONNECTION_MANAGER_VARCOUNT][TV_MAX_VAL_LEN];
const char *cm_varval_def[] = {
"http-get:*:audio/mpeg:*",
"5", "5", "5", "5",
"Play,Stop", "5", "5", "5",
"http-get:*:audio/mpeg:*"
};

/*! The amount of time (in seconds) before advertisements will expire. */
int default_advr_expire = 100;



/*!
 * \brief Initializes the service table for the specified service.
 */
static int SetServiceTable(
	/*! [in] one of SERVICE_RENDERING_CONTROL or, SERVICE_AV_TRANSPORT. */
	int serviceType,
	/*! [in] UDN of device containing service. */
	const char *UDN,
	/*! [in] serviceId of service. */
	const char *serviceId,
	/*! [in] service type (as specified in Description Document) . */
	const char *serviceTypeS,
	/*! [in,out] service containing table to be set. */
	struct RendererService *out)
{
	int i = 0;

	strcpy(out->UDN, UDN);
	strcpy(out->ServiceId, serviceId);
	strcpy(out->ServiceType, serviceTypeS);

	switch (serviceType) {
	case SERVICE_RENDERING_CONTROL:
		out->VariableCount = SVC_RENDERING_CONTROL_VARCOUNT;
		for (i = 0;
		     i < tv_service_table[SERVICE_RENDERING_CONTROL].VariableCount;
		     i++) {
			tv_service_table[SERVICE_RENDERING_CONTROL].VariableName[i]
			    = rc_varname[i];
			tv_service_table[SERVICE_RENDERING_CONTROL].VariableStrVal[i]
			    = rc_varval[i];
			strcpy(tv_service_table[SERVICE_RENDERING_CONTROL].
				VariableStrVal[i], rc_varval_def[i]);
		}
		break;
	case SERVICE_AV_TRANSPORT:
		out->VariableCount = SVC_AV_TRANSPORT_VARCOUNT;
		for (i = 0;
		     i < tv_service_table[SERVICE_AV_TRANSPORT].VariableCount;
		     i++) {
			tv_service_table[SERVICE_AV_TRANSPORT].VariableName[i] =
			    avt_varname[i];
			tv_service_table[SERVICE_AV_TRANSPORT].VariableStrVal[i] =
			    avt_varval[i];
			strcpy(tv_service_table[SERVICE_AV_TRANSPORT].
				VariableStrVal[i], avt_varval_def[i]);
		}
		break;
	case SERVICE_CONNECTION_MANAGER:
		out->VariableCount = SVC_CONNECTION_MANAGER_VARCOUNT;
		for (i = 0;
		     i < tv_service_table[SERVICE_CONNECTION_MANAGER].VariableCount;
		     i++) {
			tv_service_table[SERVICE_CONNECTION_MANAGER].VariableName[i] =
			    cm_varname[i];
			tv_service_table[SERVICE_CONNECTION_MANAGER].VariableStrVal[i] =
			    cm_varval[i];
			strcpy(tv_service_table[SERVICE_CONNECTION_MANAGER].
				VariableStrVal[i], cm_varval_def[i]);
		}
		break;

	default:
		assert(0);
	}

	return SetActionTable(serviceType, out);
}


int TvDeviceStateTableInit(char *DescDocURL)
{
	IXML_Document *DescDoc = NULL;
	int ret = UPNP_E_SUCCESS;
	char *servid_rc = NULL;
	char *evnturl_rc = NULL;
	char *ctrlurl_rc = NULL;

	char *servid_avt = NULL;
	char *evnturl_avt = NULL;
	char *ctrlurl_avt = NULL;

  char *servid_cm = NULL;
  char *evnturl_cm = NULL;
  char * ctrlurl_cm = NULL;

	char *udn = NULL;

	/*Download description document */
	if (UpnpDownloadXmlDoc(DescDocURL, &DescDoc) != UPNP_E_SUCCESS) {
		SampleUtil_Print("TvDeviceStateTableInit -- Error Parsing %s\n",
				 DescDocURL);
		ret = UPNP_E_INVALID_DESC;
		goto error_handler;
	}
	udn = SampleUtil_GetFirstDocumentItem(DescDoc, "UDN");
	/* Find the Tv Control Service identifiers */
	if (!SampleUtil_FindAndParseService(DescDoc, DescDocURL,
					    TvServiceType[SERVICE_RENDERING_CONTROL],
					    &servid_rc, &evnturl_rc,
					    &ctrlurl_rc)) {
		SampleUtil_Print("TvDeviceStateTableInit -- Error: Could not find Service: %s\n",
				 TvServiceType[SERVICE_RENDERING_CONTROL]);
		ret = UPNP_E_INVALID_DESC;
		goto error_handler;
	}
	/* set control service table */
	SetServiceTable(SERVICE_RENDERING_CONTROL, udn, servid_rc,
			TvServiceType[SERVICE_RENDERING_CONTROL],
			&tv_service_table[SERVICE_RENDERING_CONTROL]);

	/* Find the Tv Picture Service identifiers */
	if (!SampleUtil_FindAndParseService(DescDoc, DescDocURL,
					    TvServiceType[SERVICE_AV_TRANSPORT],
					    &servid_avt, &evnturl_avt,
					    &ctrlurl_avt)) {
		SampleUtil_Print("TvDeviceStateTableInit -- Error: Could not find Service: %s\n",
				 TvServiceType[SERVICE_AV_TRANSPORT]);
		ret = UPNP_E_INVALID_DESC;
		goto error_handler;
	}
	/* set picture service table */
	SetServiceTable(SERVICE_AV_TRANSPORT, udn, servid_avt,
			TvServiceType[SERVICE_AV_TRANSPORT],
			&tv_service_table[SERVICE_AV_TRANSPORT]);

	/* Find the Tv Picture Service identifiers */
	if (!SampleUtil_FindAndParseService(DescDoc, DescDocURL,
					    TvServiceType[SERVICE_CONNECTION_MANAGER],
					    &servid_cm, &evnturl_cm,
					    &ctrlurl_cm)) {
		SampleUtil_Print("TvDeviceStateTableInit -- Error: Could not find Service: %s\n",
				 TvServiceType[SERVICE_CONNECTION_MANAGER]);
		ret = UPNP_E_INVALID_DESC;
		goto error_handler;
	}
	/* set picture service table */
	SetServiceTable(SERVICE_CONNECTION_MANAGER, udn, servid_cm,
			TvServiceType[SERVICE_CONNECTION_MANAGER],
			&tv_service_table[SERVICE_CONNECTION_MANAGER]);


error_handler:
	/* clean up */
	if (udn)
		free(udn);
	if (servid_rc)
		free(servid_rc);
	if (evnturl_rc)
		free(evnturl_rc);
	if (ctrlurl_rc)
		free(ctrlurl_rc);
	if (servid_avt)
		free(servid_avt);
	if (evnturl_avt)
		free(evnturl_avt);
	if (ctrlurl_avt)
		free(ctrlurl_avt);
  if(servid_cm)
    free(servid_cm);
  if(evnturl_cm)
    free(evnturl_cm);
  if(ctrlurl_cm)
    free(ctrlurl_cm);
	if (DescDoc)
		ixmlDocument_free(DescDoc);

	return (ret);
}

int RendererCallbackEventHandler(Upnp_EventType EventType, void *Event,
				 void *Cookie)
{
	switch (EventType) {
	case UPNP_EVENT_SUBSCRIPTION_REQUEST:
		TvDeviceHandleSubscriptionRequest((struct
						   Upnp_Subscription_Request *)
						  Event);
		break;
	case UPNP_CONTROL_GET_VAR_REQUEST:
		TvDeviceHandleGetVarRequest((struct Upnp_State_Var_Request *)
					    Event);
		break;
	case UPNP_CONTROL_ACTION_REQUEST:
		TvDeviceHandleActionRequest((struct Upnp_Action_Request *)
					    Event);
		break;
		/* ignore these cases, since this is not a control point */
	case UPNP_DISCOVERY_ADVERTISEMENT_ALIVE:
	case UPNP_DISCOVERY_SEARCH_RESULT:
	case UPNP_DISCOVERY_SEARCH_TIMEOUT:
	case UPNP_DISCOVERY_ADVERTISEMENT_BYEBYE:
	case UPNP_CONTROL_ACTION_COMPLETE:
	case UPNP_CONTROL_GET_VAR_COMPLETE:
	case UPNP_EVENT_RECEIVED:
	case UPNP_EVENT_RENEWAL_COMPLETE:
	case UPNP_EVENT_SUBSCRIBE_COMPLETE:
	case UPNP_EVENT_UNSUBSCRIBE_COMPLETE:
		break;
	default:
		SampleUtil_Print
		    ("Error in TvDeviceCallbackEventHandler: unknown event type %d\n",
		     EventType);
	}
	/* Print a summary of the event received */
	SampleUtil_PrintEvent(EventType, Event);

	return 0;
	Cookie = Cookie;
}

int media_renderer_main(int argc, char *argv[])
{
	unsigned int portTemp = 0;
	char *ip_address = NULL;
	char *desc_doc_name = NULL;
	char *web_dir_path = NULL;
	unsigned short port = 0;
	int i = 0;

	SampleUtil_Initialize(linux_print);
  config = parse_command_line_parameters(argc, argv);
  if (config==NULL)
  {
    return 1;
  }
  return MicroMediaRendererStart(
      config->ip_address,
      config->port,
      "device-description.xml",
      config->webdir,
      linux_print);
}

//Called by renderer main
int MicroMediaRendererStart(char *ip_address, unsigned short port,
		  const char *desc_doc_name, const char *web_dir_path,
		  print_string pfun)
{
	int ret = UPNP_E_SUCCESS;
	char desc_doc_url[DESC_URL_SIZE];

	ithread_mutex_init(&MicroMediaRendererMutex, NULL);

	SampleUtil_Initialize(pfun);
	SampleUtil_Print("Initializing UPnP Sdk with\n"
			 "\tipaddress = %s port = %u\n",
			 ip_address ? ip_address : "{NULL}", port);
	ret = UpnpInit(ip_address, port);
	if (ret != UPNP_E_SUCCESS) {
		SampleUtil_Print("Error with UpnpInit -- %d\n", ret);
		UpnpFinish();

		return ret;
	}
	ip_address = UpnpGetServerIpAddress();
	port = UpnpGetServerPort();
	SampleUtil_Print("UPnP Initialized\n"
			 "\tipaddress = %s port = %u\n",
			 ip_address ? ip_address : "{NULL}", port);
	if (!desc_doc_name) {
		desc_doc_name = "device-description.xml";
	}
	if (!web_dir_path) {
		web_dir_path = DEFAULT_WEB_DIR;
	}
	snprintf(desc_doc_url, DESC_URL_SIZE, "http://%s:%d/%s", ip_address,
		 port, desc_doc_name);
	SampleUtil_Print("Specifying the webserver root directory -- %s\n",
			 web_dir_path);
	ret = UpnpSetWebServerRootDir(web_dir_path);
	if (ret != UPNP_E_SUCCESS) {
		SampleUtil_Print
		    ("Error specifying webserver root directory -- %s: %d\n",
		     web_dir_path, ret);
		UpnpFinish();

		return ret;
	}
	SampleUtil_Print("Registering the RootDevice\n"
			 "\t with desc_doc_url: %s\n", desc_doc_url);
	ret = UpnpRegisterRootDevice(desc_doc_url, RendererCallbackEventHandler,
				     &device_handle, &device_handle);
	if (ret != UPNP_E_SUCCESS) {
		SampleUtil_Print("Error registering the rootdevice : %d\n",
				 ret);
		UpnpFinish();

		return ret;
	} else {
		SampleUtil_Print("RootDevice Registered\n"
				 "Initializing State Table\n");
		TvDeviceStateTableInit(desc_doc_url);
		SampleUtil_Print("State Table Initialized\n");
		ret = UpnpSendAdvertisement(device_handle, default_advr_expire);
		if (ret != UPNP_E_SUCCESS) {
			SampleUtil_Print("Error sending advertisements : %d\n",
					 ret);
			UpnpFinish();

			return ret;
		}
		SampleUtil_Print("Advertisements Sent\n");
	}

	return UPNP_E_SUCCESS;
}


void *MicroMediaRendererCommandLoop(void *args)
{
  int stoploop = 0;
  char cmdline[100];
  char cmd[100];

  while (!stoploop)
  {
  // 	sprintf(cmdline, " ");
	//	sprintf(cmd, " ");
	//	SampleUtil_Print("\n>> ");
	//	/* Get a command line */
	//	fgets(cmdline, 100, stdin);
	//	sscanf(cmdline, "%s", cmd);
	//	if (strcasecmp(cmd, "exit") == 0) {
	//		SampleUtil_Print("Shutting down...\n");
	//		MicroMediaRendererStop();
	//		exit(0);
	//	} else {
	//		SampleUtil_Print("\n   Unknown command: %s\n\n", cmd);
	//		SampleUtil_Print("   Valid Commands:\n"
	//				 "     Exit\n\n");
	//	}
  sleep(1);

  }

}

int MicroMediaRendererStop(void)
{
  UpnpUnRegisterRootDevice(device_handle);
  UpnpFinish();
  SampleUtil_Finish();
  ithread_mutex_destroy(&MicroMediaRendererMutex);

  return UPNP_E_SUCCESS;
}
