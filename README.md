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
A typedef of `char *`. Used to represent a string. It is `NULL` terminated to stay compatitible with the C standrd library.

### FixedString
A struct describing an immutable fixed-size string.
#### Fields
  - `int len`: The length.
  - `char const *str`: The pointer the content.
  - `bool literal`: Indicates that the string is a static literal or a runtime defined one store on the heap.

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

### String\_slice
Slices a segment of a [String](#String). Negative backward indexing is allowed(i.e. index `-1` refers to the last item).
  - Arguments:
    - `String str`: The [String](#String) from which the segment will be sliced.
    - `int fi`: The start index.
    - `int li`: The end index.
  - Return([`String`](#String)): The sliced segment. If `fi` is smaller than `li` or `fi`/`li` are smaller than `0` even after they are translated to psitive indices or if they are greater or equal than the length of the string, `NULL` will be returned.

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
