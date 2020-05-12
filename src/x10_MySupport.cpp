#include "x10_MySupport.h"



void x10_MySupport::init(int zcPin, int dataPin, int dataRecievePin)
{
    myHouse.init(zcPin, dataPin, dataRecievePin);
}


bool x10_MySupport::received()
{
  return myHouse.received();
}

void x10_MySupport::reset()
{
  myHouse.reset();
}

void x10_MySupport::process()
{
  myHouse.reset();

  if (myHouse.unitCode() == 1){
        byte cmndCode = myHouse.cmndCode();
        Serial.println("Unit code");
        Serial.println(cmndCode);
      }
}


void x10_MySupport::unit_on(int unit_id, int repeatTimes)
{

  switch (unit_id) {
    case 1:
        myHouse.write(HOUSE_A, UNIT_5, repeatTimes);               
        myHouse.write(HOUSE_A, ON, repeatTimes); 
        
      break;
    case 2:
        myHouse.write(HOUSE_A, UNIT_2, repeatTimes);               
        myHouse.write(HOUSE_A, ON, repeatTimes);     
      break;

    case 3:
        myHouse.write(HOUSE_A, UNIT_3, repeatTimes);               
        myHouse.write(HOUSE_A, ON, repeatTimes); 
      break;
  }

 
}

void x10_MySupport::unit_off(int unit_id, int repeatTimes)
{
    switch (unit_id) {
    case 1:
        myHouse.write(HOUSE_A, UNIT_5, repeatTimes);               
        myHouse.write(HOUSE_A, OFF, repeatTimes);
        
      break;
    case 2:
        myHouse.write(HOUSE_A, UNIT_2, repeatTimes);               
        myHouse.write(HOUSE_A, OFF, repeatTimes);     
      break;

    case 3:
        myHouse.write(HOUSE_A, UNIT_3, repeatTimes);               
        myHouse.write(HOUSE_A, OFF, repeatTimes); 
      break;
  }
 
}

void x10_MySupport::unit_dim(int unit_id, int times, int repeatTimes)
{
  switch (unit_id) {
    case 1:
        myHouse.write(HOUSE_A, UNIT_5, repeatTimes);               
        myHouse.write(HOUSE_A, DIM, times); 
      break;
    case 2:
        myHouse.write(HOUSE_A, UNIT_2, repeatTimes);               
        myHouse.write(HOUSE_A, DIM, times);      
      break;

    case 3:
        myHouse.write(HOUSE_A, UNIT_3, repeatTimes);               
        myHouse.write(HOUSE_A, DIM, times); 
      break;
  }
 
}

void x10_MySupport::unit_bright(int unit_id, int times, int repeatTimes)
{
   switch (unit_id) {
    case 1:
        myHouse.write(HOUSE_A, UNIT_5, repeatTimes);               
        myHouse.write(HOUSE_A, BRIGHT, times); 
      break;
    case 2:
        Serial.println("unit_bright 2");
        myHouse.write(HOUSE_A, UNIT_2, repeatTimes);               
        myHouse.write(HOUSE_A, BRIGHT, times);
        Serial.println("unit_bright 2 done");    
      break;

    case 3:
        myHouse.write(HOUSE_A, UNIT_3, repeatTimes);               
        myHouse.write(HOUSE_A, BRIGHT, times); 
      break;
  }
}

void x10_MySupport::getStatus(int repeatTimes)
{
        Serial.println("Sending STATUS_REQUEST");
        
        myHouse.write(HOUSE_A, UNIT_5, repeatTimes);               
        myHouse.write(HOUSE_A, STATUS_REQUEST, repeatTimes); 
}


void x10_MySupport::getHAIL(int repeatTimes)
{
        Serial.println("Sending HAIL_REQUEST");
  
        myHouse.write(HOUSE_A, UNIT_5, repeatTimes);               
        myHouse.write(HOUSE_A, HAIL_REQUEST, repeatTimes); 
}

void x10_MySupport::reset_all_lights(int repeatTimes)
{
    unit_dim(1, 0, repeatTimes);
    unit_dim(2, 0, repeatTimes);
    unit_dim(3, 0, repeatTimes);

    unit_off(1, repeatTimes);
    unit_off(2, repeatTimes);
    unit_off(3, repeatTimes);

}