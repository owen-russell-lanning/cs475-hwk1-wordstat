/*
 * main.c
 *
 *  Created on: Jun 15, 2015
 *      Author: dchiu
 */
#include <stdio.h>
#include <string.h>
#include "defs.h"
#include "stats.h"
#include "menu.h"

/**
 * @brief sets all the values in the array to null. array must be max length
 **/
int emptyString(char *s){
	for(int i = 0; i<MAX_INPUT_LEN; i++){
		s[i] = '\0';
	}
	return 0;
}

/**
 * @brief calculates the index in the histogram for a specific character.
 * @returns the index in the histogram for the character
 **/
int getHistogramIndex(char c)
{
	// get histogram pos from ascii value
	int ascii = (int)(c);
	// shift by a value

	ascii = ascii - 65;

	// account for lower case characters
	if (ascii > ALPHABET_SIZE)
	{
		ascii = ascii - 32;
	}

	return ascii;
}

/**
 *@brief counts the vowels in a string
 *@param s string to count vowels in
 *@return count of vowels in the string
 */
int countVowels(char s[])
{

	int vowels = 0;
	for (int i = 0; i < MAX_INPUT_LEN; i++)
	{

		if (s[i] == 'e' || s[i] == 'i' || s[i] == 'o' || s[i] == 'u' || s[i] == 'y' || s[i] == 'a' || s[i] == 'E' || s[i] == 'I' || s[i] == 'O' || s[i] == 'U' || s[i] == 'Y' || s[i] == 'A')
		{
			vowels++;
		}
	}
	return vowels;
}

/**
 *@brief counts the consonants in a string
 *@param s string to count vowels in
 *@return count of consonants in the string
 */
int countConsonants(char s[])
{

	//int totalVowels = countVowels(s);
	//int totalNonVals = 0;
	//calculate vowels and non vals and then minus from the length
	

	int potentialValues = 0;
	for (int i = 0; i < MAX_INPUT_LEN; i++)
	{
		int ind = getHistogramIndex(s[i]);
		if (ind >= 0 && ind < ALPHABET_SIZE)
		{
			potentialValues++;
		}
	}

	int vowels = countVowels(s);
	return potentialValues - vowels;
}

/**
 * @brief prints the menu stats for vowels and consonants
 * @param vowels total number of vowels
 * @param consonants total number of consonants
 * */
int printMenuStats(int vowels, int consonants)
{
	int letterCount = vowels + consonants;
	printf("Vowels = %d ", vowels);

	// calculate percentage of vowels
	double onePer = letterCount / 100.0;
	double vPer = vowels / onePer;
	if (letterCount == 0)
	{
		vPer = 0;
	}
	printf("(%f%%), ", vPer);

	printf("Consonants = %d ", consonants);

	// calculate percentage of consonants
	double cPer = consonants / onePer;
	if (letterCount == 0)
	{
		cPer = 0;
	}
	printf("(%f%%), ", cPer);

	// print final count
	printf("Total = %d\n", letterCount);

	return 0;
}

/**
 * @brief prints the histogram with use frequency
 * @param histogram the programs histogram. an array of integer representing each letter
 * */
int printHistogram(int histogram[])
{

	int maxValue = 0; // find the max value in the histogram

	// find the max value in the histogram
	for (int i = 0; i < ALPHABET_SIZE; i++)
	{
		if (histogram[i] > maxValue)
		{
			maxValue = histogram[i];
		}
	}

	while (maxValue > 0)
	{
		// iterate through the histogram and print a star for those which are greater than or equal to the max value. if not, print space
		for (int i = 0; i < ALPHABET_SIZE; i++)
		{
			if (histogram[i] >= maxValue)
			{
				printf("* ");
			}
			else
			{
				printf("  ");
			}
		}

		printf("\n");
		// reduce max value
		maxValue--;
	}

	// print all letters
	printf("a b c d e f g h i j k l m n o p q r s t u v w x y z\n");

	// print letter frequencies
	for (int i = 0; i < ALPHABET_SIZE; i++)
	{
		printf("%d ", histogram[i]);
	}

	printf("\n");

	return 0;
}

void getUserInput(int *vowels, int *consonants, int *histogram)
{
	// get strings fron user
	printf("Enter strings (# to stop):\n");
	int end = 0;
	while (!end)
	{ // while not at end keep going

		// get words from user
		char rawInput[MAX_INPUT_LEN];
		scanf("%s", rawInput);

		// tokensize and iterate through tokens
		char *tok;
		tok = strtok(rawInput, " ");

		while (tok != NULL && !end)
		{

			if (tok != NULL && strcmp(tok, "") != 0)
			{
				if (strcmp(tok, "#") == 0)
				{ // look for end value
					end = 1;
				}
				else
				{
					// update word values
					// count vowels
					*vowels += countVowels(tok);
					// count consonants
					*consonants += countConsonants(tok);
					// update histogram
					for (int i = 0; i < MAX_INPUT_LEN; i++)
					{
						if (tok[i])
						{

							int histoPos = getHistogramIndex(tok[i]);
							// make sure index is still withing range
							if (histoPos >= 0 && histoPos < ALPHABET_SIZE)
							{
								// update histogram
								histogram[histoPos]++;
							}
						}
					}
				}
			}

			emptyString(tok);
			tok = strtok(NULL, " ");
		}
	}
}

/**
 * @brief Main function
 *
 * @param argc Ignore
 * @param argv Ignore
 * @return int
 */
int main(int argc, char *argv[])
{
	// declare and initialize the histogram
	int histogram[ALPHABET_SIZE] = {0};

	// declare stats
	int vowels = 0;
	int consonants = 0;
	// get input
	getUserInput(&vowels, &consonants, histogram);

	// print options
	int option = getMenuOption();
	while (option != MENU_EXIT)
	{
		if (option == MENU_STATS)
		{
			printMenuStats(vowels, consonants);
		}
		else if (option == MENU_HISTO)
		{
			printHistogram(histogram);
		}
		else if (option == MENU_INPUT)
		{
			getUserInput(&vowels, &consonants, histogram);
		}
		else
		{
			printf("Error: Unknown option %d\n", option);
		}
		option = getMenuOption();
	}

	printf("Exiting...\n");
	return 0;
}
