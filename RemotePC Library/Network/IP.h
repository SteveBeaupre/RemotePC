#pragma once

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

bool IsIPValid(char *ip)
{
	int i = 0; // char. index
	int n = 0; // number of consequtive num.
	int z = 0; // number of valid sequence done

	while(i < 16){
		
		// Store current char in variable
		char c = ip[i];

		if(isdigit(c)){
			// Inc. n if the char is a number
			n++;
		} else if(c == '.'){
			// Inc. z when we reach a point
			z++;
			// If n or z are invalid, return false
			if(n == 0 || n > 3 || z > 3)
				return false;
			// reset n
			n = 0;
		} else if(c == NULL){
			// If n or z are invalid, return false
			if(n == 0 || n > 3 || z != 3)
				return false;
		
			// return success
			return true;
		} else {
			// any other chars return false
			return false;
		}


		// Next char.
		++i;
	}
	
	return false;
}

