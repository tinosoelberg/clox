// SETTING UP ALL THE THINGS
void setup() {
 

  delay(100); // PAUSE FOR A BIT - TUBES NEED WARMING UP.. NOT SURE WHY, BUT THE OLED NEEDS A LITTLE TIME TO GET READY

  // THE OLED NEEDS TO BE SET UP TO USE THE RIGHT POWER.. SO WE TELL IT TO
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally... 
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3D)) { // Address 0x3D for 128x64
  //  Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // INITIALIZING THE MIDITICK TIMER AND ATTACHING THE ROUTINE TO AN INTERRUPT.. EXCITING STUFF
  Timer1.initialize(midiInterval);  
  Timer1.attachInterrupt(midiClockPulse); 

  // IF WE HAVE CHOSEN NOT TO START THE BEATS ON STARTUP.. THEN DON'T
  if (runLoop == false){
    Timer1.stop();
  }

  // FEELING FANCY? SHOW THE LOGO
  if (SHOW_LOGO) { splashLogo(); }

  // START READING THE ROTARY BUTTON?... GOTTA CHECK THIS ONE... MOSTLY BECAUSE THERE'S NO STARTBUTTON.BEGIN.. YET IT STILL WORKS!!
  rotaryButton.begin();

  // SETTING THE INPUTS FOR THE TWO BUTTONS - THE MCU HAS BUILT IN PULL UP RESISTORS SO NO NEED FOR EXTERNAL... WHICH IS PRETTY NEAT
  pinMode(13, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);

  // THIS IS THE OUTPUT PIN FOR CHANNEL 1 (THE ONLY CHANNEL CURRENTLY ACTUALLY FIRING CLOCK PULSES) - ALSO SETTING IT LOW TO BEGIN WITH
  pinMode(8, OUTPUT);
  digitalWrite(8, LOW);

  // SETTING THE UI STUFF - THE PICOPIXEL FONT IS FROM THE ADAFRUIT.GFX LIBRARY AND IS SOOOOOOO CUTE
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setFont(&Picopixel);

  drawUI();
  initBpm();

  // AT SOME POINT WE'LL MAKE A SETTINGS AREA WHERE YOU CAN SET THE CONTRAST OF THE DISPLAY.. SO YOU CAN HAVE LIKE A NIGHT MODE
  //display.ssd1306_command(SSD1306_SETCONTRAST);
  //display.ssd1306_command(1); // Where c is a value from 0 to 255 (sets contrast e.g. brightness)

 

  display.display();


// DEBUGGING STUFF
// // Serial.begin(57600);
//  delay(100);
//  Serial.println("- Clox online");
  
}
// END OF SETUP
