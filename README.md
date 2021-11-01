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
        bool_flag(a, "-a", "boolean flag"); //just a bool
        arg_flag(b, "-b", "string flag"); //takes an argument
        
        filter_flags(&argc, argv);
        
        if (a->valid)
          printf("a: valid");
        if (b->valid)
          printf("b: %d\n", b->content);
          
        //U can parse the rest of argv now
        for(int i = 0; i < argc; i++)
            printf("%s\n", argv[i]);
        return 0; 
      }
