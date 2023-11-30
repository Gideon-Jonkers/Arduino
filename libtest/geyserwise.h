
unsigned long lastStamp = micros();
int buffCntr = 0;
unsigned long m_bitBuffer[33];
bool m_shouldCheck = false;
bool endofMsg = false;
ICACHE_RAM_ATTR void stamp()
{
    //Serial.println("Does interrupt");
    unsigned long duration = micros() - lastStamp;
    lastStamp += duration;
    m_bitBuffer[buffCntr++] = duration;
    if (buffCntr >= 33)
        buffCntr = 0;
    m_shouldCheck = true;
    endofMsg = false;
}

unsigned long lastStamp2 = micros();
int buffCntr2 = 0;
unsigned long m_bitBuffer2[33];
bool m_shouldCheck2 = false;
bool endofMsg2 = false;
ICACHE_RAM_ATTR void stamp2()
{
    unsigned long duration = micros() - lastStamp2;
    lastStamp2 += duration;
    m_bitBuffer2[buffCntr2++] = duration;
    if (buffCntr2 >= 33)
        buffCntr2 = 0;
    m_shouldCheck2 = true;
    endofMsg2 = false;
}

struct GeyserPacket
{
  uint8_t data[4] = {0};

  void operator=(GeyserPacket other)
  {
    for(int i = 0; i< 4;i++)
    {
      this->data[i] = other.data[i];
    }
  }
};

class GeyserWiseComm
{
private:
    static int instanceCntr;
    int m_readpin,m_sendpin;
    unsigned long m_tmpBuff[33];
    int instanceId = 0;
    unsigned long lastMsg = 0;
    GeyserPacket lastPack;
public:
    GeyserWiseComm(int receivePin, int transmitPin)
    {
        m_readpin = receivePin;
        m_sendpin = transmitPin;
        instanceId = instanceCntr++;
    }

    unsigned long GetLastMsg()
    {
        return lastMsg;
    }

    GeyserPacket getLastPack()
    {
        return lastPack;
    }

    void checkBuffer()
    {
        noInterrupts();
        unsigned long m_tmpBuff[33] = {0};
        for(int i = 0; i < 33; i++)
        {
            if(instanceId == 0)
                m_tmpBuff[i] = m_bitBuffer[i];
            else
                m_tmpBuff[i] = m_bitBuffer2[i];
        }
        interrupts();
        //Serial.println("Does check");
        for(int i = 0;i < 33;i++)
        {
            if(m_tmpBuff[i] >= 6300 && m_tmpBuff[i] <= 6550)
            {
                GeyserPacket packet;
                unsigned long msg = 0;
                bool goodMsg = true;
                for(int h = 0; h < 4; h++)
                {
                    uint8_t rch = 0;
                    for(int b = 0;b < 8; b++)
                    {
                      int index = i + 1 +(h*8 + b);
                      if(index >= 33)
                          index -= 33;
                      unsigned long pduration = m_tmpBuff[index];
                      if(pduration >= 790 && pduration < 1210){}
                      else if(pduration >= 1210 && pduration < 1750)
                          rch |= (1<<b);
                      else
                      {
                          goodMsg = false;
                          break;
                      }
                    }
                    if(!goodMsg)
                        break;
                    packet.data[h] = rch;
                    msg |= (rch << (h * 8));
                }
                if(goodMsg)
                {
                  lastMsg = msg;
                    lastPack = packet;
                }
                return;
            }
        }
    }

    void trueDelay(unsigned long micr)
    {
      unsigned long startD = micros();
      while(micros() - startD < micr){}
    }

    void send(GeyserPacket pack, int trep)
    {
        //detachInterrupt(digitalPinToInterrupt(m_readpin));
        noInterrupts();
        for(int t = 0; t < trep;t++)
        {
            if(t == 1)
              trueDelay(125);
            digitalWrite(m_sendpin, HIGH); 
            trueDelay(4950);
            digitalWrite(m_sendpin, LOW); 
            trueDelay(1475);
            for(int i = 0; i < 4; i++)
            {
                for(int b = 0; b < 8; b++)
                {
                    digitalWrite(m_sendpin, HIGH); 
                    trueDelay(410);
                    digitalWrite(m_sendpin, LOW); 
                    int add = 0;
                    if((pack.data[i] & (1<<b)) > 0)
                        add = 780;
                    trueDelay(410 + add);
                }
            }
            digitalWrite(m_sendpin, HIGH);
            trueDelay(1475);
            digitalWrite(m_sendpin, LOW);
        }
        interrupts();
        if (instanceId == 0)
        {
            //attachInterrupt(digitalPinToInterrupt(m_readpin), stamp, RISING);
            lastStamp = micros();
        }
        else
        {
            //attachInterrupt(digitalPinToInterrupt(m_readpin), stamp2, RISING);
            lastStamp2 = micros();
        } 
        
    }

    void transmitHandler(GeyserPacket pack)
    {
        if (instanceId == 0)
        {
            if (m_shouldCheck)
            {
                checkBuffer();
                m_shouldCheck = false;
            }
        }
        else
        {
            if (m_shouldCheck2)
            {
                checkBuffer();
                m_shouldCheck2 = false;
            }
        }
        if (instanceId == 0)
        {
          if((micros() - lastStamp > 450) && (micros() - lastStamp < 820) && digitalRead(m_readpin) == HIGH)
            endofMsg = true;
          if (micros() - lastStamp > 1580)
          {
            if(digitalRead(m_readpin) == LOW && endofMsg)
            {
              this->send(pack,1);
              endofMsg = false;
            }
            else
              endofMsg = false;
          }
        }
        else
        {
          if((micros() - lastStamp2 > 450) && (micros() - lastStamp2 < 820) && digitalRead(m_readpin) == HIGH)
            endofMsg2 = true;
          if (micros() - lastStamp2 > 1580)
          {
            if(digitalRead(m_readpin) == LOW && endofMsg2)
            {
              this->send(pack,1);
              endofMsg2 = false;
            }
            else
              endofMsg2 = false;
          }
        }
    }

    void init()
    {
        pinMode(m_readpin, INPUT);
        pinMode(m_sendpin, OUTPUT);
        digitalWrite(m_sendpin, LOW);
        Serial.print("Instance ");
        Serial.println(instanceId);
        if (instanceId == 0)
            attachInterrupt(digitalPinToInterrupt(m_readpin), stamp, RISING);
        else
            attachInterrupt(digitalPinToInterrupt(m_readpin), stamp2, RISING);
    }

};

int GeyserWiseComm::instanceCntr = 0;
