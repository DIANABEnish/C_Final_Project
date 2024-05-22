#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H


/**
 * @brief Checks and preprocesses the input assembly file.
 * 
 * This function performs pre-processing on the input assembly file, including
 * checking for macros and processing them accordingly.
 * 
 * @param inputFile The name of the input assembly file.
 * @return Returns true if pre-processing is successful, otherwise false.
 */
boolean preprocessorFileCheck(char *inputFile);

#endif /* PREPROCESSOR_H */
