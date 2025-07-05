#include <3ds.h>

#include "../legacy_include/includes.h"

#define SOC_ALIGN       0x1000
#define SOC_BUFFERSIZE  0x100000

#define clear() printf("\033[2J\033[1;1H")

static u32 *SOC_buffer = NULL;
s32 sock = -1, csock = -1;

void failExit(const char *fmt, ...);

void socShutdown() {
	printf("waiting for socExit...\n");
	socExit();
}

int main() {

	int ret;

	// Initializing graphics
    gfxInitDefault();
	// Initializing graphics on the top screen
	consoleInit(GFX_TOP, NULL);

    printf("waiting on net..\n");

	SOC_buffer = (u32*)memalign(SOC_ALIGN, SOC_BUFFERSIZE);

	if(SOC_buffer == NULL) {
		failExit("memalign: failed to allocate\n");
	}

	// Now intialise soc:u service
	if ((ret = socInit(SOC_buffer, SOC_BUFFERSIZE)) != 0) {
    	failExit("socInit: 0x%08X\n", (unsigned int)ret);
	}

	// register socShutdown to run at exit
	// atexit functions execute in reverse order so this runs before gfxExit
	atexit(socShutdown);

    acInit();

    u32 apFlags = 0;
    Result res = ACU_GetWifiStatus(&apFlags);
    if(R_SUCCEEDED(res)) {
        printf("connected\n\n");
		mkdir("/3ds/eBrowser/cache", S_IRWXU);
        downloadFile("https://e621.net/posts.json", CACHE_JSON);

		parseJson(CACHE_JSON);
		//clear();
		//printf("\nFile Downloaded!");
    }

	// Main loop
	while (aptMainLoop())
	{
		gspWaitForVBlank();
		gfxSwapBuffers();
		hidScanInput();

		// Your code goes here
		u32 kDown = hidKeysDown();
		if (kDown & KEY_START)
			break; // break in order to return to hbmenu
	}

	socShutdown();
    acExit();
	gfxExit();
	return 0;

    // return 0;
}

void failExit(const char *fmt, ...) {

	if(sock>0) close(sock);
	if(csock>0) close(csock);

	va_list ap;

	printf(CONSOLE_RED);
	va_start(ap, fmt);
	vprintf(fmt, ap);
	va_end(ap);
	printf(CONSOLE_RESET);
	printf("\nPress B to exit\n");

	while (aptMainLoop()) {
		gspWaitForVBlank();
		hidScanInput();

		u32 kDown = hidKeysDown();
		if (kDown & KEY_B) exit(0);
	}
}