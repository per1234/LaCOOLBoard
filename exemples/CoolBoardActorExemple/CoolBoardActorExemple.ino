#include"CoolBoardActor.h"

CoolBoardActor actor;

void setup()
{

	Serial.begin(115200);

	actor.begin();

}

void loop()
{
	actor.write(1);
 
	delay(500);
	
	actor.write(0);
	
	delay(500);

}
