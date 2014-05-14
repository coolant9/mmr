#include "actions.h"
#include "stdio.h"
#include "sample_util.h"
#include <sys/types.h>
#include <signal.h>

#include <mpd/client.h>
#include <mpd/status.h>
#include <mpd/song.h>
#include <mpd/entity.h>
#include <mpd/search.h>
#include <mpd/tag.h>


int SetActionTable(int serviceType, struct RendererService *out)
{
	if (serviceType == SERVICE_RENDERING_CONTROL)
  {
		out->ActionNames[0] = "GetMute";
		out->actions[0] = MicroMediaPlay;
		out->ActionNames[1] = "GetVolume";
		out->actions[1] = GetVolume;
		out->ActionNames[2] = "GetVolumeDB";
		out->actions[2] = MicroMediaPlay;
		out->ActionNames[3] = "GetVolumeDBRange";
		out->actions[3] = MicroMediaPlay;
		out->ActionNames[4] = "ListPresets";
		out->actions[4] = MicroMediaPlay;
		out->ActionNames[5] = "SelectPreset";
		out->actions[5] = MicroMediaPlay;
		out->ActionNames[6] = "SetMute";
		out->actions[6] = MicroMediaPlay;
		out->ActionNames[7] = "SetVolume";
		out->actions[7] = SetVolume;
		out->ActionNames[8] = NULL;
		return 1;
	}
  else if (serviceType == SERVICE_CONNECTION_MANAGER)
  {
		out->ActionNames[0] = "GetCurrentConnectionInfo";
		out->ActionNames[1] = "GetProtocolInfo";
		out->ActionNames[2] = "GetCurrentConnectionIDs";
		out->actions[0] = MicroMediaPlay;
		out->actions[1] = MicroMediaPlay;
		out->actions[2] = MicroMediaPlay;
		return 1;
	}
  else if (serviceType == SERVICE_AV_TRANSPORT)
  {
  	out->ActionNames[0] = "GetCurrentTransportActions";
		out->ActionNames[1] = "GetDeviceCapabilities";
		out->ActionNames[2] = "GetMediaInfo";
  	out->ActionNames[3] = "GetPositionInfo";
		out->ActionNames[4] = "GetTransportInfo";
		out->ActionNames[5] = "GetTransportSettings";
  	out->ActionNames[6] = "Next";
		out->ActionNames[7] = "Pause";
		out->ActionNames[8] = "Play";
  	out->ActionNames[9] = "Previous";
		out->ActionNames[10] = "Seek";
		out->ActionNames[11] = "SetAVTransportURI";
		out->ActionNames[12] = "SetPlayMode";
		out->ActionNames[13] = "Stop";
		out->actions[0] = MicroMediaPlay;
		out->actions[1] = MicroMediaPlay;
		out->actions[2] = MicroMediaPlay;
		out->actions[3] = GetPositionInfo;
		out->actions[4] = GetTransportInfo;
		out->actions[5] = MicroMediaPlay;
		out->actions[6] = MicroMediaPlay;
		out->actions[7] = Pause;
		out->actions[8] = Play;
		out->actions[9] = MicroMediaPlay;
		out->actions[10] = MicroMediaPlay;
		out->actions[11] = SetAVTransportURI;
		out->actions[12] = MicroMediaPlay;
		out->actions[13] = Stop;
    return 1;
  }

	return 0;
}

int MicroMediaPlay(IXML_Document * in, IXML_Document **out,
    const char**errorString){
  printf("Hello World");
  printf("%s" , ixmlPrintDocument(in));
  return UPNP_E_SUCCESS;
}



int GetTransportInfo(IXML_Document * in, IXML_Document **out,
    const char**errorString){
  IXML_Document *response_node = NULL;

	/* lock state mutex */
	ithread_mutex_lock(&MicroMediaRendererMutex);

  response_node = UpnpMakeActionResponse("GetTransportInfo",
      TvServiceType[SERVICE_AV_TRANSPORT],
      3,
      "CurrentTransportState",
      tv_service_table[SERVICE_AV_TRANSPORT].VariableStrVal[15],
      "CurrentTransportStatus",
      tv_service_table[SERVICE_AV_TRANSPORT].VariableStrVal[9],
      "CurrentSpeed",
      tv_service_table[SERVICE_AV_TRANSPORT].VariableStrVal[29]
      );

	ithread_mutex_unlock(&MicroMediaRendererMutex);
  *out = response_node;
  printf("%s", ixmlPrintDocument(*out));
  return UPNP_E_SUCCESS;
}

