#include<bits/stdc++.h>
#include<openssl/rand.h>
using namespace std;
string guuid() {
    unsigned char uuid[16];
    RAND_bytes(uuid, sizeof(uuid));
    stringstream ss;
    ss << hex << setfill('0');
    for(int i = 0; i < 16; i++) {
        ss << setw(2) << (int)uuid[i];
        if (i == 3 || i == 5 || i == 7 || i == 9)
            ss << "-";
    }
    return ss.str();
}

