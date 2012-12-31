#ifndef __BIT_OP__
#define __BIT_OP__

/**
 * Useful MACRO bit operations.
 */

// set the bit at the position 'p' of the byte 'b'
#define BYTE_BIT_SET(b,p)   ((b) |  (0x01 << (p)))

// clear the bit at the position 'p' of the byte 'b'
#define BYTE_BIT_CLEAR(b,p) ((b) & ~(0x01 << (p)))

// get the bit at the position 'p' of the byte 'b'
#define BYTE_BIT_GET(b,p)   (((b) >> (p)) & 0x01)

// get the bit at the position 'p' of the word 'w'
#define WORD_BIT_GET(w,p)   (((w) >> (p)) & 0x0001)

#endif /* __BIT_OP__ */
