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
	string_view_t view = string_slice(&str, 0, 5).opt;
	assert(string_concat_view(&str, view) == NONE);
	assert(string_concat_view(&str, (string_view_t){}) == INVALID_ARG);
	printf(string_fmt "\n", string_arg(str));
	printf(string_fmt "\n", string_arg(view));
	string_free(&str);
	count++;
}

void test_string_static(void) {
	string_t str = {};
	char data[] = "Hello, I am Dinesh!\n";

	assert(string_static(&str, (char*) data, sizeof(data)) == NONE);
	assert(str.len == 21);
	assert(str.cap == 21);
	printf(string_fmt "\n", string_arg(str));

	string_free(&str);
	count++;
}

int main(void) {
	test_string_init();
	test_string_append();
	test_string_slice();
	test_string_concat_view();
	test_string_static();
	INFO("%d test(s) ran successfully\n", count);
	return 0;
}
