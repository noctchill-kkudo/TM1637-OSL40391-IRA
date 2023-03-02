#ifndef __TM1637OSL40391IX__
#define __TM1637OSL40391IX__

class TM1637OSL40391IRA {

public:
   TM1637OSL40391IRA(int pinCLK, int pinDIO);
   void display(int segment, int address, int Brightness);
   void clear();

protected:
   void start();
   void stop();
   bool writeByte(int b);

private:
   int mem_pinCLK;
   int mem_pinDIO;
};

#endif // __TM1637OSL40391IX__