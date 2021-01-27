  unsigned long dotResetTime = 0; // DEFINES WHEN THE BEAT INDICATOR DOT ON A CHANNEL SHOULD BE REMOVED
  bool dotsAreResat = false; // ARE THE DOTS REMOVED YET?
  int oldPosition  = 0; // JUST INITIALIZING A VAR USED FOR READING THE ROTARY ENCODER


// HERE WE GO.. MAIN LOOP
void loop(){


  // IF THE ROTARY BUTTON IS CLICKED, SWITCH THE MODE
  if (rotaryButton.pressed()){

    modeSwitch();
    modeSelect();
        
  }

  // IF THE START BUTTON IS PRESSED
  if (startButton.pressed()){

    if (runLoop) {      // IF THE LOOP/BEATS WAS RUNNING, IT MEANS WE WANT TO STOP IT
      Timer1.stop();    // SO, STOP THE TIMER
      runLoop = false;  // AND TELL EVERYTHING ELSE THAT THE LOOP IS STOPPED

      // THEN RESET EVERYTHING.. NOT SURE IF THIS IS NEEDED ACTUALLY.. BECAUSE WE RESET IT AGAIN ON START.. SHOULD CHECK
      midiTick=0;
      channel1Tick=0;
      channel2Tick=0;
      channel3Tick=0;
      channel4Tick=0;
      
      channel1ClockReady=false;
      channel2ClockReady=false;
      channel3ClockReady=false;
      channel4ClockReady=false;    
    
    } else {            // SO IF THE BUTTON WAS PRESSED AND THE LOOP/BEAT WASNT RUNNING, IT MEANS THAT WE WANT IT TO START

      // SO WE RESET EVERYTHING
      midiTick=0;
      channel1Tick=0;
      channel2Tick=0;
      channel3Tick=0;
      channel4Tick=0;
      
      channel1ClockReady=true;
      channel2ClockReady=true;
      channel3ClockReady=true;
      channel4ClockReady=true;

      Timer1.start();     // AND START THE TIMER
      runLoop = true;     // ALSO, TELL EVERYTHING ELSE THAT WE ARE NOW UP AND RUNNING AGAIN
    }       
  }


 // IF THE ROTARY ENCODER HAS BEEN TURNED.. WE PROBABLY HAVE TO DO SOMETHING..  
 int newPosition = myEnc.read();  // AND WE DO THAT BY FIRST READING THE ENCODER

  // HACK ALERT - HACK ALERT!! SO APPARENTLY BECAUSE THE ENCODER LIBRARY ATTACHES TO AN INTERRUPT TOO, IT MAGICALLY STARTS THE TIMER USED FOR THE LOOP/BEATS WHENEVER IT READS THE ENCODER.. WHICH MEANS THE TICKS STARTS FIRING
  if (runLoop == false){     // SO... IF WE KNOW THAT THE LOOP SHOULD BE STOPPED     
    Timer1.stop();           // WE STOP THE TIMER AGAIN.. OKAY THIS IS REALLY UGLY BUT WORKS FOR NOW
  }

  // CHECKING THE ENCODER TO SEE IF ITS VALUE IS DIFFERENT FROM LAST TIME WE CHECKED
  // THE ENCODER AND LIBRARY SENDS OUT FOUR COUNTS (WELL DIFFERENT BINARY PATTERNS OF THE TWO PINS)  PER PHYSICAL CLICK ON THE ENCODER - THIS IS HOW THESE ENCODERS MECHANICALLY WORK, AND IS WHY YOU CAN TELL IF IT ROTATES FORWARD OR BACKWARDS
  // HOWEVER, NOT CERTAIN WHY YOU'D EVER USE ALL FOUR - WE JUST WANT IT DOING SOMETHING ON THE PHYSICAL CLICK SO WE HAVE TO SEE WHETHER THE ENCODER LIBRARY HAS SPAT OUT MORE THAN FOUR COUNTS SINCE THE LAST CLICK READING
  if (newPosition >= (oldPosition+4) || newPosition <= (oldPosition-4)) {

    if (newPosition >= (oldPosition+4)){    // IF TURNED CLOCKWISE, THE LIBRARY ADDS COUNTS TO THE POSITION, AND WHEN IT REACHES 4...
      valueAdjust(1);                       // ... WE WANT TO ADJUST WHATEVER VALUE WE'RE IN THE PROCESS OF EDITING.. COULD BE A FACTOR SETTING ON A CHANNEL, OR COULD BE THE BPM
      oldPosition = newPosition;            // AND THEN REMEMBER THAT WE HAVE READ THE ENCODER IN THIS POSITION, SO WE CAN COMPARE
    }
    
    if (newPosition <= (oldPosition-4)){   // SAME AS ABOVE.. ONLY COUNTER CLOCKWISE
      valueAdjust(-1);
      oldPosition = newPosition;
    }
    
  }




  // IF THE MIDICLOCKPULSE ROUTINE HAS SET ONE OF THE CHANNELS READY FOR FIRING OFF A CLOCK PULSE.. WE SHOULD.. AND ALSO DISPLAY A NICE DOT ON THE SCREEN
  if ((channel1ClockReady || channel2ClockReady || channel3ClockReady || channel4ClockReady) && runLoop) {
  
  
  if (channel1ClockReady){
    digitalWrite(channel1Pin, HIGH);                          // SET THE CHANNEL'S CLOCK OUT PIN HIGH - THIS IS ONLY IMPLEMENTED FOR CHANNEL 1 FOR NOW
    display.fillCircle(46, 8, 2, SSD1306_WHITE);    // DISPLAY A NICE DOT ON THE SCREEN
    channel1ClockReady = false;                     // RESET THE READY FLAG SO WE ARE READY FOR THE NEXT PULSE
    }

  if (channel2ClockReady){
    digitalWrite(channel2Pin, HIGH);                          // SET THE CHANNEL'S CLOCK OUT PIN HIGH - THIS IS ONLY IMPLEMENTED FOR CHANNEL 1 FOR NOW
    display.fillCircle(46, 20, 2, SSD1306_WHITE);
    channel2ClockReady = false;
    }

  if (channel3ClockReady){
    digitalWrite(channel3Pin, HIGH);                          // SET THE CHANNEL'S CLOCK OUT PIN HIGH - THIS IS ONLY IMPLEMENTED FOR CHANNEL 1 FOR NOW
    display.fillCircle(46, 32, 2, SSD1306_WHITE);
    channel3ClockReady = false;
    }

  if (channel4ClockReady){
    digitalWrite(channel4Pin, HIGH);                          // SET THE CHANNEL'S CLOCK OUT PIN HIGH - THIS IS ONLY IMPLEMENTED FOR CHANNEL 1 FOR NOW
    display.fillCircle(46, 45, 2, SSD1306_WHITE);
    channel4ClockReady = false;
    }


  dotResetTime = (millis()+20); // WE WANT TO ONLY DISPLAY THE DOT FOR A SHORT WHILE.. 20MS IS BEAUTIFULLY HARDCODED HERE.. SO TO DO THAT, WE SET THE RESET TIME TO THE CURRENT TIME PLUS 20MS
  dotsAreResat = false;         

  display.display();            // AND WE DISPLAY THE STUFF IN THE DISPLAY BUFFER - IN THIS CASE IT MEANS THAT SOME FANCY DOTS HAVE BEEN ADDED

}


  // TO REMOVE THE DOTS AGAIN AND TO SET THE CHANNEL OUTPUT PINS LOW, WE LOOK AT THE CURRENT TIME AND COMPARE TO THE DOTRESET TIME WE HAD PREVIOUSLY SET
  if ((millis() >= dotResetTime) && (dotsAreResat == false)){
    
  digitalWrite(channel1Pin, LOW);          // WE SET CHANNEL1'S OUTPUT PIN LOW
  digitalWrite(channel2Pin, LOW);          // WE SET CHANNEL1'S OUTPUT PIN LOW
  digitalWrite(channel3Pin, LOW);          // WE SET CHANNEL1'S OUTPUT PIN LOW
  digitalWrite(channel4Pin, LOW);          // WE SET CHANNEL1'S OUTPUT PIN LOW

  // AND RESET ALL THE DOTZZ.. PROBABLY COULD JUST BE ONE BIG SQUARE.. OH WELL
  display.fillCircle(46, 8,  2, SSD1306_BLACK); //Reset the dots
  display.fillCircle(46, 20, 2, SSD1306_BLACK); //Reset the dots
  display.fillCircle(46, 32, 2, SSD1306_BLACK); //Reset the dots
  display.fillCircle(46, 45, 2, SSD1306_BLACK); //Reset the dots
  display.display(); //Display the dots
  dotsAreResat = true;
  
  }

} // OVER AND OVER AND OVER

//DONE.. 
