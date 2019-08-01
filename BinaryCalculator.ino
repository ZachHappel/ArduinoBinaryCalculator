/*
 * Zacharie Happel 
 * Computer Organization & Architecture
 * Final Project
 * 
 * 
 * Attempting to implement the various, specific topics covered in class, some feature implementation 
 * may seem over-done / overly complicated. While trying to not get too carried away with myself, a lot of time 
 * and effort has been put into the below code. Topics such as registry storage of data and ...
 * 
 * 
 * 
 */


#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); 


String current_view;

bool main_menu_selected = true;
bool mm_loaded = false;
int menu_index = 0;



const int up_button_pin = 8;
const int down_button_pin = 9;
const int left_button_pin = 7;
const int right_button_pin = 13;

int up_state = 0;   
int down_state = 0;
int left_state = 0;
int right_state = 0;

const char leftArrow = (char)60;
const char downArrow = (char)60;

String line_one = "";
String line_two = "";
String line_tospec = "";

// I had to make these
byte up_arrow[8] = {
  B00100,
  B01110,
  B10101,
  B00100,
  B00100,
  B00100,
  B00100,
};

byte down_arrow[8] = {
  B00100,
  B00100,
  B00100,
  B00100,
  B10101,
  B01110,
  B00100,
};

String previous_line_two;
String previous_line_three;


String menu_screens[4] = {"1. Addition", "2. Subtraction", "3. Multipl.", "4. Division"};


void setup() {
  Serial.begin(9600);
  //LiquidCrystal lcd(RS, E, (RW,) D4, D5, D6, D7)
 
  /* 
   *  Current Set-up:
   *  Below, the indented "Pin:" and associated numbers refers to the current wiring of the attached LCD's pins
   *  and the respective Arduino Uno pins in which they are connected
   *  
   *    - Register Select (RS): 
   *        Specifies the register within the LCD to which the data shall be written
   *        Pin: 12
   *        
   *    - Read Write (R/W, RW):
   *        Pin utilized when selecting either read or write operations
   *        Pin: 1
   *        
   *    - Enable: 
   *        Pin that enables writing to the registers within the LCD
   *        Pin: 11
   *        
   *    - Data Pins:
   *      In total eight pins (D0-D8), each represented by a bit, are used to define and transmit data. Data is  
   *      represented in the form of these bits. Used when reading and writing.
   *      
   *      Data pins increment from left to right (if you were looking head-on at the LCD)
   *      Pins:
   *        - D4: 5      (D4 is the left-most data pin utilized in the wiring) 
   *        - D5: 4 
   *        - D6: 3
   *        - D7: 2
   *       ( Talk about a spiders nest of wires...)
   *        
   *      Potentiometer: 
   *      The righ-most leg (again, viewing from heads-on) of the potentiometer is wired to Pin 15 of the LCD.
   *      On the same row, as the leg of the potentiometer in which the Pin 15 is connected, on the adjacent side
   *      is a wire connecting the potentiometer to ground. 
   *      
   *      This part up of the setup is confusing to me
   *      Pin: 15
   *      
   */

   /* 
    *  Typically, the RW pin is tied to ground. I instead needed to connect it to a pin in order to read the contents
    *  of the LCD screen
    */

  
  
  // Button Init
  pinMode(up_button_pin, INPUT);
  pinMode(down_button_pin, INPUT);
  pinMode(left_button_pin, INPUT);
  pinMode(right_button_pin, INPUT);

  pinMode(6, INPUT); 
  
    // LCD Init
   // + char(leftarrow))
   
  lcd.createChar(0, up_arrow); // Needs to be before begin
  lcd.createChar(1, down_arrow); // Needs to be before begin
  lcd.begin(16,2);
}




void formattedLCDPrint(String i_line_one, String i_line_two, bool line_one_arrow, bool line_two_arrow) {
  
  line_one = createScreenText(i_line_one, line_one_arrow);
  line_two = createScreenText(i_line_two, line_two_arrow);
  lcd.clear();
  lcd.print(line_one);
  if (line_one_arrow) {lcd.write(byte(0));}
  lcd.setCursor(0,1);
  lcd.print(line_two);
  if (line_two_arrow) {lcd.write(byte(1));}
  lcd.blink();
  Serial.println("Loaded screen");
  delay(500); 
}


/** menu_index can equal 0
           *  menu_index can NOT equal (sizeOf(menu_screens) - 1)  
           *                            Current length = 4
           *                            menu_screens[4-1] = "3. Division" 
           *  Downward navigation:
           *  line_two is aquired by inheriting the value of menu_screens[menu_index + 1]
           *  line_one = line_two      (bottom shifts to top)                                                       
           *  line_two = menu_screens[
          */


