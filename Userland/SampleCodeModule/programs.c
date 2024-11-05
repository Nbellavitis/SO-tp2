#include "include/program.h"
#include "include/lib.h"
#define MAX_COMMAND_LENGTH 256
#define EOF -1
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
void testSync(int argc, char *argv[]) {

    if (argc != 2) {
        print(0xFFFFFFFF, "Usage: testSync <number_of_processes>\n");
        return;
    }
    test_sync(argc, argv);
    return;

}

void cat(){
    char c ;
    getC(&c);
    char comm[MAX_COMMAND_LENGTH]={0};
    int i=0;
    while (c != EOF){
      if(c != 0){
        putC(c,WHITE);
        comm[i++]=c;
        if(c=='\n'){
            comm[i]='\0';
            print(WHITE,"%s", comm);
            comm[0]='\0';
            i=0;
        }
       }
    getC(&c);
    }
    putC('\n',WHITE);
    return;
}




void wc(){
    char c ;
    getC(&c);
    int i=0;
    while (c != EOF){
        if(c != 0){
            if(c=='\n'){
                i++;
            }
        }
        getC(&c);
    }
    putInt(i,WHITE);
    putC('\n',WHITE);
}

void filter(){
    char c ;
    getC(&c);
    while (c != EOF){
        if(c != 0){
            if(c != 'a' && c != 'e' && c != 'i' && c != 'o' && c != 'u'){
                putC(c,WHITE);
            }
        }
        getC(&c);
    }
    putC('\n',WHITE);
}