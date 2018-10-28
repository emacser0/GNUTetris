#include <stdio.h>
int a32,b32;
long long a,b,s,b1,b2,o=1; //long long for 32-bit architecture
int
main() {
    scanf("%d%d",&a32,&b32);
    a=a32;
    b=b32;
    b<<=1;
    for(int i=0;i<32;i++,a<<=1) {
        b2=b&o;
        o<<=1;
        b1=b&o;
        if(!b1!=!b2) {
            if(b1) {
                s-=a;
            }
            else {
                s+=a;
            }
        }
    }
    printf("%lld\n",s);
}