void loop() {

  int screen_amount = (sizeof(menu_screens) / sizeof(menu_screens[0]));
  
  up_state = digitalRead(up_button_pin);
  down_state = digitalRead(down_button_pin);
  left_state = digitalRead(left_button_pin);
  right_state = digitalRead(right_button_pin);
  
  if (main_menu_selected == true && mm_loaded == false){
    Serial.println("Loading Main Menu...");
    mainMenu(); // Load main menu 
    mm_loaded = true;
    current_view = "MainMenu";
  } else { }// Main menu loaded }

  /**
   * menu_screens[0] = Addition, 
   * menu_screens[1] = Subtraction, 
   * menu_screens[2] = Multiplication, 
   * menu_screens[3] = Division
    */

  if (mm_loaded == true) {

     // Down button pressed 


    if (down_state == HIGH || up_state == HIGH) {

        // DOWN 
        if (down_state == HIGH) {
          
          if (menu_index != screen_amount )  { 
             line_one = menu_screens[menu_index];
             line_two = menu_screens[menu_index + 1];
             menu_index += 1; // Update index
             Serial.println("Current Menu Index: " );
             Serial.print(menu_index); 
              
          } else {
             Serial.println("Unable to navigate down");
          }
          
       // UP 
      } else if (up_state == HIGH) {
          if (menu_index != 0) {
             line_one = menu_screens[menu_index - 2];   // menu_index -2
             line_two = menu_screens[menu_index - 1]; // menu_index - 1
             menu_index -= 1; // Update index
             Serial.println("Current Menu Index: " );
             Serial.print(menu_index); 
          } else {
            Serial.println("Error: Cannot navigate upwards");
          }    
      }

      // Format afterwards so that 
      if (menu_index == 0) {
        Serial.println("Menu index detected to be 0"); 
        formattedLCDPrint("Main Menu: ", line_two, false, true); 
      } else if (menu_index == screen_amount - 1) {
        Serial.println("Menu index detected to be 3"); 
        Serial.println("Screen amount @ this point: ");
        Serial.println(screen_amount);
        formattedLCDPrint(line_one, line_two, true, false);
      } else {
        formattedLCDPrint(line_one, line_two, true, true);
      }

        
      
    }  
    
  }
    
}

/**
 * Accepts both line one and line two, and booleans indicating whether or not each line should include an
 * arrow at the end 
 */




/*
 *  Serial.println("@ Main Menu");
    line_one = createScreenText("Main Menu:", false);
    line_two = createScreenText("1. Addition", true); 
    lcd.print(line_one);
    lcd.write(byte(0));
    lcd.setCursor(0,1);
    lcd.print(line_two);
    lcd.write(byte(1));
    lcd.blink();
    //lcd.autoscroll();
    Serial.println("Main menu loaded."); 
 */
  


/**
 * 'has_arrows' indicates that the string should be fifteen characters long instead of sixteen
 *  this leaves space for the arrow to be appended to the end
 */
String createScreenText(String screen_text, bool has_arrows) {
  if (screen_text.length() > 16) {
    line_tospec = "Error";
    return line_tospec; 
  } else if (screen_text.length() >= 15 && has_arrows == true) {
    line_tospec = "Error";
    return line_tospec;
  } else {
    int text_length = screen_text.length();
    int difference = 16 - text_length;
    line_tospec = screen_text;

    if (has_arrows) {
       for (int whitespace = 1; whitespace <= difference - 1; whitespace ++) {
        line_tospec+= " ";
      } return line_tospec;
    } else {
      for (int whitespace = 1; whitespace <= difference; whitespace ++) {
        line_tospec+= " ";
      } return line_tospec;   
    }

  } 
}

//const array menu_options[4] = { { 





void mainMenuNav(String button_direction){
  if (button_direction == "UP") {
    Serial.println("Main Screen Nav: UP Button Pressed");
    
  } else if (button_direction == "DOWN") {
    Serial.println("Main Screen Nav: DOWN Button Pressed");
    
    line_one = line_two; // Cycle bottom line upwards
    line_two = createScreenText("2. Subtraction", true);  // Create text, make it fit, and include arrows
    
  }
  
}



void mainMenu(){ 
    Serial.println("@ Main Menu");
    line_one = createScreenText("Main Menu:", false);
    line_two = createScreenText("1. Addition", true); 
    lcd.setCursor(0,0);
    lcd.print(line_one);
    lcd.write(byte(0));
    lcd.setCursor(0,1);
    lcd.print(line_two);
    lcd.write(byte(1));
    lcd.blink();
    //lcd.autoscroll();
    Serial.println("Main menu loaded."); 
}




void convertBinaryToDec(String binary_input) {

  char converted_str_to_array[binary_input.length() + 1];
  
  binary_input.toCharArray(converted_str_to_array, binary_input.length() + 1);
  
  int amount_of_bits = binary_input.length();
  
  for (int i = 0; i <= amount_of_bits - 1; i ++) {
    Serial.print(converted_str_to_array[i]);
    delay(100);
    Serial.print(" ");
  }

  Serial.println(" ");
  
  for (int i = 0; i <= amount_of_bits - 1; i ++) {
    Serial.print(amount_of_bits - i -1);
    delay(100);
    Serial.print(" ");
  }

  delay(1000);
  
}






void autoscrollMenuOptions(int option_number){ 

  //Scroll only top
  if (option_number == 1) {
    
  } else if (option_number == 2) {
    
  } else if (option_number == 3) {
    
  } else {
    Serial.println("Auto Scroll Error: Invalid option provided");
  }
  
}
