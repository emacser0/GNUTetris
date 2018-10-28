#include <stdio.h>
int a32,b32,r,i;
long long a,b,s,o=1; //long long for 32-bit architecture
int
main() {
    scanf("%d%d",&a32,&b32);
    a=a32,b=b32;b<<=1;
    for(;i<32;i++,a<<=1) {
        r=b&o;
        o<<=1;
        s+=!(b&o)!=!r?!(b&o)?a:-a:0;
    }
    printf("%lld\n",s);
}
