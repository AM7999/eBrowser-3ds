#include "../include/includes.h"

#define SOC_ALIGN      0x1000
#define SOC_BUFFERSIZE 0x100000


static u32 *SOC_BUFFER = NULL;
s32 sock = -1, csock = -1;

using std::cout; 
using std::endl;
using std::string;

using json = nlohmann::json;

string optionsMenu[] = {
	"Parse Json",
	"Download Images"
};

const char* apiURL = "https://e621.net/posts.json";
const char* cacheJson = "/3ds/eBrowser/cache/posts.json";

const int numOptions = sizeof(optionsMenu) / sizeof(optionsMenu[0]);

void failExit(const char *fmt, ...);

void postView() {
	
}

void socShutdown() {
	cout << "waiting for socExit..." << endl;
	socExit();
}

char *readFile(char *filename) {
    FILE *f = fopen(filename, "rt");
    assert(f);
    fseek(f, 0, SEEK_END);
    long length = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *buffer = (char *) malloc(length + 1);
    buffer[length] = '\0';
    fread(buffer, 1, length, f);
    fclose(f);
    return buffer;
}

size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream) {
    size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
    return written;
}

bool downloadFile(const char *filename_and_path, const char *url) {
    CURL *curl_handle;
    const char *fileOut = filename_and_path;
    FILE *pagefile;

    curl_global_init(CURL_GLOBAL_ALL);

    curl_handle = curl_easy_init();
 
    curl_easy_setopt(curl_handle, CURLOPT_URL, url);
    curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "eBrowser for 3ds, By AM7999");
    curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_data);

    pagefile = fopen(fileOut, "wb");
    if(pagefile) {
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, pagefile);
        curl_easy_perform(curl_handle);
        fclose(pagefile);
    } else if(!pagefile) {
        return false;
    }
 
    curl_easy_cleanup(curl_handle);
 
    curl_global_cleanup();

    return true;
}


// @!-- ENTRY POINT --!@ //
int main() {
    int ret; 

	int selected = 0;

    gfxInitDefault();
    consoleInit(GFX_TOP, NULL);

    cout << "Waiting on net...\n" << endl;

    SOC_BUFFER = (u32*)memalign(SOC_ALIGN, SOC_BUFFERSIZE);

    if(SOC_BUFFER == NULL) {
		failExit("memalign: failed to allocate\n");
	}

	// Now intialise soc:u service
	if ((ret = socInit(SOC_BUFFER, SOC_BUFFERSIZE)) != 0) {
    	failExit("socInit: 0x%08X\n", (unsigned int)ret);
	}

    atexit(socShutdown);

    u32 apFlags = 0;
	acInit();
    Result res = ACU_GetWifiStatus(&apFlags);
    if(R_SUCCEEDED(res)) {
        cout << "connected\n\n";
        mkdir("/3ds/eBrowser/cache", S_IRWXU);
		
		printf("I'm at the downloading step now");

		downloadFile(cacheJson, apiURL);
    }

	while (aptMainLoop()) {
		hidScanInput();
		gfxSwapBuffers();

		// Your code goes here
		u32 kDown = hidKeysDown();

		if(kDown & KEY_DOWN) {
			selected = (selected + 1) % numOptions;
		}
		if(kDown & KEY_UP) {
			selected = (selected - 1) % numOptions;
		}

		consoleClear();

		cout << "------ Main Menu - eBrowser for 3ds ------" << "\n\n";
		for(int i = 0; i < numOptions; i++) {
			if(i == selected)
				cout << " > " << optionsMenu[i] << '\n';
			else 
				cout << "   " << optionsMenu[i] << '\n';

			cout << i << endl;
		}

		if(kDown & KEY_A && selected == 0) {
			//TODO - implement this
		}
		if(kDown & KEY_A && selected == 1) {
			string file = readFile("/3ds/eBrowser/cache/posts.json");
			json jsonFile = json::parse(file);

			cout << jsonFile["posts"][0] << endl;
		}

		if (kDown & KEY_START)
			break; // break in order to return to hbmenu
		
		gspWaitForVBlank();
	}

	socShutdown();
	acExit();
	gfxExit();
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