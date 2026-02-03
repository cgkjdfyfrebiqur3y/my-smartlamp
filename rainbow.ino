#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        6 // Change this to the pin you're using

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 8 // You mentioned you have 8 NeoPixels

// Initialize the NeoPixel library
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// Number of steps for color interpolation (determined by analog input)
int STEPS = 255;  // Default step size

// Rainbow colors (RGB values)
int rainbow[13][3] = {
  {255, 0, 0},      // Red
  {255, 51, 0},     // Red-Orange
  {255, 102, 0},    // Orange
  {255, 153, 0},    // Orange-Yellow
  {255, 204, 0},    // Yellow
  {204, 255, 0},    // Yellow-Green
  {102, 255, 0},    // Green-Yellow
  {0, 255, 0},      // Green
  {0, 255, 255},    // Cyan
  {0, 204, 255},    // Light Blue
  {0, 0, 255},      // Blue
  {75, 0, 130},     // Indigo
  {148, 0, 211}     // Violet
};

// Function to interpolate between two colors
void interpolateColor(int startColor[3], int endColor[3], int steps) {
  int red, green, blue;

  // Interpolate in a smooth way and store the result in the pixel buffer
  for (int i = 0; i < steps; i++) {
    red = startColor[0] + (endColor[0] - startColor[0]) * i / steps;
    green = startColor[1] + (endColor[1] - startColor[1]) * i / steps;
    blue = startColor[2] + (endColor[2] - startColor[2]) * i / steps;

    // Update pixels' colors for the current step
    for (int j = 0; j < NUMPIXELS; j++) {
      pixels.setPixelColor(j, pixels.Color(red, green, blue));
    }
  }

  // After interpolation is complete, show the updated colors once
  pixels.show();
}

void setup() {
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  pixels.show();   // Send the updated pixel colors to the hardware.
  
  // Initialize analog pin A5 for reading
  pinMode(A5, INPUT);
  
  // Start serial communication for debugging
  Serial.begin(9600);
}

void loop() {
  // Read the analog value from pin A5 (0-1023)
  int analogValue = analogRead(A5);
  
  // Map the analog value (0-1023) to a range of 0 to 255 for brightness
  int brightness = map(analogValue, 0, 1023, 0, 255);

  // Debugging: Print the brightness value to Serial Monitor
  Serial.print("Analog Value: ");
  Serial.print(analogValue);
  Serial.print("  Mapped Brightness: ");
  Serial.println(brightness);

  // Set the NeoPixel brightness based on the analog input (A5)
  pixels.setBrightness(brightness);

  // Map the analog value (0-1023) to a range of 10 to 255 for smooth color transitions
  STEPS = map(analogValue, 0, 1023, 10, 255);

  // Iterate through the extended rainbow colors and interpolate between them
  for (int i = 0; i < 13; i++) {
    int next = (i + 1) % 13; // Get the next color index (wrap around at the end)

    // Interpolate between the current and next color in the rainbow
    interpolateColor(rainbow[i], rainbow[next], STEPS);
  }
}
