#include <3ds.h>
#include <cstdio>
#include <cstring>

const char* options[] = {
    "Start Game",
    "Options",
    "Credits",
    "Exit"
};

const int numOptions = sizeof(options) / sizeof(options[0]);

int main() {
    gfxInitDefault();
    consoleInit(GFX_TOP, NULL);  // Use top screen

    int selected = 0;

    while (aptMainLoop()) {
        hidScanInput();
        u32 kDown = hidKeysDown();

        // Navigate
        if (kDown & KEY_DOWN) {
            selected = (selected + 1) % numOptions;
        }
        if (kDown & KEY_UP) {
            selected = (selected - 1 + numOptions) % numOptions;
        }

        // Clear screen
        consoleClear();

        // Draw menu
        printf("=== Main Menu ===\n\n");
        for (int i = 0; i < numOptions; ++i) {
            if (i == selected)
                printf(" > %s\n", options[i]);  // Highlighted
            else
                printf("   %s\n", options[i]);
        }

        // Confirm selection
        if (kDown & KEY_A) {
            printf("\nYou selected: %s\n", options[selected]);
            printf("\nPress Start to exit...");
        }

        if (kDown & KEY_START) break;

        gspWaitForVBlank();
    }

    gfxExit();
    return 0;
}