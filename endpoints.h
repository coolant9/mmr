#ifndef _HOME_GSALUJA_PROJECTS_COOLANT_UPNP_MICRO_MEDIA_RENDERER_ENDPOINTS.H
#define _HOME_GSALUJA_PROJECTS_COOLANT_UPNP_MICRO_MEDIA_RENDERER_ENDPOINTS.H

#include <stdio.h>
#include <sys/types.h>
#include <signal.h>

#include "sample_util.h"
#include <mpd/client.h>
#include <mpd/status.h>
#include <mpd/song.h>
#include <mpd/entity.h>
#include <mpd/search.h>
#include <mpd/tag.h>


struct song_status{
  unsigned int total_duration;
  unsigned int elapsed_duration;
};

bool play(const char *);
int get_alsa_level();
void set_alsa_level(unsigned int);
int stop();
struct song_status * update_counters();
enum mpd_state player_state();


#endif // _HOME_GSALUJA_PROJECTS_COOLANT_UPNP_MICRO_MEDIA_RENDERER_ENDPOINTS.H
