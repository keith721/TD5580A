/* 
 * TD5580A.h - Library for TD5580A-based MP3 audio board
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

#ifndef TD5500A_h
#define TD5580A_h

#include <Arduino.h>

#define CMD_NEXT             0x01 // Play next file
#define CMD_PREV             0x02 // Play previous file
#define CMD_PLAYN            0x03 // Play n file from (1-255)
#define CMD_VOL_UP           0x04 // Volume +
#define CMD_VOL_DOWN         0x05 // Volume -
#define CMD_SET_VOL          0x06 // Set Volume (0-30)
#define CMD_SET_EQ           0x07 // Set Equalization 0..6 0:Normal, 1:Rock, 3:Pop, 4:Classic, 5:Country, 6:Jazz
#define CMD_PLAY_SLOOP       0x08 // Single loop play.
#define CMD_SET_SOURCE       0x09 // Set source: 1-USB Disk, 2-SD Card, 3-Aux, 4-Flash, 5-PC, 6-Sleep
#define CMD_SLEEP_MODE       0x0A // low power mode (sleep)
#define CMD_WAKE_UP          0x0B // Reserved
#define CMD_RESET            0x0C // Chip Reset
#define CMD_PLAY             0x0D // Play
#define CMD_PAUSE            0x0E // Pause
#define CMD_PLAY_F_FILE      0x0F // Play Folder and File
#define CMD_AMPLIFIER        0x10 // Amplifier settings (none)
#define CMD_PLAY_LOOPS       0x11 // Play all tracks in a loop
#define CMD_SET_FOLDER       0x12 // Specify the MP3 folder name - must be 'MP3'
#define CMD_PLAY_ADVERT      0x13 // Insert ADVERT file - folder name must be 'ADVERT'
#define CMD_PLAY_F_FILE2     0x14 // Play Folder and File (Same as 0x0F)
#define CMD_STOP_ADVERTS     0x15 // Stop inserting ads
#define CMD_STOP_PLAY        0x16 // Stop playing
#define CMD_FOLDER_CYCLE     0x17 // Loop playback of files in specified folder
#define CMD_PLAY_SHUFFLE     0x18 // All files are played randomly in physical order
#define CMD_SET_SPLAY        0x19 // Current track loop play [00] close single cycle play [01]
#define CMD_DAC_IMP_HIGH     0x1A // Enable DAC high impedance 0-Disable; 1-Enable 
#define CMD_PWR_ON_VOL_MEM   0x1B // 0-Power-on memory; 1-30 Fixed power-on volume
#define CMD_SET_BAUD_RATE    0x1C // 0-9600;1-19200;2-38400; 3-57600; 4-115200; Restart to take effect
#define CMD_FACTORY_RESET    0x1D // Restore factory settings
#define CMD_COMBO_REPORT     0x20 // Combination Report

// Query Commands
#define CMD_QU_DEV_INSERTED   0x3A // qInserted();  // device inserted? 1-UDISK, 2-SD Card, 3-PC
#define CMD_QU_DEV_UNPLUGGED  0x3B // qUnplugged(); // device unplugged
#define CMD_QU_UDISK_COMPL    0x3C // qUdiskComp(); // UDISK completed playing
#define CMD_QU_TF_SD_COMPL    0x3D // qTfSdComp();  // TF/SD card completed playing
#define CMD_QU_FLASH_COMPL    0x3E // qFlshComp();  // flash mem completed playing
#define CMD_SEND_INIT_PARMS   0x3F // sendInitParms();  // send initialization parameters

// Status / Responses
#define CMD_ERROR_RESEND      0x40 // Return error, request resend
#define CMD_RESPONSE          0x41 // Response
#define QRY_STATUS            0x42 // qStatus();    // query the status
#define QRY_VOLUME            0x43 // qVolume();    // query the volume. // Sometime board fails
#define QRY_EQUALIZATION      0x44 // qEqualiz();   // query the Equalization setting
#define QRY_PLAYBACK_MODE     0x45 // qPlayback();  // query the Playback mode
#define QRY_SW_VERSION        0x46 // qSwVersion(); // query the Software version
#define QRY_TOTAL_FILES_USB   0x47 // qFilesUsb();  // query the number of files on USB-stick
#define QRY_TOTAL_FILES_TFC   0x48 // qFilesTfc();  // query the number of files on TF/SD card
#define QRY_TOTAL_FILES_FLSH  0x49 // qFilesFlash(); // query the number of files on Flash storage
#define QRY_RESERVED_4A       0x4A //
#define QRY_TRACK_USB         0x4B // qTrackUsb();  // Query the current track of the USB disk
#define QRY_TRACK_SD_CARD     0x4C // qPlaying();   // query the current track on the SD card
#define QRY_TRACK_CHK_FLASH   0x4D // qChkTrk();    // Check FLASH current track
#define CMD_QUERY_FLDR_TRACKS 0x4E // qFTracks();   // query the number of tracks folders
#define CMD_QUERY_TOT_TRACKS  0x48 // qTTracks();   // query the total of tracks
#define CMD_QUERY_FLDR_COUNT  0x4F // qTFolders();  // query the number of folders

/*
 * When the module is powered on, it requires a certain amount of time to initialize.
 * This time depends on the USB flash drive, SD card, flash memory, etc, and on the 
 * total number of files/tracks on the device.  Typically, from 1.5 to 3.0 seconds.
 * When this data is not returned, check the wiring / connections / hardware.
 */
