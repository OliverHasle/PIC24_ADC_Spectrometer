#define START_PULSE_LEN_CYC     2500    // Configurable - must be at least 6! 
#define INTEGRATION_AFTER_START 48    // the integration time ends 48 cycles after START pulse low
#define VIDEO_ENABLE_OFFSET     88    // Output of the video signals starts 88 cycles after START pulse low
#define VIDEO_SIGNAL_LENGTH     288   // number of spectral lines that are read

//Defines for KISS protocol
#define KISS_FEND 0xC0
#define KISS_FESC 0xDB
#define KISS_TFEND 0xDC
#define KISS_TFESC 0xDD

