
// MIDICLOCKPULSE IS THE MAIN TIME KEEPER AND IS FIRED BY AN INTERRUPT AT A SET TIME INTERVAL; THE MIDI TICK TIME.. WHICH IS A 24TH OF THE INTERVAL BETWEEN BEATS
// IT KEEPS COUNT OF ALL THE INDIVIDUAL CHANNEL'S TIMING AND MARKS A CHANNEL READY TO FIRE A PULSE WHEN THE COUNTER MEETS THE CHANNELS PRE-SET FACTOR
// IT IS KEPT AS SIMPLE AS POSSIBLE AS IT IS INTERRUPT DRIVEN, AND WE ALL KNOW THAT IF AN INTERRUPT ROUTINE TAKES LONGER TO PERFORM THAT THE INTERRUPT TIME, IT WILL CREATE A RIFT IN THE TIME/SPACE CONTINUUM (OR.. AT LEAST HANG THE MCU) 
void midiClockPulse() {


  if (midiTick == 24) {   // KEEPING TRACK OF WHERE IN THE CURRENT BEAT WE ARE.. NOT USED FOR ANYHTING YET, BUT COULD COME IN HANDY IF ACTUAL MIDI OUT IS ADDED
    midiTick = 0;
  }
  midiTick++;

  // EACH TIME THE MIDICLOCKPULSE IS FIRED (24 TIMES PER BEAT), EACH CHANNEL'S COUNTER IS INCREASED
  // BY MULTIPLYING THE (24 MIDI TICKS) BY THE FACTOR SET FOR A CHANNEL, WE KNOW WHEN ITS TIME TO FIRE OFF A CHANNEL PULSE
  // WE TELL THE MAIN LOOP, BY SETTING CHANNEL(X)CLOCKREADY TO TRUE .. SAME IS DONE FOR EACH CHANNEL

  if (channel1Tick >= (MIDI_TICKS_PER_BEAT * factors[Channel_1_Factor]) ) {  
    channel1Tick = 0;
    channel1ClockReady = true;
  }
  channel1Tick++;

  if (channel2Tick >= (MIDI_TICKS_PER_BEAT * factors[Channel_2_Factor]) ) {
    channel2Tick = 0;
    channel2ClockReady = true;
  }
  channel2Tick++;

  if (channel3Tick >= (MIDI_TICKS_PER_BEAT * factors[Channel_3_Factor]) ) {
    channel3Tick = 0;
    channel3ClockReady = true;
  }
  channel3Tick++;

  if (channel4Tick >= (MIDI_TICKS_PER_BEAT * factors[Channel_4_Factor]) ) {
    channel4Tick = 0;
    channel4ClockReady = true;
  }
  channel4Tick++;

}


// MODESWITCH IS CALLED WHEN THE ROTARY ENCODER IS CLICKED AND JUST INCREASES THE MODE VARIABLE TO THE NEXT SETTING - IT RETURNS IT TO 0 (THE BPM MODE) AFTER ALL THE CHANNEL SETTINGS 
void modeSwitch() {
  mode++;
  if (mode == 5) {
    mode = 0;
  }
}


