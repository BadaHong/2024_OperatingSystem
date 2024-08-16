#include "types.h"
#include "stat.h"
#include "user.h"

#define NUM_LOOP 10000

int main(int argc, char* argv[]){
        int p = fork();
        if(p == 0){ //if child process
                for(int i = 0; i < NUM_LOOP; i++){
                        printf(1, "Child\n");
                        yield();
                }
        }
        else{
                for(int i = 0; i < NUM_LOOP; i++){
                        printf(1, "Parent\n");
                        yield();
                }
        }
        exit();
}

