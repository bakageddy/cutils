#ifndef CUTIL_STRING_H
#define CUTIL_STRING_H

#ifndef CUTIL_STRING_INITIAL_CAPACITY
#define CUTIL_STRING_INITIAL_CAPACITY 16
#endif // CUTIL_STRING_INITIAL_CAPACITY

#include <stdint.h>

typedef struct {
	char *data;
	uint32_t len;
	uint32_t cap;
} string_t;

typedef struct {
	char *data;
	uint32_t len;
} string_view_t;

typedef enum {
	NONE = 0,
	INVALID_ARG,
	OUT_OF_MEMORY,
	OUT_OF_BOUNDS,
	CONTAINER_EMPTY,
} err_code;

typedef struct {
	err_code tag;
	char opt;
} optchar_t;

typedef struct {
	err_code tag;
	string_view_t opt;
} optstring_view_t;

#define string_fmt "%.*s"
#define string_arg(x) (x).len, (x).data

err_code string_init(string_t *str);
err_code string_append(string_t *str, char c);
err_code string_concat(string_t *dst, const string_t src);
err_code string_concat_view(string_t *dst, const string_view_t src);
optstring_view_t string_slice(string_t *str, uint32_t idx, uint32_t len);
void string_free(string_t *str);

#define CUTIL_STRING_IMPLEMENTATION // Summa
#ifdef CUTIL_STRING_IMPLEMENTATION

#include <stddef.h>
#include <stdlib.h>

err_code string_init(string_t *str) {
	if (str == NULL) return INVALID_ARG;
	str -> data = (char *) calloc(CUTIL_STRING_INITIAL_CAPACITY, sizeof(char));
	if (str -> data == NULL) return OUT_OF_MEMORY;
	str -> len = 0;
	str -> cap = CUTIL_STRING_INITIAL_CAPACITY;
	return NONE;
}

err_code string_grow(string_t *str) {
	if (str == NULL) return INVALID_ARG;
	char *temp = (char *) realloc(str -> data, str -> cap * 2 * sizeof(*str -> data));
	if (temp == NULL) return OUT_OF_MEMORY;
	str -> data = temp;
	str -> cap *= 2;
	return NONE;
}

err_code string_append(string_t *str, const char c) {
	if (str == NULL) return INVALID_ARG;
	if (str -> cap == str -> len) {
		err_code res = string_grow(str);
		if (res != NONE) return res;
	}
	str -> data[str -> len] = c;
	str -> len += 1; //  NOTE: Don't make it confusing by using ++
	return NONE;
}

// I am not taking it as a pointer since I don't change src
err_code string_concat(string_t *dst, const string_t src) {
	if (dst == NULL || src.data == NULL) return INVALID_ARG;
	for (uint32_t i = 0; i < src.len; i++) {
		err_code res = string_append(dst, src.data[i]);
		if (res != NONE) return res;
	}
	return NONE;
}

err_code string_concat_view(string_t *dst, const string_view_t src) {
	if (dst == NULL || src.data == NULL) return INVALID_ARG;
	for (uint32_t i = 0; i < src.len; i++) {
		err_code res = string_append(dst, src.data[i]);
		if (res != NONE) return res;
	}
	return NONE;
}

optstring_view_t string_slice(string_t *str, const uint32_t idx, const uint32_t len) {
	if (str == NULL || idx >= str -> len || idx + len - 1 >= str -> len) {
		return (optstring_view_t) {
			.tag = INVALID_ARG,
			.opt = { .data = NULL, .len = 0 },
		};
	}
	return (optstring_view_t) {
		.tag = NONE,
		.opt = { .data = str -> data + idx, .len = len }
	};
}

void string_free(string_t *str) {
	free(str -> data);
	str -> data = NULL;
	str -> len = 0;
	str -> cap = 0;
}

#endif // CUTIL_STRING_IMPLEMENTATION
#endif // CUTIL_STRING_H
