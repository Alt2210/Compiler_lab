//gcc -I. -I/opt/homebrew/include -L/opt/homebrew/lib -o unit_test_scanner unit_test_scanner.c reader.c charcode.c token.c error.c parser.c -lcmocka


#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "reader.h"
#include "charcode.h"
#include "token.h"
#include "error.h"
#include "scanner.c"
#include "parser.h"

extern Token *lookAhead;

static void compare_file_contents(const char *file1, const char *file2) {
    FILE *f1 = fopen(file1, "r");
    FILE *f2 = fopen(file2, "r");
    assert_non_null(f1);
    assert_non_null(f2);

    char line1[1024], line2[1024];
    while (fgets(line1, sizeof(line1), f1) &&
           fgets(line2, sizeof(line2), f2)) {
        assert_string_equal(line1, line2);
    }

    fclose(f1);
    fclose(f2);
}

static void test_parser(const char *input_filename,
                        const char *expected_output_filename) {

    int status = openInputStream((char*)input_filename);
    assert_int_equal(status, IO_SUCCESS);

    // redirect stdout
    char temp_filename[256];
    snprintf(temp_filename, sizeof(temp_filename), "temp_%s.txt", input_filename + 8);

    int stdout_backup = dup(fileno(stdout));
    FILE *temp_file = freopen(temp_filename, "w", stdout);
    assert_non_null(temp_file);

    // init token đầu tiên
    lookAhead = getValidToken();

    // gọi parser chính
    compile((char*)input_filename);

    fflush(stdout);
    dup2(stdout_backup, fileno(stdout));
    close(stdout_backup);

    // so sánh file output
    compare_file_contents(temp_filename, expected_output_filename);

    remove(temp_filename);
    closeInputStream();
}

static void test_input1(void **state) {
    test_parser("../test/example1.kpl", "../test/result1.txt");
}

static void test_input2(void **state) {
    test_parser("../test/example2.kpl", "../test/result2.txt");
}

static void test_input3(void **state) {
    test_parser("../test/example3.kpl", "../test/result3.txt");
}

static void test_input4(void **state) {
    test_parser("../test/example4.kpl", "../test/result4.txt");
}

static void test_input5(void **state) {
    test_parser("../test/example5.kpl", "../test/result5.txt");
}

static void test_input6(void **state) {
    test_parser("../test/example6.kpl", "../test/result6.txt");
}

static void test_input7(void **state) {
    test_parser("../test/example7.kpl", "../test/result7.txt");
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_input1),
        cmocka_unit_test(test_input2),
        cmocka_unit_test(test_input3),
        cmocka_unit_test(test_input4),
        cmocka_unit_test(test_input5),
        cmocka_unit_test(test_input6),
        cmocka_unit_test(test_input7),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
