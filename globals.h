/* Maximum number of integer values allowed  */
#define MAX_INTEGER_NUMBER 40

/* Maximum length of a string allowed  */
#define MAX_STRING_LENGTH 80

/* Maximum length of a label  */
#define MAX_LABEL_LENGTH 31

/* Maximum length of an error */
#define MAX_ERROR_MESSAGE_LENGTH 150

/* Maximum length of an opcode */
#define MAX_OPCODE_LENGTH 3

/* Maximum number of digits allowed in an integer value */
#define MAX_INTEGER_SIZE 5

/* Maximum value for a 14-bit integer */
#define MAX_14BIT_NUMBER 8191

/* Minimum value for a 14-bit  integer */
#define MIN_14BIT_NUMBER -8192

/* Maximum value for a 12-bit integer */
#define MAX_12BIT_NUMBER 2047

/* Minimum value for a 12-bit integer */
#define MIN_12BIT_NUMBER -2048

/* Number of registers available in the program */
#define COUNT_REGISTER 8

/* Number of different instructions available */
#define COUNT_COMMAND 16

/* Number of reserved  keywords in the program */
#define COUNT_RESERVED_WORD 7

/* Maximum length of a macro name  */
#define MAX_MACRO_NAME_LENGTH 31

/* Maximum length of a single line in the source code */
#define MAX_LINE_LENGTH 80

/* Total capacity of the memory available */
#define MEMORY_CAPACITY 3996

/* Starting address of the memory */
#define BASE_MEMORY_ADRESS 100

/* Size of a word in bits (assuming a base-4 system) */
#define BASE_4_WORD_SIZE 7

/*represent a constant*/
#define CONSTANT 0

/*represent a label in the code section */
#define CODE_LABEL 1

/* represent a label in the data section */
#define DATA_LABEL 2

/*represent an external label  */
#define EXTERN_LABEL 3

/*represent the target operand of an instruction */
#define TARGET_OPERAND 0

/*represent the source operand of an instruction */
#define SOURCE_OPERAND 1

/* represent an externalsymbol */
#define EXTERNAL 1

/*represent a relocatable symbol or address */
#define RELOCATABLE 2