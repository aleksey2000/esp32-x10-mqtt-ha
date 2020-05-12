#include <WebServer.h>
#include <Support.h>
#include <Update.h>
#include <string> 


class web
{
private:
    const char* serverVersion;
    WebServer *webServer;
public:
    web(WebServer &websrv, char progVerison[]);
    void webServerInit();
    
};
