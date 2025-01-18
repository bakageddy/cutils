#define CUTIL_STRING_IMPLEMENTATION
#include "string.h"
#include "debug.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

int count = 0;

void test_string_init() {
	string_t str = {};

	err_code res = string_init(&str);
	assert(res == NONE);
	assert(str.data != NULL);
	assert(str.cap == CUTIL_STRING_INITIAL_CAPACITY);
	assert(str.len == 0);

	string_free(&str);
	assert(str.len == 0);
	assert(str.cap == 0);
	assert(str.data == NULL);
	count++;
}

void test_string_append() {
	string_t str = {};
	string_init(&str);

	err_code res = string_append(&str, 'a');
	assert(res == NONE);
	assert(str.len == 1);
	assert(str.data[str.len - 1] == 'a');

	for (int i = 1; i <= 16; i++) {
		assert(string_append(&str, 'a' + i) == NONE);
	}
	assert(str.len == (16 + 1));
	assert(str.cap == 32);
	assert(memcmp(str.data, "abcdefghijklmnopq", str.len) == 0);
	string_free(&str);
	count++;
}

void test_string_slice() {
	string_t str = {};
	string_init(&str);
	for (int i = 0; i < 10; i++) {
		string_append(&str, 'a' + i);
	}
	optstring_view_t substr = string_slice(&str, 3, 4);
	assert(substr.tag == NONE);
	for (int i = 0; i < 4; i++) {
		assert((substr.opt.data + i) == (str.data + 3 + i));
	}
	string_free(&str);
	count++;
}

void test_string_concat_view() {
	string_t str = {};
	string_init(&str);
	for (int i = 0; i < 10; i++) {
		string_append(&str, 'a' + i);
	}
	// string_view_t view = str
	string_free(&str);
	count++;
}

int main(void) {
	test_string_init();
	test_string_append();
	test_string_slice();
	test_string_concat_view();
	INFO("%d test(s) ran successfully\n", count);
	return 0;
}
