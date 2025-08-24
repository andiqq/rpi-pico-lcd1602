@main
struct Main {

  static func main() {

    let lcd = LCD1602() 

    while true {

      lcd.setCursor(line:0, position: 0)
      lcd.writeStaticString("Hello world from")

      lcd.setCursor(line:1, position: 0)
      lcd.writeStaticString("embedded Swift !")
      
      sleep_ms(1000)
    }
  }
}
