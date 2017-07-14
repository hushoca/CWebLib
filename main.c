#include "incs/weblib.h"

typedef struct{
	char *name;
	char *value;
} HTML_ATTRIBUTE;

typedef struct{
	char *name;
	char *value;
	HTML_ATTRIBUTE **attributes;
} HTML_TAG;

/***
 * NOTE FOR FUTURE: This is horrifying. Rewrite it!
 */
int find_substring(char *in, char *substring){
	int len = strlen(in);
	int subLen = strlen(substring);
	for(int i = len; i > 0; i--){
		if(in[i] == substring[subLen - 1]){
			if(i - subLen - 1 >= 0){
				for(int a = 0; a < subLen; a++){
					if(in[i - a + 1] == substring[a]){
						if(a == 0){
							return i - a;
						}
						continue;
					}else{
						break;
					}
				}
			}
		}
	}
	return -1;
}

void parse_tag(char *in, HTML_TAG *out){
	char *firsTagStart = strchr(in, '<');
	firsTagStart+=sizeof(char);

	char *firstTagEnd = strchr(in, '>');

	char *name = malloc((firstTagEnd - firsTagStart) * sizeof(char));
	memcpy(name, firsTagStart, firstTagEnd - firsTagStart);
	name[firstTagEnd - firsTagStart] = '\0';

	char *valueStart = firstTagEnd + 1;
	char substring[20];
	sprintf(&substring, "</%s>", name);
	puts(&substring);
	int valueEnd = find_substring(valueStart, substring);


	int diff = strlen(valueStart) - valueEnd;
	int length = strlen(valueStart) - diff + 1;
	char *value = malloc(length * sizeof(char));
	memcpy(value, valueStart, length);

	out->name = name;
	out->value = value;


}

void clean_tag(HTML_TAG *tag){
	free(tag->name);
	free(tag->value);
}

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

	HTML_TAG tag, tag2;
	parse_tag(req.result.html, &tag);
	parse_tag(tag.value, &tag2);

	//printf("HEADING (size = %d) : \n----------\n%s", req.result.heading_length, req.result.heading);
	//printf("\n\nHTML (size = %d) : \n----------\n%s", req.result.html_length, req.result.html);

	printf("> TAG1 name IS [%s]. TAG value IS [%s].\n\n", tag.name, tag.value);

	printf("> TAG2 name IS [%s]. TAG value IS [%s].\n\n", tag2.name, tag2.value);

	clean_tag(&tag);

	cleanup_http_req(&req);

//	printf("\n\n----------------\n");
//	char *a = "<body>alibaba123</body></body>  ";
//	char *b = "</body>";
//	int loc = find_substring(a, b);
//	printf("location is %d\n", loc);
	weblib_close();
}
