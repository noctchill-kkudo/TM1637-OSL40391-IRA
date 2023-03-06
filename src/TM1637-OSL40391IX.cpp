extern "C" {
    #include <stdlib.h>
}
#include <chrono>
#include <thread>
#include <Arduino.h>
#include <TM1637-OSL40391IX.h>

//
//     DIG.1               DIG.2                     DIG.3               DIG.4        
//       A                                                                            
//    -------             -------                   -------      @      -------       
//   ||     ||           ||     ||                 ||     ||      D3   ||     ||      
// F ||     || B         ||     ||        @        ||     ||           ||     ||      
//   ||     ||           ||     ||         D1      ||     ||           ||     ||      
//    ---G---             -------                   -------             -------       
//   ||     ||           ||     ||                 ||     ||           ||     ||      
// E ||     || C         ||     ||        @        ||     ||           ||     ||      
//   ||     ||           ||     ||         D2      ||     ||           ||     ||      
//    ---D---     .       -------     .             -------     .       -------     . 
//                DP                                                                  
//


const int SettingMode = 0b01000100;
const int Address[6] = {
    0b11000000, //CH0 [0]
    0b11000001, //CH1 [1]
    0b11000010, //CH2 [2]
    0b11000011, //CH3 [3]
    0b11000100, //CH4 [4]
    0b11000101, //Ch5 [5]
};
const int Brightness[9] = {
    0b10000000, // Display OFF      [0]
    0b10001000, // Brightness lv.1  [1]
    0b10001001, // Brightness lv.2  [2]
    0b10001010, // Brightness lv.4  [3]
    0b10001011, // Brightness lv.8  [4]
    0b10001100, // Brightness lv.16 [5]
    0b10001101, // Brightness lv.12 [6]
    0b10001110, // Brightness lv.13 [7]
    0b10001111, // Brightness lv.14 [8]
    //*To clear the display, perform an exclusive or with 0b00001000.
};
const int number[10] = {
    0x3f, //0  [0]
    0x06, //1  [1]
    0x5b, //2  [2]
    0x4f, //3  [3]
    0x66, //4  [4]
    0x6d, //5  [5]
    0x7d, //6  [6]
    0x27, //7  [7]
    0x7f, //8  [8]
    0x6f  //9  [9]
};
const int alphabet[26] = {
    0x77, //A  [0]
    0x7c, //B  [1]
    0x39, //C  [2]
    0x5e, //D  [3]
    0x79, //E  [4]
    0x71, //F  [5]
    0x3D, //G  [6]
    0x74, //H  [7]
    0x04, //I  [8]
    0x0E, //J  [9]
    0x70, //K  [10]
    0x38, //L  [11]
    0x37, //M  [12]
    0x54, //N  [13]
    0x5C, //O  [14]
    0x73, //P  [15]
    0x67, //Q  [16]
    0x50, //R  [17]
    0x65, //S  [18]
    0x31, //T  [19]
    0x3E, //U  [20]
    0x1C, //V  [21]
    0x52, //W  [22]
    0x76, //X  [23]
    0x6E, //Y  [24]
    0x49, //Z  [25]
};
const int symbol[6] = {
    0x80, //.(DP)            [0]
    0x01, //.(D1)            [1]
    0x02, //.(D2)            [2]
    0x04, //.(D3)            [3]
    0x03, //:(colon)         [4]
    0x20, //-(minus)(hyphen) [5]
//*For D1 and D2, D3, specify a different address.
};


void TM1637OSL40391IRA::start()
{
    pinMode(mem_pinCLK, INPUT);
    std::this_thread::sleep_for(std::chrono::microseconds(100));
    pinMode(mem_pinDIO, INPUT);
    std::this_thread::sleep_for(std::chrono::microseconds(100));
    pinMode(mem_pinDIO, OUTPUT);
    std::this_thread::sleep_for(std::chrono::microseconds(100));
}

void TM1637OSL40391IRA::stop()
{
    pinMode(mem_pinDIO, OUTPUT);
    std::this_thread::sleep_for(std::chrono::microseconds(100));
    pinMode(mem_pinCLK, INPUT);
    std::this_thread::sleep_for(std::chrono::microseconds(100));
    pinMode(mem_pinDIO, INPUT);
    std::this_thread::sleep_for(std::chrono::microseconds(100));
}

bool TM1637OSL40391IRA::writeByte(int b)
{
    int data = b;
    for(int i=0; i<8; i++){
        pinMode(mem_pinCLK, OUTPUT);
        std::this_thread::sleep_for(std::chrono::microseconds(100));

        if(data & 0x01){
            pinMode(mem_pinDIO, INPUT);
        }else{
            pinMode(mem_pinDIO, OUTPUT);
        }
        std::this_thread::sleep_for(std::chrono::microseconds(100));

        pinMode(mem_pinCLK, INPUT);
        std::this_thread::sleep_for(std::chrono::microseconds(100));
        data = data >> 1;
    }
    pinMode(mem_pinCLK, OUTPUT);
    pinMode(mem_pinDIO, INPUT);
    std::this_thread::sleep_for(std::chrono::microseconds(100));

    pinMode(mem_pinCLK, INPUT);
    std::this_thread::sleep_for(std::chrono::microseconds(100));
    int ack = digitalRead(mem_pinDIO);
    if(ack==0)
        pinMode(mem_pinDIO, OUTPUT);
    
    std::this_thread::sleep_for(std::chrono::microseconds(100));
    pinMode(mem_pinCLK, OUTPUT);
    std::this_thread::sleep_for(std::chrono::microseconds(100));

    return ack;
}

//↓↓public

TM1637OSL40391IRA::TM1637OSL40391IRA(int pinCLK, int pinDIO)
{
    mem_pinCLK = pinCLK;
    mem_pinDIO = pinDIO;

    pinMode(mem_pinCLK, INPUT);
    pinMode(mem_pinDIO, INPUT);
    digitalWrite(mem_pinCLK, LOW);
    digitalWrite(mem_pinDIO, LOW);
    TM1637OSL40391IRA::clear();
}

void TM1637OSL40391IRA::display(int segment, int address, int brightness)
{
    //Command1
    start();
    writeByte(SettingMode);
    stop();

    //Command2
    start();
    writeByte(address);
    writeByte(segment);
    stop();

    //Command3
    start();
    writeByte(brightness);
    stop();
}

void TM1637OSL40391IRA::clear()
{
    for(int k=0; k<5; k++){
       start();
       writeByte(Address[0]|k);
       writeByte(0x00);
       stop();
    }
}
