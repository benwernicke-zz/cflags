# cflags
Small Header only library to parse argv for flags
# Usage:
      #define FLAG_CAPACITY <number of flags>
      #include "flag.h>
  
  
      int main(int argc, char** argv)
      {
        flag_t* <flag_name> = set_flag( <BOOL/STR, "-<flag_name>");
        filter_flags(&argc, argv);
                                             
        //u can now access flags by:
        if (<flag_name>->valid)
          printf("%s\n", <flag_name>->content); // in case of string
  
        // every flag and every parameter for a flag get filtered out of argv
        return 0;
      }

# Example
      #define FLAG_CAPACITY 2
      #include "flag.h"
      
      int main(int argc, char** argv)
      {
        flag_t* a = set_flag(STR, "-a");
        flag_t* b = set_flag(BOOL, "--all");
        filter_flags(&argc, argv);
        
        if (a->valid)
          printf("%s\n", a->content);
        if (b->valid)
          printf("%d\n", b->content);
        return 0; 
      }
