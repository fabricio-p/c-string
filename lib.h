#ifndef _CSTRING_H_
#define _CSTRING_H_

#include <string.h>
#include <stdint.h>
#include <stdbool.h>

typedef char const *String;
#define CVECTOR_POINTERMODE
#include <c-vector/lib.h>
CVECTOR_WITH_NAME(char, StringBuffer);
CVECTOR(String);
typedef struct FixedString FixedString;

#define String_from_strlit(litstr)    \
  ((String)                           \
   &(                                 \
     ((struct {                       \
        int l;                        \
        char s[sizeof(litstr)];       \
       })                             \
       {sizeof(litstr), litstr}).s    \
    )                                 \
  )

#define StringBuffer_from_strlit(litstr)  \
  ((StringBuffer)                         \
   &(                                     \
     ((struct {                           \
        int l;                            \
        char s[sizeof(litstr) - 1];       \
       })                                 \
       {sizeof(litstr) - 1, litstr}).s    \
    )                                     \
  )

__cvector_inline__
CVECTOR_STATUS StringBuffer_push_bytes(StringBuffer *sb, char const *str,
                                       int n) {
  for (int i = 0; i < n; i++) {
#ifdef CVECTOR_SILENT
    if (StringBuffer_push(sb, str[i])) {
      return 1;
    }
#else
    StringBuffer_push(sb, str[i]);
#endif
  }
  CVECTOR_RETURN(0);
}

__cvector_inline__
CVECTOR_STATUS StringBuffer_push_str(StringBuffer *sb, char const *str) {
  CVECTOR_RETURN(StringBuffer_push_bytes(sb, str, strlen(str)));
}

__cvector_inline__
String String_new(void) {
  return (String)StringBuffer_with_length(1);
}

__cvector_inline__
String String_from_bytes(char const *bytes, int n) {
  StringBuffer str = StringBuffer_with_length(n + 1);
  if (str == NULL)
    return NULL;
  memcpy(str, bytes, n);
  str[n] = '\0';
  return (String)str;
}

__cvector_inline__
String String_from(char const *str) {
  return String_from_bytes(str, strlen(str));
}

__cvector_inline__
int String_len(String str) {
  return StringBuffer_len((StringBuffer)str) - 1;
}

__cvector_inline__
String String_clone(String str) {
  return String_from_bytes((char *)str, String_len(str));
}

__cvector_inline__
String String_concat(String str1, String str2) {
  int len1 = String_len(str1);
  int len2 = String_len(str2);
  StringBuffer str = StringBuffer_with_length(len1 + len2 + 1);
  if (str == NULL) {
    return NULL;
  }
  memcpy(str, str1, len1);
  memcpy(&(str[len1]), str2, len2);
  str[len1 + len2] = '\0';
  return (String)str;
}

static
uint32_t String_hash(String str) {
  int len = String_len(str);

  const size_t m = 0x5bd1e995;
  const int r = 24;

  size_t h = len ^ 0;

  for (; len >= 4; )
  {
    size_t k = *str;

    k *= m; 
    k ^= k >> r; 
    k *= m; 
    
    h *= m; 
    h ^= k;

    str += 4;
    len -= 4;
  }
  
  // Handle the last few bytes of the input array

  switch (len)
  {
  case 3: { h ^= str[2] << 16; } __attribute__((fallthrough));
  case 2: { h ^= str[1] <<  8; } __attribute__((fallthrough));
  case 1: h ^= str[0];
          h *= m;
  };

  // Do a few final mixes of the hash to ensure the last few
  // bytes are well-incorporated.

  h ^= h >> 13;
  h *= m;
  h ^= h >> 15;

  return h;

}

__cvector_inline__
String String_slice(String str, signed int fi, signed int li) {
  if (fi < 0) {
    fi += String_len(str) + 1;
  }
  if (li < 0) {
    li += String_len(str) + 1;
  }
  if (fi < 0 || fi > String_len(str) || li < fi || li < 0 ||
      li > String_len(str)) {
    return NULL;
  }
  int len = li - fi;
  StringBuffer slice = StringBuffer_with_length(len + 1);
  if (slice == NULL) {
    return NULL;
  }
  memcpy(slice, &(str[fi]), len);
  return (String)slice;
}

