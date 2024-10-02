#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "cbmp.h"
#include "functions.h"

int main() {
    printf("\nStarting tests...\n");
    fflush(stdout);
    printf("------------------------------\n");

    test_assert_equal();
    printf("------------------------------\n");
    fflush(stdout);

    test_invert();
    printf("------------------------------\n");
    fflush(stdout);

    test_greyscale();
    printf("------------------------------\n");
    fflush(stdout);

    test_binary_threshold();
    printf("------------------------------\n");
    fflush(stdout);

    test_binary_erosion();
    printf("------------------------------\n");
    fflush(stdout);

    printf("All tests completed.\n");
    fflush(stdout);
    return 0;
}
