#pragma once

//These helper macro will release or delete safely the passed object
#define SAFE_RELEASE(x)	       if((x) != NULL){(x)->Release();(x) = NULL;}
#define SAFE_DELETE_ARRAY(x)   if((x) != NULL){delete[] (x);(x) = NULL;}
#define SAFE_DELETE_OBJECT(x)  if((x) != NULL){delete (x);(x) = NULL;}
