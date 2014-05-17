#include "endpoints.h"

bool play(const char *streamURL){
  bool retval = false;
  struct mpd_connection* conn;
  conn = mpd_connection_new("192.168.1.2", 6600, 0);
  if(conn != NULL)
  {
    mpd_run_clear(conn);
    retval = mpd_run_add(conn, streamURL);

    struct mpd_song *songInfo;
    songInfo = mpd_recv_song(conn);
    if (songInfo != NULL){
    unsigned int duration = mpd_song_get_duration(songInfo);

    printf("DURATION:%d", duration);

    free(songInfo);
    }
    else{
      printf("Could not fetch duration");
    }
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


