int i,j,k;

main() 
{
	int l,m,n;
	printf("%d, %d ,%d\n",i ,j ,k);
	l=1;
	m=2;
	n=3;
	printf("%d, %d ,%d\n",l ,m ,n);
	n=func(l,m,n);
	printf("%d, %d ,%d\n",l ,m ,n);
	printf("%d, %d ,%d\n",i ,j ,k);
	return 0;
}

func(o,p,q)
{
	printf("%d, %d ,%d\n",o ,p ,q);
	i=o;
	j=p;
	k=q;
	o=4;
	p=5;
	q=6;
	printf("%d, %d ,%d\n",i ,j ,k);
	return 999;
}

