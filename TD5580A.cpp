/*
 * TD5580A.cpp - Library for TD5580A-based MP3 audio board
 * 
 * Created by Salvador Rueda Pau, July 23, 2016.
 * Modified by Misael Reyes
 * 
 * Adapted from https://github.com/misa3L994/GD3300/
 * Adapted for TD5580A by Keith Benoit, January, 2025.
 * http://www.tudasemi.com/static/upload/file/20240905/1725499313437991.pdf
 * 
 * This is currently a WORK IN PROGRESS.  It does play MP3 files.
 */

#include <Arduino.h>
#include <TD5580A.h>

// Show debug data messages
//
void TD5580A::showDebug(bool op) {
  _showDebugMessages = op;
}

// Start the mp3 player serial stream
//
void TD5580A::begin(Stream &s) {
  serial = &s;
}

// Play the next track 
// CMD = 0x01
//
void TD5580A::playNext() {
  sendCommand(CMD_NEXT);
}

// Play the previous track
// CMD = 0x02
//
void TD5580A::playPrevious() {
  sendCommand(CMD_PREV);
}

// Play specific track N
// CMD = 0x03
//
void TD5580A::play(byte tknum) {
  sendCommand(CMD_PLAYN, tknum);
}

// Volume + / Up / Increase
// CMD = 0x04
///
void TD5580A::volUp() {
  sendCommand(CMD_VOL_UP);
}

// Volume - / Down / Decrease
// CMD = 0x05
//
void TD5580A::volDown() {
  sendCommand(CMD_VOL_DOWN);
}

// Set volume (0..30)
// CMD = 0x06
// 24 may be max useful value
//
void TD5580A::setVol(byte volume) {
  sendCommand(CMD_SET_VOL, volume);
}

// Set equalization (0..6)
// CMD = 0x07
// 0:Normal, 1:Rock, 2:???, 3:Pop, 4:Classical, 5:Country, 6:Jazz
//
void TD5580A::setEq(byte eq) {
  sendCommand(CMD_SET_VOL, eq);
}

// Single loop specified track
// CMD = 0x08
//
void TD5580A::playSL(byte tknum) {
  sendCommand(CMD_PLAY_SLOOP, tknum);
}

// Specify playback Source 1..6
// CMD = 0x09
//
void TD5580A::setSource(byte src) {
  sendCommand(CMD_SET_SOURCE, src);
}

// Set low-power (sleep) mode - NOT SUPPORTED
// CMD = 0x0A
//
void TD5580A::setSleep() {
  sendCommand(CMD_SLEEP_MODE);
}

// RESERVED
// CMD = 0x0B
//

// Reset the player
// CMD = 0x0C
//
void TD5580A::reset() {
  sendCommand(CMD_RESET);
}

// Play 
// CMD = 0x0D
//
void TD5580A::play() {
  sendCommand(CMD_PLAY);
}

// Pause
// CMD = 0x0E
//
void TD5580A::pause() {
  sendCommand(CMD_PAUSE);
}

// Play file N in folder F
// CMD = 0x0F
//
void TD5580A::playF(byte f, byte n) {
  sendCommand(CMD_PLAY_F_FILE, f, n);
}

// Amplification settings (not supported)
// CMD = 0x10
//

// Play all tracks in a loop
// CMD = 0x11
//
void TD5580A::playAllLoop() {
  sendCommand(CMD_PLAY_LOOPS);
}

// Specify the MP3 folder 
//   (!! folder name MUST be "MP3" !!)
// CMD = 0x12 
//
void TD5580A::setFolder(byte f) {
  sendCommand(CMD_SET_FOLDER, f);
}

// Insert ADVERT files
// CMD = 0x13
//
void TD5580A::insertAdvert(byte n) {
  sendCommand(CMD_PLAY_ADVERT, n);
}

// Play Folder and File (Same as 0x0F)
// CMD = 0x14
//
// void TD5580A::playF14(byte f, byte n) {
//   sendCommand(CMD_PLAY_F_FILE2, f, n);
// }

