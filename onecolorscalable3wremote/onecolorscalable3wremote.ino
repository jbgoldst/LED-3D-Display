#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"
//^ for remote control

RF24 radio(A4,A5);
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };
typedef enum { role_ping_out = 1, role_pong_back } role_e;
const char* role_friendly_name[] = { "invalid", "Ping out", "Pong back"};
role_e role = role_pong_back;

const int joyH = A0;
const int joyV = A1;
int hor;
int ver;
long message;

unsigned long keypressed = 0;
int numstodisp[10];
int curscreen = 0;

// the setup function runs once when you press reset or power the board
const int height = 9;
const int width = 70;
boolean fg[height*width];//size is (LED height)*(width of the image)*(3 for colors)
const int totreps = 1;//total number of rotations for any picture
int curcolor = 2;
unsigned long curtime = 0;//curtime in microseconds
unsigned long revtime = 70250;//guess the right time, think it is 81435//66650//72311//70500
/*
rotnum%3==0    red
rotnum%3==1    green
rotnum%3==2    blue
*/
//red = 11;
//green = 12;
//blue = 13;



int blockOffset = 0;

void setup() {
  // initialize digital pin 13 as an output.
  for(int a = 2; a < 11; a++)
  {
     pinMode(a, OUTPUT);
  }
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  //initialize lines

  for(int ccol = 0; ccol < width; ccol++)
  {
    for(int x = 0; x < height; x++)
    {
      fg[(ccol*height)+x]=false;
    }      
  }
    //
  // Print preamble
  //
  
  pinMode(joyH, INPUT);
  pinMode(joyV, INPUT);
  
  Serial.begin(57600);
  printf_begin();
  printf("\n\rRF24/examples/GettingStarted/\n\r");
  printf("ROLE: %s\n\r",role_friendly_name[role]);
  
  //
  // Setup and configure rf radio
  //

  radio.begin();

  // optionally, increase the delay between retries & # of retries
  radio.setRetries(15,15);

  // optionally, reduce the payload size.  seems to
  // improve reliability
  //radio.setPayloadSize(8);

  //
  // Open pipes to other nodes for communication
  //

  // This simple sketch opens two pipes for these two nodes to communicate
  // back and forth.
  // Open 'our' pipe for writing
  // Open the 'other' pipe for reading, in position #1 (we can have up to 5 pipes open for reading)

  //if ( role == role_ping_out )
  {
    //radio.openWritingPipe(pipes[0]);
    //radio.openReadingPipe(1,pipes[1]);
  }
  //else
  {
    radio.openWritingPipe(pipes[1]);
    radio.openReadingPipe(1,pipes[0]);
  }

  //
  // Start listening
  //

  radio.startListening();

  //
  // Dump the configuration of the rf unit for debugging
  //

  radio.printDetails();
  for (int i = 0; i < 10; i++)
  {
    numstodisp[i]=i;
  }
}

// the loop function runs over and over again forever
void loop() {
  getNewPic();
  
  /*for(int x = 0; x < height*width; x++)
  {
    fg[x]=true;
  }*/
  
  for(int x = 0; x < totreps; x++)
  {//does entire process 4 times before frame changes
    for(int c = 0; c < width; c++)
    {//for 12 columns on each
      showpixforcol(c);
    }
    /*for(int c = 0; c < width; c++)
    {//for 12 columns on each
      showpixforcol(c);
    }*/
    delayMicroseconds(revtime/2);
    delayMicroseconds(revtime-((revtime/(2*width))*2*width));
  }
  //revtime-=10;
}

void showpixforcol(int curcolumn)
{
  int pix = 0;
  switch (curcolor) {
      case 0: digitalWrite(A1, HIGH); break;
      case 1: digitalWrite(A2, HIGH); break;
      case 2: digitalWrite(A3, HIGH); break;
  }
  
  delayMicroseconds(1);
  for(int x = 0; x < height; x++)
  {
    pix = (curcolumn*height)+x;
    if(fg[pix]==true)
      digitalWrite(2+x,HIGH);
    else
      digitalWrite(2+x,LOW);
      //delayMicroseconds(1);
  }
  delayMicroseconds((revtime/(width))-2);
  if(radio.available())
  {
    radio.read(&keypressed, sizeof(unsigned long));
    printf("Got : %lu",keypressed);
  }
  //delayMicroseconds((revtime/(width))-2);
//    delayMicroseconds((revtime/(2*width))-2*height-1);
  for(int x = 0; x < height; x++)
  {
    pix = (curcolumn*height)+x;
    digitalWrite(2+x,LOW);
    //delayMicroseconds(1);
  }
  digitalWrite(11+curcolor,LOW);
  delayMicroseconds(1);
}

