
/**
 * 0~9 => num
 * 10 => POW
 * 11 => MODE
 * 12 => MUTE
 * 13 => PAUSE/PLAY
 * 14/15 => PREV/NEXT
 * 
 */
int signals[] = {
  16738455, 
  16724175, 16718055, 16743045,
  16716015, 16726215, 16734885,
  16728765, 16730805, 16732845,
  16753245, 16736925, 16769565,
  16720605, 16712445, 16761405 
};

int getSignal(int sig){
  for(int i=0;i<sizeof(signals) / sizeof(int);i++){
    if(signals[i] == sig) return i;
  }
  return -1;
}

void remote(int sig){
  if(sig == 10){
    // Power control
    if(lcd_state == 0) togglePower(!on);
    // Only work on main menu
  } else if(sig == 11){
    // Mode change
    if(lcd_state == 0){
      lcd_state = 2; // Change to date
    } else if(lcd_state == 2){
      lcd_state = 0; // Change to alarm
    } 
    changeState(lcd_state);
  } else if(sig == 12){
    // Delete button
    
  } else if(sig == 13){
    // Confirm button

    if(lcd_state == 2){
      confirm(true);
      lcd_state = 0;
      changeState(0);
    }
    
    
  } else if(sig == 14){
    // Previous button

    if(lcd_state == 2){
      prev_cursor();
    }
    
  } else if(sig == 15){
    // Next button
    
    if(lcd_state == 2){
      next_cursor();
    }
    
  } else if(sig <= 9){
    if(lcd_state == 2){
      setNumber(sig);
      display_date();
      next_cursor();
    }
  }
}
