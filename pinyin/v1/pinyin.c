#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <wchar.h>
#include <sys/types.h>
#include <locale.h>
#include "conv.h"

static void get_pinyin(const char *line, char *pinyin);
static int int_code(const char *code);

struct ucs_pinyin {
	int id;
	char *code;
	char *pinyin;
};

#include "data.inc"

char *
topinyin(char *cnstr, size_t size) {
	size_t s;
	wchar_t *wcbuf;
	unsigned short *ucbuf, id;
	char *pinyin;
	int i;

	if (cnstr == NULL || size == 0) return NULL;
	
	wcbuf = malloc(size * sizeof(wchar_t));
	if (wcbuf == NULL) return NULL;
	
	setlocale(LC_ALL, "zh_CN.UTF-8");
	
	s = mbstowcs(wcbuf, cnstr, size);
	if (s == (size_t) -1) {
		return NULL;
	}
	
	free(wcbuf);
	
	ucbuf = malloc(s * sizeof(short));
	if (ucbuf == NULL) {
		return NULL;
	}
	
	utf82unicode(cnstr, size, (char *)ucbuf, s * sizeof(short));
	
	pinyin = malloc(s * 10);
	if (pinyin == NULL) {
		free(ucbuf);
		return NULL;
	}
	
	memset(pinyin, 0, s * 10);
	for (i = 0; i < s; i++) {
		id = ucbuf[i];
		if (id >= 0x4E00 && id <= 0x9FA5) {
			strcat(pinyin, pyarr[id - 0x4E00].pinyin);
		} else if (id <= 0x7F) {
			sprintf(pinyin + strlen(pinyin), "%c", (char)id);
		}
	}
	
	free(ucbuf);
	
	return pinyin;
}

int
main(int argc, char **argv) {
	printf("%s\n", topinyin(argv[1], strlen(argv[1])));
	
	return 0;
}


int
parse() {
	const char *in_file = "./unicode_pinyin.txt";
	const char *out_file = "./pinyin.sql";
	//const char *out_file = "./out.c";
	FILE *in, *out;
        char line[256] = {0};
        char code[5] = {0};
        char pinyin[20] = {0};
	char value[200] = {0};
	int i;
	
	
	in = fopen(in_file, "r");
	if (in == NULL) {
		perror("fopen failed");
		return -1;
	}
	
	out = fopen(out_file, "w+");
	if (out == NULL) {
		perror("fopen faield");
		return -1;
	}

	fprintf(out, "insert into tmp_base_pinyin(code, pinyin) values \\\n");	
	
	i = 0;
	while(fgets(line, sizeof(line) - 1, in) != NULL) {
		line[strlen(line) - 1] = 0;
		printf("%s\n", line);
		
		memcpy(code, line, 4);
		get_pinyin(line + 6, pinyin);
		
		fprintf(out, "('%s', '%s'),", code, pinyin);
		
		//printf("id: %d, code: %s, pinyin: %s\n", int_code(code), code, pinyin);
		i++;
		if (i % 4 == 0) {
                        fprintf(out, " \\\n");
                }
	}
	
	fclose(in);
	fclose(out);
}

static int int_code(const char *code) {
	return (int)strtol(code, NULL, 16);
}

static void get_pinyin(const char *buf, char *pinyin) {
	char *p, *n;
	int len;
	
	p = strchr(buf, '0');
	n = strchr(buf, '1');
	if (p == NULL) {
		p = n;
	} else {
		if (n && p && n < p) p = n;
	}
	
	n = strchr(buf, '2');
	if (p == NULL) {
                p = n;
        } else {
                if (n && p && n < p) p = n;
        }
	
	n = strchr(buf, '3');
	if (p == NULL) {
                p = n;
        } else {
                if (n && p && n < p) p = n;
        }
	
	n = strchr(buf, '4');
	if (p == NULL) {
                p = n;
        } else {
                if (n && p && n < p) p = n;
        }
	
	n = strchr(buf, '5');
	if (p == NULL) {
                p = n;
        } else {
                if (n && p && n < p) p = n;
        }
	
	n = strchr(buf, '6');
        if (p == NULL) {
                p = n;
        } else {
                if (n && p && n < p) p = n;
        }
	
	n = strchr(buf, '7');
        if (p == NULL) {
                p = n;
        } else {
                if (n && p && n < p) p = n;
        }
	
	n = strchr(buf, '8');
        if (p == NULL) {
                p = n;
        } else {
                if (n && p && n < p) p = n;
        }
	
	n = strchr(buf, '9');
	if (p == NULL) {
                p = n;
        } else {
                if (n && p && n < p) p = n;
        }
	
	if (p) goto end;
	
	pinyin[0] = 0;
	return;
end:
	len = p - buf;
	memcpy(pinyin, buf, len);
	pinyin[len] = 0;
}
