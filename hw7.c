/*
 * File: hw7.c
 * Course: TCSS 333 - Winter 2016
 * Assignment 7 - Floating Point to Binary
 * Copyright 2016 Benjamin Abdipour
 */

#include <stdio.h>
#include <stdlib.h>
//#include <string.h>
#define MAX_BITS 32
#define MAX_BITS_PLUS_BULL MAX_BITS + 1
#define BIAS 127
#define EXPONENT_BIT_SIZE 8
#define FRACTION_BIT_SIZE 23
#define SIGN_BIT_SIZE 1

struct myFloat {
    unsigned int exponent : EXPONENT_BIT_SIZE;
    unsigned int fraction : FRACTION_BIT_SIZE;
    unsigned int sign : SIGN_BIT_SIZE;
};

char * binaryToChar(int * theInt) {
    int i = 0;
    char * theChar = NULL;

    for (i = 0; i < (sizeof(theChar) / sizeof(theChar[0])); i++) {
        if (theInt[i] == 0) {
            char tempChar = '0';
            theChar = &tempChar;
        } else {
            char tempChar = '1';
            theChar = &tempChar;
        }
        theChar++;
    }
    theChar = NULL;

    return theChar;
}

char * intToBinary(int theInt) {
    char * theBinaryReverse = calloc(MAX_BITS_PLUS_BULL, sizeof(char));
    char * theBinaryReverseHeader = theBinaryReverse;
    int indexReverse = 0;

    while (theInt != 0) {
        theBinaryReverse[indexReverse] = ((theInt % 2) == 0 ? '0' : '1');
        indexReverse++;
        theInt /= 2;
    }
    theBinaryReverse[indexReverse] = '\0';

    char * theBinary = calloc(indexReverse, sizeof(char));
    char * theBinaryHeader = theBinary;
    int index = 0;
    indexReverse--;

    for (; indexReverse >= 0; indexReverse--) {
        theBinary[index] = theBinaryReverseHeader[indexReverse];
        index++;
    }
    theBinary[index] = '\0';

    return theBinaryHeader;
}

char * intToBinaryLeadingZero(int theInt, int length) {
    char * theBinaryReverse = calloc(length + 1, sizeof(char));
    char * theBinaryReverseHeader = theBinaryReverse;
    int indexReverse = 0;

    while (theInt != 0) {
        theBinaryReverse[indexReverse] = ((theInt % 2) == 0 ? '0' : '1');
        indexReverse++;
        theInt /= 2;
    }

    while (indexReverse < length) {
        theBinaryReverse[indexReverse] = '0';
        indexReverse++;
    }

    theBinaryReverse[indexReverse] = '\0';

    char * theBinary = calloc(length + 1, sizeof(char));
    char * theBinaryHeader = theBinary;
    int index = 0;
    indexReverse--;

    for (; indexReverse >= 0; indexReverse--) {
        theBinary[index] = theBinaryReverseHeader[indexReverse];
        index++;
    }
    theBinary[index] = '\0';

    return theBinaryHeader;
}

char * floatToBinary(float theFloat) {
    char * theBinary = calloc(MAX_BITS_PLUS_BULL, sizeof(char));
    char * theBinaryHeader = theBinary;
    int index = 0;
    float multiplication = theFloat;

    while (multiplication != 0) {
        multiplication -= (int) multiplication;
        multiplication = multiplication * 2;
        theBinary[index] = ((int) multiplication == 0 ? '0' : '1');
        index++;
    }
    theBinary[index] = '\0';

    return theBinaryHeader;
}

///////// useful if you want to use with strings
//char * getExponent(int theInt) {
//    char * theBinary = intToBinary(theInt);
//    int len = strlen(theBinary);
//    theBinary = intToBinary(len - 1 + BIAS);
//
//    return theBinary;
//}
//////////////////

///////// useful if you want to use with strings
//char * getBits(char theSign, char * theExponent, char * intBinary,
//        char * fltBinary) {
//    char * theBinary = calloc(MAX_BITS_PLUS_BULL, sizeof(char));
//    theBinary[0] = theSign;
//    strcat(theBinary, theExponent);
//    intBinary++;
//    strcat(theBinary, intBinary);
//    strcat(theBinary, fltBinary);
//    int count = strlen(theBinary);
//
//    for (; count < MAX_BITS; count++) {
//        theBinary[count] = '0';
//    }
//    theBinary[strlen(theBinary)] = '\0';
//
//    return theBinary;
//}
//////////////////

///////// useful if you want to use with strings
//char * getFraction(char * theExponent, char * theBinary) {
//    int jumpIndex = 1 + strlen(theExponent);
//    int index = 0;
//
//    for (; index < jumpIndex; index++) {
//        theBinary++;
//    }
//
//    return theBinary;
//}
//////////////////

