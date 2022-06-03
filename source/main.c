#include <stdio.h>
#include <switch.h>

#define WEBNX_AUTHOR "bqio"
#define WEBNX_VERSION "1.2.2"
#define WEBNX_APP_URL "https://nxdb.bqio.ru"
#define MAX_MESSAGE_SIZE (256)

void createWebSession(WebSession *session, WebCommonConfig *config, bool *appletIsAppear)
{
    webPageCreate(config, WEBNX_APP_URL);
    webConfigSetWhitelist(config, "^http*");
    webConfigSetBootMode(config, WebSessionBootMode_AllForegroundInitiallyHidden);
    webConfigSetFooter(config, false);
    webConfigSetLeftStickMode(config, WebLeftStickMode_Cursor);
    webConfigSetPointer(config, false);
    webConfigSetBootDisplayKind(config, WebBootDisplayKind_Black);
    webConfigSetBackgroundKind(config, WebBackgroundKind_Unknown2);
    webConfigSetTouchEnabledOnContents(config, false);
    webConfigSetPageCache(config, true);
    webConfigSetJsExtension(config, true);
    webSessionCreate(session, config);
    webSessionStart(session, NULL);
    webSessionAppear(session, appletIsAppear);
}

int main(int argc, char **argv)
{
    PadState pad;
    WebCommonConfig config;
    WebSession session;

    bool appletIsAppear;
    bool messageIsReceived;

    char message[MAX_MESSAGE_SIZE];
    u64 outSize;

    padConfigureInput(1, HidNpadStyleSet_NpadStandard);
    padInitializeDefault(&pad);

    consoleInit(NULL);

    printf("WebNX ver. " WEBNX_VERSION " by " WEBNX_AUTHOR ".\n\n");
    printf("Press A button to open application.\n\n");
    printf("Press + button to exit.\n\n\n");

    while (appletMainLoop())
    {
        padUpdate(&pad);
        u64 kDown = padGetButtonsDown(&pad);

        if (kDown & HidNpadButton_Plus)
            break;
        if (kDown & HidNpadButton_A)
            createWebSession(&session, &config, &appletIsAppear);

        consoleUpdate(NULL);

        if (appletIsAppear)
        {
            webSessionTryReceiveContentMessage(&session, message, sizeof(message), &outSize, &messageIsReceived);

            if (messageIsReceived)
            {
                webSessionRequestExit(&session);
                printf("Downloading %s...\n\n", message);
            }
        }
    }

    consoleExit(NULL);
    return 0;
}