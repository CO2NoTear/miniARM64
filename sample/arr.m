func(a,b)
{
    return a+b;
}
main()
{
    int arr[5];
    int a, b, c, d ;
    char z;
    arr[0] = 0;
    arr[1] = 1;
    arr[2] = 2;
    arr[3] = 3;
    arr[4] = 4;
    a = 20;
    b = a + 2;
    z = 'z';
    c = 2;
    # 0 + 1 + 2 + 2 = 5
    arr[3] = arr[0]+arr[1]+arr[2]+c;
    arr[4]=6;
    # 22 + 5 = 27
    arr[1]=b+5;
    # 27 + 2 = 29
    arr[2]=func(arr[1],arr[2]);
    # d = 22 + 2 = 24
    d = b + c;
    # a = -22*2 + 24 = -20
    a = - b * c + d ;

    if ( a < 0 ) { a = - b * c ; }
    # a = -22 * 2 = -44

    d = 999;
    printf("a = %d\n", a);
    printf("b = %d\n", b);
    printf("c = %d\n", c);
    printf("d = %d\n", d);
    printf("arr[0] = %d\n", arr[0]);
    printf("arr[1] = %d\n", arr[1]);
    printf("arr[2] = %d\n", arr[2]);
    printf("arr[3] = %d\n", arr[3]);
    return 0;
}

