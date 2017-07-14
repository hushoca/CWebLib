/*
 * weblib.h
 *
 *  Created on: 13 Jul 2017
 *      Author: hhoca
 */

#ifndef INC_WEBLIB_H_
#define INC_WEBLIB_H_

#ifdef __WIN32__
# include <winsock2.h>
#else
# include <sys/socket.h>
#include <arpa/inet.h>
#endif

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "operations.h"

typedef enum{ERROR_INIT_WINSOCK,
			ERROR_CREATING_SOCKET,
			START_SUCCESSFUL} StartResult;

typedef enum{ERROR_CONNECT,
			ERROR_HTTP_REQUEST_NULL,
			ERROR_SEND,
			ERROR_UNKNOWN_RESPONSE,
			FETCH_SUCCESSFUL} FetchResult;

typedef struct{
	int html_length;
	int heading_length;
	char *html;
	char *heading;
}HTTP_Req_Result;

typedef struct{

	short majorVer;
	short minorVer;
	HTTP_Req_Result result;

}HTTP_Req;

static SOCKET activeSocket;

StartResult weblib_start();
FetchResult weblib_fetch(char *ip, char *buffer, int len, HTTP_Req* req);

void cleanup_http_req(HTTP_Req *req);
void weblib_close();

#endif /* INC_WEBLIB_H_ */
