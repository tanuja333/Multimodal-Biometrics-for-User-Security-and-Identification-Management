//module-r305
//At power on, it takes about 500ms for initialization. During this period, the Module can’t accept 
//commands for upper computer. 
//supply -DC 3.6V-6.0V
//here only reads the fingerprint
//please enroll using sys software
//it has 3 steps to read & check finger print
//step 1-collect finger image
//step 2-To generate character filefrom image
//step 3- search finger library for for previously stored finger print
//use softuart to see the output
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 11); // RX, TX
LiquidCrystal lcd(3, 4, 5, 6, 7, 8);//LCD RS-3,En-4,D4-5,D5- 6,D6-7,D7-8
char a,e,x;
void setup()
{
   mySerial.begin(57600);
mySerial.println("haii?"); //checks the softuart
  Serial.begin(57600);
  delay(500);
  collectfingerimage();
  generatecharacterfilefromimage();
   searchfingerlibrary();  
}

void loop()
{
}

void searchfingerlibrary(void)
{
Serial.write(0XEF);//header --1st byte of header (total 2 bytes)
Serial.write(0X01);//header --2nd byte of header (total 2 bytes)
Serial.write(0XFF);//address--1st byte of address (total 4 bytes)
Serial.write(0XFF);//address--2
Serial.write(0XFF);//address--3
Serial.write(0XFF);//address--4
Serial.write(0X01);//Package identifier--01=Commands ,07=Acknowledgement (total 1 byte)
Serial.write(0);// --1st byte of Package length  (total 2 byte)
Serial.write(8);// --2nd byte of Package length  (total 2 byte)
Serial.write(4);////instruction code to search finger library (total 1 byte)
Serial.write(1);//buffer id number where the image is stored (total 1 byte)
Serial.write(0);//Start Page of searching(searching start address)(total 2 bytes)
Serial.write(0);//(total 2 bytes)
Serial.write(0);//searching numbers(total 2 bytes)
Serial.write(10);//searching numbers(total 2 bytes)
Serial.write(0);//check sum(total 2 bytes)
Serial.write(24);//check sum(total 2 bytes)

while(!(Serial.available()>0)); 
while(a != 7){  //wait until receiving Acknowledgement ..ie 0x7
a=Serial.read(); 
 mySerial.write(a);
}

do {  //wait until receiving package length ..ie 0x7
a=Serial.read(); 
 mySerial.write(a);
}while(a != 7);  

if(Serial.available()>0){ //reads the confirmation code
a=Serial.read(); 
 mySerial.write(a);
}
 if(a==0) mySerial.println("ok      ");// Confirmation code=00H: found the matching finer; 
 else if(a==1) mySerial.println("error 1    ");//Confirmation code=01H: error when receiving package; 
 else if(a==9) mySerial.println("no match ");//Confirmation code=09H: No matching in the library (both the PageID and 
                                           // matching score are 0); 
else { mySerial.println("error 0  ");}
if(a==0)
{
if(Serial.available()>0){e=Serial.read();} // avoides  the 1st byte of matching templates location

if(Serial.available()>0){ // reads  the 2nd byte of matching templates location
a=Serial.read(); }
mySerial.println("id="); 
 if(a==4) {mySerial.println("one");x=1;}
 else if(a==5){ mySerial.println("two");x=2;}
 else mySerial.println("error");  
mySerial.write(a);
}
if(Serial.available()>0){a=Serial.read(); mySerial.write(a); }// avoides MatchScore  (total 2 bytes)
if(Serial.available()>0){a=Serial.read(); mySerial.write(a); }// avoides MatchScore  (total 2 bytes)
if(Serial.available()>0){a=Serial.read(); mySerial.write(a); }//avoides checksum
if(Serial.available()>0){a=Serial.read(); mySerial.write(a); }//avoides checksum
}


