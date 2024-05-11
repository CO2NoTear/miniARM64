#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int opt;
    while ((opt = getopt(argc, argv, "o")) != -1) {
        switch (opt) {
            case 'o':
                printf("Optimizer: %s \n", optarg);
                break;
                break;
            default:    
                break;
        }
    }

    if(argc > 3 | argc < 2)
	{
        printf("Oops, Bad arguments using. Use %s -h for more information.\n", argv[0]);
	// 	printf("usage: %s filename [options] \n \
    // Options for optimizers:\n \
    //     Use -o=N to apply different Mahcine-Independent optimizers, where N is a number indicate that: \n \
    //     1 -> 123\n \
    //     2 -> 1234\n \
    //     6 -> All The Optimizers! \n \
    //     Also, feel free to use multiple optimizers for better optimition like -o=23. \n", argv[0]);
		exit(0);
	}


    return 0;
}