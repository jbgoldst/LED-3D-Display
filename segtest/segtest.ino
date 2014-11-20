// the setup function runs once when you press reset or power the board
const int height = 9;
const int width = 4;
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
  
  
  for(int x = 0; x < height*width*3; x++)
  {
    fg[x]=false;
  }
  for(int x = 0; x < height/2; x++)
  {
    fg[x]=true;
  }
  for(int x = height*width+height/2; x < height*width+2*height; x++)
  {
    fg[x]=true;
  }
  for(int x = height; x < 2*height; x++)
  {
    fg[x]=true;
  }
  for(int x = 2*height*width+2*height; x < 2*height*width+3*height; x++)
  {
    fg[x]=true;
  }
  
  //end of loops
}

// the loop function runs over and over again forever
void loop() {
  for(int x = 0; x < totreps; x++)
  {//does entire process 4 times before frame changes
    for(int ccolor = 0; ccolor < 3; ccolor++)
    {//3 different colors
    //do this twice for front and back
      for(int c = 0; c < width; c++)
      {//for 12 columns on each
        showpixforcol(c, ccolor);
      }
      for(int c = 0; c < width; c++)
      {//for 12 columns on each
        delayMicroseconds(revtime/(2*width));
      }
    }
  }
  //revtime+=10;
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
    {
      digitalWrite(2+x,HIGH);
      delayMicroseconds(1);
    }
    else
    {
      digitalWrite(2+x,LOW);
      delayMicroseconds(1);
    }
  }
  delayMicroseconds((revtime/(2*width))-(2*height)-2);
  for(int x = 0; x < height; x++)
  {
    pix = (curcolor*height*width)+(curcolumn*height)+x;
    digitalWrite(2+x,LOW);
    delayMicroseconds(1);
  }
  digitalWrite(11+curcolor,LOW);
  delayMicroseconds(1);
}
