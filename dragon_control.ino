//use 220 Ohm resistors to connect potentiometer (instead of using jumper wires (because resestance of jumper wires isn't definite and dependes on orientation of wire) 
//fit potentiometer properly in breadboard and check that values outputted by it doesn't get changed just by touching it (This is imp to do in this project because analog output of potentiometer
//should be as consistant as possible 

int anin=0;
int var=0;
int pvar=0;
int a=4;//it is used to act as error range (i.e any change in [reading-a , reading+a] is rejected

int b_x=5;
int b_y=6;
int b_z=7;
//int x=0,y=0,z=0;


enum coord {b_null=0,x=1,y=2,z=3};
coord obj=b_null;
int count=0;
void setup()
{
    Serial.begin(9600);
    while(!Serial);

    pinMode(b_x,INPUT);
    pinMode(b_y,INPUT);
    pinMode(b_z,INPUT);

    
  
}

void loop()
{

  
  if(digitalRead(b_x)==HIGH)
  {
    obj=x;
  }

   if(digitalRead(b_y)==HIGH)
  {
    obj=y;
  }

   if(digitalRead(b_z)==HIGH)
  {
    obj=z;
  }
  

  pvar=var;
  var=analogRead(anin);
  if((var>pvar-a && var<pvar+a) || var==0 )
  {
    var=pvar;


    switch(obj)
    {
      case x:
      Serial.print(String(var));
      Serial.print("x");
      Serial.print("a");//its imp to send a terminating char
      break;

      case y:
      Serial.print(String(var));
      Serial.print("y");
      Serial.print("a");//its imp to send a terminating char
      break;

      case z:
      Serial.print(String(var));
      Serial.print("z");
      Serial.print("a");//its imp to send a terminating char
      break;

      case b_null:
      Serial.print(String(var));
      Serial.print("y");
      Serial.print("a");//its imp to send a terminating char
      break;
    }
  }


  
  delay(1);

  while((in_signal=Serial.read())!=(-1))
//void setup() and all global things on arduino are executed once , then void loop() is executed as long as arduino is powered on
// If arduino is not powered off and c++ program is again executed
 //then c++ program will be fresh but arduino program will not start afreseh due to which values of global var like "obj" will not be default but will be same as last modified val
 //eg if last selected axis is z then next time c++ prog is started (without restarting arduino)  it will stay z( and not b_null as intended)
 //A solution to this prob is to send signals from C++ program to Arduino's prog about weather formar is going to keep running('y') or is about to be closed ('n')
 //In latter case 'n' will be sent to Arduino detecting which Arduino explictly sets imp global var to their default values for use in next session
  {
    if(in_signal=='n')
    {
      
      obj=b_null; 
      
    }
    
  }

  delay(1);

  
  Serial.end();



}

