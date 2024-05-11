main()
{
    int x, j, k, t, s;
    x = 2;
    j = x * 5;
    k =  1;
    if(k){
        k = 10 + j;
        if(k > 10){
            s = -1;
        }
        else{
            s = -2;
        }
    }
    else{
        k = 10 / j;
    }
    t = k - j;
    print(x);
    print(j);
    print(k);
    print(t);
}