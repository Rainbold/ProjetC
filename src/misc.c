#include <SDL/SDL_image.h> // IMG_Load
#include <misc.h>
#include <time.h>
#include <assert.h>

void error(const char *s, ...) {
	va_list ap; 
	va_start(ap, s);

	vfprintf(stderr, s, ap);
	va_end(ap);
	exit(EXIT_FAILURE);
}

SDL_Surface* load_image(const char* filename) {
	SDL_Surface* img = IMG_Load(filename);
	if (!img)
		error("IMG_Load: %s\n", IMG_GetError());

	return img;
}

int rand_ab(int a, int b)
{
	assert(a<b);
	
	int divisor = RAND_MAX/(b-a+1);
	int res;

	do {
		res = rand() / divisor;
	} while(res > b);

	return res+a;
}
/* if the file is in hexa */
int map_is_valid_format1(const char* mapFile)
{
   FILE* f = NULL;
   unsigned char byte = 0;

   f = fopen(mapFile, "rb");

   if(!f) 
      return 0;

   if( fread(&byte, 1, sizeof(byte), f) == 0 || byte != 0x6c )
      return 0;
   if( fread(&byte, 1, sizeof(byte), f) == 0 || byte != 0x76 )
      return 0;
   if( fread(&byte, 1, sizeof(byte), f) == 0 || byte != 0x6c )
      return 0;

   fclose(f);

   return 1;
}
/* if the file finish with .lvl */
int map_is_valid_format2(const char* mapFile)
{
	int len = strlen(mapFile);

   	if(len >= 5 && mapFile[len-1] == 'l' &&  mapFile[len-2] == 'v' && mapFile[len-3] == 'l' && mapFile[len-4] == '.')
   		return 1;
   	else
   		return 0;
}

int max(int a, int b) {
	if(b <= a)
		return a;
	else
		return b;
}
