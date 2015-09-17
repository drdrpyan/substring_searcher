#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void read_file(FILE *f, char *seq, char *seq_name){
   char ch;
   
   //For file name parsing ...
   while((ch=fgetc(f)) !='\n'){
     
      printf("%c",ch);
      
   }
   
   // sequece ... delete newlines
   while((ch=fgetc(f)) !=EOF){
      
      //if(ch!='\n') printf("%c",ch);
      printf("%c", ch);
      
   }
   printf("\n");
  
}

int main(int argc, char *argv[])
{
   FILE *fi;
   char *seq, *seq_name;

   if(argc<2){ printf("Argument Error !\n"); exit(1);}
   fi = fopen(argv[1], "r");
   if(fi==NULL){ printf("File open Error !\n"); exit(1); }
   printf("Input file : %s\n", argv[1]);

   read_file(fi, seq, seq_name);

  //printf("%s\n%s\n", seq_name, seq);       

}

