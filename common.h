#ifndef _HOME_GSALUJA_PROJECTS_COOLANT_UPNP_MICRO_MEDIA_RENDERER_COMMON.H
#define _HOME_GSALUJA_PROJECTS_COOLANT_UPNP_MICRO_MEDIA_RENDERER_COMMON.H

#include "ithread.h"
#include "upnp.h"

/*! Mutex for protecting the global state table data
 * in a multi-threaded, asynchronous environment.
 * All functions should lock this mutex before reading
 * or writing the state table data. */
ithread_mutex_t MicroMediaRendererMutex;


//#define NAME_SIZE
#define TV_MAXVARS 30
#define TV_MAXACTIONS 14


#define SERVICE_RENDERING_CONTROL 0
#define SERVICE_AV_TRANSPORT 1
#define SERVICE_CONNECTION_MANAGER 2

/*! Number of services. */
#define TV_SERVICE_SERVCOUNT  3

/*! Number of control variables */
#define SVC_RENDERING_CONTROL_VARCOUNT   7

/*! Max value length */
#define TV_MAX_VAL_LEN 1024

/*! Number of picture variables */
#define TV_PICTURE_VARCOUNT   30

#define TV_CONTROL_POWER	0

#endif // _HOME_GSALUJA_PROJECTS_COOLANT_UPNP_MICRO_MEDIA_RENDERER_COMMON.H
