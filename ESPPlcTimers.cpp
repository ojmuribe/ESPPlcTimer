#include "ESPPlcTimers.h"

// Parameterless constructor. 10s by default.
timerON::timerON()
{
  t.pt = 10000;
}

// Constructor with parameters. Set the timer to preset in ms
timerON::timerON(uint32_t preset)
{
  t.pt = preset;
}

// Timer update function
void timerON::update()
{
  if (!t.in)
  {
    t.q = false;
    t.et = t.pt;
  }
  t.q = (t.in && (t.et == 0));
  if (millis() >= lastTime)
  {
    lastTime = millis();
  }
  if (t.in && t.et > 0)
    t.et--;
}

// Set the preset value of timer
void timerON::preset(uint32_t time)
{
  t.pt = time;
}

// Returns the output of timer
bool timerON::q()
{
  return t.q;
}

// Returns the time remaining until the timer ends
uint32_t timerON::et()
{
  return t.et;
}

// Timer activation input
void timerON::tin(bool timerIn)
{
  t.in = timerIn;
}

// Parameterless constructor. 10s by default.
timerOFF::timerOFF()
{
  t.pt = 10000;
}

// Constructor with parameters. Set the timer to preset in ms
timerOFF::timerOFF(uint32_t preset)
{
  t.pt = preset;
}

// Timer update function
void timerOFF::update()
{
  t.q = (!t.in && (t.et > 0));
  if (millis() >= lastTime)
  {
    lastTime = millis();
  }
  if (!t.in && t.et > 0)
    t.et--;
  if (t.in)
  {
    t.q = true;
    t.et = t.pt;
  }
}

// Set the preset value of timer
void timerOFF::preset(uint32_t time)
{
  t.pt = time;
}

// Returns the output of timer
bool timerOFF::q()
{
  return t.q;
}

// Returns the time remaining until the timer ends
uint32_t timerOFF::et()
{
  return t.et;
}

// Timer activation input
void timerOFF::tin(bool timerIn)
{
  t.in = timerIn;
}

/**********************************************
            Auxiliary functions
***********************************************/

// Rising edge of a boolean
bool edgeUp(bool input, bool &prev)
{
  bool out;

  out = false;
  if ((input != prev) && input)
  {
    out = true;
  }
  prev = input;
  return out;
}

// Falling edge of a boolean
bool edgeDown(bool input, bool &prev)
{
  bool out;

  out = false;
  if ((input != prev) && !input)
  {
    out = true;
  }
  prev = input;
  return out;
}

// Generates a symmetrical pulse of a period of interval ms
bool sqPulse(uint32_t interval)
{
  uint32_t counter = millis() % interval;
  return counter < interval / 2;
}

// Generates an asymmetrical pulse with an ON time of tONms and an OFF time of tOFF ms
bool asyncPulse(uint32_t tON, uint32_t tOFF)
{
  uint32_t counter = millis() % (tON + tOFF);
  return counter < tON;
}

// Set bit function
void setBool(bool &in)
{
  in = true;
}

// Reset bit function
void resetBool(bool &in)
{
  in = false;
}

// Set-Reset flip-flop
bool setReset(bool setIn, bool resetIn, bool &prev)
{
  if (setIn)
    prev = true;

  if (resetIn)
    prev = false;

  return prev;
}

// Reset-Set flip-flop
bool ResetSet(bool setIn, bool resetIn, bool &prev)
{
  if (resetIn)
    prev = false;
  if (setIn)
    prev = true;
  return prev;
}

// j-k flip-flop
bool jk(bool jIn, bool kIn, bool ckIn, bool &prev)
{
  if (ckIn)
  {
    if (jIn && !kIn)
      prev = true;
    if (!jIn && kIn)
      prev = false;
    if (jIn && kIn)
      prev = !prev;
  }
  return prev;
}

// t flip-flop
bool t(bool tIn, bool ckIn, bool &prev)
{
  return jk(tIn, tIn, ckIn, prev);
}