int GetPositionInfo(IXML_Document * in, IXML_Document **out,
    const char**errorString){
  IXML_Document *response_node = NULL;

	/* lock state mutex */
	ithread_mutex_lock(&MicroMediaRendererMutex);

  response_node = UpnpMakeActionResponse("GetPositionInfo",
      TvServiceType[SERVICE_AV_TRANSPORT],
      8,
      "Track", "1",
      "TrackDuration", "0:04:00",
      "TrackMetaData", "test.mp3",
      "TrackURI",
      tv_service_table[SERVICE_AV_TRANSPORT].VariableStrVal[27],
      "RelTime",
      "00:01:00",
      "AbsTime",
      "00:01:00",
      "RelCount",
      "1",
      "AbsCount",
      "1"
      );

	ithread_mutex_unlock(&MicroMediaRendererMutex);

  *out = response_node;
  printf("%s", ixmlPrintDocument(*out));
  return UPNP_E_SUCCESS;
}

static int child_process=0;

int SetAVTransportURI(IXML_Document * in, IXML_Document **out,
    const char**errorString){
  char *value = NULL;
	(*errorString) = NULL;
	if (!(value = SampleUtil_GetFirstDocumentItem(in, "CurrentURI"))) {
		(*errorString) = "Invalid URL";
		return UPNP_E_INVALID_PARAM;
	}

	/* lock state mutex */
	ithread_mutex_lock(&MicroMediaRendererMutex);

  strcpy(tv_service_table[SERVICE_AV_TRANSPORT].VariableStrVal[27], value);
  strcpy(tv_service_table[SERVICE_AV_TRANSPORT].VariableStrVal[15], "STOPPED");

	ithread_mutex_unlock(&MicroMediaRendererMutex);

  free(value);

  *out = UpnpMakeActionResponse("SetAVTransportURI",
      TvServiceType[SERVICE_AV_TRANSPORT],
      0,
      NULL);

  return UPNP_E_SUCCESS;
}

bool play(const char *);

int Play(IXML_Document * in, IXML_Document **out,
    const char **errorString)
{

  IXML_Document *response_node = NULL;
  response_node = UpnpMakeActionResponse("Play",
      TvServiceType[SERVICE_AV_TRANSPORT],
      0,
      NULL);

  *out=response_node;
  char buf[1024];

	/* lock state mutex */
	ithread_mutex_lock(&MicroMediaRendererMutex);

  strcpy(tv_service_table[SERVICE_AV_TRANSPORT].VariableStrVal[15], "PLAYING");
  strcpy(buf, tv_service_table[SERVICE_AV_TRANSPORT].VariableStrVal[27]);

	ithread_mutex_unlock(&MicroMediaRendererMutex);
  play(buf);

  return UPNP_E_SUCCESS;
}

int Pause(IXML_Document * in, IXML_Document **out,
    const char**errorString){
  IXML_Document *response_node = NULL;
  response_node = UpnpMakeActionResponse("Pause",
      TvServiceType[SERVICE_AV_TRANSPORT],
      0,
      NULL);

	/* lock state mutex */
	ithread_mutex_lock(&MicroMediaRendererMutex);

  if(strcmp(tv_service_table[SERVICE_AV_TRANSPORT].VariableStrVal[15],
        "PLAYING") == 0){
    strcpy(tv_service_table[SERVICE_AV_TRANSPORT].VariableStrVal[15], "PAUSED_PLAYBACK");
  }
  else
  {
    strcpy(tv_service_table[SERVICE_AV_TRANSPORT].VariableStrVal[15], "PLAYING");
  }

	ithread_mutex_unlock(&MicroMediaRendererMutex);

  pause();

  *out=response_node;
  return UPNP_E_SUCCESS;
}

