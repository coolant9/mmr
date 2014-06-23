#include <stdio.h>
#include <getopt.h>
#include <malloc.h>
#include "config_parser.h"
#include "common.h"

int load_file_defaults(const char *file)
{
  return 0;
}

void show_help()
{
  printf("Command line flags include:\n");
  printf("--mpd_port OR -m :Port where player listens for control commands\n");
  printf("--address_mpd OR -a :Address where player listens for control commands\n");
  printf("--ip_address OR -i :IP address on which to listen to.\n");
  printf("--port OR -p :Port number to use for recieving UPnP messages.\n");
  printf("--webdir OR -w :Filesystem path where web descriptions are placed.\n");
}

void free_config(struct program_config *options)
{
  if(options!=NULL)
  {
    if (options->address_mpd != NULL)
    {
      free(options->address_mpd);
    }
    if (options->ip_address != NULL)
    {
      free(options->ip_address);
    }
    if (options->webdir != NULL)
    {
      free(options->webdir);
    }
    options->address_mpd = NULL;
    options->ip_address = NULL;
    options->webdir = NULL;
    free(options);
  }
}

char *alloc_and_copy(char *destination, char *source){
  if (destination!=NULL)
  {
    free(destination);
  }
  unsigned int len = strnlen(source) + 1;
  destination = (char *)malloc(len*sizeof(char));
  if(destination != NULL)
  {
    strcpy(destination, source);
  }
  else
  {
    printf("[ERROR]Could not allocate memory");
    return NULL;
  }
  return destination;
}

struct program_config * parse_command_line_parameters(int argc, char *argv[])
{
    struct program_config * options = (struct program_config *)malloc(sizeof(struct program_config));
    options->address_mpd = NULL;
    options->ip_address = NULL;
    options->webdir = NULL;

    if (options == NULL)
    {
      return NULL;
    }

    //Init the options to defaults.
    options->address_mpd = alloc_and_copy(options->address_mpd, MPD_CONTROL_ADDR);
    options->mpd_port = MPD_CONTROL_PORT;

    int c;
    int digit_optind = 0;
    int aopt = 0, bopt = 0;
    char *copt = 0, *dopt = 0;
    static struct option long_options[] = {
        {"ip_address", optional_argument, 0, 'i'},
        {"port", optional_argument, 0, 'p'},
        {"webdir", optional_argument, 0, 'w'},
        {"mpd_port", optional_argument, 0, 'm'},
        {"address_mpd", optional_argument, 0, 'a'},
        {"help", no_argument, 0, 'h'},
        {NULL, 0, NULL, 0}
    };

    int option_index = 0;
    while ((c = getopt_long(argc, argv, "i:w:m:a:p:h",
                 long_options, &option_index)) != -1) {
        int this_option_optind = optind ? optind : 1;
        switch (c) {
        case 0:
            switch(option_index)
            {
              case 0:
                options->ip_address = alloc_and_copy(options->ip_address, optarg);
                break;
              case 1:
                options->port = atoi(optarg);
                break;
              case 2:
                options->webdir = alloc_and_copy(options->webdir, optarg);
                break;
              case 3:
                options->address_mpd = alloc_and_copy(options->address_mpd, optarg);
                break;
              case 4:
                options->mpd_port = atoi(optarg);
                break;
              case 5:
                show_help();
                free_config(options);
                options=NULL;
                break;
            }
            break;
        case 'i':
            printf ("setting ip_address to %s\n", optarg);
            options->ip_address = alloc_and_copy(options->ip_address, optarg);
            break;
        case 'p':
            printf ("setting port to %s\n", optarg);
            options->port = atoi(optarg);
            break;
        case 'w':
            printf("Setting webdir to %s\n", optarg);
            options->webdir = alloc_and_copy(options->webdir, optarg);
            break;
        case 'a':
            printf ("Setting player_control_addr '%s'\n", optarg);
            options->address_mpd = alloc_and_copy(options->address_mpd, optarg);
            break;
        case 'm':
            printf ("Setting player_control_port '%s'\n", optarg);
            options->mpd_port = atoi(optarg);
            break;
        case 'h':
        case '?':
            show_help();
            free_config(options);
            options=NULL;
            break;
        default:
            printf ("?? getopt returned character code 0%o ??\n", c);
        }
    }
    if (optind < argc) {
        printf ("non-option ARGV-elements: ");
        while (optind < argc)
            printf ("%s ", argv[optind++]);
        printf ("\n");
    }
  return options;
}

void dump_current_values(struct program_config * config)
{
  printf("address_mpd:%s\r\n", config->address_mpd);
  printf("mpd_port:%d\r\n", config->mpd_port);
}

#ifdef X
int main(int argc, char *argv[])
{
  struct program_config * config = parse_command_line_parameters(argc, argv);
  dump_current_values(config);
}
#endif
