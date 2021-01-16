#include <iostream>
#include <ctime>
#include<set>
#include "rsa.cpp"

using namespace std;

int getRandomNumberSmallerThan(int n){
//     returns a number which is smaller than the input number n
    int k = 0;

    while(n){
        k++;
        n = n>>1;
    }

    int bits[32] = {0};

    bits[k-1]=1;
    bits[0]=1;

    for(int i=1; i<k-1; i++){
        bits[i] = (getRandomNumber()&1);
    }

    int u=0;

    for(int i=k-1; i>=0; i--) 
        u = 2*u + bits[i];

    printf("\nline:231");
    printf("\nk = %d, u = %d",k,u);

    return u;
}


int main(){
    srand((unsigned) time(0));

    Rsa alice(true);
    Rsa trent(false);

    Algorithms algo;

    printf("\n");

    printf("\nline:177");
    alice.printKeys();

    printf("\n");

    printf("\nline:185");
    trent.printKeys();

    printf("\n");

    printf("\nline:207");
    int h = alice.getCertificateHash("Alice");
    int s = trent.decrypt(h);
    printf("\ns = "); printBinary32(s);

    printf("\n");

    printf("\nline:209");
    printf("\nh(r) = %d, s = %d",h,s);

    printf("\n");

    int u = getRandomNumberSmallerThan(alice.n);

    printf("\n");
    printf("\nline:233");
    printf("\nu = "); printBinary32(u);

    printf("\n");
    printf("\nline:239");

    int h_u = algo.getHash(u);
    int v = alice.decrypt(h_u);
    int ev = alice.encrypt(v,false);

    printf("\nu = %d, h(u) = %d, v = %d, Ev = %d", u, h_u, v, ev);

    printf("\n");

    printf("\nline:242");
    alice.encrypt(v,true);
    printf("\n");

    return 0;
}
