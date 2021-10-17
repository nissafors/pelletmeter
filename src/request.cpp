#include "request.h"
#include "Arduino.h"

// Max uint length + json length
static const int BODY_BUF_SIZE = 10 + 10;
static char bodyBuf[BODY_BUF_SIZE];
// postTmpl len + host/port headroom + BODY_BUF_SIZE
static const int REQUEST_BUF_SIZE = 78 + 64 + BODY_BUF_SIZE;
static char requestBuf[REQUEST_BUF_SIZE];

const char* postTmpl = "POST / HTTP/1.1 \r\nHost: %s:%d\r\nContent-Type: application/json\r\nContent-Length: %d\r\n\r\n%s";
const char* bodyTmpl = "{\n  cm: %d\n}";

char* createPostRequest(const int measurement, const char* host, const int port)
{
    // Populate body
    snprintf(bodyBuf, BODY_BUF_SIZE, bodyTmpl, measurement);

    // Create request
    int contentLen = strlen(bodyBuf);
    snprintf(requestBuf, REQUEST_BUF_SIZE, postTmpl, host, port, contentLen, bodyBuf);

    return requestBuf;
}
