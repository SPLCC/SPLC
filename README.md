# CS323-Compiler-Project

## Phase 1

### Extended Feature List
- Error Detection:
  - Hanging else
  
  - Invalid function definition inside functions
  
  - Various errors about missing parenthesis/square bracket
  
  - Various errors about missing operands:
  
    ![image-20231028170745357](images/img-2.png)
  
  - Errors about invalid constant form
  
    ![image-20231028221130144](images/img-5.png)
  
    
  
- Features:
  - Optimized error/warning output
  
    ![image-20231028154347773](images/img-1.png)
  
    **This may cause the `diff`** **utility not to work when batch verifying**, as ASCII control sequences are used to color the output and they will not be recognized by `diff`.
  
    The parsing tree will not be colored.
  
  - Allowing unary operators: +/-
  
  - Allowing unary prefix/postfix operator: ++/--
  
  - Allowing for loop declaration
  
    - Allow any type of for loop combination:
  
      ```c
      for ([optional definition]; [optional expression]; optional expression)
          stmt
      ```
  
    - Run parser on `test_ex/test_6.spl` for details.
  
  - Allowing the following floating-point declaration, given by `[0-9]*\.[0-9]+([eE][-+]?[0-9]+)?`:
  
    ```c
    float y = .3e-13;
    ```
  
  - Allow single-line/cross-line comments
  
    ```c
    int main()
    {
        int a = 0; // This is a single comment.
        int b = 0; // This is a cross-line comment \
                      as you can tell by changing the language server interpreting this comment to match the C language.
        int c = 233; // Hi! C
        /*
        This is a cross-line comment.
        */
        int d = c;
        /* Hi!
        */
       
        */ // This is a hanging comment, which should not be recognized
    }
    ```
  
    ![image-20231028203442987](images/img-3.png)
  
  - Allow single-line/cross-line strings:
  
    - Allowed escape characters: `\[abefnrtv\'"?]`
  
    ```c
    
    int main()
    {
        char msg1 = "Hi Just want to test!";
    
        char msg2 = "Hi \
                        Just want to test!";
    
        char msg3 = "Hi"
                    "Thanks!";
        printf("[%s]", msg);
        return 0;
    }
    ```
  
    <img src="images/img-4.png" alt="image-20231028211819722" style="zoom: 50%;" />
  
    
