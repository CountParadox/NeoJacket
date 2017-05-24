#include <Adafruit_NeoPixel.h>
// Pattern types supported:
enum pattern { NONE, RAINBOW_CYCLE, COLOR_WIPE};
// Patern directions supported:
enum  direction { FORWARD, REVERSE };

#define BUTTON_PIN   2

#define PIXEL_PIN    6

#define PIXEL_COUNT 1

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);
class NeoPatterns : public Adafruit_NeoPixel
{
  public: 

  //Member variables:
  pattern ActivePattern; // which pattern is running
  direction Direction;     // direction to run the pattern

  unsigned long Interval;  // milliseconds between updates
  unsigned long lastUpdate; // last update of position

  uint32_t Color1, Color2;  // What colors are in use
  uint16_t TotalSteps;  // total number of steps in the pattern
  uint16_t Index;  // current step within the pattern

  void (*OnComplete)();

    // Constructor - calls base-class constructor to initialize strip
    NeoPatterns(uint16_t pixels, uint8_t pin, uint8_t type)
    :Adafruit_NeoPixel(pixels, pin, type)
    {}
    void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}
    void Update()
    {
        if((millis() - lastUpdate) > Interval) // time to update
        {
            lastUpdate = millis();
            switch(ActivePattern)
            {
                case RAINBOW_CYCLE:
                    RainbowCycleUpdate();
                    break;
                case COLOR_WIPE:
                    ColorWipeUpdate();
                    break;
                default:
                    break;
            }
        }
    }
        // Increment the Index and reset at the end
    void Increment()
    {
        if (Direction == FORWARD)
        {
           Index++;
           if (Index >= TotalSteps)
            {
                Index = 0;
                if (OnComplete != NULL)
                {
                    OnComplete(); // call the comlpetion callback
                }
            }
        }
        else // Direction == REVERSE
        {
            --Index;
            if (Index <= 0)
            {
                Index = TotalSteps-1;
                if (OnComplete != NULL)
                {
                    OnComplete(); // call the comlpetion callback
                }
            }
        }
    }
    void RainbowCycle(uint8_t interval, direction dir = FORWARD)
    {
        ActivePattern = RAINBOW_CYCLE;
        Interval = interval;
        TotalSteps = 255;
        Index = 0;
        Direction = dir;
    }
    void RainbowCycleUpdate()
    {
        for(int i=0; i< numPixels(); i++)
        {
            setPixelColor(i, Wheel(((i * 256 / numPixels()) + Index) & 255));
        }
        show();
        Increment();
    }
    void ColorWipe(uint32_t color, uint8_t interval, direction dir = FORWARD)
    {
        ActivePattern = COLOR_WIPE;
        Interval = interval;
        TotalSteps = numPixels();
        Color1 = color;
        Index = 0;
        Direction = dir;
    }
    // Update the Color Wipe Pattern
    void ColorWipeUpdate()
    {
        setPixelColor(Index, Color1);
        show();
        Increment();
    }
    // Input a value 0 to 255 to get a color value.
    // The colours are a transition r - g - b - back to r.
    uint32_t Wheel(byte WheelPos)
    {
        WheelPos = 255 - WheelPos;
        if(WheelPos < 85)
        {
            return Color(255 - WheelPos * 3, 0, WheelPos * 3);
        }
        else if(WheelPos < 170)
        {
            WheelPos -= 85;
            return Color(0, WheelPos * 3, 255 - WheelPos * 3);
        }
        else
        {
            WheelPos -= 170;
            return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
        }
    }
};



NeoPatterns Strip(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

bool oldState = HIGH;
int showType = 0;

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Strip.begin();
  Strip.ColorWipe((0,0,0), 0);
  Strip.setBrightness(255);
  Serial.begin(9600);
}

void loop() {
  
  bool newState = digitalRead(BUTTON_PIN);
  if (newState == LOW && oldState == HIGH) {
    delay(20);
    newState = digitalRead(BUTTON_PIN);
    if (newState == LOW) {
      showType++;
      if (showType > 9)
        showType=0;
      startShow(showType);
    }
  }
  oldState = newState;
  Strip.Update();
}
void startShow(int i) {
  switch(i){
    case 0: Strip.ActivePattern = COLOR_WIPE;
            Strip.Index = 0;
            Strip.Color1 = Strip.Color(0, 0, 0); //Black/off
            Strip.Interval = 20;
            Strip.Update();
            break;
    case 1: Strip.ActivePattern = RAINBOW_CYCLE;  //Rainbow
            Strip.Index = 0;
            Strip.Interval = 10;
            Strip.TotalSteps = 255;
            Strip.Update();
            break;     
    case 2: Strip.ActivePattern = COLOR_WIPE;
            Strip.Index = 0;
            Strip.Color1 = Strip.Color(255, 0, 0); // Red
            Strip.Interval = 20;
            Strip.Update();
            break;
    case 3: Strip.ActivePattern = COLOR_WIPE;
            Strip.Index = 0;
            Strip.Color1 = Strip.Color(0, 255, 0); // Green
            Strip.Interval = 20;
            Strip.Update();
            break;
    case 4: Strip.ActivePattern = COLOR_WIPE;
            Strip.Index = 0;
            Strip.Color1 = Strip.Color(0, 0, 255); //Blue
            Strip.Interval = 20;
            Strip.Update();
            break;
    case 5: Strip.ActivePattern = COLOR_WIPE;
            Strip.Index = 0;
            Strip.Color1 = Strip.Color(255, 255, 255); //White
            Strip.Interval = 20;
            Strip.Update();
            break;
    case 6: Strip.ActivePattern = COLOR_WIPE;
            Strip.Index = 0;
            Strip.Color1 = Strip.Color(255, 0, 255); //Pink
            Strip.Interval = 20;
            Strip.Update();
            break; 
    case 7:  Strip.ActivePattern = COLOR_WIPE;
            Strip.Index = 0;
            Strip.Color1 = Strip.Color(0, 255, 255); //Turquoise
            Strip.Interval = 20;
            Strip.Update();
            break;
    case 8: Strip.ActivePattern = COLOR_WIPE;
            Strip.Index = 0;
            Strip.Color1 = Strip.Color(255, 255, 0); //Yellow
            Strip.Interval = 20;
            Strip.Update();
            break;
    
 //   case 9:    rainbow(20);                                   
  }
}
