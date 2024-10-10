#include "include/program.h"
#include "include/lib.h"
void priotest(int argc, char *argv[]) {
    test_prio();
}

void processtest(int argc, char *argv[]) {

    if (argc != 2) {

        print(0xFFFFFFFF, "Usage: proctest <number_of_processes>\n");
        return;
    }


    if (satoi(argv[1]) <= 0) {

        print(0xFFFFFFFF, "Mayor a 0 papi\n");
        return;
    }

    char *testParams[] = {argv[1]};

    int testResult = test_processes(1, testParams);
    return;
    print(0xFFFFFFFF,"Process test %s\n", testResult == 0 ? "passed" : "failed");

}