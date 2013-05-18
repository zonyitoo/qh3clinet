#include "eapauth.h"

int main(int argc, char **argv) {
    EAPAuth auth("zhongyut", "Zonyitoo1314", "eth0");

    auth.serve_forever();

    return 0;
}

