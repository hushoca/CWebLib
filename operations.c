#include "incs/operations.h"

int seperate_str(char *in, char *del, int *locs){
	int delCount = strlen(del);
	locs[0] = 0;
	int loc = 1;
	for(int i = 0; in[i] != '\0'; i++){
		if(in[i] == del[0]){
			bool divide = true;
			for(int a = 1; a < delCount; a++){
				if(in[i + a] == '\0' || in[i + a] != del[a]){
					divide = false;
					break;
				}
			}

			if(divide){
				in[i] = '\0';
				i += delCount;
				locs[loc++] = i;
			}
		}
	}
	return loc;
}