#define STS_USB_DRIVE_ONLINE  0x01 // USB device is attached and online
#define STS_SD_CARD_ONLINE    0x02 // TF or SD card is inserted and online
#define STS_U_DISK_ONLINE     0x03 // USB Disk is attached and online
#define STS_PC_ONLINE         0x04 // PC device is attached and online
#define STS_FLASH_ONLINE      0x08 // Flash device is attached and online

/************ Options **************************/

class TD5580A{

  public:  
    void sendCommand(byte command);
    void sendCommand(byte command, byte dat2);
    void sendCommand(byte command, byte dat1, byte dat2);

    void showDebug(bool on);     // true to show debug messages
    void begin(Stream&);
     
	  uint16_t MP3Answer();

    void playNext();             // 0x01 Play next file
    void playPrevious();         // 0x02 Play previous file
    void play(byte n);           // 0x03 Play n file
    void volUp();                // 0x04 Volume Up
    void volDown();              // 0x05 Volume Down
    void setVol(byte v);         // 0x06 Set Volume (0-30)
    void setEq(byte e);          // 0x07 Set equalization (0..6)
    void playSL(byte n);         // 0x08 Specified track single loop
    void setSource(byte d);      // 0x09 Specify playback source (1..6)
    void setSleep();             // 0x0A Set low-power (sleep) mode (NOT SUPPORTED)
                                 // 0x0B RESERVED
    void reset();                // 0x0C Send reset command
    void play();                 // 0x0D Play
    void pause();                // 0x0E Pause
    void playF(byte f, byte n);  // 0x0F Play Folder f File n
                                 // 0x10 Amplification Settings (NOT SUPPORTED)                    
    void playAllLoop();          // 0x11 Play all loops
    void setFolder(byte n);      // 0x12 Specify the MP3 folder (MP3 or ADVERT)
    void insertAdvert(byte n);   // 0x13 Insert ADVERT file
    //   playF(byte f, byte n);  // 0x14 Play Folder f File n (Same as 0x0F)
    void stopAdverts();          // 0x15 Stop inserting advertisements
    void stop();                 // 0x16 Stop all playback actions
    void loopFolder(byte f);     // 0x17 Loop playback of specified folder
    void playShuffle();          // 0x18 Play all tracks in random order
    void playL(bool on);         // 0x19 Current track loop play
    void setDacHighImp(byte n);  // 0x1A Set DAC High Impedance (0=OFF, 1=ON)
    void setPowerOnVol(byte v);  // 0x1B Set Power-On Volume Level 

    void playSL(byte f, byte n); // Play Single loop play n file by name from f folder.
    
    void playF(byte f);          // Play all files in the f folder

    // Query commands
    void qPlaying();   // query the file currently playing
    void qStatus();    // query the status
    void qVolume();    // query the volume
    void qFTracks();   // query the number of track folders
    void qTTracks();   // query the total of tracks
    void qTFolders();  // query the number of folders

    void sleep();     // Send sleep command
    void wakeup();    // Send wake up command

    
  private:
    Stream* serial;
    String  sanswer();
    String  sbyte2hex(byte b);
    int     shex2int(char *s, int n);
    bool    _showDebugMessages;
    byte    ansbuf[15] = {0};      // Buffer for query command responses
};

#endif
