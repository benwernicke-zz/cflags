# cflags
Small Header only library to parse argv for flags
# Usage:
      #define FLAG_CAPACITY <number_of_flags>
      #include "flag.h>
  
  
      int main(int argc, char** argv)
      {
        //arg_flag takes argument
        arg_flag(<variable_name>, <flag_name>, <flag_description>);

        //bool_flag takes no argument
        bool_flag(<variable_name>, <flag_name>, <flag_description>);
        filter_flags(&argc, argv);
                                             
        //u can now access flags by:
        if (<flag_name>->valid)
          printf("%s\n", (type)<flag_name>->content); // in case of string
  
        // every flag and every parameter for a flag get filtered out of argv
        return 0;
      }

# Example
      #define FLAG_CAPACITY 2
      #include "flag.h"
      
      int main(int argc, char** argv)
      {
        flag_t* a = set_flag(BOOL, "-a", "boolean flag"); //just a bool
        flag_t* b = set_flag(STR, "-b", "string flag"); //takes an argument
        bool_flag(a, "-a", "a nice bool flag");
        arg_flag(b, "-b", "a nice arg flag");
        filter_flags(&argc, argv);
        
        if (a->valid)
          printf("%s\n", a->content);
        if (b->valid)
          printf("%d\n", b->content);
        return 0; 
      }
