#ifndef INCL_H
#define INCL_H

// Macros

#define CACHE_JSON "/3ds/eBrowser/cache/posts.json"


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>
#include <malloc.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <fcntl.h>

#include <assert.h>

// extern
#include <curl/curl.h>
#include <jansson.h>

// my headers
#include "net.h"
#include "json.h"
#include "fs.h"

#endif // INCL_H