#include<iostream>
#include<set>

using namespace std;

int getModulo(int num, int n){
    /**
     * The method finds num modulo n
     **/

    if(num<0){
        while(num<0) num+=n;
        return num;
    }

    int q = num/n;
    num = num - (n*q); // gives the remainder of num/n
    return num;
}


int getRandomNumber(){
    /**
     * returns a random number
     **/

    int num = rand();
    return num;
}


int multModulo(int num1, int num2, int n){
    /**
     * The method returns result of (num1 * num2) mod n
     * Uses the getModulo() mehtod defined above
     **/

    return getModulo(getModulo(num1,n) * getModulo(num2,n), n);
}


void printBinary32(int n){
    
    /**
     * This method prints value of n in a 32 bit binary representation
     **/
    
    int bits[32] = {0};
    
    for(int i=0; i<32; i++){
        bits[i] = (n&1);
        n = n>>1;
    }

    for(int i=31; i>=0; i--) printf("%d",bits[i]);
}


class primalityResult{
    /**
     * This class represents the result of primality testing of a number n for a particular value of variable a.
     * This class can is used for tracking the value of a for which the test failed and is used for purpose of printing trace
     **/
    public:
        bool prime;
        int n;
        int a;

        primalityResult(bool prime, int n, int a){
            this->prime = prime;
            this->n = n;
            this->a = a;
        }
};

class Algorithms{

    public:
        int fastExponentiation(int x[], int a, int n, bool print);

        bool primalityTesting(int n, int a, bool print);

        primalityResult * isPrime(int n);

        int extendedEuclidean(int e, int phi_n, bool print);

        int getHash(int n);

        int getHash(int bits[], int size);

};

int Algorithms::fastExponentiation(int x[], int a, int n, bool print){
    /**
     * This method implements fast exponentiation algorithm to find a^x mod n. 
     * x is taken in the binary representation as input
     **/

    int y=1;
    bool skip = true;

    if(print) 
        printf("\ni |xi |y     |y");

    for(int i=31; i>=0; i--){
        if(x[i]==0 && skip) continue;
        skip = false;

        y = multModulo(y,y,n);
        if(print) 
            printf("\n%-d |%-2d |%-5d |",i,x[i],y);

        if(x[i]==1) 
            y = multModulo(a,y,n);

        if(print) printf("%d",y);
    }

    return y;
}

bool Algorithms::primalityTesting(int n, int a, bool print){
    /** 
     * print: variable use to indicate whether to print execution trace
     * 
     * This method does primality testing for n using a, 1 <= a < n.
     * The method computes a^(n-1) mod n
     * 
     * This method uses multModulo() method
     **/

    // convert n to 7 bit binary representation
    int x[7];
    int t = n-1;

    for(int i=0; i<7; i++){
        x[i]=(t&1);
        t = t>>1;
    }

    // Perform primality testing

    int y=1;

    if(print){
        printf("\nn = %d, a = %d ",n,a);
        printf("\ni   |xi  |z   |y   |y ");
    }
            
    bool skip = true;  // variable used to skip leading zeros

    for(int i=6; i>=0; i--){
        if(skip && x[i]==0) continue;
        skip = false;
                
        int z = y;
        y = multModulo(y,y,n);

        if(print)
            printf("\n%-3d |%-3d |%-3d |%-3d |",i,x[i],z,y);

        if(y==1 && z!=1 && z!=n-1){
            //  z^2 mod n = 1 for 1 < z < n-1 ==> Number is not prime  

            if(print){
                i--;

                // printing remaining bits
                while(i>=0){
                    printf("\n%-3d |%-3d |    |    |", i,x[i]);
                    i--;
                }
                        
                printf("\n%d is not a prime number because %d^2 mod %d = 1 and %d != 1 and %d != %d - 1",n,z,n,z,z,n);
            }
                        
            return false;
        } 

        if(x[i]==1) 
            y = multModulo(y,a,n);

        if(print)
            printf("%-3d",y);
    }

    if(y!=1){

        // a^(n-1) mod n != 1 ==> Number is not prime 

        if(print)
            printf("\n%d is not a prime number because %d^%d mod %d != 1",n,a,n-1,n);
                
        return false;
    } 

    if(print)
        printf("\n%d is perhaps a prime",n);
            
    return true;
}

