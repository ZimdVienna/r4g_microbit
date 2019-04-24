
// Define Frequencies of musical notes

#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976

/*  SONGS   */

const int tusch[]       =   {   NOTE_C4,0,NOTE_G3,NOTE_G3,NOTE_A3,
                                NOTE_G3,0,NOTE_B3,NOTE_C4,-1  };

const int song2[]       =   {   NOTE_C5,NOTE_B4,NOTE_G4,NOTE_C5,NOTE_B4,
                                NOTE_E4,NOTE_C5,NOTE_C4,NOTE_G4,NOTE_A4,
                                NOTE_C5,-1  };

const int starWars[]    =   {   NOTE_G3,NOTE_G3,NOTE_G3,NOTE_C3,NOTE_G3,
                                NOTE_F3,NOTE_E3,NOTE_D3,NOTE_C4,NOTE_G3,
                                NOTE_F3,NOTE_E3,NOTE_D3,NOTE_C4,NOTE_G3,
                                NOTE_F3,NOTE_E3,NOTE_F3,NOTE_D3,-1  };

const int superMario[]  =   {   NOTE_E4,NOTE_E4,0,NOTE_E4,0,
                                NOTE_C4,NOTE_E4,0,NOTE_G4,0,
                                NOTE_G3,-1 };

const int waltz[]       =   {   NOTE_A3,NOTE_A3,NOTE_CS4,NOTE_E4,NOTE_E4,
                                0,NOTE_E4,NOTE_E4,0,NOTE_CS4,
                                NOTE_CS4,0,NOTE_A3,NOTE_A3,NOTE_CS4,
                                NOTE_E4,NOTE_E4,0,NOTE_E4,NOTE_E4,
                                0,NOTE_D4,NOTE_D4,-1 };

const int tango[]       =   {   NOTE_G3,0,NOTE_A3,NOTE_G3,NOTE_F3,
                                NOTE_E3,NOTE_F3,NOTE_G3,0,NOTE_E3,
                                0,NOTE_G3,0,NOTE_A3,NOTE_G3,
                                NOTE_F3,NOTE_E3,NOTE_G3,NOTE_F3,0,
                                NOTE_D3,-1    };

const int beHappy[]     =   {   NOTE_B4,0,NOTE_GS4,NOTE_FS4,
                                NOTE_GS4,NOTE_DS4,NOTE_DS4,NOTE_FS4,
                                NOTE_DS4,NOTE_CS4,NOTE_B3,NOTE_CS4,NOTE_DS4,
                                NOTE_CS4,0,NOTE_FS4,NOTE_DS4,NOTE_CS4,
                                NOTE_B3,NOTE_CS4,NOTE_DS4,NOTE_CS4,0,NOTE_DS4,
                                NOTE_CS4,NOTE_B3,NOTE_GS3,NOTE_B3,-1};

const int rainbow[]     =   {   NOTE_C4,NOTE_C5,NOTE_B4,NOTE_G4,NOTE_A4,
                                NOTE_B4,NOTE_C5,NOTE_C4,NOTE_A4,NOTE_G4,
                                0,NOTE_A3,NOTE_F4,NOTE_E4,NOTE_C4,
                                NOTE_D4,NOTE_E4,NOTE_F4,NOTE_D4,NOTE_B3,
                                NOTE_C4,NOTE_D4,NOTE_E4,NOTE_C4,-1  };

// Delays to hold the notes (eg. 1/8, 1/4, 1/16 notes)
const int tusch_b[]     =   {160,67,160,160,250,400,67,250,250,0};
const int beat2[]       =   {160,160,160,800,800,160,160,160,160,800,800,0};
const int starWars_b[]  =   {160,160,160,800,800,160,160,160,800,400,160,160,160,800,400,160,160,160,800,0};
const int superMario_b[]=   {167,167,76,250,76,167,167,76,250,250,250,0};
const int waltz_b[]     =   {250,250,250,250,400,80,320,400,80,320,400,80,250,250,250,250,320,80,320,400,80,320,400,0};
const int tango_b[]     =   {250,76,250,176,176,180,176,250,76,400,1050,250,76,250,176,176,176,180,250,76,400,0};
const int beHappy_b[]   =   {500,125,375,375,250,125,375,250,125,250,125,250,125,750,1000,250,125,250,125,250,125,750,1000,250,125,250,125,250,125,750};
const int rainbow_b[]   =   {500,500,250,125,125,250,250,500,500,750,250,500,500,250,125,125,250,250,250,125,125,250,250,750};

// Songbook
const int *SONGS[] = {tusch,song2,starWars,superMario,waltz,tango,beHappy,rainbow};
const int *BEATS[] = {tusch_b,beat2,starWars_b,superMario_b,waltz_b,tango_b,beHappy_b,rainbow_b};
const int storedSongs = 8;