// MODESELECT ALSO CALLED WHEN THE ROTARY ENCODER IS CLICKED AND SETS UP THE GRAPHICAL UI WHEN THE MODE IS SWITCHED
// BECAUSE WE WANT TO AVOID DRAWING TOO MUCH AT ONCE, WE ONLY RESET THE PREVIOUS MODE INSTEAD OF REDRAWING EVERYTHING - NOT PRETTY, BUT VERY EFFECTIVE AT MIKING THINGS LOOK NICE 
void modeSelect() {
  drawUI();


  switch (mode) {
    case 1:
      {
        drawBpm();
        drawChannel1Highlighted();
        break;
      }
    case 2:
      {
        drawChannel1();
        drawChannel2Highlighted();
        break;
      }
    case 3:
      {
        drawChannel2();
        drawChannel3Highlighted();
        break;
      }
    case 4:
      {
        drawChannel3();
        drawChannel4Highlighted();
        break;
      }
    case 0:
      {
        drawChannel4();
        display.fillRoundRect(54, 6, 69, 53, 3, SSD1306_WHITE);
        drawBlackBpm();
        //display.display();
        break;
      }
  }

display.display();


}


 
// VALUE ADJUST IS CALLED WHEN THE ROTARY ENCODER IS TURNED AND CHANGES THE VALUE OF THE FACTOR FOR THE CURRENTLY SELECTED CHANNEL/MODE.. 
// IF MODE 0 - THEN IT CHANGES THE VALUE OF THE BPM
// IT IS CALLED WITH A VAR (IN THIS APP ITS 1 OR -1) WHICH TELLS IT HOW MUCH TO ADJUST THE VALUE BY
// OH.. THE NUMBER OF VALUES AND THE MAXBPM ARE HARDCODED HERE.. SHOULD PROBABLY MAKE THOSE VARS.. 
void valueAdjust(int value) {



  switch (mode) {
    case 1:
      {
        Channel_1_Factor = Channel_1_Factor + value;

        if (Channel_1_Factor > 16) {
          Channel_1_Factor = 16;
        }

        if (Channel_1_Factor <= 0) {
          Channel_1_Factor = 0;
        }

          drawChannel1Highlighted();
          display.display();
          break;
      }
    case 2:
      {
        Channel_2_Factor = Channel_2_Factor + value;

        if (Channel_2_Factor > 16) {
          Channel_2_Factor = 16;
        }

        if (Channel_2_Factor <= 0) {
          Channel_2_Factor = 0;
        }

        drawChannel2Highlighted();
        display.display();
        break;
      }
    case 3:
      {
        Channel_3_Factor = Channel_3_Factor + value;

        if (Channel_3_Factor > 16) {
          Channel_3_Factor = 16;
        }
        
        if (Channel_3_Factor <= 0) {
          Channel_3_Factor = 0;
        }
        
        drawChannel3Highlighted();
        display.display();

        break;
      }
    case 4:
      {
        Channel_4_Factor = Channel_4_Factor + value;

        if (Channel_4_Factor > 16) {
          Channel_4_Factor = 16;
        }
        
        if (Channel_4_Factor <= 0) {
          Channel_4_Factor = 0;
        }

        drawChannel4Highlighted();
        display.display();

        break;
      }
    case 0:
      {

        bpm = bpm + value;

        if (bpm > 140) {
          bpm = 140;
        }
        
        if (bpm < 0) {
          bpm = 0;
        }

        // SAME CALCULATIONS AS IN THE INITIALIZATION WHEN THE BEAT CHANGES.. PERHAPS A ROUTINE INSTEAD OF THIS DOUBLE CODING.. WELL.. 
        intervalPerBeat = (60000000/bpm);
        midiInterval = (intervalPerBeat/MIDI_TICKS_PER_BEAT);    //interval is the number of milliseconds defined by tempo formula.
        Timer1.setPeriod(midiInterval);

        // WHEN THE BPM IS CHANGED - WE WANNA SEE IT ON THE DISPLAY RIGHT? DELETE THE CURRENT ONE.. FIRE OFF A NEW ONE
        display.fillRoundRect(54, 6, 69, 53, 3, SSD1306_WHITE);
        drawBlackBpm();
        display.display();

        break;
      }

    // EEERMM.. THIS LOOKS LIKE ITS NOT REALLY NEEDED.. REMEMBER TO REMOVE I GUESS (DONT HAVE THE BOARD HANDY RIGHT NOW)
    display.display();

  }



}



// SOME UGLY HARDCODED ROUTINES TO DRAW UI COMPONENTS WHEN SWITCHING MODES OR CHANGING VALUES... SOMETIMES ITS NECESSARY TO CHANGE THE TEXT TO BLACK, AND WHEN IT IS, WE CHANGE IT BACK TO WHITE AFTERWARDS LIKE THE GOOD AN TIDY PEOPLE WE ARE
void drawChannel1Highlighted(){
  display.fillRoundRect(15, 4, 26, 9, 1, SSD1306_WHITE);
  display.setCursor(23, 10);
  display.setTextColor(SSD1306_BLACK);
  display.println(factorNames[Channel_1_Factor]);
  display.setTextColor(SSD1306_WHITE);
  //display.display();
}


void drawChannel2Highlighted(){
  display.fillRoundRect(15, 16, 26, 9, 1, SSD1306_WHITE);
  display.setCursor(23, 22);     // Start at top-left corner
  display.setTextColor(SSD1306_BLACK);
  display.println(factorNames[Channel_2_Factor]);
  display.setTextColor(SSD1306_WHITE);
  //display.display();
}


void drawChannel3Highlighted(){
  display.fillRoundRect(15, 28, 26, 9, 1, SSD1306_WHITE);
  display.setCursor(23, 34);     // Start at top-left corner
  display.setTextColor(SSD1306_BLACK);
  display.println(factorNames[Channel_3_Factor]);
  display.setTextColor(SSD1306_WHITE);  
  //display.display();
}


void drawChannel4Highlighted(){
  display.fillRoundRect(15, 40, 26, 9, 1, SSD1306_WHITE);
  display.setCursor(23, 46);     // Start at top-left corner
  display.setTextColor(SSD1306_BLACK);
  display.println(factorNames[Channel_4_Factor]);
  display.setTextColor(SSD1306_WHITE);
  //display.display();
}