primalityResult * Algorithms::isPrime(int n){
    /**
     * This method checks if n is prime or not
     * 
     * The method uses primalityTesting() method which checks whether n is prime for a particular a where 1 < a < n.
     * The method uses primalityTesting() for 20 random a's 
     * 
     * The method return an object of primalityResult which can be used for printing purposes
     **/
            
    set<int> checked;
    checked.insert(0);
            
    int a;
    for(int i=0; i<20; i++){

        do{
            a = getModulo( getRandomNumber(), n);
        }
        while(checked.count(a));
            
        if(!primalityTesting(n,a,false)) 
            return new primalityResult(false,n,a);
    }
            
    return new primalityResult(true, n, a);
}


int Algorithms::extendedEuclidean(int e, int phi_n, bool print){
    /**
     * This method implements extendedEuclidean for values e and phi_n
     * print is used for indicating whether to print execution trace
     * 
     * The method uses getModulo() method
     *
     * Returns e inverse with repect to modulo phi_n
     **/

    // q = current quotient, q1 = quotient at i-1, q2, quotient at i-2
    int q, q1, q2; 

    // r1 = r at i-1, r2 = r at i-1, r = current r  
    int r=phi_n, r1=e, r2;

    // s1 = s at i-1, s2 = s at s-1, s = current s
    int s=1, s1=0, s2=0;

    // t1 = t at i-1, t2 = t at i-1, t = current t
    int t=0, t1=0, t2=0;
    int i=1;

    if(print){
        printf("\ne = %d",e);
        printf("\ni |qi   |ri   |ri+1 |ri+2 |si   |ti ");
    }

    while(true){
        if(i==1){
            s = 1;
        }
        else if(i==2){
            s1 = s;      s = 0;
            t1 = t;      t = 1;
            q1 = q;
        }
        else{
            q2 = q1;    q1 = q;
            s2 = s1;    s1 = s;
            t2 = t1;    t1 = t;

            s = s2 - (q2 * s1);
            t = t2 - (q2 * t1);
        }

        if(r1==0){
            if(print) 
                printf("\n%-d |     |%-4d |     |     |%-4d |%-4d ",i,r,s,t);

            break;
        }

        q = r/r1;
        r2 = getModulo(r, r1);
                
        if(print) 
            printf("\n%-d |%-4d |%-4d |%-4d |%-4d |%-4d |%-4d ",i,q,r,r1,r2,s,t);
                
        r = r1;
        r1 = r2;
        i++;
    }

    if(r==1)
        return getModulo(t,phi_n);
     
//    returns -1 if inverse is not found
    return -1;
}

int Algorithms::getHash(int n){
    int bits[32]={0};

    for(int i=0; i<32 && n!=0; i++){
        bits[i] = (n&1);
        n = n>>1;
    }

    return getHash(bits, 32);
}

int Algorithms::getHash(int bits[], int size){

    int h[8] = {0};

    for(int i=0; i<size; i+=8){
        h[0] = h[0]^bits[i];
        h[1] = h[1]^bits[i+1];
        h[2] = h[2]^bits[i+2];
        h[3] = h[3]^bits[i+3];
        h[4] = h[4]^bits[i+4];
        h[5] = h[5]^bits[i+5];
        h[6] = h[6]^bits[i+6];
        h[7] = h[7]^bits[i+7];
    }

    int hash=0;
    for(int i=7; i>=0; i--)
        hash = (2*hash)+h[i];
        
    return hash;
}
