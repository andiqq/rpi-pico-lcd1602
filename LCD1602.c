#include "LCD1602.h"
#include "hardware/i2c.h"
//#include "pico/binary_info.h"

// commands
static const int clear_display =    0b0001;
static const int return_home =      0b0010;
static const int entrymode_set =    0b0100;
static const int display_control =  0b1000;
static const int cursor_shift =     0b00010000;
static const int function_set =     0b00100000;
static const int set_cgram_addr =   0b01000000;
static const int set_ddram_addr =   0b10000000;

// start address first and second line, offset to set_ddram_addr
static const int first_line =       0;
static const int second_line =      0x40;

// flags for display entry mode
static const int entry_shiftinc =   0b0001;
static const int entry_left =       0b0010;

// flags for display and cursor control
static const int blink_on =         0b0001;
static const int cursor_on =        0b0010;
static const int display_on =       0b0100;

// flags for display and cursor shift
static const int move_right =       0b0100;
static const int display_move =     0b1000;

// flags for function set
static const int lcd_5x10dots =     0x04;
static const int lcd_2line =        0x08;
static const int lcd_8bitmode =     0x10;

// flag for backlight control
static const int backlight =        0b1000;


static const int lcd_enable_bit =   0b0100;

// By default these LCD display drivers are on bus address 0x27
static const int addr = 0x27;

// Modes for lcd_send_byte
static const int character = 1;
static const int command = 0;

/* Quick helper function for single byte transfers */
void i2c_write_byte(uint8_t val) {
//#ifdef i2c_default
    i2c_write_blocking(i2c_default, addr, &val, 1, false);
//#endif
}

static void lcd_toggle_enable(uint8_t val) {
    // Toggle enable pin on LCD display
    // We cannot do this too quickly or things don't work
#define DELAY_US 600
    sleep_us(DELAY_US);
    i2c_write_byte(val | lcd_enable_bit);
    sleep_us(DELAY_US);
    i2c_write_byte(val & ~lcd_enable_bit);
    sleep_us(DELAY_US);
}

// The display is sent a byte as two separate nibble transfers
void lcd_send_byte(uint8_t val, int mode) {
    uint8_t high = mode | (val & 0xF0) | backlight;
    uint8_t low = mode | ((val << 4) & 0xF0) | backlight;

    i2c_write_byte(high);
    lcd_toggle_enable(high);
    i2c_write_byte(low);
    lcd_toggle_enable(low);
}

void lcd_clear(void) {
    lcd_send_byte(clear_display, command);
}

// go to location on LCD
void lcd_set_cursor(int line, int position) {
    int val = set_ddram_addr + (!line ? first_line : second_line) + position;
    lcd_send_byte(val, command);
}

static inline void lcd_char(char val) {
    lcd_send_byte(val, character);
}

void lcd_string(const char *s) {
    while (*s) {
        lcd_char(*s++);
    }
}

void lcd_init() {
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
    // Make the I2C pins available to picotool
    //bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C));

    lcd_send_byte(0x03, command);
    lcd_send_byte(0x03, command);
    lcd_send_byte(0x03, command);
    lcd_send_byte(0x02, command);

    lcd_send_byte(entrymode_set | entry_left, command);
    lcd_send_byte(function_set | lcd_2line, command);
    lcd_send_byte(display_control | display_on, command);
    lcd_clear();
}