void getNewPic()
{
  /*for(int ccol = 0; ccol < width; ccol++)
  {
    for(int x = 0; x < height; x++)
    {
      fg[(ccol*height)+x]=true;//make all red
    }      
  }
  curcolor=(curcolor+1)%3;*/
  clearScreen();
  //rectBlock(2,8,2);
  //rectBlock(2,8,1);
  //rectBlock(2,8,0);
  //addBlankCol(2);
  unsigned long tempdevnull=0;
  
  if(keypressed==1)
  {
    curcolor=(curcolor+1)%3;
    keypressed=0;
  }
  if(keypressed==3)
  {
    curcolor=(curcolor-1)%3;
    keypressed=0;
  }
  if(keypressed==2)
  {
    printf("got up\n");
    numstodisp[9]=numstodisp[9]+1;
    int tnumasdf = 9;
    while(numstodisp[tnumasdf]==10 && tnumasdf>0)
    {
      numstodisp[tnumasdf]=0;
      numstodisp[tnumasdf-1]=numstodisp[tnumasdf-1]+1;
      tnumasdf--;
    }
    keypressed=0;
  }
  if(keypressed==4)
  {
    printf("got down\n");
    numstodisp[9]=numstodisp[9]-1;
    int tnumasdf = 9;
    while(numstodisp[tnumasdf]==-1 && tnumasdf>0)
    {
      numstodisp[tnumasdf]=9;
      numstodisp[tnumasdf-1]=numstodisp[tnumasdf-1]-1;
      tnumasdf--;
    }
    keypressed=0;
  }
  for (int i = 0; i < 10; i++)
  {
    addOneInt(i);
    //printf("%i",i);
  }
  //printf("%i",curcolor);
  //printf("\n");
  //goes in reverse
  //for(int i = 0; i < height*width/4; i++)
  //  fg[i]=true;
  
  while(radio.available())
  {
    radio.read(&tempdevnull, sizeof(unsigned long));
  }
}

void addBlankCol (int blankCols) {
    int col = 0;
    for (; col < blankCols; col++) {
        if (blockOffset + col >= width) {break;}
        for (int row = 0; row < height; row++) {
            fg[(col + blockOffset) * height + row] = false;
           }
    }
    
    blockOffset += col;
}

void clearScreen () {
    blockOffset = 0;
    for(int x = 0; x < height*width; x++)
    {
        fg[x]=false;
    }
}
void rectBlock (int rowOffset, int rectWidth, int color) {
    if (blockOffset + rectWidth >= width) {return;}
    for (int col = 0; col < rectWidth; col++) {
        for (int row = 0; row < height; row++) {
           fg[((col + blockOffset) * height) + row] = (row >= rowOffset) ? true : false;
        }
    }
    
    curcolor = color;
    blockOffset += rectWidth;
}

void rectBlock () {
    rectBlock(2, 15, 1);
}

void triangleBlock (int rowOffset, int triWidth, int color) {
    if (blockOffset + triWidth >= width) {return;}
    for (int col = 0; col < triWidth / 2 + triWidth % 2; col++) {
        for (int row = 0; row < height; row++) {
            if (row < rowOffset) fg[(col * height) + row] = false;
            else {fg[((col + blockOffset) * height) + row] = ((row - rowOffset) <= col) ? true : false;}
        }
    }
    for (int col = 0; col < triWidth / 2; col++) {
        for (int row = 0; row < height; row++) {
            if (row < rowOffset) {fg[(((col + blockOffset) + triWidth / 2 + triWidth % 2) * height) + row] = false;}
            else {fg[(((col + blockOffset) + triWidth / 2 + triWidth % 2) * height) + row] = ((row - rowOffset) > col) ? true : false;}
        }
    }
    
    curcolor = color;
    blockOffset += triWidth;
}

void triangleBlock () {
    triangleBlock (0, 4, 0);
}

