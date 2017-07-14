#include "incs/weblib.h"

int main(int argc, char **argv){

	if(argc < 2){
		printf("Not enough arguments!");
		return -1;
	}

	if(weblib_start() == START_SUCCESSFUL){
		printf("Connected successfully!\n\n");
	}else{
		printf("Could not connect!\n\n");
		return -1;
	}

	char buffer[5000];

	HTTP_Req req;
	req.majorVer = 1;
	req.minorVer = 1;

	if(weblib_fetch(argv[1], buffer, 5000, &req) != FETCH_SUCCESSFUL){
		printf("Fetch unsuccessful!");
		return -1;
	}

	printf("HEADING (size = %d) : \n----------\n%s", req.result.heading_length, req.result.heading);
	printf("\n\nHTML (size = %d) : \n----------\n%s", req.result.html_length, req.result.html);

	cleanup_http_req(&req);

	weblib_close();
}
