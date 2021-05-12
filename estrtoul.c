#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void
die(const char* e, ...){
	fprintf(stdout, "sara: %s\n", e);
	exit(1);
}

void*
ecalloc(size_t nmemb, size_t size){
	void* p;

	if ( !(p = calloc(nmemb, size)) )
		die("ecalloc failed");

	return p;
}

int
slen(const char* str){
	int i = 0;

	while (*str){ str++; i++; }

	return i;
}

/* more flexible strtoul
 * if input is strtoul-parseable, returns that result
 * else, if input is of the form "1 << 8", etc., then that will be evaluated
 * 	and returned.
 * if you provide non-canonical bitshifted input, expect the unexpected.
 * if you are using this in the midst of some strtok-ing, you will need to
 * 	change everything to strtok_r, or else you will lose your spot.
 */
unsigned int
estrtoul(char* str, int nchar){
	char* check, * tmp, * local_str, * saveptr;
	char** tmp_ops;
	int i, j;
	unsigned int ret, to_store;
	unsigned int* tmp_vals;
	int nops = 0, nvals = 0, panic = 0;
	char** ops = ecalloc(1, sizeof(char*));
	unsigned int* vals = ecalloc(1, sizeof(unsigned int));

	local_str = strndup(str, nchar*sizeof(char));

	ret = strtoul(local_str, &check, 10);

	/* if entire string was valid, all good */
	if (*local_str != '\0' && *check == '\0')
		return ret;
	
	if ( (tmp = strtok_r(local_str, " ", &saveptr)) ){
		to_store = (unsigned int) strtoul(tmp, &check, 10);

		/* if you don't start with a value, ya dun fer */
		if ( !(*tmp != '\0' && *check == '\0') )
			return 0;

		vals[0] = to_store;
		nvals++;
	}

	while ( (tmp = strtok_r(NULL, " ", &saveptr)) ){
		printf("tmp is: %s\n", tmp);
	
		if (strcmp(tmp, ">>") == 0 || strcmp(tmp, "<<") == 0){
			/* if double operators, we're done */
			if (ops[nops] && (strcmp(ops[nops], ">>") == 0 || strcmp(ops[nops], "<<") == 0)){
				break;

			} else {
				nops++;

				if ( (tmp_ops = realloc(ops, nops * sizeof(char *))) ){
					ops = tmp_ops;
					ops[nops-1] = tmp;

				} else {
					panic = 1;
				}
			}

		} else if ( (to_store = strtoul(tmp, (char**) NULL, 10)) ){
			nvals++;

			if ( (tmp_vals = realloc(vals, nvals * sizeof(int))) ){
				vals = tmp_vals;
				vals[nvals-1] = to_store;

			} else {
				panic = 1;
			}
		}

		if (panic){
			free(ops);
			free(vals);
			free(local_str);
			fprintf(stderr, "estrtoul: failed to realloc ops or vals\n");
			return 0;
		}
	}

	/* trim any excess
	 * will prevent catastrophic failure from malformed value input, but
	 * junk will ensue!
	 */
	while (nvals > nops + 1 && nvals > 0)
		nvals--;

	while (nops > nvals - 1 && nops > 0)
		nops--;

	for (i=0,j=i-1;i < nvals;i++,j++){
		if (i == 0){
			ret = vals[i];
			continue;
		}

		if (strcmp(ops[j], ">>") == 0)
			ret >>= vals[i];
		else if (strcmp(ops[j], "<<") == 0)
			ret <<= vals[i];
	}

	free(ops);
	free(vals);
	free(local_str);

	return ret;
}

int
main(){
	char msg[] = "firefox firefox-leedle '1 << 8 >> 1' 1 1 -1";
	char* endptr, * saveptr;
	int nargs = 0, i = 0;
	char** tmp_args;
	char* tmp = strtok_r(msg, " ", &saveptr);

	if (tmp)
		nargs++;

	printf("%d: %s\n", nargs++, tmp);

	if ( (tmp = strtok_r(NULL, " ", &saveptr)) ){
		printf("%d: %s\n", nargs++, tmp);
	}

	if ( (tmp = strtok_r(NULL, "'", &saveptr)) ){
		printf("%d: %s\n", nargs++, tmp);
		printf("----\n");
		printf("strtoul: %u\n", strtoul(tmp, &endptr, 10));
		printf("estrtoul: %u\n", estrtoul(tmp, slen(tmp)));
		printf("literal: %u\n", 1 << 8 >> 1);
		printf("endptr: %s\n", endptr);
		printf("----\n");
	}

	if ( (tmp = strtok_r(NULL, " ", &saveptr)) ){
		printf("%d: %s\n", nargs++, tmp);
	}

	if ( (tmp = strtok_r(NULL, " ", &saveptr)) ){
		printf("%d: %s\n", nargs++, tmp);
	}

	if ( (tmp = strtok_r(NULL, " ", &saveptr)) ){
		printf("%d: %s\n", nargs++, tmp);
	}

	printf("%d\n", nargs);

	return 0;
}