void addOneInt(int i)//i'm only taking your last digit
{
    if (blockOffset + 5 >= width) {return;}
    
    switch(i % 10) {
    case 0: {boolean inttoarr[] = {false,false,true,true,true,true,true,false,false,
            false,true,false,false,false,true,false,true,false,
            false,true,false,false,true,false,false,true,false,
            false,true,false,true,false,false,false,true,false,
            false,false,true,true,true,true,true,false,false};
              for(int x = 0; x < sizeof(inttoarr); x++)
        {
            fg[blockOffset*height+(sizeof(inttoarr)-x)]=inttoarr[x];
        }}
        break;
    case 1:{
        boolean inttoarr[] = {false,false,false,false,false,false,false,false,false,
                false,false,false,false,false,false,false,false,false,
                        false,true,true,true,true,true,true,true,false,
                                false,false,true,false,false,false,false,false,false,
                                false,false,false,true,false,false,false,false,false};
        
          for(int x = 0; x < sizeof(inttoarr); x++)
        {
            fg[blockOffset*height+(sizeof(inttoarr)-x)]=inttoarr[x];
        }}
        break;
    case 2:{
        boolean inttoarr[] = {   
        false,false,true,true,false,false,false,true,false,
        false,true,false,false,true,false,false,true,false,
        false,true,false,false,false,true,false,true,false,
        false,true,false,false,false,false,true,true,false,
        false,false,true,false,false,false,false,true,false

        };
          for(int x = 0; x < sizeof(inttoarr); x++)
        {
            fg[blockOffset*height+(sizeof(inttoarr)-x)]=inttoarr[x];
        }}
        break;
    case 3:{  boolean inttoarr[]= {false,false,true,true,false,true,true,false,false,
                false,true,false,false,true,false,false,true,false,
                false,true,false,false,true,false,false,true,false,
                false,true,false,false,false,false,false,true,false,
                false,false,true,false,false,false,true,false,false
        };
          for(int x = 0; x < sizeof(inttoarr); x++)
        {
            fg[blockOffset*height+(sizeof(inttoarr)-x)]=inttoarr[x];
        }}
        break;
    case 4: { boolean inttoarr[]= {        false,false,false,false,false,true,false,false,false,
                false,false,false,true,true,true,false,false,false,
                false,false,false,false,false,true,true,true,false,
                false,false,false,false,false,true,false,false,false,
        false,true,true,true,true,true,false,false,false,
        };
          for(int x = 0; x < sizeof(inttoarr); x++)
        {
            fg[blockOffset*height+(sizeof(inttoarr)-x)]=inttoarr[x];
        }}
        break;
    case 5:{ boolean inttoarr[]= {
        
               false,true,false,false,false,true,true,false,false,
                                false,true,false,false,true,false,false,true,false,
                                false,true,false,false,true,false,false,true,false,
                                false,true,false,false,true,false,false,true,false,
                false,true,true,true,true,false,true,false,false
        };
          for(int x = 0; x < sizeof(inttoarr); x++)
        {
            fg[blockOffset*height+(sizeof(inttoarr)-x)]=inttoarr[x];
        }}
        break;
    case 6:{ boolean inttoarr[] = {
        false,false,false,false,false,true,true,false,false,
              false,true,false,false,true,false,false,true,false,
              false,false,true,false,true,false,false,true,false,
              false,false,false,true,true,false,false,true,false,
              false,false,false,false,true,true,true,false,false
    };
          for(int x = 0; x < sizeof(inttoarr); x++)
        {
            fg[blockOffset*height+(sizeof(inttoarr)-x)]=inttoarr[x];
        }}
        break;
    case 7:{ boolean inttoarr[]= {
        false,true,true,true,false,false,false,false,false,
                false,true,false,false,true,false,false,false,false,
                false,true,false,false,false,true,false,false,false,
                false,true,false,false,false,false,true,false,false,
                false,true,false,false,false,false,false,true,false
        };
          for(int x = 0; x < sizeof(inttoarr); x++)
        {
            fg[blockOffset*height+(sizeof(inttoarr)-x)]=inttoarr[x];
        }}
        break;   
    case 8:{ boolean inttoarr[] = {
        false,false,true,true,false,true,true,false,false,
                false,true,false,false,true,false,false,true,false,
                false,true,false,false,true,false,false,true,false,
                false,true,false,false,true,false,false,true,false,    
            false,false,true,true,false,true,true,false,false
    };
          for(int x = 0; x < sizeof(inttoarr); x++)
        {
            fg[blockOffset*height+(sizeof(inttoarr)-x)]=inttoarr[x];
        }}
        break;
    case 9:{ boolean inttoarr[] = {
        false,false,true,true,true,false,false,false,false,
              false,true,false,false,true,true,false,false,false,
              false,true,false,false,true,false,true,false,false,
              false,true,false,false,true,false,false,true,false,
            false,false,true,true,false,false,false,false,false};
          for(int x = 0; x < sizeof(inttoarr); x++)
        {
            fg[blockOffset*height+(sizeof(inttoarr)-x)]=inttoarr[x];
        }}
        break;
    default:{ boolean inttoarr[] = {
      false,false,false,false,false,false,false,false,false,
      false,false,false,false,false,false,false,false,false,
      false,false,false,false,false,false,false,false,false,
      false,false,false,false,false,false,false,false,false,
      false,false,false,false,false,false,false,false,false};
        for(int x = 0; x < sizeof(inttoarr); x++)
        {
            fg[blockOffset*height+(sizeof(inttoarr)-x)]=inttoarr[x];
        }}
      break;
    }
       
    blockOffset += 5; 
    addBlankCol(2);   
    
}
