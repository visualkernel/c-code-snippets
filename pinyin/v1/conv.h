#ifndef _CONV_H
#define _CONV_H

#include <iconv.h>

//编码转换:从一种编码转为另一种编码
inline static int
code_conv(char *from_charset,char *to_charset,
	  char *inbuf, size_t inlen, char *outbuf, size_t outlen) {
	iconv_t cd;
	char **pin = &inbuf;
	char **pout = &outbuf;
	
	cd = iconv_open(to_charset, from_charset);
	if (cd==0) return -1;
	
	memset(outbuf, 0, outlen);
	
	if (iconv(cd, pin, &inlen, pout, &outlen) == -1) return -1;
	
	iconv_close(cd);

	return 0;
}


/**
 * GBK to UTF-8
 */
inline static int
gbk2utf8(char *inbuf, size_t inlen, char *outbuf, size_t outlen) {
	return code_conv("GBK","UTF-8", inbuf, inlen, outbuf, outlen);   
}

/**
 * UTF-8 to GBK
 */
inline static int
utf82gbk(char *inbuf, size_t inlen, char *outbuf, size_t outlen) {
	return code_conv("UTF-8","GBK", inbuf, inlen, outbuf, outlen);
}


inline static int
utf82unicode(char *inbuf, size_t inlen, char *outbuf, size_t outlen) {
	return code_conv("UTF-8","UCS-2", inbuf, inlen, outbuf, outlen);	
}

#endif
