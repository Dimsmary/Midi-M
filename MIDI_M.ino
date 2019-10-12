 int velocity = 100;//velocity of MIDI notes, must be between 0 and 127
 //higher velocity usually makes MIDI instruments louder
 
 int noteON = 144;//144 = 10010000 in binary, note on command
 int noteOFF = 128;//128 = 10000000 in binary, note off command
 int note1 = 45;
 int on1 = 0;
 int note2 = 48;
 int on2 = 0;
 int note3 = 43;
 int on3 = 0;
 int note4 = 53;
 int on4 = 0;
 int note5 = 54;
 int on5 = 0;
 int note6 = 55;
 int on6 = 0;
 
 
void setup() {
  //  Set MIDI baud rate:
  Serial.begin(38400);
  pinMode(2,INPUT_PULLUP);
  pinMode(3,INPUT_PULLUP);
  pinMode(4,INPUT_PULLUP);
  pinMode(5,INPUT_PULLUP);
  pinMode(6,INPUT_PULLUP);
  pinMode(7,INPUT_PULLUP);
  pinMode(8,INPUT_PULLUP);
  pinMode(9,INPUT_PULLUP);
}

void loop() {
  
  if(!digitalRead(2) && on1 == 0){
    MIDImessage(noteON, note1, velocity);//turn note on
    on1 = 1;
  }
  if(digitalRead(2) && on1 == 1){
    MIDImessage(noteOFF, note1, velocity);//turn note off
    on1 = 0;
  }

    if(!digitalRead(3) && on2 == 0){
    MIDImessage(noteON, note2, velocity);//turn note on
    on2 = 1;
  }
  if(digitalRead(3) && on2 == 1){
    MIDImessage(noteOFF, note2, velocity);//turn note off
    on2 = 0;
  }
  
    if(!digitalRead(4) && on3 == 0){
    MIDImessage(noteON, note3, velocity);//turn note on
    on3 = 1;
  }
  
  if(digitalRead(4) && on3 == 1){
    MIDImessage(noteOFF, note3, velocity);//turn note off
    on3 = 0;
    
  }
//    if(!digitalRead(5) && on4 == 0){
//    MIDImessage(noteON, note4, velocity);//turn note on
//    on4 = 1;
//  }
//  if(digitalRead(5) && on4 == 1){
//    MIDImessage(noteOFF, note4, velocity);//turn note off
//    on4 = 0;
//  }
  
}

//send MIDI message
void MIDImessage(int command, int MIDInote, int MIDIvelocity) {
  Serial.write(command);//send note on or note off command 
  Serial.write(MIDInote);//send pitch data
  Serial.write(MIDIvelocity);//send velocity data
}
