#ifndef __TM1637OSL40391IX__
#define __TM1637OSL40391IX__

class TM1637OSL40391IRA {

public:
   TM1637OSL40391IRA(int pinCLK, int pinDIO);
   void display(int segment, int address, int Brightness);
   void clear();
   
   int SettingMode = 0b01000100;
   int Address[6] = {
      0b11000000, //CH0 [0]
      0b11000001, //CH1 [1]
      0b11000010, //CH2 [2]
      0b11000011, //CH3 [3]
      0b11000100, //CH4 [4]
      0b11000101, //Ch5 [5]
   };
   int Brightness[9] = {
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
   int number[10] = {
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
   int alphabet[26] = {
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
   int symbol[6] = {
      0x80, //.(DP)            [0]
      0x01, //.(D1)            [1]
      0x02, //.(D2)            [2]
      0x04, //.(D3)            [3]
      0x03, //:(colon)         [4]
      0x20, //-(minus)(hyphen) [5]
      //*For D1 and D2, D3, specify a different address.
   };

protected:
   void start();
   void stop();
   bool writeByte(int b);

private:
   int mem_pinCLK;
   int mem_pinDIO;
};

#endif // __TM1637OSL40391IX__