/*
* IoTCentral getting started app for Iomote X400 gateway
* 
* License: MIT license

* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

#include <iomoteClass.h>

#define Serial   SerialUSB 
#define SEND_DATA_TIME      1*60*1000// 1 packet each minute

#define SCOPE_ID        "YourScopeId"
#define DEVICE_ID       "YourDeviceId"
#define PRIMARY_KEY     "YourPrimaryKey"

char buff[3750]; // max message size is 3750 bytes
unsigned long sendTime = 0;
unsigned long cnt = 0;

void setup()
{
	Serial.write("X400 Getting Started!\r\n");
    //  This instruction initializes the board, it's mandatory for any sketch to correctly work with the X400 Cloud Operations!
    Iomote.begin("iot central getting started", 1,0,0); 

    // iotcentral (https://yourIotCentralAppName.azureiotcentral.com):
    Iomote.iotCentralInfo(SCOPE_ID, DEVICE_ID, PRIMARY_KEY);
}

void loop()
{
    unsigned long tim = millis();
    if((tim > (sendTime + SEND_DATA_TIME)) || (Iomote.buttonRead() == 0))
    {
        sendTime = millis();
        memset(buff, '\0', 3750);
        
        // Send current counter value to cloud
        sprintf(buff, "{\"cnt\":%lu}", cnt);

        //  With the sendData command the data is put in a queue, and wil be 
        //  sent to IoT Hub. 
        int8_t sendResult = Iomote.sendMessage(buff);
        if(sendResult == 0)
        {
            cnt++; // Increase counter value
            Serial.write("Data correctly enqueued and ready to be sent:\r\n");
            Serial.println(buff);
        }
        else
        {
            Serial.write("Unable to send data now! Result: ");
            Serial.println(sendResult);
        }
        delay(1000);
    }
}
