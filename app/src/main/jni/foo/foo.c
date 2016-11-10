//
//  foo.c
//  foo
//
//  Created by ilegra on 11/8/16.
//  Copyright © 2016 ilegra. All rights reserved.
//

#include "foo.h"
#include <string.h>
#include <ctype.h>


int foo(int num){
    return 10 + num;
}

char* printString(char *in){

    int i;

    for(i=0; i< strlen(in); i++){
        in[i] = toupper(in[i]);
    }

    return in;
}