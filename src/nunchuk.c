#include "stm32f0xx.h"
#include "lcd.h"
#include "nunchuk.h"
#include <stdint.h>
#include <string.h> // for memset() declaration
#include <math.h>   // for MA_PI

int flg_mv_right = 0;
int flg_mv_left = 0;
int flg_v = 0;
int flg_c = 0;

// Be sure to change this to your login...
const char login[] = "subbiah";

void nano_wait(unsigned int);
const char font[];

#define GPIOEX_ADDR 0x00  // ENTER GPIO EXPANDER I2C ADDRESS HERE
#define EEPROM_ADDR 0x00  // ENTER EEPROM I2C ADDRESS HERE

uint8_t NUNCHUK_INIT = 0;

void init_i2c(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;

//    GPIOB->MODER &= ~0xF000;
//    GPIOB->MODER |= 0xA000; // PB 6-7 Alternate Function
//    GPIOB->AFR[0] &= ~(0xf << (4*(6)));
//    GPIOB->AFR[0] |=   0x4 << (4*(6));
//    GPIOB->AFR[0] &= ~(0xf << (4*(7)));
//    GPIOB->AFR[0] |=   0x4 << (4*(7));
    GPIOB->MODER |= 2<<(2*6) | 2<<(2*7);
    GPIOB->AFR[0] |= 1<<(4*6) | 1<<(4*7);


    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

    I2C1->CR1 &= ~I2C_CR1_PE;
    I2C1->CR1 &= ~I2C_CR1_ANFOFF;
    I2C1->CR1 &= ~I2C_CR1_ERRIE;
    I2C1->CR1 &= ~I2C_CR1_NOSTRETCH;

    I2C1->TIMINGR = 0;
    I2C1->TIMINGR &= ~I2C_TIMINGR_PRESC;// Clear prescaler
    I2C1->TIMINGR |= 0 << 28; // Set prescaler to 0
    I2C1->TIMINGR |= 3 << 20; // SCLDEL
    I2C1->TIMINGR |= 1 << 16; // SDADEL
    I2C1->TIMINGR |= 3 << 8; // SCLH
    I2C1->TIMINGR |= 9 << 0; // SCLL

    I2C1->OAR1 &= ~I2C_OAR1_OA1EN;
    I2C1->OAR2 &= ~I2C_OAR2_OA2EN;

    I2C1->CR2 &= ~I2C_CR2_ADD10;
    I2C1->CR2 |= I2C_CR2_AUTOEND;

    I2C1->CR1 |= I2C_CR1_PE;
}


//===========================================================================
// 2.3 I2C helpers
//===========================================================================

void i2c_waitidle(void) {
    while ( (I2C1->ISR & I2C_ISR_BUSY) == I2C_ISR_BUSY); // while busy, wait.
}

void i2c_start(uint32_t devaddr, uint8_t size, uint8_t dir) {
    // dir: 0 = master requests a write transfer
    // dir: 1 = master requests a read transfer
    uint32_t tmpreg = I2C1->CR2;
    tmpreg &= ~(I2C_CR2_SADD | I2C_CR2_NBYTES |
    I2C_CR2_RELOAD | I2C_CR2_AUTOEND |
    I2C_CR2_RD_WRN | I2C_CR2_START | I2C_CR2_STOP);
    if (dir == 1)
    tmpreg |= I2C_CR2_RD_WRN; // Read from slave
    else
    tmpreg &= ~I2C_CR2_RD_WRN; // Write to slave
    tmpreg |= ((devaddr<<1) & I2C_CR2_SADD) | ((size << 16) & I2C_CR2_NBYTES);
    tmpreg |= I2C_CR2_START;
    I2C1->CR2 = tmpreg;
}

void i2c_stop(void) {
    if (I2C1->ISR & I2C_ISR_STOPF)
    return;
    // Master: Generate STOP bit after current byte has been transferred.
    I2C1->CR2 |= I2C_CR2_STOP;
    // Wait until STOPF flag is reset
    while( (I2C1->ISR & I2C_ISR_STOPF) == 0);
    I2C1->ICR |= I2C_ICR_STOPCF; // Write to clear STOPF flag
}

