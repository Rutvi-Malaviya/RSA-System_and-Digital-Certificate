#include<iostream>
#include "algorithms.cpp"

using namespace std;

class Rsa{

    int p, q, d;
    Algorithms algo;

    public:
        int e, n;

        Rsa(bool print){
            generateKeys(print);
        }
    
        int getNumber(bool print);

        void generateKeys(bool print);

        int decrypt(int c);

        int encrypt(int m, bool print);

        void printKeys();

        int getCertificateHash(string name);

};

int Rsa::getNumber(bool print){
    /** 
    * print: bool variable if the method prints a trace or not
     * 
     * This metod generate a random number of 7 bits with first and last bits set to 1
     * This method uses the getRandomNumber() method to get random numbers which are used for generating remaining 5 bits of the number
     **/
            
    int bit[7];

    // first and last bit of the number are always 1
    bit[0]=1;
    bit[6]=1;
    int number = 65; // 65 is 1000001 which is the number with first and last bit set to 1

    if(print)
        printf("line:24");

    for(int i=5; i>0; i--){
        int t = getRandomNumber();
                
        if(t&1){
            bit[i]=1;
            number = number | (1 << i);

            if(print)
                printf("\nb_%d|%d|1",i,t);
        }
        else{
            bit[i]=0;

            if(print)
                printf("\nb_%d|%d|0",i,t);
        }
    }

    if(print){
        printf("\nNumber|%d|",number);
        printBinary32(number);
    }
            
    return number;
}


void Rsa::generateKeys(bool print){
    /** This method takes care of generating key used for RSA.
     * The method implements logic from generating prime number to using extended euclidean algorithm to get d for a given e
     * 
     * Returns the RSA key generated
     **/

    // Generate 2 random numbers
    p = getNumber(print);
    q = getNumber(false);

    if(print) printf("\n");

    // Check whether the numbers generated are prime
    primalityResult *r1 = algo.isPrime(p);
    primalityResult *r2 = algo.isPrime(q);

    // print trace if required and make sure that both p and q are prime
    if(r1->prime){
        if(!r2->prime){
            // p is prime, q is not prime

            if(print){
                printf("\nline:139");
                algo.primalityTesting(r2->n, r2->a, print);
            }

            do{
                q = getNumber(false);
            }
            while(p==q || (!((r2 = algo.isPrime(q))->prime)));

            if(print){
                printf("\n\nline:145");
                algo.primalityTesting(r1->n, r1->a, print);
            }
        }
        else{         
            // Both p and q are prime

            if(print){
                primalityResult *r;

                do{
                    int t = getNumber(false);
                    t--;   
                    r = algo.isPrime(t);
                }
                while(r->prime);

                printf("\nline:139");
                algo.primalityTesting(r->n, r->a, print);

                printf("\n\nline:145");
                algo.primalityTesting(r1->n, r1->a, print);
            }

            if(p==q){
                do{
                    q = getNumber(false);
                }
                while((!((r2 = algo.isPrime(q))->prime)) || p==q);
            }
        }
    }
    else{
        if(!r2->prime){
            // Both p and q are not prime

            if(print){ 
                printf("\nline:139");
                algo.primalityTesting(r2->n, r2->a, print);
            }

            do{
                q = getNumber(false);
            }
            while(p==q ||(!((r2 = algo.isPrime(q))->prime)));

            do{
                p = getNumber(false);
            }
            while((!((r1 = algo.isPrime(p))->prime)) || p==q);

            if(print){
            printf("\n\nline:145");
                algo.primalityTesting(r1->n, r1->a, print);
            }
        }
        else{
            // p is not prime, q is prime

            if(print){
                printf("\nline:139");
                algo.primalityTesting(r1->n, r1->a, print);
            }

            do{
                p = getNumber(false);
            }
            while((!((r1 = algo.isPrime(p))->prime)) || p==q);

            if(print){
                printf("\n\nline:145");
                algo.primalityTesting(r2->n, r2->a, print);
            }
        }
    }

    // Perform extended euclidean algorithm to find e and d (encryption and decryption keys)

    if(print) printf("\n\nline:162");

    n = p*q;
    int phi_n = (p-1)*(q-1);

    // Check for each e starting 3 whether there exists an inverse for e in modulo phi_n
    e=3;
    d=-1;
    while(((d = algo.extendedEuclidean(e,phi_n, print))==-1) && e<phi_n) 
        e++;

    if(print){
        printf("\n");
        printf("\nline:173");
        printf("\nd = %d",d);
    } 
}


int Rsa::decrypt(int c){
    /** This method decrypts the cipher c using the decryption key d.
     * Uses fast exponentiation algorithm to calculate c^d mod n
     **/

    int t = d;
    int x[32] = {0};
        
    for(int i=0; i<32 && t!=0; i++){
        x[i] = (t&1);
        t = t>>1;
    }

    return algo.fastExponentiation(x, c, n, false);
}


int Rsa::encrypt(int m, bool print){
    /** This method encrypt the message m using the encryption key e.
     * Uses fast exponentiation algorithm to calculate m^e mod n
     **/

    int t = e;
    int x[32] = {0};

    for(int i=0; i<32 && t!=0; i++){
        x[i] = (t&1);
        t = t>>1;
    }

    return algo.fastExponentiation(x, m, n, print);
}


void Rsa::printKeys(){

    printf("\np = %d, q = %d, n = %d, e = %d, d = %d", p, q, n, e, d);
    printf("\np = ");
    printBinary32(p);
    printf("\nq = ");
    printBinary32(q);
    printf("\nn = ");
    printBinary32(n);
    printf("\ne = ");
    printBinary32(e);
    printf("\nd = ");
    printBinary32(d);

}


int Rsa::getCertificateHash(string name){
    /**
    * The method finds the r for digital certificate
     * Uses the getHash() function
    **/

    int r[14*8] = {0};
    int j = name.size();
    int ind=0;

    // Generating the certificate r

    for(int i=0; i<6*8; i+=8){
        // 6 byte name left padded by space

        int t;
        if(j-i>0){
            t=' ';
        }  
        else{
            t =  name[ind];
            ind++;
        } 

        for(int k=i+7; k>=i; k--){
            r[k] = (t&1);
            t = t>>1;
        }
    }

    int temp_n = n, temp_e = e;

    for(int i = 79; i >= 48; i--){
        // 4 byte n, padded by leading 0

        r[i] = (temp_n & 1);
        temp_n = temp_n >> 1;
    }
        
    for(int i = 14 * 8 - 1; i >= 80; i--){
        // 4 byte e, padded by leading 0

        r[i] = (temp_e & 1);
        temp_e = temp_e >> 1;
    }

    printf("\nr = ");
    for(int i=0; i<14*8; i++) 
        printf("%d",r[i]);

    // Find the hash of r

    int hash = algo.getHash(r, 14*8);
    printBinary32(hash);

    return hash;
}