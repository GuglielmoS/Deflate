#ifndef __BIT_OP__
#define __BIT_OP__

/**
 * Useful MACRO bit operations.
 */

// sets the bit at the position 'p' of the byte 'b'
#define BYTE_BIT_SET(b,p)   ((b) |  (0x01 << (p)))

// clears the bit at the position 'p' of the byte 'b'
#define BYTE_BIT_CLEAR(b,p) ((b) & ~(0x01 << (p)))

// gets the bit at the position 'p' of the byte 'b'
#define BYTE_BIT_GET(b,p)   (((b) >> (p)) & 0x01)

// gets the bit at the position 'p' of the word 'w'
#define WORD_BIT_GET(w,p)   (((w) >> (p)) & 0x0001)

// sets the bit at the position 'p' of the word 'w'
#define WORD_BIT_SET(w,p)   ((w) | (0x0001 << (p)))

#endif /* __BIT_OP__ */
