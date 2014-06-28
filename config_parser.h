#ifndef _HOME_GSALUJA_PROJECTS_COOLANT_UPNP_MICRO_MEDIA_RENDERER_CONFG_PARSER_H
#define _HOME_GSALUJA_PROJECTS_COOLANT_UPNP_MICRO_MEDIA_RENDERER_CONFG_PARSER_H

struct program_config{
  char *address_mpd;
  unsigned int mpd_port;
  char *ip_address;
  unsigned int port;
  char *webdir;
};

struct program_config * parse_command_line_parameters(int argc, char *argv[]);

#endif // _HOME_GSALUJA_PROJECTS_COOLANT_UPNP_MICRO_MEDIA_RENDERER_CONFG_PARSER_H