void drawChannel1(){
  display.fillRoundRect(15, 4, 26, 9, 1, SSD1306_BLACK);
  display.drawRoundRect(15, 4, 26, 9, 1, SSD1306_WHITE);
  display.setCursor(23, 10);
  display.println(factorNames[Channel_1_Factor]);
  //display.display();
}


void drawChannel2(){
  display.fillRoundRect(15, 16, 26, 9, 1, SSD1306_BLACK);
  display.drawRoundRect(15, 16, 26, 9, 1, SSD1306_WHITE);
  display.setCursor(23, 22);     // Start at top-left corner
  display.println(factorNames[Channel_2_Factor]);
  //display.display();
}


void drawChannel3(){
  display.fillRoundRect(15, 28, 26, 9, 1, SSD1306_BLACK);
  display.drawRoundRect(15, 28, 26, 9, 1, SSD1306_WHITE);
  display.setCursor(23, 34);     // Start at top-left corner
  display.println(factorNames[Channel_3_Factor]);
  //display.display();
}


void drawChannel4(){
  display.fillRoundRect(15, 40, 26, 9, 1, SSD1306_BLACK);
  display.drawRoundRect(15, 40, 26, 9, 1, SSD1306_WHITE);
  display.setCursor(23, 46);     // Start at top-left corner
  display.println(factorNames[Channel_4_Factor]);
  //display.display();
}



// DRAWUI JUST SETS UP THE OVERALL UI COMPONENTS ON THE SCREEN.. IS CALLED ON INITIALIZATION
void drawUI() {
  display.clearDisplay();
  display.drawRoundRect(4, 4, 10, 9, 1, SSD1306_WHITE);
  display.drawRoundRect(4, 16, 10, 9, 1, SSD1306_WHITE);
  display.drawRoundRect(4, 28, 10, 9, 1, SSD1306_WHITE);
  display.drawRoundRect(4, 40, 10, 9, 1, SSD1306_WHITE);

  display.drawRoundRect(4, 52, 37, 9, 1, SSD1306_WHITE);

  display.setCursor(8, 10);     // Start at top-left corner
  display.println("1");
  display.setCursor(8, 22);     // Start at top-left corner
  display.println("2");
  display.setCursor(8, 34);     // Start at top-left corner
  display.println("3");
  display.setCursor(8, 46);     // Start at top-left corner
  display.println("4");

   display.setCursor(12, 58);     // Start at top-left corner
   display.println("C L O X");

  display.drawRoundRect(52, 4, 73, 57, 5, SSD1306_WHITE);

  drawChannel1();
  drawChannel2();
  drawChannel3();
  drawChannel4();

  drawBpm();
  display.display(); 
}




// TWO DIFFERENT ROUTINES FOR DRAWING THE LAAAARGE BPM NUMBER ON THE UI
// WE USE A PRRRRETTY BIG FONT FROM THE ADAFRUIT.GFX LIBRARY.. 
// ...IN FACT, SO BIG (HAHA) THAT WE HAVE CUT IT OFF DRAMATICALLY, AND IT NOW ONLY CONTAINS THE NUMBERS.. 28K FLASH ISNT A LOT WHEN TRYING TO FIT A FULL 18PT FONT SET
void drawBpm() {
  //display.fillRoundRect(54, 6, 68, 52, 4, SSD1306_WHITE);
  display.setCursor(57, 42);     // Start at top-left corner
  display.setFont(&FreeMonoBold18pt7b);
  //display.setTextColor(SSD1306_BLACK); // Draw white text
  display.println(int(bpm));
  display.setTextSize(1);      // Normal 1:1 pixel scale
  //display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setFont(&Picopixel);

}


void drawBlackBpm() {
  //display.fillRoundRect(54, 6, 68, 52, 4, SSD1306_WHITE);
  display.setCursor(57, 42);     // Start at top-left corner
  display.setFont(&FreeMonoBold18pt7b);
  display.setTextColor(SSD1306_BLACK); // Draw white text
  display.println(int(bpm));
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setFont(&Picopixel);

}

void initBpm() {
  display.fillRoundRect(54, 6, 69, 53, 3, SSD1306_WHITE);
  drawBlackBpm();

}



// FLASHY LOGO STUFF - ONLY CALLED ON SETUP AND IS DISPLAYED WITH THIS AWESOME HARDCODED DURATION
void splashLogo() {

  display.clearDisplay();
  display.drawBitmap(
    (display.width()  - CloxLogo_Width ) / 2,
    (display.height() - CloxLogo_Height) / 2,
    CloxLogo, CloxLogo_Width, CloxLogo_Height, 1);

  display.display();

  delay(2000); // Pause for 2 seconds
  display.clearDisplay();
  display.display();
}
