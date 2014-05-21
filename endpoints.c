#include "endpoints.h"

void getstreamfrompls(const char *URL, char * stream)
{
  char buff[1024];
  const char *command = "cat %s | grep File1 | cut -f2 -d '='";
  sprintf(buff, command, URL);
  printf("[shoutcast] command = %s" ,  buff);

  FILE *rp;
  rp = popen( buff, "r" );
  if(rp != NULL)
  {
    fscanf(rp, "%s", stream);
    printf("Got stream %s" ,  stream);
    fclose(rp);
  }
}

enum mpd_state player_state()
{
  struct mpd_connection* conn=NULL;
  enum mpd_state retVal;
  conn = mpd_connection_new("192.168.1.2", 6600, 0);
  if(conn != NULL)
  {
    struct mpd_status *ms = NULL;
    ms = mpd_run_status(conn);
    if (ms!=NULL)
    {
      retVal = mpd_status_get_state(ms);
      printf("player_state : %d", retVal);
      mpd_status_free(ms);
    }
  }
  return retVal;
}

struct song_status * update_counters()
{
  struct song_status* retVal = NULL;
  struct mpd_connection* conn=NULL;
  conn = mpd_connection_new("192.168.1.2", 6600, 0);
  if(conn != NULL)
  {
    struct mpd_status *ms = NULL;
    ms = mpd_run_status(conn);
    if (ms!=NULL)
    {
      retVal = malloc(sizeof(struct song_status));
      unsigned int total_time = mpd_status_get_total_time(ms);
      unsigned int elapsed_time = mpd_status_get_elapsed_time(ms);
      mpd_status_free(ms);
      retVal->elapsed_duration = elapsed_time;
      retVal->total_duration = total_time;
      printf("Elapsed = %d, Total = %d", elapsed_time, total_time);
    }
    else
    {
      printf("Unablt to get counters");
    }
  }
  return retVal;
}

bool play(const char *streamURL){
  bool retval = false;
  struct mpd_connection* conn;
  conn = mpd_connection_new("192.168.1.2", 6600, 0);
  if(conn != NULL)
  {
    mpd_run_clear(conn);
    char stream[1024];
    printf("[stream] : %s", streamURL);
    if ( strstr(streamURL, "shoutcast") != NULL)
    {
      getstreamfrompls(streamURL, &stream);
    }
    else
    {
      strcpy(stream, streamURL);
    }
    retval = mpd_run_add(conn, stream);

    struct mpd_song *songInfo;
    songInfo = mpd_run_get_queue_song_pos(conn, 0);
    if (songInfo != NULL){
    unsigned int duration = mpd_song_get_duration(songInfo);

    printf("DURATION:%d", duration);

    free(songInfo);
    }
    else
    {
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