void generatecharacterfilefromimage(void)
{
 do{   
 // delay(500);
      Serial.write(0XEF);//header --1st byte of header (total 2 bytes)
      Serial.write(0X01);//header --2nd byte of header (total 2 bytes)
      Serial.write(0XFF);//address--1st byte of address (total 4 bytes)
      Serial.write(0XFF);//address--2
      Serial.write(0XFF);//address--3
      Serial.write(0XFF);//address--4
   Serial.write(0X01);//Package identifier--01=Commands ,07=Acknowledgement (total 1 byte)
   Serial.write(0X00);// --1st byte of Package length  (total 2 byte)
   Serial.write(0X04);//Package length
   Serial.write(0X02);//instruction code to generate character file from image
   Serial.write(0X01);//BufferID 1  (total 1 byte)(to generate character file fromthe original finger image in ImageBuffer and 
                       //store the file in CharBuffer1 or CharBuffer2.
                       //BufferID of CharBuffer1 and CharBuffer2 are 1h and 2h respectively
   Serial.write(0X00);//check sum(total 2 bytes)
   Serial.write(0X08);//check sum(total 2 bytes)
      
 while(!(Serial.available()>0));//waiting for reply
while(a != 3){ //wait until receiving package length ..ie 0x3
a=Serial.read(); 
 mySerial.write(a);
};
if(Serial.available()>0){ //reads the confirmation code
a=Serial.read(); 
 mySerial.write(a);
}
if(a==0) mySerial.println("ok      ");//Confirmation code=00H: generate character file complete; 
 else if(a==1) mySerial.println("error 1    ");//Confirmation code=01H: error when receiving package; 
 else if(a==6) mySerial.println("er 2");//Confirmation code=06H: fail to generate character file due to the over-disorderly 
                                        //fingerprint image;  
 else if(a==7) mySerial.println("error 3  ");//Confirmation code=07H: fail to generate character file due to lackness of character 
                                             //point or over-smallness of fingerprint image; 
                                            
  else if(a==0x15) mySerial.println("error 4  ");//Confirmation code=15H: fail to generate the image
                                              // for the lackness of valid primary image;  
 else { mySerial.println("error 5 ");}
e=Serial.read(); //avoides checksum
e=Serial.read(); //avoides checksum
  }while(a!=0); //waits until the generation character file is completed;     
delay(1000);
}

void collectfingerimage(void)  //page number 12 in manual of r305
{
  do{ 
      Serial.write(0XEF);//header --1st byte of header (total 2 bytes)
      Serial.write(0X01);//header --2nd byte of header (total 2 bytes)
      Serial.write(0XFF);//address--1st byte of address (total 4 bytes)
      Serial.write(0XFF);//address--2
      Serial.write(0XFF);//address--3
      Serial.write(0XFF);//address--4
      Serial.write(0X01);//Package identifier--01=Commands ,07=Acknowledgement (total 1 byte)
      Serial.write(0X00);// --1st byte of Package length  (total 2 byte)
      Serial.write(0X03);//Package length
      Serial.write(0X01);//instruction code to collect image
      Serial.write(0X00);//check sum(total 2 bytes)
      Serial.write(0X05);//check sum(total 2 bytes)
  //delay(500);
while(!(Serial.available()>0));//waiting for reply
while(a != 3){ //wait until receiving package length ..ie 0x3
a=Serial.read(); 
 mySerial.write(a);
}
if(Serial.available()>0){  //reads the confirmation code
a=Serial.read(); 
 mySerial.write(a);
}
if(a==0) mySerial.println("ok      "); //Confirmation code=00H: finger collection successs; 
 else if(a==1) mySerial.println("error 4    "); //Confirmation code=01H: error when receiving packag
 else if(a==2) mySerial.println("no finger");//Confirmation code=02H: can’t detect finger; 
 else if(a==3) mySerial.println("error 2  ");//Confirmation code=03H: fail to collect finger; 
 else { mySerial.println("error 0  ");}
e=Serial.read(); //avoides checksum
e=Serial.read();//avoides checksum
  }while(a!=0);//waits until to get any finger
}


