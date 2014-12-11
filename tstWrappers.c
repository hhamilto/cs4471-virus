#include <stdio.h>

int main(int argc char** agrv){
FILE *file; 
struct stat fileStat;

//call open on file
file = open(argv[1], "r");

if(file == null){
printf("no file\n",);
}

//call fstat
if(stat(argv[1],&fileStat) !=0){
	printf("no file\n",);
}

//print the file size an 1st 8 bites in hex
printf("file size:%d\n", fileStat-> st_size);

printf("1st eight bytes in hex:");

for(i = 0 i < 8 i++){
printf("%x", fgetc(file));
}

// call close
close(file);

return 0;	
}