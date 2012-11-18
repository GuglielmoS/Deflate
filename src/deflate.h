#ifndef __DEFLATE__
#define __DEFLATE__

/*** USEFUL MACROS ***/

// shift left the first two element of a vector v
#define SHIFT2B(vec) vec[0]=vec[1];vec[1]=vec[2];

void Deflate_encode(const char *in_file_name, const char *out_file_name);
void Deflate_encode(const char *in_file_name, const char *out_file_name);

#endif /* __DEFLATE__ */
