#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <sys/dir.h>
#include <sys/stat.h>
#include <time.h>

/*
Copyright (c) 2024 Devine Lu Linvega

Permission to use, copy, modify, and distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE.
*/

/* clang-format off */

static int   slen(char *s) { int i = 0; while(s[i] && s[++i]) { ; } return i; } /* string length */
static char *scpy(char *src, char *dst, int len) { int i = 0; while((dst[i] = src[i]) && i < len - 2) i++; dst[i + 1] = '\0'; return dst; } /* string copy */
static char *scat(char *dst, const char *src) { char *ptr = dst + slen(dst); while(*src) *ptr++ = *src++; *ptr = '\0'; return dst; } /* string cat */

/* clang-format on */

int
main(int argc, char** argv)
{
	DIR *d;
	FILE *f;
	char index_html[0x40];
	struct dirent *dir;
	if(argc == 1){
		fprintf(stderr, "usage %s: folder/\n", argv[0]);
		return 0;
	}
	if(!(d = opendir(argv[1]))){
		fprintf(stderr, "Missing directory: %s\n", argv[1]);
		return 0;
	}
	if(!(f = fopen(scat(scpy(argv[1], index_html, 0x80), "index.html"), "w"))){
		fprintf(stderr, "Cannot create index: %s\n", index_html);
		return 0;
	}
	fputs("<!DOCTYPE html>\n<html lang='en'>\n", f);
	fputs("<head>\n", f);
	fprintf(f,"\t<meta charset='utf-8'><link rel='stylesheet' type='text/css' href='style.css'>\n");
	fputs("</head>\n", f);
	fputs("<body id='directory'>\n\t<ul>\n", f);
	while((dir = readdir(d)))
		if(dir->d_name[0] != '.')
			fprintf(f, "\t\t<li><a href='%s'>%s</a></li>\n", dir->d_name, dir->d_name);
	fputs("\t</ul>\n</body>\n</html>", f);
	fclose(f);
	closedir(d);
	printf("Created %s.\n", index_html);
	return 0;
}

