#ifndef _CSTRING_H_
#define _CSTRING_H_

#define CVECTOR_FATPOINTER
#include <c-vector/lib.h>
#include <string.h>
#include <stdbool.h>

CVECTOR_WITH_NAME(char, StringBuffer);
typedef char* String;

__cvector_inline__
CVECTOR_STATUS StringBuffer_push_bytes(StringBuffer *sb, char *str,
										int n) {
	for (int i = 0; i < n; i++) {
#ifdef CVECTOR_SILENT
		if (StringBuffer_push(sb, str[i]))
			return 1;
#else
		StringBuffer_push(sb, str[i]);
#endif
	}
	CVECTOR_RETURN(0);
}

__cvector_inline__
CVECTOR_STATUS StringBuffer_push_str(StringBuffer *sb, char *str) {
	CVECTOR_RETURN(StringBuffer_push_bytes(sb, str, strlen(str)));
}

__cvector_inline__
String String_new() {
	return StringBuffer_with_length(1);
}

__cvector_inline__
String String_from_bytes(char *bytes, int n) {
	String str = StringBuffer_with_length(n + 1);
	if (str == NULL)
		return NULL;
	memcpy(str, bytes, n);
	str[n] = '\0';
	return str;
}

__cvector_inline__
String String_from(char *str) {
	return String_from_bytes(str, strlen(str));
}

__cvector_inline__
int String_len(String str) {
	return StringBuffer_len(str) - 1;
}

__cvector_inline__
String String_clone(String str) {
	return String_from_bytes(str, String_len(str));
}

__cvector_inline__
String String_concat(String str1, String str2) {
	int len1 = String_len(str1);
	int len2 = String_len(str2);
	String str = StringBuffer_with_length(len1 + len2 + 1);
	if (str == NULL)
		return NULL;
	memcpy(str, str1, len1);
	memcpy(str + len1, str2, len2);
	str[len1 + len2] = '\0';
	return str;
}

__cvector_inline__
String String_slice(String str, signed int fi, signed int li) {
	if (fi < 0)
		fi += String_len(str) + 1;
	if (li < 0)
		li += String_len(str) + 1;
	if (fi < 0 || fi > String_len(str) || li < fi || li < 0 ||
			li > String_len(str))
		return NULL;
	int len = li - fi;
	String slice = StringBuffer_with_length(len + 1);
	if (slice == NULL)
		return NULL;
	memcpy(slice, str + fi, len);
	return slice;
}

__cvector_inline__
CVECTOR_STATUS String_append(String *str, char *bstr) {
	cvector_get_header(*str)->len -= 1;
#ifdef CVECTOR_SILENT
	StringBuffer_push_str(str, bstr);
	StringBuffer_push(str, '\0');
#else
	if (StringBuffer_push_str(str, bstr))
	{
		CVECTOR_RETURN(1);
	}
	if (StringBuffer_push(str, '\0'))
	{
		CVECTOR_RETURN(1);
	}
	CVECTOR_RETURN(0);
#endif
}

__cvector_inline__
bool String_equal(String str1, String str2) {
	return String_len(str1) == String_len(str2) &&
			!memcmp(str1, str2, String_len(str1));
}

__cvector_inline__
void String_cleanup(String str) { StringBuffer_cleanup(str); }


__cvector_inline__
String StringBuffer_to_string(StringBuffer sb) {
	return String_from_bytes(sb, StringBuffer_len(sb));
}

__cvector_inline__
String StringBuffer_transform_to_string(StringBuffer *sb) {
	StringBuffer_push(sb, '\0');
	return *sb;
}

#endif /* _CSTRING_H_ */
