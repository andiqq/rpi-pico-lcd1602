public struct LCD1602 {
    let clearDisplay: UInt8 = 0x01
    let returnHome: UInt8 = 0x02
    let entryModeSet: UInt8 = 0x04
    let displayControl: UInt8 = 0x08
    let cursorShift: UInt8 = 0x10
    let functionSet: UInt8 = 0x20
    let setCRamAddr: UInt8 = 0x40
    let setDRamAddr: UInt8 = 0x80

    let entryShiftIncrement: UInt8 = 0x01
    let entryLeft: UInt8 = 0x02

    let blinkOn: UInt8 = 0x01
    let cursorOn: UInt8 = 0x02
    let displayOn: UInt8 = 0x04

    let moveRight: UInt8 = 0x04
    let displayMove: UInt8 = 0x08

    let set5x10Dots: UInt8 = 0x04
    let set2Line: UInt8 = 0x08
    let set8BitMode: UInt8 = 0x10

    let backlight: UInt8 = 0x08
    let enableBit: UInt8 = 0x04

    let address: UInt8 = 0x27

    let character: UInt8 = 1
    let command: UInt8 = 0

    let maxLines: UInt8 = 2
    let maxChars: UInt8 = 16

    func writeByte(value: UInt8) {
        var val = value
        i2c_write_blocking(get_default_i2c(), address, &val, 1, false)
    }

    func toggleEnable(value: UInt8) {
        let delay: UInt64 = 600
        sleep_us(delay)
        writeByte(value: value | enableBit)
        sleep_us(delay)
        writeByte(value: value & ~enableBit)
        sleep_us(delay)
    }

    func sendByte(value: UInt8, mode: UInt8) {
        let high: UInt8 = mode | (value & 0xF0) | backlight
        let low: UInt8 = mode | ((value << 4) & 0xF0) | backlight

        writeByte(value: high)
        toggleEnable(value: high)
        writeByte(value: low)
        toggleEnable(value: low)
    }

    func clear() {
        sendByte(value: clearDisplay, mode: command)
    }

    func setCursor(line: UInt8, position: UInt8) {
        let value: UInt8 = (line == 0) ? 0x80 + position : 0xC0 + position;
        sendByte(value: value, mode: command)
    }

    func writeChar(_ val: UInt8) {
    sendByte(value: val, mode: character)
}

func writeStaticString(_ string: StaticString) {
    for i in 0..<string.utf8CodeUnitCount {
        let byte = string.utf8Start[i]
        writeChar(byte)
    }
}

    init() {
        
        i2c_init(get_default_i2c(), 100_000)
        gpio_set_function(UInt32(PICO_DEFAULT_I2C_SDA_PIN), GPIO_FUNC_I2C)
        gpio_set_function(UInt32(PICO_DEFAULT_I2C_SCL_PIN), GPIO_FUNC_I2C)
        gpio_pull_up(UInt32(PICO_DEFAULT_I2C_SDA_PIN))
        gpio_pull_up(UInt32(PICO_DEFAULT_I2C_SCL_PIN))

        sendByte(value: 0x03, mode: command)
        sendByte(value: 0x03, mode: command)
        sendByte(value: 0x03, mode: command)
        sendByte(value: 0x02, mode: command)

        sendByte(value: entryModeSet | entryLeft, mode: command)
        sendByte(value: functionSet | set2Line, mode: command)
        sendByte(value: displayControl | displayOn | ~cursorOn | ~blinkOn, mode: command)


        clear()
        
    }
}
    



  