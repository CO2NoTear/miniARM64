#include <stdio.h>
int main(){
    int x = 1;
    int z;
    int y;
    scanf("%d\n", &y);
    if(y >= 2){
        z = 1;
    }
    else{
        y = 1;
    }
    x = z;
    printf("%d\n", z);
}