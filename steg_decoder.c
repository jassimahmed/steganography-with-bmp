/* The file steg_decoder.c
   is a larger systems example to demonstrate
   input, output and memory manipulation in C.
   
   Build this file with: 
   gcc -o decoder steg_decoder.c
   
   When both this, and the paired encoder are compiled, 
   then you can pass a message using a bmp file with these commands:
   
   $ ./encoder mario_16_bit.bmp mario_message.bmp Your Message Here.
   $ ./decoder mario_message.bmp
   
  
*/

#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[] )
{
  const int bmp_header_size = 54;
  const int max_msg_size = 1000;

  int i;
  int c;
  int img_idx = 0;
  int msg_idx = 0;
  int bit_idx = 0;

  if( argc < 2 ){
    printf( "Usage: %s message_bmp.\n", argv[0] );
    return 1;
  }

  char msg[max_msg_size];
  FILE *img_in = NULL;
  
  img_in = fopen( argv[1], "rb" );

  if( img_in == NULL ){
    printf( "Could not open the input image file.\n" );
    return 1;
  }

	// Set msg to 0. 
  for( i=0; i < max_msg_size; i++ )
    msg[i] = 0;

  // This while loop is the main decoding action. We loop through the image
  // and extract the lowest-order bits from each byte into msg
  while( ( c = fgetc( img_in )) != EOF ){
 
		// Skip bmp header files 
   if( img_idx >= bmp_header_size ){
    
     // Check the lowest-order bit     
		 if( ( c & 1 )  > 0 )
        // If it was 1, use OR to set msg where we next need to write 
        // into msg to be 1 (OR does not change any of its other bits)
        msg[msg_idx] |= 1 << bit_idx;

      // Move on to the next bit
      bit_idx++;

			// Move to the next byte
      if( bit_idx >= 8 ){
       if( msg[msg_idx] == '\0' )
          break;
        bit_idx = 0;
        msg_idx++;
      }      
    }
    
    img_idx++;
  }

  printf( "Processed %d bytes.\n", img_idx );
  printf( "The secret message is: %s\n", msg );

  fclose(img_in);

  return 0;
}