int i2c_checknack(void) {
    if (I2C1->ISR & I2C_ISR_NACKF) {
        return 1;
    } else {
        return 0;
    }
}

void i2c_clearnack(void) {
    I2C1->ICR |= I2C_ICR_NACKCF;
}

int i2c_senddata(uint8_t devaddr, const void *data, uint8_t size) {
    int i;
    if (size <= 0 || data == 0) return -1;
    uint8_t *udata = (uint8_t*)data;
    i2c_waitidle();
    // Last argument is dir: 0 = sending data to the slave device.
    i2c_start(devaddr, size, 0);
    for(i=0; i<size; i++) {
    // TXIS bit is set by hardware when the TXDR register is empty and the
    // data to be transmitted must be written in the TXDR register. It is
    // cleared when the next data to be sent is written in the TXDR reg
    // The TXIS flag is not set when a NACK is received.
    int count = 0;
    while( (I2C1->ISR & I2C_ISR_TXIS) == 0) {
        count += 1;
        if (count > 1000000) return -1;
        if (i2c_checknack()) { i2c_clearnack(); i2c_stop(); return -1; }

   }
    // TXIS is cleared by writing to the TXDR register.
    I2C1->TXDR = udata[i] & I2C_TXDR_TXDATA;

   }
    // Wait until TC flag is set or the NACK flag is set.
    while((I2C1->ISR & I2C_ISR_TC) == 0 && (I2C1->ISR & I2C_ISR_NACKF) == 0);
    if ( (I2C1->ISR & I2C_ISR_NACKF) != 0)
    return -1;
    i2c_stop();
    return 0;
}

int i2c_recvdata(uint8_t devaddr, void *data, uint8_t size) {
    int i;
    if (size <= 0 || data == 0) return -1;
    uint8_t *udata = (uint8_t*)data;
    i2c_waitidle();
    // Last argument is dir: 1 = receiving data from the slave device.
    i2c_start(devaddr, size, 1);
    for(i=0; i<size; i++) {
    int count = 0;
    while( (I2C1->ISR & I2C_ISR_RXNE) == 0) {
    count += 1;
    if (count > 1000000) return -1;
    if (i2c_checknack()) { i2c_clearnack(); i2c_stop(); return -1; }
    }
    udata[i] = I2C1->RXDR;
    }
    // Wait until TC flag is set or the NACK flag is set.
    while((I2C1->ISR & I2C_ISR_TC) == 0 && (I2C1->ISR & I2C_ISR_NACKF) == 0);
    if ( (I2C1->ISR & I2C_ISR_NACKF) != 0)
    return -1;
    i2c_stop();
    return 0;
}


////===========================================================================
//// 2.5 GPIO Expander
////===========================================================================
//void gpio_write(uint8_t reg, uint8_t val) {
//    uint8_t data[2] = { reg, val }; // Data pkg
//    i2c_senddata(0x20, data, 2);
//}
//
//uint8_t gpio_read(uint8_t reg) {
//    uint8_t data[1] = { reg }; // Data pkg
//    i2c_senddata(0x20, data, 1); // Sending pointer
//    i2c_recvdata(0x20, data, 1); // Reading from device
//    return data[0];
//}
//
//void init_expander(void) {
//    // Set IODIR (0x00) pin0-3 OUTPUT pin4-7 INPUT (0xF0)
//    gpio_write(0x00, 0xF0);
//    // Set GPPU (0x06) pin4-7 PULL DOWN (0xF0)
//    gpio_write(0x06, 0xF0);
//    // Set IPOL (0x01) pin4-7 REVERSE POLARITY (0xF0)
//    gpio_write(0x01, 0xF0);
//}
//
//void drive_column(int c) {
//    gpio_write(10, ~(1 << (3 - c)) );
//}
//
//int read_rows() {
//    uint8_t data = gpio_read(9);
//    data &= 0xf0;
//    for (int i = 0; i < 4; i++) {
//        uint8_t bit = data & (1 << (4 + i));
//        bit >>= (4 + i);
//        bit <<= (3 - i);
//        data |= bit;
//    }
//    return data & 0xf;
//}


