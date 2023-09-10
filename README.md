# ESPPlcTimer
Library for ESP32 of Timers and other functions commonly used in PLCs

  ESPPlcTimers.h
  
  Define timers to use PLC style
  
  (c)2023 Juan M. Uribe

************* THESE LIBRARIES USE FREE RTOS  ****************

*************     TO USE ONLY WITH ESP32     ****************


*************************************************************
*************    OPERATING INSTRUCTIONS      ****************
*************************************************************

  1. Declare timers at the begining of the program,
  before setup() followed the update function.
  Time is specified in ms.

    timerON t1(10000); // On delay timer of 10s
    timerOFF t2(20000); // Off delay timer of 20s

  Parameterless constructor assigns a time of 10s.

    timerON t3(); // On delay timer of 10s


  For each defined timer you have to add an entry in the update function.

    t1.update();
    t2.update();
    t3.update();
    ......

  The update function is linked to a FREE RTOS parallel task.

  -------------- COPY THIS BEFORE setup() -----------------
  
    // ESPPlcTimers updaters
    TaskHandle_t timerHandlerTask;

    void timerHandler(void *pvParameters)
    {
      (void)pvParameters;
      for (;;)
      {
        t1.update();
        t2.update();
        t3.update();
        ......... //rest of timers
      }
    }
  ---------------------------------------------------------------

  2. Add the timer handler at the begining of setup() (see example):

    void setup() {

      TIMERHANDLER;

      .......... rest of the setup() statements
    };

  3. Use the timers in loop() (see example):

    void loop() {
      ....... statements in loop()
      t1.tin(digitalRead(GPIO_NUM_13)); //GPIO 13 is t1 TON input
      digitalWrite(LED_BUILTIN, t1.q()); //output t1 turns on the built-in led
      t2.tin(digitalRead(GPIO_NUM_12)); //GPIO 12 is t2 TOFF input
      digitalWrite(GPIO_NUM_15, t2.q()); //output t2 turns on the GPIO 15
      ....... rest of loop() statements
  };

*/

/*------------------- AUXILIARY FUNCTIONS ------------------------

**************** edgeUp //Rising edge detection  *****************

bool edgeUp(bool input, bool &prev);

Returns a boolean value that is true one CPU cycle if the input have chanbe from false to true.
An auxiliary static bool must to be assigned to prev.

Example:
static bool aux;
if (edgeUp(digitalRead(GPIO_NUM_13, prev))){
  Serial.println("Rising edge detection");
}

**************** edgeDown //Falling edge detection ****************

bool edgeDown(bool input, bool &prev);

Returns a boolean value that is true one CPU cycle if the input have changed form true to false.
An auxiliary static bool must to be assigned to prev.

Example:
static bool aux;
if (edgeDown(digitalRead(GPIO_NUM_13, prev))){
  Serial.println("Falling edge detection");
}

******************** sqPulse //Symmetrical pulse *******************

bool sqPulse(uint32_t interval);

Returns a boolean value that is true for half of the specified period
and false for the other half.

Example: blinking at 1Hz
digitalWrite(LED_BUILDTIN, sqPulse(1000))

******************** asyncPulse //Asymmetrical pulse *******************

bool asyncPulse(uint32_t tON, uint32_t tOFF);

Returns a boolean value that is true during the TON specified time
and false during the TOFF specified time.

Example: asymmetrical blinking of 1Hz, led on 300ms
digitalWrite(LED_BUILDTIN, sqPulse(300, 700))

******************** setBool *******************
void setBool(bool &in);

Set true the boolean value passed in in.
The in variable must to be global or have to be declared
static bool if local.

******************** resetBool *****************
void resetBool(bool &in);

Set false the boolean value passed in in.
The in variable must to be global or have to be declared
static bool if local.

******************** Set-Reset flip-flop *****************
bool setReset(bool setIn, bool resetIn, bool &prev);

When the setIn input is true, the output becomes true.
When the resetIn input is true, the output becomes false.
Reset have priority over set.
An auxiliary static bool must to be assigned to prev.

******************** Reset-Set flip-flop *****************
bool ResetSet(bool setIn, bool resetIn, bool &prev);

Works like Set-Reset but here Set have the priority
over Reset.
An auxiliary static bool must to be assigned to prev.

******************** j-k flip-flop *****************
bool jk(bool jIn, bool kIn, bool ckIn, bool &prev);

When ckIn is false the output is unchanged.
if ckIn is true, then works like a set-reset flip-flop:
j is set
k is reset
If j and k are both false the output is unchanged,
but if j and k are both true the output changes state.
Example: a push button changes state of an output
  void loop() {
    static bool aux, aux2;
    bool input = edgeUp(digitalRead(GPIO_NUM_12), aux);
    digitalWrite(LED_BUILTIN, jk(input, input, true, aux2));
  };

******************** t flip-flop *****************
bool t(bool tIn, bool ckIn, bool &prev);

Works like a j-k flip-flop but with only one input
assigned to both j and k.
When ckIn is false the output is unchanged.
If ckIn is true then when t is false the output in unchanged,
but if t is true the output changes state.
Si mantenemos t a true y ponemos una señal con una frecuenca en cK
Holding t true and applying a frequency signal to cK
we will obtain an output signal of half the frequency.
(often used as a frecuency divider by 2)
Example: a push button changes state of an output
  void loop() {
    static bool aux, aux2;
    bool input = edgeUp(!digitalRead(GPIO_NUM_12), aux);
    digitalWrite(LED_BUILTIN, t(input, true, aux2));
  };

-----------------------------------------------------------------
