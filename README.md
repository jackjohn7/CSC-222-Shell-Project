[//]: # (This is a markdown file. It can be viewed on github.com/JingusJohn/CSC-222-Shell-Project with proper formatting)

# CSC-222-Shell-Project

This repo was made public on February 25, 2023 after the due date for the project.

## Execution

Code can be ran by executing the following commands (omit comments):

```bash
./run.sh # to compile the program

./run.sh dev # to compile the program and execute
```

***OR FOR PROFESSOR***

```bash
gcc -o techshell techshell.c shelllib.c
./techshell
```

## Authors

- Jack Branch
- Zak Kilpatrick

## Technical Details (Runtime Walkthrough)

1. A prompt is generated by `makePrompt` function.
   1. Creates a string with current working directory and $ appended
2. Retrieve user input
   1. Use `fgets` to get the input
   2. Remove trailing whitespace from input
3. Handle the user's input with `handleInput`
   1. If the user inputs "exit", exit the program.
      1. This is done by returning 0 to the caller.
   2. Otherwise, tokenize the input
      1. Iterate through each token in the input and look for special tokens.
         1. Certain tokens are permitted some aren't.
         2. An enum is used to track the status of tokens.
            1. Statuses include:
               1. NoRedirect
               2. RedirectInput
               3. RedirectOutput
               4. UnsupportedRedirect
               5. Error
               6. ChangeDirectory
         3. If there is more than one special token detected, `Error` is set.
         4. If an unsupported token is detected, `UnsupportedRedirect` is set.
            1. See [Unsupported Tokens](#unsupported-tokens)
         5. If "\<" is detected, `RedirectInput` is set.
         6. If "\>" is detected, `RedirectOutput` is set.
         7. If "cd" is detected, `ChangeDirectory` is set.
         8. If no special token is detected, `NoRedirect` is set.
      2. Return the enum status.
   3. Handle input absed on the status
      1. NoRedirect
         1. Make child process and check for errors
         2. If no errors, execute using `execvp`.
         3. Child process exits when execution completed.
         4. Parent process waits for child to conclude.
      2. RedirectInput
         1. Split input by "\<" into two substrings
            1. Left substring represents the command
            2. Right substring represents the input file
         2. Trim right substring of any leading or trailing whitespace using `trimWhitespace`.
         3. Make child process and check for errors
         4. Open input file in read only using `fopen`.
         5. Handle any errors
            1. Error numbers 2 and 13 are handled specifically.
            2. Any other error is just handled generically.
            3. User is informed of the error and the child process exits.
         6. Redirect STDIN in descriptor table to the descriptor of the opened input file
            1. This redirection only takes place in the child process.
            2. Done using `dup2`.
         7. Execute left side of the command.
         8. Exit child process
         9. Return 0 when child process is finished.
      3. RedirectOutput
         1. Split input by "\>" into two substrings just as with RedirectInput
            1. Left substring represents the command
            2. Right substring represents the output file destination
         2. Trim right substring of any leading or trailing whitespace using `trimWhitespace`
         3. Make child process and check for errors
         4. Open output file in write only using `fopen`.
         5. Handle any errors
            1. Error numbers 2 and 13 are handled specifically.
            2. Any other error is handled generically.
            3. User is informed of the error and the child process exits
         6. Redirect STDOUT in descriptor table to the descriptor of the opened input file
            1. This redirection only takes place in the child process.
            2. Done using `dup2`.
         7. Execute left side of the command
         8. Exit child process.
         9. Return 0 when child process is finished.
      4. ChangeDirectory
         1. Split string by the space between "cd" and the destination.
         2. Use `chdir` to change directory to the destination specified in the input.
         3. Return chdir's return value and handle accordingly.
            1. Errors 2 and 13 are handled specifically.
            2. Others are handled generically.
      5. UnsupportedRedirect
         1. Inform the user that an unsupported redirect was detected.
      6. Error
         1. Inform the user that too many special tokens were detected.
   4. Return 1
4. Repeat.


### Unsupported Tokens

The following tokens are not supported by the shell:

- \>\>
- \<\<
- \&\>
- \&\>\>
- 2\>
- 2\>\>
- \|