// 0x15 Stop inserting ads
// CMD = 0x15
//
void TD5580A::stopAdverts() {
  sendCommand(CMD_STOP_ADVERTS);
}

// Stop playing current track
// CMD = 0x16
//
void TD5580A::stop() {
  sendCommand(CMD_STOP_PLAY);
}

// Play all files in folder F
// CMD = 0x17
//
void TD5580A::loopFolder(byte f) {
  sendCommand(CMD_FOLDER_CYCLE, f, 0);
}

// Play all files in random order
// CMD = 0x18
//
void TD5580A::playShuffle() {
  sendCommand(CMD_PLAY_SHUFFLE);
}

// Current track loop play
// CMD = 0x19
// 0-Open single loop; 1-Close single loop
//
void TD5580A::playL(bool on) {
  if(on)sendCommand(CMD_SET_SPLAY, 0, 0);
  else sendCommand(CMD_SET_SPLAY, 0, 1);
}

// Enable DAC high impedance
// CMD = 0x1A 
// 0 - Turn OFF DAC high impedance
// 1 - Turn ON  DAC high impedance
//
void TD5580A::setDacHighImp(byte n) {
  sendCommand(CMD_DAC_IMP_HIGH, n);
}

// Set Power-On Volume Level 
// CMD = 0x1B 
// value from 1..30 (24 seems to be practical maximum)
//
void TD5580A::setPowerOnVol(byte v) {
  sendCommand(CMD_PWR_ON_VOL_MEM, v);
}

/*
 * Query functions - return uint16_t numeric values
 */

// Query player status
//
uint16_t TD5580A::qStatus() {
  sendCommand(QRY_STATUS);
  return MP3Answer();
}

// Query player volume
//
uint16_t TD5580A::qVolume() {
  sendCommand(QRY_VOLUME);
  return MP3Answer();
}

// Query number of tracks in the current folder
//
uint16_t TD5580A::qFTracks() {
  sendCommand(CMD_QUERY_FLDR_TRACKS);
  return MP3Answer();
}

// Query total number of tracks
//
uint16_t TD5580A::qTTracks() {
  sendCommand(CMD_QUERY_TOT_TRACKS);
  return MP3Answer();
}

// Query total number of folders
//
uint16_t TD5580A::qTFolders() {
  sendCommand(CMD_QUERY_FLDR_COUNT);
  return MP3Answer();
}

// Put player in low-power (sleep) mode
//
void TD5580A::sleep() {
  // Send sleep command
  sendCommand(CMD_SLEEP_MODE);
}

// Wakeup player from low-power (sleep) mode
//
void TD5580A::wakeup() {
  sendCommand(CMD_WAKE_UP);
}

// Send a one byte command with no data
// This zero fills both data bytes
//
void TD5580A::sendCommand(byte command) {
  sendCommand(command, 0, 0);
}

// Send a one byte command with one byte of data
// This zero fills the first data byte.
//
void TD5580A::sendCommand(byte command, byte dat2) {
  sendCommand(command, 0, dat2);
}

// Send a one-byte command with two bytes of data
//
void TD5580A::sendCommand(byte command, byte dat1, byte dat2) {

  byte Send_buf[8] = {0};    // Initialize data bytes buffer
  String mp3send = "";       // Initialize string data buffer

  // Command Structure HEAD ADDR LEN  CMD ACK DATA CKL CKH END
  // Command Structure 0x7E 0xFF 0x06 CMD ACK DATA CK1 CK2 0xEF

  //#ifndef NO_SERIALMP3_DELAY
  delay(20);
  //#endif

  Send_buf[0] = 0x7E;    // HEAD byte constant
  Send_buf[1] = 0xFF;    // ADDR byte constant
  Send_buf[2] = 0x06;    // LEN  length excluding HEAD, END, CHECKSUM, and LENGTH bytes
  Send_buf[3] = command; // CMD
  Send_buf[4] = 0x00;    // ACK  feedback 0x00 NO FEEDBACK, 0x01 FEEDBACK REQUESTED
  Send_buf[5] = dat1;    // DATA data byte 1
  Send_buf[6] = dat2;    // DATA data byyte 2
  Send_buf[7] = 0xEF;    // END  byte constant

/* Checksum bytes 
 *         cksum = 0 - (   ADDR     +     LEN     +     CMD     +     ACK     +   DATA 1    + DATA 2 ); 
 */
  //uint16_t cksum = 0 - (Send_buf[1] + Send_buf[2] + Send_buf[3] + Send_buf[4] + Send_buf[5] + Send_buf[6]);
  //Send_buf[7] = lowByte(cksum);
  //Send_buf[8] = highByte(cksum);

  for(int i=0; i<8; i++)
  {
    serial->write(Send_buf[i]) ;
    mp3send += sbyte2hex(Send_buf[i]);
  }

  if (_showDebugMessages) {
     Serial.print("Sending: ");
     Serial.println(mp3send);     // display hex bytes sent to DFPlayer
  }

  // #ifndef NO_SERIALMP3_DELAY
  delay(1000);
  // Wait between sending commands.
  // #endif
}