int main(int argc, char *argv[]) {
    setvbuf(stdout, NULL, _IONBF, 0);

    printf("Enter a float: ");
    float myFloat = 0.0f;
    scanf("%f", &myFloat);
    int intPart = (int) myFloat;
    float fltPart = myFloat - intPart;
    struct myFloat myfloat;
    if (myFloat < 0) {
        myfloat.sign = 1;
        intPart *= (-1);
    } else {
        myfloat.sign = 0;
    }

    struct myFloat myfloatTemp1;
    struct myFloat myfloatTemp2;
    myfloatTemp1.fraction = intPart;

    myfloatTemp2.fraction = myfloatTemp1.fraction;

    int leadingZero = 0;
    while (myfloatTemp2.fraction >= myfloatTemp1.fraction
            && myfloatTemp1.fraction != 0) {
        myfloatTemp2.fraction <<= 1;
        if (myfloatTemp2.fraction > myfloatTemp1.fraction) {
            leadingZero++;
            myfloatTemp1.fraction <<= 1;
        } else {
            break;
        }
    }

    int length = 0;
    int offset = 0;
    if (intPart != 0) {
        length = FRACTION_BIT_SIZE - leadingZero;
        offset = length - 1;
    } else {
        offset = -3;  //offset of a 0.xxxx format number
    }

    myfloat.exponent = offset + BIAS;

    myfloatTemp1.fraction = fltPart;
    myfloatTemp2.fraction = fltPart;
    leadingZero = 0;
    while (myfloatTemp2.fraction >= myfloatTemp1.fraction
            && myfloatTemp1.fraction != 0) {
        myfloatTemp2.fraction <<= 1;
        if (myfloatTemp2.fraction > myfloatTemp1.fraction) {
            leadingZero++;
            myfloatTemp1.fraction <<= 1;
        } else {
            break;
        }
    }

    char * floatPtr = floatToBinary(fltPart);
    myfloatTemp1.fraction = 0;
    int index = 0;
    while (floatPtr[index] != '\0') {
        if (floatPtr[index] == '1') {
            (myfloatTemp1.fraction |= 0x01);
        } else {
            (myfloatTemp1.fraction |= 0x00);
        }

        myfloatTemp1.fraction <<= 1;
        index++;
    }

    char * exponentTemp = intToBinary(intPart);
    myfloatTemp2.exponent = 0;

    int k;
    for (k = 1; k < sizeof(exponentTemp); k++) {
        myfloatTemp2.exponent <<= 1;
        if (exponentTemp[k] == '1') {
            myfloatTemp2.exponent |= 0x01;
        }
    }

    myfloat.fraction = 0;
    int lengthTemp = 0;
    char * intTemp = intToBinary(intPart);
    for (k = 0; intTemp[k] != '\0'; k++) {
        lengthTemp++;
        if (intTemp[k] == '0') {
            myfloat.fraction <<= 1;
            myfloat.fraction |= 0x00;
        } else if (intTemp[k] == '1') {
            myfloat.fraction <<= 1;
            myfloat.fraction |= 0x01;
        }
    }

    char * fltTemp = floatToBinary(fltPart);
    for (k = 0; fltTemp[k] != '\0'; k++) {
        lengthTemp++;
        if (fltTemp[k] == '0') {
            myfloat.fraction <<= 1;
            myfloat.fraction |= 0x00;
        } else if (fltTemp[k] == '1') {
            myfloat.fraction <<= 1;
            myfloat.fraction |= 0x01;
        }
    }

    if (intPart != 0) {
        for (k = 0; k < (FRACTION_BIT_SIZE - lengthTemp + 1); k++) {
            myfloat.fraction <<= 1;
        }
    } else {
        myfloat.fraction >>= 1;
    }

    char * fraction = intToBinaryLeadingZero(myfloat.fraction,
    FRACTION_BIT_SIZE);
    printf("Your float was read as: %f\n", myFloat);
    printf("Your float in %d bits: %d%s%s\n", MAX_BITS, myfloat.sign,
            intToBinaryLeadingZero(myfloat.exponent, EXPONENT_BIT_SIZE),
            intToBinaryLeadingZero(myfloat.fraction, FRACTION_BIT_SIZE));
    printf("Sign: %d\n", myfloat.sign);
    printf("Exponent: %s\n",
            intToBinaryLeadingZero(myfloat.exponent, EXPONENT_BIT_SIZE));
    printf("Fraction: %s\n\n",
            intToBinaryLeadingZero(myfloat.fraction, FRACTION_BIT_SIZE));
    printf("Creating the fraction:\n");
    float newFloat;
    if (myfloat.sign == 0) {
        newFloat = 1.0f;
    } else {
        newFloat = -1.0f;
    }
    printf("fraction = %f (the implicit 1)\n", newFloat);
    float myNewFraction = newFloat;

    while (fraction[0] != '\0') {
        myNewFraction /= 2;
        if (fraction[0] == '0') {
            printf("fraction = %f, after skipping %f\n", newFloat,
                    myNewFraction);
        } else {
            newFloat += myNewFraction;
            printf("fraction = %f, after adding %f\n", newFloat, myNewFraction);
        }
        fraction++;
    }
    printf("\nApplying the exponent:\n");
    printf("unbiased exponent = %d\n", offset);
    int count = 0;
    if (offset < 0) {
        offset *= -1;
        for (; count < offset; count++) {
            printf("times 2 = %f\n", newFloat /= 2);
        }
    } else {
        for (; count < offset; count++) {
            printf("times 2 = %f\n", newFloat *= 2);
        }
    }

    printf("\nFinal Answer:  %f\n", newFloat);

    return 0;
}
