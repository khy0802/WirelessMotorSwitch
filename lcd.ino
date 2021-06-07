


void setupLCD()
{
  // initialize the LCD
  lcd.begin();
  lcd.createChar(0, bell);
  
}


int before_mill = 0;

void loop_lcd(){

  int mills = millis();
  if(mills == before_mill) return;
  before_mill = mills;

  mill++;

  if(mill == 1000){
    addTime();
    mill = 0;
  }

  
  
  switch(lcd_state){
    case 0:
      display_date();
      
      if(mill == 0){
        if(checkAlarm()){
          lcd_hasAlarm = false;
          togglePower(on);
        }
      }
      
      
      //display_temp();
      break;
    case 1:
      // Toggling switch. Can't do anything :(
      break;
    case 2: // Date
      display_date();
      
      break;
    case 3: // Alarm
      display_alarm_date();
      break;
  }
}




/**
 * main menu
 */


// Used in min:sec blinking
int date_mill = 1;

void delayDate(int s){
  sec += s;
}

void display_date(){

  date_mill++;
  if(date_mill > 2000) date_mill = 1;

  
  if(date_mill == 1000 || date_mill == 2000){
    writeDate();
    if(date_mill <= 1000){
      lcd.setCursor(13, 0);
      lcd.print(":");
    } else
      lcd.print(" ");
      
    
    if(lcd_hasAlarm){
      lcd.setCursor(15, 1);
      lcd.write(0);
    }
    lcd.setCursor(cursor_loc, 0);
    
  }
  
}

void display_alarm_date(){
  date_mill++;
  if(date_mill > 2000) date_mill = 1;

  if(date_mill == 1000 || date_mill == 2000){

    writeAlarmDate();
  }
  
}

/*
unsigned long temp_update_ms = 0;

void display_temp(){

  temp_update_ms++;

  if(mill % 10000 != 0) return;
  
  lcd.setCursor(0, 1);
  long d = active_sensor();

  if(temp > 100){
    lcd.print("LOADING...");
      lcd.print(temp);
      lcd.print("C");
  lcd.print(" ");
  lcd.print((int) humi);
  lcd.print("%");
  
  }

}
*/

void writeDate(){
  lcd.setCursor(0, 0);
    lcd.print(year);
    lcd.print(".");
    
    printNum(month);
    lcd.print(".");
  
    printNum(day);
    lcd.print(" ");
  
    printNum(hour);
    lcd.print(" ");
  
    printNum(minute);
  
}

void writeAlarmDate(){
  lcd.setCursor(0, 0);
    lcd.print(a_year);
    lcd.print(".");
    
    printNum(a_month);
    lcd.print(".");
  
    printNum(a_day);
    lcd.print(" ");
  
    printNum(a_hour);
    lcd.print(":");
  
    printNum(a_minute);
  lcd.setCursor(cursor_loc, 0);
}

void printNum(int i){
  if(i < 10) lcd.print("0");
  lcd.print(i);
}

void addTime(){

  sec++;
  Serial.println(sec);

  if(sec == 60){
    sec = 0;
    minute++;
  }

  if(minute == 60){
    minute = 0;
    hour++;
  }

  if(hour == 24){
    hour = 0;
    day++;
  }

  int d = months[month - 1];
  if(month == 2){
    if(isLeapYear(year)) d = 30;
  }
  if(day > d){
    day = 1;
    month++;
  }

  if(month == 13){
    month = 1;
    year++;
  }
}


/**
 * date setting screen
 * alarm setting screen
 */

int getNumber(){
  if(cursor_loc <= 3){
    return ((int) (year / pow(10, 3-cursor_loc))) % 10;
  }  else if(cursor_loc == 5){
    return month / 10;
  } else if(cursor_loc == 6){
    return month % 10;
  } else if(cursor_loc == 8){
    return day / 10;
  } else if(cursor_loc == 9){
    return day % 10;
  } else if(cursor_loc == 11){
    return hour / 10;
  } else if(cursor_loc == 12){
    return hour % 10;
  } else if(cursor_loc == 14){
    return minute / 10;
  } else if(cursor_loc == 15){
    return minute % 10;
  }
  return -1;
}

void setAlarmNumber(int i){
  int remain, p;
  if(cursor_loc <= 3){
    a_year = calculateRemains(a_year, 3, i);
  } else if(cursor_loc <= 6){
    a_month = calculateRemains(a_month, 6, i);
    if(a_month > 12) a_month = 12;
  } else if(cursor_loc <= 9){
    a_day = calculateRemains(a_day, 9, i);
    int mday = months[month-1];
    if(month == 2 && isLeapYear(year)) mday = 30;
    if(a_day > mday) a_day = mday;
  } else if(cursor_loc <= 12){
    a_hour = calculateRemains(a_hour, 12, i);
    if(a_hour > 23) a_hour = 23;
  } else if(cursor_loc <= 15){
    a_minute = calculateRemains(a_minute, 15, i);
    if(a_minute > 59) a_minute = 59;
  }
}

void setNumber(int i){
  int remain, p;
  if(cursor_loc <= 3){
    year = calculateRemains(year, 3, i);
  } else if(cursor_loc <= 6){
    month = calculateRemains(month, 6, i);
    if(month > 12) month = 12;
  } else if(cursor_loc <= 9){
    day = calculateRemains(day, 9, i);
    int mday = months[month-1];
    if(month == 2 && isLeapYear(year)) mday = 30;
    if(day > mday) day = mday;
  } else if(cursor_loc <= 12){
    hour = calculateRemains(hour, 12, i);
    if(hour > 23) hour = 23;
  } else if(cursor_loc <= 15){
    minute = calculateRemains(minute, 15, i);
    if(minute > 59) minute = 59;
  }
  lcd.setCursor(cursor_loc, 0);
}

int calculateRemains(int base, int len, int i){
    int p = pow(10, len-cursor_loc);

    if(p > 90) p++;
    // if p > 10 => result : 99, 999... ??
    int remain = (base / p) % 10;
    int r = base + (i-remain)*p;
    Serial.println(p);
    Serial.println(remain);
    
    return r;
}

void next_cursor(){
  cursor_loc++;
  if(cursor_loc == 4 
  || cursor_loc == 7 
  || cursor_loc == 10
  || cursor_loc == 13) cursor_loc++;

  if(cursor_loc == 16){
    cursor_loc = 0;
  }
  lcd.setCursor(cursor_loc, 0);
}

void prev_cursor(){
  cursor_loc--;
  if(cursor_loc == 4 
  || cursor_loc == 7 
  || cursor_loc == 10
  || cursor_loc == 13) cursor_loc--;

  if(cursor_loc == -1){
    cursor_loc = 15;
  }
  lcd.setCursor(cursor_loc, 0);
}

void confirm(boolean reset){
  if(reset){
    sec = 0;
    mill = 0;
  }
  lcd_state = 0;
  changeState(0);
}

/*
 * ON/OFF Toggled screen
 */
void display_toggle(boolean on){
  lcd.clear();
  if(on){
    lcd.setCursor(0, 0);
    lcd.print("     < ON >     ");
  } else {
    lcd.setCursor(0, 1);
    lcd.print("     < OFF >     ");
  }

}
