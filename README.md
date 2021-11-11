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
        arg_flag(a, "-a", "string flag"); //takes an argument
        bool_flag(b, "-b", "boolean flag"); //just a bool
        pos_flag(p, "p", "positional flag", 1); //flag must be in a certain position of argv - in this case 1
       
        filter_flags(&argc, argv);
        
        if (a->valid)
            printf("a: %s\n", b->content);
        if (b->valid)
          printf("b: valid\n");
        if (p->valid)
          printf("p: valid\n");

          
        //U can parse the rest of argv now
        for(int i = 0; i < argc; i++)
            printf("%s\n", argv[i]);
        return 0; 
      }
