#include "sample_util.h"
#include "micro_media_renderer.h"

#include <stdarg.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	int rc;
	ithread_t cmdloop_thread;

	int sig;
	sigset_t sigs_to_catch;

	int code;

	rc = media_renderer_main(argc, argv);
	if (rc != UPNP_E_SUCCESS) {
		return rc;
	}

	/* start a command loop thread */
	code = ithread_create(&cmdloop_thread, NULL, MicroMediaRendererCommandLoop, NULL);
	if (code !=  0) {
		return UPNP_E_INTERNAL_ERROR;
	}

	/* Catch Ctrl-C and properly shutdown */
	sigemptyset(&sigs_to_catch);
	sigaddset(&sigs_to_catch, SIGINT);
	sigwait(&sigs_to_catch, &sig);
	SampleUtil_Print("Shutting down on signal %d...\n", sig);

	rc = MicroMediaRendererStop();

  return rc;
}

