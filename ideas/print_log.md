What is the diff b/t printing and logging?
* Printing: 
    * for output essential to your program's operation
    * the output is meant for end-user eyes
    * Default: stdout
* Logging:
    * for output that may help to determine how your program is working
    * to display internal errors/progress (errors, progress, etc)
    * can omit w/o affecting program usability
    * Default: stderr (for debugging after all)

There are several questions to solve here:
1. Naming convention
2. Submodule in std location
3. Variadic arguments (var args) vs macros for implementation

I could do new line and format variants:
    print
    println
    printf
    printlnf
    fprintf
    fprintln
    fprintf
    fprintlnf

Optional arguments: 
* sep (default: ' ')
* end (default: '\n')
    * Non-ln versions would have '\0'
* file (default: stdout)
* flush (default: false)
* ? fmt(default: None)
    * Could become ambiguous if first param is a string literal

Logging features:
    * Optional: formatting
        * Colors
            * Should have defaults and be customizable
            * Should also have the option to print the log level as text
                * For colorblind accessibility
            * Should be able to disable (e.g. what if logging into a file?)
        * Date & time logging @ event
            * Helpful but may slow code
            * Should definitely be optional
            * Custom format strings should be possible
    * Concept of levels:
        * Classically: trace, debug, info, warn, error, and fatal
        * Should be able to set the log level
            * At compile time I guess (not runtime)
            * Can code within a log statement be safely optimized out?
                * What if it has side effects?
        * Should be convenience methods, not just an enum