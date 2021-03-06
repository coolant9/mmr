#ifndef _HOME_GSALUJA_PROJECTS_COOLANT_UPNP_MICRO_MEDIA_RENDERER_COMMON_H
#define _HOME_GSALUJA_PROJECTS_COOLANT_UPNP_MICRO_MEDIA_RENDERER_COMMON_H

#include "ithread.h"
#include "upnp.h"
#include "config_parser.h"

/*! Mutex for protecting the global state table data
 * in a multi-threaded, asynchronous environment.
 * All functions should lock this mutex before reading
 * or writing the state table data. */
ithread_mutex_t MicroMediaRendererMutex;

//Configuration for the program.
struct program_config *config;

//#define NAME_SIZE
#define BUFF_MAXVARS 30
#define BUFF_MAXACTIONS 14


#define SERVICE_RENDERING_CONTROL 0
#define SERVICE_AV_TRANSPORT 1
#define SERVICE_CONNECTION_MANAGER 2

/*! Number of services. */
#define MMR_SERVICE_SERVCOUNT  3

/*! Number of control variables */
#define SVC_RENDERING_CONTROL_VARCOUNT   7

/*! Max value length */
#define BUFF_MAX_VAL_LEN 1024

/*! Number of picture variables */
#define SVC_AV_TRANSPORT_VARCOUNT   30

#define SVC_CONNECTION_MANAGER_VARCOUNT 10

//Configuration
#define MPD_CONTROL_ADDR "127.0.0.1"
#define MPD_CONTROL_PORT 6600

#endif // _HOME_GSALUJA_PROJECTS_COOLANT_UPNP_MICRO_MEDIA_RENDERER_COMMON_H