////===========================================================================
//// 2.5 EEPROM functions
////===========================================================================
//void eeprom_write(uint16_t loc, const char* data, uint8_t len) {
//    //i2c_senddata();
//    uint8_t array[34];
//    array[0] = (loc >> 8); // Write upper 8 bits to array[0]
//    array[1] = (loc & 0xFF); // Write lower 8 bits to array[1]
//    for (int i = 0; i < len; i++) {
//        array[i+2] = data[i]; // Copy over data to array (shifted by 2)
//    }
//    i2c_senddata(0x50, array, len+2); // 1010000 A0=A1=A2=0
//}
//
//int eeprom_write_complete(void) {
//    i2c_waitidle();
//    i2c_start(0x50, 0, 0);
//    while((((I2C1->ISR & I2C_ISR_TC) == 0) && ((I2C1->ISR & I2C_ISR_NACKF) == 0)));
//    if (i2c_checknack()) {
//        // NACKF flag is set
////        i2c_clearnack();
////        I2C1->ICR &= ~I2C_ICR_NACKCF;
//        i2c_clearnack();
//        i2c_stop();
//        return 0;
//    } else {
//        i2c_stop();
//        return 1;
//    }
//
//}
//
//void eeprom_read(uint16_t loc, char data[], uint8_t len) {
//    TIM7->CR1 &= ~TIM_CR1_CEN; // Pause keypad scanning.
//
//    uint8_t array[2];
//    array[0] = (loc >> 8); // Write lower 8 bits to array[0]
//    array[1] = (loc & 0xFF);// Write upper 8 bits to array[1]
//    i2c_senddata(0x50, array, 2); // Set storage address
//    i2c_recvdata(0x50, data, len); // read into data
//
//    TIM7->CR1 |= TIM_CR1_CEN; // Resume keypad scanning.
//}



//===========================================================================
// Main and supporting functions
//===========================================================================
void nano_wait(unsigned int n) {
    asm(    "        mov r0,%0\n"
            "repeat: sub r0,#83\n"
            "        bgt repeat\n" : : "r"(n) : "r0", "cc");
}

//void eeprom_blocking_write(uint16_t loc, const char* data, uint8_t len) {
//    TIM7->CR1 &= ~TIM_CR1_CEN; // Pause keypad scanning.
//    eeprom_write(loc, data, len);
//    while (!eeprom_write_complete());
//    TIM7->CR1 |= TIM_CR1_CEN; // Resume keypad scanning.
//}
//
//void clear_display(void) {
//    for (int i=0; i < 8; i++) {
//        msg[i] &= ~0xff;
//    }
//}

void init_nunchuk(void) {
    init_i2c();
    uint8_t init_data_1[2] = {0xF0, 0x55};
    i2c_senddata(0x52, init_data_1, 2);
    uint8_t init_data_2[2] = {0xFB, 0x00};
    i2c_senddata(0x52, init_data_2, 2);
    NUNCHUK_INIT = 1;
}

void read_nunchuk(uint8_t *buffer) {
  if (NUNCHUK_INIT) {
    uint8_t addr[1] = {0x00};
    i2c_senddata(0x52, addr, 1);
    nano_wait(200000);
    i2c_recvdata(0x52, buffer, 8);
  }
}

void print_nunchuk_xy(int x, int y) {
  if(NUNCHUK_INIT) {
    uint8_t buffer[8] = {0};
    memset(buffer, 0, 8);
    read_nunchuk(buffer);
    char output_x[10] = "x:   ";
    char output_y[10] = " y:   ";
    char full_out[20] = "";
    LCD_DrawString(100,100,0xF000,0x0000, "           ", 16, 0);
    int n_x = buffer[0];
    int n_y = buffer[1];
    itoa(n_x, &output_x[2], 10);
    itoa(n_y, &output_y[3], 10);
    strcat(full_out, output_x);
    strcat(full_out, output_y);
    LCD_DrawString(x,y,0xF000,0x0000, full_out, 16, 0);
  }
}

