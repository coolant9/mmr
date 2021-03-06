#ifndef _HOME_GSALUJA_PROJECTS_COOLANT_UPNP_MICRO_MEDIA_RENDERER_ACTIONS_H
#define _HOME_GSALUJA_PROJECTS_COOLANT_UPNP_MICRO_MEDIA_RENDERER_ACTIONS_H


#include "upnp.h"
#include "requests.h"

void set_alsa_level(unsigned int set_volume);

int SetActionTable(int serviceType, struct RendererService *out);

//Callbacks

int MicroMediaPlay(IXML_Document * in, IXML_Document **out,
    const char**errorString);

int GetPositionInfo(IXML_Document * in, IXML_Document **out,
    const char**errorString);

int GetTransportInfo(IXML_Document * in, IXML_Document **out,
    const char**errorString);

int SetAVTransportURI(IXML_Document * in, IXML_Document **out,
    const char**errorString);

int Stop(IXML_Document * in, IXML_Document **out,
    const char**errorString);

int GetVolume(IXML_Document * in, IXML_Document **out,
    const char**errorString);

int SetVolume(IXML_Document * in, IXML_Document **out,
    const char**errorString);

int Next(IXML_Document * in, IXML_Document **out,
    const char**errorString);

int Pause(IXML_Document * in, IXML_Document **out,
    const char**errorString);

int Play(IXML_Document * in, IXML_Document **out,
    const char**errorString);

int Previous(IXML_Document * in, IXML_Document **out,
    const char**errorString);

int Seek(IXML_Document * in, IXML_Document **out,
    const char**errorString);

int GetProtocolInfo(IXML_Document * in, IXML_Document **out,
    const char**errorString);
#endif // _HOME_GSALUJA_PROJECTS_COOLANT_UPNP_MICRO_MEDIA_RENDERER_ACTIONS_H
