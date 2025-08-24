@main
struct Main {

  static func main() {

    let lcd = LCD1602() 
    
    while true {

      lcd.setCursor(line:0, position: 0)
      lcd.lcd_string("Hello world from")

      lcd.setCursor(line:1, position: 0)
      lcd.lcd_string2("embedded Swift !")
      
      sleep_ms(1000)
    }
  }
}