void update_flags(void) {
  if(NUNCHUK_INIT) {
    uint8_t buffer[8] = {0};
    memset(buffer, 0, 8);
    read_nunchuk(buffer);
    if (buffer[0] > MID_VAL_X + DEAD_ZONE) {
        // Move right
        flg_mv_right = 1;
        flg_mv_left = 0;
    } else if (buffer[0] < MID_VAL_X - DEAD_ZONE) {
        // Move left
        flg_mv_left = 1;
        flg_mv_right = 0;
    } else {
        // Don't Move
        flg_mv_left = 0;
        flg_mv_right = 0;
    }
    if (buffer[5] & 0x02) {
        flg_c = 0;
    } else {
        flg_c = 1;
    }
    if (buffer[5] & 0x01) {
        flg_v = 0;
    } else {
        flg_v = 1;
    }
  }
}

void print_flags(int x, int y) {
    char output[17] = "R:  L:  C:  V:  ";
    output[2] = '0' + flg_mv_right;
    output[6] = '0' + flg_mv_left;
    output[10] = '0' + flg_c;
    output[14] = '0' + flg_v;
    LCD_DrawString(x,y,0xF000,0x0000, output, 16, 0);
}

//int main(void)
//{
////    msg[0] |= font['E'];
////    msg[1] |= font['C'];
////    msg[2] |= font['E'];
////    msg[3] |= font[' '];
////    msg[4] |= font['3'];
////    msg[5] |= font['6'];
////    msg[6] |= font['2'];
////    msg[7] |= font[' '];
//
////    asm volatile ("bl autotest" : :);
////    autotest();
//    // LED array SPI
////    setup_spi2_dma();
////    enable_spi2_dma();
////    init_spi2();
//
//    // This LAB
//
//    // 2.2 Initialize I2C
//    init_i2c();
////
////     2.3 Example code for testing
////    for(;;) {
////        i2c_waitidle();
////        i2c_start(GPIOEX_ADDR,0,0);
////        i2c_clearnack();
////        i2c_stop();
////    }
//
////    i2c_init();
////    for(;;) {
////        i2c_waitidle();
////        i2c_start(0x20,0,0);
////        i2c_clearnack();
////        i2c_stop();
////    }
////    for(;;) {
////        uint8_t data[2] = { 0x00, 0xff };
////        i2c_senddata(0x20, data, 2);
////    }
//
////    for(;;) {
////        uint8_t data[2] = { 0x00, 0x00 };
////        i2c_senddata(0x20, data, 1); // Select IODIR register
////        i2c_recvdata(0x20, data, 1);
////    }
////
////    // 2.5 Expander setup
//    init_expander();
//    init_tim7();
////
////    // 2.6 Interface for reading/writing memory.
//    serial_ui();
////
//    for(;;) {
//        uint8_t key = get_keypress();
//        append_display(font[key]);
//    }
//}

const char font[] = {
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x00, // 32: space
    0x86, // 33: exclamation
    0x22, // 34: double quote
    0x76, // 35: octothorpe
    0x00, // dollar
    0x00, // percent
    0x00, // ampersand
    0x20, // 39: single quote
    0x39, // 40: open paren
    0x0f, // 41: close paren
    0x49, // 42: asterisk
    0x00, // plus
    0x10, // 44: comma
    0x40, // 45: minus
    0x80, // 46: period
    0x00, // slash
    // digits
    0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x67,
    // seven unknown
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    // Uppercase
    0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71, 0x6f, 0x76, 0x30, 0x1e, 0x00, 0x38, 0x00,
    0x37, 0x3f, 0x73, 0x7b, 0x31, 0x6d, 0x78, 0x3e, 0x00, 0x00, 0x00, 0x6e, 0x00,
    0x39, // 91: open square bracket
    0x00, // backslash
    0x0f, // 93: close square bracket
    0x00, // circumflex
    0x08, // 95: underscore
    0x20, // 96: backquote
    // Lowercase
    0x5f, 0x7c, 0x58, 0x5e, 0x79, 0x71, 0x6f, 0x74, 0x10, 0x0e, 0x00, 0x30, 0x00,
    0x54, 0x5c, 0x73, 0x7b, 0x50, 0x6d, 0x78, 0x1c, 0x00, 0x00, 0x00, 0x6e, 0x00
};
