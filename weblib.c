#include "incs/weblib.h"

StartResult weblib_start(){
	#ifdef __WIN32__
		WSADATA wsa;
		if(WSAStartup(MAKEWORD(2,2),&wsa) != 0){
			return ERROR_INIT_WINSOCK;
		}
	#endif

	activeSocket = socket(AF_INET, SOCK_STREAM, 0);

	if(activeSocket == -1){
		return ERROR_CREATING_SOCKET;
	}

	return START_SUCCESSFUL;
}

//void make_single_line(char *data){
//	for(int i = 0; data[i] != '\0'; i++){
//		if(data[i] == '\r' || data[i] == '\n'){
//			data[i] = ' ';
//		}
//	}
//}

FetchResult weblib_fetch(char *ip, char *buffer, int len, HTTP_Req* req){

	//initialize the socket
	struct sockaddr_in server;
	server.sin_addr.S_un.S_addr = inet_addr(ip);
	server.sin_family = AF_INET;
	server.sin_port = htons(80);

	//connect to the IP address
	if(connect(activeSocket, (struct sockaddr*)&server, sizeof(server)) == -1){
		return ERROR_CONNECT;
	}

	//get the request message ready
	char msg[100];
	if(req != NULL){
		sprintf(msg, "GET / HTTP/%d.%d\r\n\r\n", req->majorVer, req->minorVer);
	}else{
		return ERROR_HTTP_REQUEST_NULL;
	}

	//send message
	if(send(activeSocket, msg, strlen(msg), 0) < 0){
		return ERROR_SEND;
	}

	//get the result
	int recSize = 0;
	recSize = recv(activeSocket, buffer, len, 0);
	buffer[recSize] = '\0';

	//whole message is read and correctly terminated at this point.

	//SEPERATING WITH \r\n\r\n GIVES THE HTML AND HEADING.
	int locations[5]; // max should be 2, but just to be safe
	int count = seperate_str(buffer, "\r\n\r\n", locations);
	if(count != 2){
		return ERROR_UNKNOWN_RESPONSE;
	}

	//copy the html and heading into structure
	//needs to be freed after.
	req->result.heading = strdup(buffer);
	req->result.heading_length = strlen(req->result.heading);
	req->result.html = strdup(buffer + locations[1]);
	req->result.html_length = strlen(req->result.html);

	return FETCH_SUCCESSFUL;

}

//frees up the copied results
void cleanup_http_req(HTTP_Req *req){
	free(req->result.heading);
	free(req->result.html);
}

//cleans up the WSA and closes the socket
void weblib_close(){
	closesocket(activeSocket);
	#ifdef __WIN32__
		WSACleanup();
	#endif
}