__cvector_inline__
CVECTOR_STATUS String_append(String *str, char const *bstr) {
  cvector_get_header((char *)*str)->len -= 1;
#ifdef CVECTOR_SILENT
  StringBuffer_push_str(str, bstr);
  StringBuffer_push(str, '\0');
#else
  if (StringBuffer_push_str((StringBuffer *)str, bstr)) {
    CVECTOR_RETURN(1);
  }
  if (StringBuffer_push((StringBuffer *)str, '\0')) {
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

#define String_cleanup(str) StringBuffer_cleanup(((StringBuffer)str))

__cvector_inline__
String StringBuffer_to_string(StringBuffer sb) {
  return String_from_bytes(sb, StringBuffer_len(sb));
}

__cvector_inline__
String StringBuffer_transform_to_string(StringBuffer *sb) {
  StringBuffer_push(sb, '\0');
  return *sb;
}

static Vector_String String_split(String str, char const *p) {
  Vector_String vec = Vector_String_new();
  int p_len = strlen(p);
  char const *const end = &(str[StringBuffer_len((StringBuffer)str)]);
  char const *start = str;
  char const *ptr = str;
  for (; ptr < end;) {
    if (memcmp(ptr, p, p_len) == 0) {
      int s_len = ptr - start;
      Vector_String_push(&vec, String_from_bytes(start, s_len));
      ptr += p_len;
      start = ptr;
    } else {
      ++ptr;
    }
  }
  Vector_String_push(&vec, String_from_bytes(start, end - start));
  return vec;
}

static Vector_String String_split_by_char(String str, char c)
{
  Vector_String vec = Vector_String_new();
  char const *const end = &(str[StringBuffer_len((StringBuffer)str)]);
  char const *start = str;
  char const *ptr = str;
  for (; ptr < end;) {
    if (*ptr == c) {
      int s_len = ptr - start;
      Vector_String_push(&vec, String_from_bytes(start, s_len));
      start = ++ptr;
    } else {
      ++ptr;
    }
  }
  Vector_String_push(&vec, String_from_bytes(start, end - start));
  return vec;
}

static void String_trim_start(String str) {
  char const *ptr = str;
  for (char c = *ptr;
       c == ' ' || c == '\t' || c == '\n' || c == '\0';
       c = *++ptr);
  int len = StringBuffer_len((StringBuffer)str) - (ptr - str);
  memmove((char *)str, ptr, len);
  ((StringBuffer)str)[len - 1] = '\0';
  cvector_get_header((char *)str)->len = len;
}

static void String_trim_end(String str) {
  int len = StringBuffer_len((StringBuffer)str);
  char const *end = &(str[len - 2]);
  for (char c = *end;
       c == ' ' || c == '\t' || c == '\n' || c == '\0';
       c = *--end);
  len = end - str + 2;
  ((StringBuffer)str)[len - 1] = '\0';
  cvector_get_header((char *)str)->len = len;
}

__cvector_inline__
void String_trim(String str) {
  String_trim_start(str);
  String_trim_end(str);
}

struct FixedString {
  int        len;
  char const *str;
};

#define FixedString_from(str) ((FixedString){ sizeof(str) - 1, str })

__cvector_inline__
FixedString String_to_fixed(String str) {
  int len = String_len(str);
  char *data = malloc(len + 1);
  memcpy((char *)str, data, len + 1);
  return (FixedString){ len, (char const *)data };
}
__cvector_inline__
void FixedString_cleanup(FixedString *fstr) {
  if (fstr != NULL) {
    if (fstr->str != NULL) {
      free((void *)fstr->str);
    }
    fstr->len = 0;
    fstr->str = NULL;
  }
}

#endif /* _CSTRING_H_ */
