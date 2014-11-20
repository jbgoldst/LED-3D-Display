// the setup function runs once when you press reset or power the board
const int height = 9;
const int width = 16;
boolean fg[height*width*3];//size is (LED height)*(width of the image)*(3 for colors)
const int totreps = 4;//total number of rotations for any picture
int rotnum = 0;
unsigned long curtime = 0;//curtime in microseconds
unsigned long revtime = 81435;//guess the right time, think it is 81435
/*
rotnum%3==0    red
rotnum%3==1    green
rotnum%3==2    blue
*/
//red = 11;
//green = 12;
//blue = 13;

void setup() {
  // initialize digital pin 13 as an output.
  for(int a = 2; a < 14; a++)
  {
     pinMode(a, OUTPUT);
  }
  pinMode(13, OUTPUT);
  //initialize lines
  for(int ccolor = 0; ccolor < 3; ccolor++)
  {
    for(int ccol = 0; ccol < width; ccol++)
    {
      if(ccol%3==ccolor)
      {
        for(int x = 0; x < height; x++)
        {
          fg[(ccolor*height*width)+(ccol*height)+x]=true;
        }
      }
      else
      {
        for(int x = 0; x < height; x++)
        {
          fg[(ccolor*height*width)+(ccol*height)+x]=false;
        }
      }
    }
  /*
  for(int ccolor = 0; ccolor < 3; ccolor++)
  {
    for(int ccol = 0; ccol < width; ccol++)
    {
      if(ccol%3==ccolor)
      {
        for(int x = 0; x < height; x++)
        {
          fg[(ccolor*height*width)+(ccol*height)+x]=true;
        }
      }
      else
      {
        for(int x = 0; x < height; x++)
        {
          fg[(ccolor*height*width)+(ccol*height)+x]=false;
        }
      }
    }
  }*/
  //end of loops
}

// the loop function runs over and over again forever
void loop() {
  /*
  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);              // wait for a second
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);              // wait for a second
  */
  for(int x = 0; x < totreps; x++)
  {//does entire process 4 times before frame changes
    for(int ccolor = 0; ccolor < 1; ccolor++)
    {//3 different colors
    //do this twice for front and back
      for(int c = 0; c < width; c++)
      {//for 12 columns on each
        showpixforcol(c, ccolor);
      }
      for(int c = 0; c < width; c++)
      {//for 12 columns on each
        showpixforcol(c, ccolor);
      }
    }
  }
  revtime+=10;
}

void showpixforcol(int curcolumn, int curcolor)
//@requires curframe < width;
{
  int pix = 0;
  digitalWrite(11+curcolor,HIGH);
  delayMicroseconds(1);
  for(int x = 0; x < height; x++)
  {
    pix = (curcolor*height*width)+(curcolumn*height)+x;
    if(fg[pix]==true)
      digitalWrite(2+x,HIGH);
    else
      digitalWrite(2+x,LOW);
      //delayMicroseconds(1);
  }
  delayMicroseconds((revtime/(2*width))-2*height-1);
  for(int x = 0; x < height; x++)
  {
    pix = (curcolor*height*width)+(curcolumn*height)+x;
    digitalWrite(2+x,LOW);
    //delayMicroseconds(1);
  }
  digitalWrite(11+curcolor,LOW);
  delayMicroseconds(1);
}
