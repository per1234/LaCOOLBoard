/**
*	Copyright (c) 2018 La Cool Co SAS
*
*	Permission is hereby granted, free of charge, to any person obtaining a copy
*	of this software and associated documentation files (the "Software"), to deal
*	in the Software without restriction, including without limitation the rights
*	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*	copies of the Software, and to permit persons to whom the Software is
*	furnished to do so, subject to the following conditions:
*
*	The above copyright notice and this permission notice shall be included in all
*	copies or substantial portions of the Software.
*
*	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*	SOFTWARE.
*
*/

/**
*	CoolBoardExample	
*
*	This example shows typical use
*	of the CoolBoard.
*		
*	Save this example in another location
*	in order to safely modify the configuration files
*	in the data folder.
*
*/

#include<CoolBoardSensors.h>

CoolBoardSensors sensors;

void setup()
{
	

	Serial.begin(115200);

	sensors.allActive();

	sensors.begin();


}

void loop()
{
	Serial.println("sensors data is ");
	Serial.println(sensors.read());
	delay(1000);
		
	Serial.println("individual readings");
	
	Serial.print("Vbat : ");Serial.println(sensors.readVBat());
	
	Serial.print("Earth Moisture : ");Serial.println(sensors.readMoisture());
	
	Serial.print("Visible light : ");Serial.println(sensors.lightSensor.ReadVisible()) ;
	
	Serial.print("InfraRed light : ");Serial.println(sensors.lightSensor.ReadIR() );
		
	Serial.print("Ultraviolet light: ");Serial.println(sensors.lightSensor.ReadUV()/100 );
	
	Serial.print("Pressure : ");Serial.println(sensors.envSensor.readFloatPressure());
	
	Serial.print("Air Humidity");Serial.println(sensors.envSensor.readFloatHumidity()) ;
	
	Serial.print("Temperature :");Serial.print(sensors.envSensor.readTempC());Serial.println("°C");
	
	delay(1000);


}
