
/*
 PICTURES
 To load an image from flash and display it:
 MicroBitImage i((ImageData*)heart);
 uBit.display.print(MicroBitImage, int x, int y, int alpha , int delay);
 Images are stored to flash instead of sram (Microbit images) as follows
 */

const uint8_t smallHeart[] __attribute__ ((aligned (4))) = { 0xff, 0xff, 5, 0, 5, 0,
    0,0,0,0,0,
    0,1,0,1,0,
    0,1,1,1,0,
    0,0,1,0,0,
    0,0,0,0,0 };

const uint8_t heart[] __attribute__ ((aligned (4))) = { 0xff, 0xff, 5, 0, 5, 0,
    0,1,0,1,0,
    1,1,1,1,1,
    1,1,1,1,1,
    0,1,1,1,0,
    0,0,1,0,0 };

const uint8_t happy[] __attribute__((aligned (4))) = { 0xff, 0xff, 5, 0, 5, 0,
    0,1,0,1,0,
    0,1,0,1,0,
    0,0,0,0,0,
    1,0,0,0,1,
    0,1,1,1,0 };

const uint8_t sad[] __attribute__((aligned (4))) = { 0xff, 0xff, 5, 0, 5, 0,
    0,1,0,1,0,
    0,1,0,1,0,
    0,0,0,0,0,
    0,1,1,1,0,
    1,0,0,0,1 };

const uint8_t angry[] __attribute__((aligned (4))) = { 0xff, 0xff, 5, 0, 5, 0,
    1,0,0,0,1,
    0,1,0,1,0,
    0,0,0,0,0,
    1,1,1,1,1,
    1,0,0,0,1 };

const uint8_t asleep[] __attribute__((aligned(4))) = { 0xff, 0xff, 5, 0, 5, 0,
    0,0,0,0,0,
    1,1,0,1,1,
    0,0,0,0,0,
    0,1,1,1,0,
    0,0,0,0,0 };

const uint8_t surprised[] __attribute__((aligned(4))) = { 0xff, 0xff, 5, 0, 5, 0,
    0,1,0,1,0,
    0,0,0,0,0,
    0,0,1,0,0,
    0,1,0,1,0,
    0,0,1,0,0 };

const uint8_t yes[] __attribute__((aligned(4))) = { 0xff, 0xff, 5, 0, 5, 0,
    0,0,0,0,0,
    0,0,0,0,1,
    0,0,0,1,0,
    1,0,1,0,0,
    0,1,0,0,0 };

const uint8_t no[] __attribute__((aligned(4))) = { 0xff, 0xff, 5, 0, 5, 0,
    1,0,0,0,1,
    0,1,0,1,0,
    0,0,1,0,0,
    0,1,0,1,0,
    1,0,0,0,1 };

const uint8_t arrow_left[] __attribute__((aligned(4))) = { 0xff, 0xff, 5, 0, 5, 0,
    0,0,1,0,0,
    0,1,0,0,0,
    1,1,1,1,1,
    0,1,0,0,0,
    0,0,1,0,0 };

const uint8_t arrow_right[] __attribute__((aligned(4))) = { 0xff, 0xff, 5, 0, 5, 0,
    0,0,1,0,0,
    0,0,0,1,0,
    1,1,1,1,1,
    0,0,0,1,0,
    0,0,1,0,0 };

const uint8_t *PICTURES[] = {happy, heart, smallHeart, sad, angry, asleep, surprised, yes, no};
const int storedPictures = 9;