// String sanswer(void);
// int iansbuf = 0;                 // Index for answer buffer.
// static uint8_t ansbuf[10] = {0}; // Buffer for the answers.

uint8_t val;



/******************************************************************
 * function : MP3Answer()                                         *
 * purpose  : returns the bit corresponding to the status request *
 * Values returned in MP3Answer();                                *
 * 0x0001 -> memory card insert
 * 0x0002 -> Complete play
 * 0x0003 -> Error
 * 0x0004 -> Data receive correct
 *  -> status
 *  - 0x000A-> stopped
 *  - 0x000B-> playing  
 *  - 0x000C-> pause  
 * 0x0005 -> Vol playing
 * 0x0006 -> File count
 * 0x0007 -> playing
 * 0x0008 -> folder file  count
 * 0x0009 -> folder count
 * 
 */

/* Misael Reyes */
uint16_t TD5580A::MP3Answer() {
//
//                     HEAD ADDR LEN RESP  ACK  ??? Dat Ck1  Ck2  END
// Response Structure  0x7E 0xFF 0x06 RSP 0x00 0x00 DAT 0xFE 0xBA 0xEF
//
// RSP Response code
// DAT Response additional data
  
  uint16_t decodedMP3Answer = 0x0000;     // numeric value returned
  String   answer           = sanswer();  // read from DFPlayer serial port
  String   received         = "";         // hex bytes received from DFPlayer
  
  /*
   * Load different numeric values based
   *  upon which call is being answered.
   *  That is indicated by byte 4, ansbuf[3]
   */
  switch (ansbuf[3])
  {
  /*
    * 0x3A == Device insertion (SD / USB / Flash device)
    * 0x3B == Device unplugged
    * 0x3C == UDISK Playback Completed
    * 0x3D == SD card playback completed
    * 0x3E == Flash playback completed
    * 0x3F == Send initialization parameters (set player status)
    */
    case 0x3A:
      decodedMP3Answer = 0x0001;
      Serial.println ("\nMemory card inserted.");
      break;

    case 0x3B:
      decodedMP3Answer = 0x0002;
      Serial.println("\nDevice unplugged.");
      break;

    case 0x3C:
      decodedMP3Answer = 0x0003;
      Serial.println("\nDevice unplugged.");
      break;

    case 0x3D:
      decodedMP3Answer = 0x0004;
      Serial.println("\nUDISK Playback Completed");
      break;

    case 0x3E:
      decodedMP3Answer = 0x0005;
      Serial.println("\nFlash Playback Completed");
      break;

    case 0x3F:
      decodedMP3Answer = 0x0006;
      Serial.println("\nSent initialization parameters");
      break;

    /*
     * 0x40 == Return error, request resend
     * 0x41 == Response
     * 0x42 == Query current status
     * 0x43 == Query the current volume
     * 0x44 == Query the current EQ
     * 0x45 == Query the current playback mode
     */
    case 0x40:
      decodedMP3Answer = 0x0007;
      Serial.println("\nError, resend!");
      break;

    case 0x41:    
      decodedMP3Answer = 0x0008;
      Serial.println("\nResponse received.");
      break;

    case 0x42:	     
      switch(ansbuf[6]) {
        case 0: decodedMP3Answer = 0x0A; break;  // |= (1<<4) | (1<<12); break;  // " -> Status: stopped"; break;
        case 1: decodedMP3Answer = 0x0B; break;  // |= (1<<4) | (1<<11); break;  // " -> Status: playing"; break;
        case 2: decodedMP3Answer = 0x0C; break;  // |= (1<<4) | (1<<10); break;  // " -> Status: paused"; break;
      }
      break;

    case 0x43:
      //+= " -> Vol playing: " + String(ansbuf[6], DEC);
      decodedMP3Answer = ansbuf[6];	  
      break;

    case 0x48:
      //" -> File count: "
      decodedMP3Answer = ansbuf[6];  
      break;

    case 0x4C:
      //" -> Playing: "
      decodedMP3Answer = ansbuf[6];
      break;

    case 0x4E:
      //" -> Folder file count: "
      decodedMP3Answer = ansbuf[6];  
      break;

    case 0x4F:
      //" -> Folder count: "
      decodedMP3Answer = ansbuf[6];	
      break;
    }

//ansbuf[6] = 0; // Clear ansbuff.
  ansbuf[3] = 0; // Clear ansbuff.
  return decodedMP3Answer;
}



