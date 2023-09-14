# EXAMPLE

To use ESPPlcTimer here you can find a small program.

    #include "ESPPlcTimers.h"

    timerON t1(10000); // TON timer
    timerOFF t2(20000); // TOFF timer
    
    // ESPPLCTimers updaters
    TaskHandle_t timerHandlerTask;

    void timerHandler(void *pvParameters)
    {
      (void)pvParameters;
      for (;;)
      {
        t1.update();
        t2.update();
      }
    }

    void setup() {
    
      TIMERHANDLER;
    
      Serial.begin(115200);
      pinMode(GPIO_NUM_13, INPUT_PULLUP); //input for the TON timer (green button)
      pinMode(GPIO_NUM_12, INPUT_PULLUP); //input for the TOFF timer (blue button)
      pinMode(LED_BUILTIN, OUTPUT); //output for the TON timer (led built-in)
      pinMode(GPIO_NUM_15, OUTPUT); //output for the TOFF timer (led green)
          
    };
    
    void loop() {
      t1.tin(!digitalRead(GPIO_NUM_13));
      digitalWrite(LED_BUILTIN, t1.q());
      t2.tin(!digitalRead(GPIO_NUM_12));
      digitalWrite(GPIO_NUM_15, !t2.q());
    };

It's easy to check using Wokwi. Just add the follow circuit:

![image](https://github.com/ojmuribe/ESPPlcTimer/assets/88215048/eda6e9b5-a209-4f49-8023-51bdceac2362)

This is the diagram.json of the circuit. You can upload to Wokwi:

    {
        "version": 1,
        "author": "Juan Manuel Uribe Gómez",
        "editor": "wokwi",
        "parts": [
            { "type": "wokwi-esp32-devkit-v1", "id": "esp", "top": 4.31, "left": 47.37, "attrs": {} },
            {
              "type": "wokwi-pushbutton",
              "id": "btn1",
              "top": 70.92,
              "left": -124,
              "attrs": { "color": "green" }
        },
        {
              "type": "wokwi-pushbutton",
              "id": "btn2",
              "top": 3.47,
              "left": -124,
              "attrs": { "color": "blue" }
        },
        {
              "type": "wokwi-led",
              "id": "led1",
              "top": 62.36,
              "left": 196.8,
              "attrs": { "color": "green" }
        }
      ],
      "connections": [
        [ "esp:TX0", "$serialMonitor:RX", "", [] ],
        [ "esp:RX0", "$serialMonitor:TX", "", [] ],
        [ "btn1:1.r", "esp:D13", "green", [ "v0.7", "h22.31", "v59.19" ] ],
        [ "btn2:2.l", "btn1:2.l", "green", [ "h-19.36", "v67.45" ] ],
        [ "btn2:1.r", "esp:D12", "green", [ "v0.43", "h50.78", "v106.3" ] ],
        [ "led1:A", "esp:D15", "green", [ "v0" ] ],
        [ "led1:C", "esp:GND.1", "green", [ "v0" ] ],
        [ "btn1:2.l", "esp:GND.2", "green", [ "h-19.36", "v50.39" ] ]
      ],
      "dependencies": {}
    }

In the Wokwi editor dont't forget to load the ESPPlcTimer library:

![image](https://github.com/ojmuribe/ESPPlcTimer/assets/88215048/81d854e6-5810-4797-98b6-d39ea93a485c)




