# c-string
A library providing string functions and utilities in C.
Before using it, do `cd c-string/ && ./setup.sh` to install libraries.

## Example
```c
#include <c-string/lib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

int main(void) {
  String str = String_from("Hello, world!");
  printf("%s\n", str); // Works, since it is NULL terminated.
  String_append(&str, "\nfoo bar");
  printf("%s\n", str);
  String str2 = String_clone(str);
  assert(String_equal(str, str2)); // Identical
  String_append(&str2, "\nbaz");
  assert(!String_equal(str, str2)); // Different, since one of them changed.
  String str3 = String_slice(str2, 0, 5);
  assert(!strcmp(str3, "Hello")); // You can use strcmp because it is NULL terminated.
  // Always be sure to cleanup(aka. free) the strings to prevent memory leaks.
  String_cleanup(str);
  String_cleanup(str2);
  String_cleanup(str3);
}
```

## Types

### StringBuffer
A typedef of `char *`. Used to represent a vector of characters. Is mainly used to represent the underlying buffer of [String](#String) but can also be used like `StringBuffer` in Java, `std::stringstream` in C++ or `StringSink` in AssemblyScript. Made with [c-vector](https://github.com/fabriciopashaj/c-vector).

### String
A typedef of `char const *`. Used to represent a string. It is `NULL` terminated to stay compatitible with the C standrd library.

### Vector\_String
A vector of [String](#String)s, just `CVECTOR(String)`, mafe with [c-vector](https://github.com/fabriciopashaj/c-vector). Used by [`String_split`](#String\_split) and [`String_split_by_char`](#String_split_by_char).

### FixedString
A struct describing an immutable fixed-size string.
#### Fields
  - `int len`: The length.
  - `char const *str`: The pointer the content.

## Functions
Together with the functions declared below, there are other functions that operate on [StringBuffer](#StringBuffer) that are generated from the [CVECTOR\_WITH\_NAME macro from c-vector](https://github.com/fabriciopashaj/c-vector#Fat-pointer-mode).

### StringBuffer\_push\_push\_bytes
Pushes a number of bytes one-by-one into the buffer.
  - Arguments:
    - `StringBuffer *sb`: The buffer where the bytes will be pushed.
    - `char *bytes`: The pointer to the bytes that will be pushed.
    - `int n`: Then number of bytes that will br pushed.
  - Return(`CVECTOR_STATUS`): Read [silent-mode of c-vector](https://github.com/fabriciopashaj/c-vector#Silent-mode).

### StringBuffer\_push\_str
Pushes a `NULL` terminated string into the buffer.
  - Arguments:
    - `StringBuffer *sb`: The buffer where the string will be pushed.
    - `char *str`: The string that will be pushed.
  - Return(`CVECTOR_STATUS`): Read [silent-mode of c-vector](https://github.com/fabriciopashaj/c-vector#Silent-mode).

### String\_new
Creates a new empty [String](#String).
  - Arguments: None
  - Return(`String`): The newly created empty [String](#String).

### String\_fake
Creates a fake [String](#String) from a string literal by using compound literals, valid only inside the scope that it is created.
  - Arguments:
    - `char const *`: A C string literal.
  - Return([String](#String)): The fake [String](#String).

### String\_from\_bytes
Creates a new [String](#String) from a chunk of bytes.
  - Arguments:
    - `char *bytes`: The pointer to the bytes that will be used as content.
    - `int n`: The number of bytes that wil be used.
  - Return([`String`](#String)): The new [String](#String) with the chunk if bytes as content.

### String\_from
Creates a new [String](#String) from a plain `NULL` terminated string.
  - Arguments:
    - `char *str`: The `NULL` terminated string.
  - Return([`String`](#String)): The new [String](#String) with the `NULL` terminated string as content.

### String\_len
  - Arguments:
    - `String str`: The string.
  - Return(`int`): The length of the string.

### String\_clone
Clones a [String](#String). Works like how `strdup` duplicates a `NULL` terminated string.
  - Arguments:
    - `String str`: The string that will be cloned.
  - Return([`String`](#String)): A new identical [String](#String).

### String\_concat
Concatenates the contents of two [String](#String)s into one.
  - Arguments:
    - `String str1`: The first string.
    - `String str2`: The second string.
  - Return([`String`](#String)): A new [String](#String) with the concatenated contents.

### String\_slice
Slices a segment of a [String](#String). Negative backward indexing is allowed(i.e. index `-1` refers to the last item).
  - Arguments:
    - `String str`: The [String](#String) from which the segment will be sliced.
    - `int fi`: The start index.
    - `int li`: The end index.
  - Return([`String`](#String)): The sliced segment. If `fi` is smaller than `li` or `fi`/`li` are smaller than `0` even after they are translated to psitive indices or if they are greater or equal than the length of the string, `NULL` will be returned.

### String\_hash
Hashes a [String](#String) using the MurmurHash-2 non-cryptographic hashing function.
  - Arguments:
    - `String str`: The [String](#String) that will be hashed.
  - Return(`uint32_t`): The hash of `str`.

### String\_append
Appends a plain `NULL` terminated string at the end.
  - Arguments:
    - `String *str`: The pointer to the [String](#String) variable. It will probably reallocate the string to a bigger memory location, so it needs to reassign the variable too.
    - `char *bstr`: The `NULL` terminated string that will be appended.
  - Return(`CVECTOR_STATUS`): Read [silent-mode of c-vector](https://github.com/fabriciopashaj/c-vector#Silent-mode). On verbose-mode, it will return `1` if pushing any of the character fails, otherwise `1`.

### String\_equal
Compares two [String](#String)s.
  - Arguments:
    - `String str1`,
    - `String str2`: The strings that will be compared.
  - Return(`bool`): `true` if both [String](#String)s are equal in length and content, otherwise `false`.

### String\_cleanup
Frees the [String](#String) from memory.
  - Arguments:
    - `String str`: The string that will be freed.
  - Return(`void`)

### String\_to\_fixed
Creates and returns the [fixed](#FixedString) version of a [String](#String)
  - Arguments:
    - `String str`: The source.
  - Return([`FixedString`](#FixedString)): The fixed string with the contents of the first argument.

### String\_split
Splits a [String](#String) by a character sequence.
  - Arguments:
    - `String str`: The string that will be splitted.
    - `char const *seq`: The character sequence.
  - Return([`Vector_String`](#Vector_String)): The new strings that resulted from the split.

### String\_split\_by\_char
Like [`String_split`](#String_split), but splits by a character instead of a sequence.
  - Arguments:
    - `String str`: The string that will be splitted.
    - `char c`: The character.
  - Return([`Vector_String`](#Vector_String)): The new strings that resulted from the split.

### String\_trim\_start
Removes the leading `\t`, ` `, `\n` and `\0` characters from the start of a [String](#String).
  - Arguments:
    - `String str`: The [String](#String) that will be operated on.
  - Return(`void`)

### String\_trim\_end
Works like [String\_trim\_start](#String_trim_start) but operates at the end of the [String](#String).
  - Arguments:
    - `String str`: The [String](#String) that will be operated on.
  - Return(`void`)

### String\_trim
Applies [String\_trim\_start](#String_trim_start) [String\_trim\_end](#String_trim_end) on the [String](#String).
  - Arguments:
    - `String str`: The [String](#String) that will be operated on.
  - Return(`void`)

### StringBuffer\_to\_string
Creates and returns a new [String](#String) with the current collected bytes of the [StringBuffer](#StringBuffer) as content.
  - Arguments:
    - `StringBuffer sb`: The [StringBuffer](#StringBuffer) that holds the contents.
  - Return([`String`](#String)): The newly created [String](#String).

### StringBuffer\_transform\_to\_string
Modifies the [StringBuffer](#StringBuffer) and transforms it into a [String](#String). Is useful when you are done writing to the [StringBuffer](#DtringBuffer) and now you want a [String](#String) with that content, but you want to prevent memory allocations.
  - Arguments:
    - `StringBuffer sb`: The [StringBuffer](#StringBuffer) that will be transformed.
  - Return([`String`](#String)): Essentially `sb`.

### FixedString\_from
Creates a [FixedString](#FixedString) from a plain static string literal.
  - Arguments:
    - `char *str`: The string literal.
  - Return([`FixedString`](#FixedString)): The newly created [FixedString](#FixedString).

### FixedString\_cleanup
`free()`s the `str` field if it isn't `NULL` and zeroes all the fields of the struct.
  - Arguments:
    - `FixedString *fstr`: The [FixedString](#FixedString) that will be operated on.
  - Return(`void`)
