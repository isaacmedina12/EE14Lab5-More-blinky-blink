// main.c
// Author: Isaac Medina
// Description: Uses the functions in timer.c to implement a "breathing" fade pattern (L9);
//              Impliments the full rainbow effect for L10. 
// Date: 2/19/2025

// Include files:
#include "ee14lib.h" // Access all functions
#include "stm32l432xx.h" // In case a reference is needed the STM32 header file
#include <stdint.h> // Easier syntax, if needed.

// Define RGB LED pins:
#define RED_PIN D1 // PA_9 | TIM1_2 | Red pin
#define GREEN_PIN D0 // PA_10 | TIM1_3 | Green pin
#define BLUE_PIN D9 // PA_8 | TIM1_1 | Blue pin

// Define common PWM timer (TIM1 supports all of my chosen pins):
#define LED_TIMER TIM1 


// make_rgb()
// Input: 3 unsigned 16-bit integer values representing the brightness levels for 
//        red, green, and blue in the RGB LED. (0 = max brightness; 65535 = off).
// Description: Set the RGB colors by configuring the PWM duty cycle for each channel.
//                      !!!!INVERTED BECAUSE OF COMMON ANODE!!!!
// Output: No explicit return value, but updates the LED colors via PWM.
void make_rgb(unsigned int red, unsigned int green, unsigned int blue){
    timer_config_channel_pwm(LED_TIMER, RED_PIN, 65535 - red); 
    timer_config_channel_pwm(LED_TIMER, GREEN_PIN, 65535 - green);
    timer_config_channel_pwm(LED_TIMER, BLUE_PIN, 65535 - blue);
}


// --------L9: Implement the PWM fading ("breathing effect") on an LED channel--------

// breathing_effect()
// Input: No input.
// Description: Creates a smooth "breathing" effect where the brightness of 
//              the Red LED gradually increases and decreases in a loop.
// Output: No explicit output, but controls the LED brightness over time to implement
//         L9.
void breathing_effect(){

     // Fade in
    for (unsigned int i = 65535; i > 0; i -= 1) {  
        timer_config_channel_pwm(LED_TIMER, RED_PIN, i);
    }

    // Fade out
    for (unsigned int i = 0; i <= 65535; i += 1) {  
        timer_config_channel_pwm(LED_TIMER, RED_PIN, i);
    } 

} 


// ---------L10: Use all 3 RGB LED channels to make a full rainbow RGB effect-------

// rainbow_fade()
// Input: No input.
// Description: Cycles through a rainbow sequence to show different RGB colors.
//              Essentially, implement L9 for a full spectrum of colors.
// Output: No explicit output, but continuously updates the LED colors to implement
//         L10.
void rainbow_sequence() {

    while (1) { // Infinite while loop

        // Smooth transition between colors

        // Red -> Orange
        for (int i = 0; i <= 65535; i += 10) {
            make_rgb(65535, i, 0); 
        }

        // Orange -> Yellow -> Green
        for (int i = 65535; i >= 0; i -= 10) {
            make_rgb(i, 65535, 0); 
        }

        // Green -> Cyan
        for (int i = 0; i <= 65535; i += 10) {
            make_rgb(0, 65535, i); 
        }

        // Cyan -> Blue (Fix underflow)
        for (int i = 65535; i > 0; i -= 10) {
            make_rgb(0, i, 65535); 
        }

        // Blue -> Purple
        for (int i = 0; i <= 65535; i += 10) {
           make_rgb(i, 0, 65535);
        }

       // Purple -> Red (Fix underflow)
        for (int i = 65535; i > 0; i -= 10) {
            make_rgb(65535, 0, i); 
        }
    }
}


// main()
// Input: None.
// Description: The main function initializes system components and runs the LED effects. 
//              It first applies a breathing effect to the red LED (L9), and then transitions 
//              to a full RGB fade effect (L10) to display a continuous rainbow sequence.
// Output: Returns 0, but should never get there since it loops the rainbow_sequence() function 
//         forever. 
int main(){

    // Step 1: Configure PWM channels for each LED color.
    // Initially, set all LEDs to OFF (65535 for common anode inversion).
    timer_config_channel_pwm(LED_TIMER, RED_PIN, 65535); // Red
    timer_config_channel_pwm(LED_TIMER, GREEN_PIN, 65535); // Greem
    timer_config_channel_pwm(LED_TIMER, BLUE_PIN, 65535); // Blue

    // Step 2: Implement L9
    // The red LED will gradually brighten and dim in a loop for a smooth "breathing" effect.

    // Set the PWM frequency at some frequency:
    timer_config_pwm(LED_TIMER, 1000); // I chose 1000 Hz, but this can be anything really

    // Create a while loop to run it a couple times:
    int i = 0;
    while (i < 2){
        for (volatile int j = 0; j < 70000; j++); // Delay for added effect

        //Call the breathing effect function to implement L9
        breathing_effect(); 
        
        // Increment i
        i++;
    } 

    // Step 3: Implement L10:
    while (1){ // Infinite while loop to Run the rainbow loop indefinitly

        // Call the rainbow sequence function to implement L10
        rainbow_sequence();
    }

    // Return value for main (should never get here)
    return 0;   
}