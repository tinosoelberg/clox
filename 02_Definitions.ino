
// SCREEN STUFF
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// WANNA LOGO?
#define SHOW_LOGO 1

// MIDI CLOCK IS FIXED AT 24 TICKS PER BEAT REGARDLESS OF BPM - WE'RE SETTING THE DEFAULT BPM TO BE 120
int MIDI_TICKS_PER_BEAT = 24;
int bpm = 120;

// WHOAH MATH! TIME/INTERVAL BETWEEN BEATS IS CALCULATED HERE
long intervalPerBeat = (60000000/bpm);

// AAAAND DIVIDED BY 24 FOR THE CORRECT MIDI TICK TIMING
long midiInterval = (intervalPerBeat/MIDI_TICKS_PER_BEAT);    //interval is the number of milliseconds defined by tempo formula.

// THE RUNLOOP VARIABLE DECIDES WHETHER THE BEAT IS STARTED OR STOPPED - INITIALLY IT IS STOPPED, BECAUSE PLUGGING IN COULD BE SCARY OTHERWISE
bool runLoop = false;
 

// JUST SOME INITIALIZATION OF THE ROTARY ENCODER AND THE TWO BUTTONS - BASICALLY TELLING THE ENCODER AND BUTTON LIBRARY WHICH PINS ARE USED.
// THE ENCODER USES PINS 0 AND 1 AS THEY HAVE INTERRUPT CAPABILITIES SO THEY WILL BE READ EVEN IF THE LOOP IS BUSY WITH SAY.. DRAWING STUFF ON THE SCREEN
Encoder myEnc(0, 1);
Button rotaryButton(13);
Button startButton(11);

byte channel1Pin = 9;
byte channel2Pin = 10;
byte channel3Pin = 7;
byte channel4Pin = 5;


// SETTING UP A COUPLE OF ARRAYS OF THE TIME DIVISION OR MULTIPLICATION FACTORS AVAILABLE
float factors[] =       {0.06, 0.12, 0.25, 0.5, 1, 2, 3, 4, 5, 6, 7, 8, 12, 16, 24, 32, 64};
String factorNames[]  = {"/16","/8","/4","/2"," 1","*2","*3","*4","*5","*6","*7","*8","*12","*16", "*24", "*32", "*64"};


// SETTING UP SOME INITIAL OR DEFAULT FACTORS FOR EACH CHANNEL
int Channel_1_Factor = 2;
int Channel_2_Factor = 4;
int Channel_3_Factor = 5;
int Channel_4_Factor = 8;


// THE TICK VARIABLES CONTAIN INFO HOW MANY MIDITICKS A CHANNEL HAS COUNTED SINCE THE LAST TIME IT FIRED A PULSE.. FOR STARTERS WE'RE AT 0.. ALSO, WE START THE MIDI TICK COUNTING AT 0
int midiTick=0;
int channel1Tick=0;
int channel2Tick=0;
int channel3Tick=0;
int channel4Tick=0;


// THE CHANNELCLOCKREADY (KINDA ROLLS OF YOUR TONGUE) TELLS THE MAIN LOOP WHETHER A CHANNEL IS READY TO FIRE OFF A PULSE - MEANING THE CHANNEL'S TICK COUNT HAS REACHED WHATEVER IT NEEDS (DEPENDING ON THE FACTOR)
bool channel1ClockReady=true;
bool channel2ClockReady=true;
bool channel3ClockReady=true;
bool channel4ClockReady=true;

// THE MODE VAR DECRIBES IF THE DEVICE IS IN THE BPM SETTING MODE OR ONE OF THE CHANNEL SETTING MODES.. 0 IS BPM
byte mode = 0;
