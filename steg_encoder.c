/* The program is a larger systems example to demonstrate
   input, output and memory manipulation in C.
   
   Build this file with: 
   gcc -o encoder steg_encoder.c
   
   When both this, and the paired decoder are compiled, 
   then you can pass a message using a bmp file with these commands:
   
   $ ./encoder mario_16_bit.bmp mario_message.bmp Your Message Here.
   $ ./decoder mario_message.bmp

	Check mario_message.bmp to be exactly same as the original file,
	since only the least-significant bit was changed to contain the message. 
     
*/

#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[] )
{
  const int bmp_header_size = 54;
  const int max_msg_size = 1000;
  int i;
  int curr_message_position=0;
  int read_code;
  int msg_idx = 0;
  int img_idx = 0;
  int bit_idx = 0;
  char c;
  char msg[max_msg_size];
  FILE *img_in = NULL;
  FILE *img_out = NULL;
  
  if( argc < 4 ){
    printf( "Usage: %s source_bmp output_bmp message.\n", argv[0] );
    return 1;
  }
  
  img_in = fopen( argv[1], "rb" );
  if( img_in == NULL ){
    printf( "Could not open the input image file.\n" );
    return 1;
  }

  img_out = fopen( argv[2], "wb" );
  if( img_out == NULL ){
    printf( "Could not open the output file.\n" );
    return 1;
  }  

	// Loops through the command line message arguments and store them
	// in msg as a string. 
  for( i=3; i<argc; i++ ){
    strcpy( msg+curr_message_position, argv[i] );
    curr_message_position += strlen(argv[i]);

    if( i<argc-1 )
      msg[curr_message_position] = ' ';
      curr_message_position++;
  }
  msg[curr_message_position] = '\0';
  
  // This while loop is the main place where steganography encoding happens.
  // The bits of msg are woven into the lowest-order bit in each byte of the image
  while( ( read_code = fgetc( img_in )) != EOF ){
    c = (char)read_code; // c holds the original pixel value


		// First 54 bits are skipped, they're header files. 
   if( img_idx >= bmp_header_size && msg_idx <= strlen( msg ) ){ 
    
      // bit_mask will have exactly one "1" bit and seven "0" bits
      char bit_mask = 1 << bit_idx;
      
      // Use bit_mask to probe the appropriate bit in the message.
      // If it is 1, then use the OR operation to set the lowest-order
      // bit of c to 1. If it is 0, then use the AND operation to set the
      // lowest-order bit of c to 0.     
		  if( ( msg[msg_idx] & bit_mask) > 0 )
        c |= 1;
      else
        c &= 254;

      // Move the bit index forward 
      bit_idx++;

      // Move to the next byte
      if( bit_idx >= 8 ){
        bit_idx = 0;
        msg_idx++;
      }      
    }

		// Copy each byte into a new file 
    fputc( c, img_out );
    img_idx++;
  }

  printf( "I have hidden the secret within an image of %d bytes.\n", img_idx );

  fclose(img_in);
  fclose(img_out);

  return 0;
}