//===========================================================
/***********************************************************/
/* Function: sbyte2hex. Returns a byte data in HEX format. */
/* Parameter:- uint8_t b. Byte to convert to HEX.          */
/* Return: String                                          */

String TD5580A::sbyte2hex(byte b)
{
  String shex;

// Serial.print("0x");
// if (b < 16) Serial.print("0");
// Serial.print(b, HEX);
// ansbuf[6] = 0; // Clear ansbuff.

  shex="0X";
  if (b < 16) shex += "0";
  shex += String(b, HEX);
  shex += " ";
  return shex;
}


/********************************************************************/
/* Function: sanswer. Returns a String answer from mp3 UART module.	*/
/* Return: String.  the answer                                      */

String TD5580A::sanswer(void) {
  // Response Structure  0x7E 0xFF 0x06 RSP 0x00 0x00 DAT 0xFE 0xBA 0xEF
  //
  // RSP Response code
  // DAT Response additional data

  // if there are something available start to read from mp3 serial.
  // if there are "0x7E" it's a beginning.
  //
  //  read while something readed and it's not the end "0xEF"

  byte b;
  String mp3answer = "";                // Answer from the DFPlayer.

  int iansidx = 0;

  while (serial->available() ) {        // && b!=0xef
   //do{
    b = serial->read();
  
    /*
     * 0x7E == Start byte
     *   reset ansbuf index to zero
     *   reset string response to null
     */
    if (b == 0x7E) {
      iansidx = 0;
      mp3answer = "";  
    }

    /*
     * Add each byte received to ansbuf
     * and also to the string response
     */
    ansbuf[iansidx] = b;
    mp3answer += sbyte2hex(ansbuf[iansidx]);
    iansidx++;

    /*
     *  0x3A .. 0x3F
     *
     * 0x3A == Device insertion (SD / USB / Flash device)
     * 0x3B == Device unplugged
     * 0x3C == UDISK Playback Completed
     * 0x3D == SD card playback completed
     * 0x3E == Flash playback completed
     * 0x3F == Send initialization parameters (set player status)
     */
    if ( b >= 0x3A && b <= 0x3F ) {
      ansbuf[3]=b;
    }

    /*
     * 0xEF == End byte
     */
    if (b == 0xef) {
      // Serial.print("ansbuf[3]:0x");
      // Serial.println(ansbuf[3],HEX);
      // Serial.print("ansbuf[6]:0x");
      // Serial.println(ansbuf[6],HEX);
    }
   //}while(b != 0xEF);
   // while there are something to read and it's not the end "0xEF"
  }

  /*
   * Debug - print output from DFPlayer device
   */
  if (_showDebugMessages) {
    Serial.println("mp3answer = " + String(mp3answer));
  }

  return mp3answer;
 }