int Stop(IXML_Document * in, IXML_Document **out,
    const char**errorString){
  IXML_Document *response_node = NULL;
  response_node = UpnpMakeActionResponse("Play",
      TvServiceType[SERVICE_AV_TRANSPORT],
      0,
      NULL);

	/* lock state mutex */
	ithread_mutex_lock(&MicroMediaRendererMutex);

  strcpy(tv_service_table[SERVICE_AV_TRANSPORT].VariableStrVal[15], "STOPPED");

	ithread_mutex_unlock(&MicroMediaRendererMutex);
  stop();
  *out=response_node;
  return UPNP_E_SUCCESS;
}

extern char tvc_varval;

//Rendering Control Actions.

int SetVolume(IXML_Document * in, IXML_Document **out,
    const char**errorString){
  char *value = NULL;
	(*out) = NULL;
	(*errorString) = NULL;
	if (!(value = SampleUtil_GetFirstDocumentItem(in, "DesiredVolume"))) {
		(*errorString) = "Invalid URL";
		return UPNP_E_INVALID_PARAM;
	}
  int set_volume = atoi(value);

	/* lock state mutex */
	ithread_mutex_lock(&MicroMediaRendererMutex);

  strcpy(tv_service_table[SERVICE_RENDERING_CONTROL].VariableStrVal[3], value);

	ithread_mutex_unlock(&MicroMediaRendererMutex);

  set_alsa_level(set_volume);
  IXML_Document *response_node = NULL;
  response_node = UpnpMakeActionResponse("SetVolume",
      TvServiceType[SERVICE_RENDERING_CONTROL],
      1,
      "NewVolume", value
      );
  *out = response_node;
  return UPNP_E_SUCCESS;
}

int GetVolume(IXML_Document * in, IXML_Document **out,
    const char**errorString){
  IXML_Document *response_node = NULL;
  int current_volume = get_alsa_level();
  char buff[1024];
  sprintf(buff, "%.0f", ((float)current_volume/38)*100);
  response_node = UpnpMakeActionResponse("GetVolume",
      TvServiceType[SERVICE_RENDERING_CONTROL],
      1,
      "CurrentVolume", buff
      );
  *out = response_node;
  printf("%s", ixmlPrintDocument(*out));
  return UPNP_E_SUCCESS;
}

// Move the following functions to player implementation.
bool play(const char *streamURL){
  bool retval = false;
  struct mpd_connection* conn;
  conn = mpd_connection_new("192.168.1.2", 6600, 0);
  if(conn != NULL)
  {
    mpd_run_clear(conn);
    retval = mpd_run_add(conn, streamURL);
    if(retval)
    {
      retval = mpd_send_play(conn);
    }
    mpd_connection_free(conn);
  }
  else{
    printf("Error connecting to mpd server");
    printf("Error code : %d", mpd_connection_get_error(conn));
  }
  return retval;
}

int get_alsa_level(){
  const char *command = "amixer get Speaker | grep \"Front Left:\" | cut -d \" \" -f 6";
  FILE *rp;
  int cur_vol=-1;
  rp = popen( command, "r" );
  if(rp != NULL)
  {
    fscanf(rp, "%d", &cur_vol);
    fclose(rp);
  }
  return cur_vol;
}

void set_alsa_level(unsigned int set_volume){
  const char *command = "amixer set Speaker %.0f";
  char complete_command[1024];
  sprintf(complete_command, command, ((float)set_volume/100)*38);
  SampleUtil_Print("Running command %s" , complete_command);
  FILE *rp;
  rp = popen( complete_command, "r" );
}

int stop()
{
  bool retval = false;
  struct mpd_connection* conn;
  conn = mpd_connection_new("192.168.1.2", 6600, 0);
  if(mpd_connection_get_error(conn) != MPD_ERROR_SUCCESS)
  {
    retval = mpd_send_stop(conn);
  }
  return retval;
}

int pause()
{
  bool retval = false;
  struct mpd_connection* conn;
  conn = mpd_connection_new("192.168.1.2", 6600, 0);
  if(mpd_connection_get_error(conn) != MPD_ERROR_SUCCESS)
  {
    retval = mpd_send_toggle_pause(conn);
  }
  return retval;
}


