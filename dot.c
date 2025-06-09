/* dot.c 리팩토링: 선풍기 날개 애니메이션 통합 */

#include <dot.h>
#include "usart.h"
#include "stdio.h"
#include "string.h"





uint8_t smile[8] = {
    0b11111111, 0b10111101, 0b01011010, 0b11111111,
    0b11111111, 0b01111110, 0b01111110, 0b10000001
};
uint8_t smile_1[8] = {
    0b11111111, 0b10111101, 0b01011010, 0b11111111,
    0b11111111, 0b01111110, 0b01111110, 0b10000001
};

uint8_t sad[8] = {
    0b00011000, 0b11111111, 0b11111111, 0b10111101,
    0b11111111, 0b10000001, 0b01111110, 0b01111110
};

uint8_t fan_frame_1[8] =
{
		0b11110111,
		0b11110111,
		0b11110111,
		0b00000111,
		0b11100000,
		0b11101111,
		0b11101111,
		0b11101111
};

uint8_t fan_frame_2[8] =
{
		0b10111111,
		0b11011110,
		0b11101101,
		0b11100011,
		0b11000111,
		0b10110111,
		0b01111011,
		0b11111101
};

uint8_t fan_frame_3[8] =
{
		0b01111110,
		0b10111101,
		0b11011011,
		0b11100111,
		0b11100111,
		0b11011011,
		0b10111101,
		0b01111110
};


uint8_t flame_frame_1[8] =
{
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11001111,
		0b10110011,
		0b10011001,
		0b11100111
};

uint8_t flame_frame_2[8] =
{
		0b11111111,
		0b11111111,
		0b11001111,
		0b11010011,
		0b10011001,
		0b00111100,
		0b10111100,
		0b11000011

};

uint8_t flame_frame_3[8] =
{
	0b11111011,
	0b11111001,
	0b11010101,
	0b10101100,
	0b01111110,
	0b01111110,
	0b10111101,
	0b11000011

};


uint8_t humidification_frame_1[8] =
{
	0b10111110,
	0b10110110,
	0b10110110,
	0b11110111,
	0b11111111,
	0b11011111,
	0b11011101,
	0b11011101

};

uint8_t humidification_frame_2[8] =
{
	0b11111111,
	0b01111110,
	0b01110110,
	0b01110110,
	0b11110111,
	0b11111111,
	0b11011111,
	0b11011101

};


uint8_t humidification_frame_3[8] =
{
	0b11111111,
	0b11111111,
	0b01111101,
	0b01111101,
	0b01110101,
	0b11110111,
	0b11110111,
	0b11011111

};

uint8_t humidification_frame_4[8] =
{
	0b11111111,
	0b11111111,
	0b11111111,
	0b01111101,
	0b01111101,
	0b01110101,
	0b11110111,
	0b11110111

};


uint8_t dehumidification_frame_1[8]=
{
		0b11111111,
		0b11100111,
		0b10000001,
		0b10000001,
		0b00000000,
		0b00000000,
		0b10000001,
		0b11000011,
};

uint8_t dehumidification_frame_2[8]=
{
		0b11111111,
		0b11111111,
		0b11111111,
		0b10000001,
		0b00000000,
		0b00000000,
		0b10000001,
		0b11000011,
};

uint8_t dehumidification_frame_3[8]=
{
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b11111111,
		0b00000000,
		0b10000001,
		0b11000011,
};





uint8_t* smile_frames[] = { smile, smile_1 };
uint8_t* fan_frames[] = { fan_frame_1, fan_frame_2, fan_frame_3};
uint8_t* flame_frames[] = {flame_frame_1, flame_frame_2, flame_frame_3};
uint8_t* humidification_frames[] ={humidification_frame_1, humidification_frame_2, humidification_frame_3, humidification_frame_4};
uint8_t* dehumidification_frames[] = {dehumidification_frame_1, dehumidification_frame_2, dehumidification_frame_3};

// 선풍기 켜지고, 가습기 켜짐.
uint8_t* fan_and_humidification_frames[] ={fan_frame_1, fan_frame_2, fan_frame_3,fan_frame_1,fan_frame_2,
		humidification_frame_1, humidification_frame_2, humidification_frame_3, humidification_frame_4};


// 히터켜지고, 제습기 켜짐
uint8_t* flame_and_dehumidification_frames[] ={flame_frame_1, flame_frame_2, flame_frame_3,
		dehumidification_frame_3, dehumidification_frame_2, dehumidification_frame_1 };


// 선풍기, 제습 켜짐
uint8_t* fan_and_dehumidification_frames[] = {fan_frame_1, fan_frame_2, fan_frame_3,fan_frame_1, fan_frame_2,
		dehumidification_frame_3, dehumidification_frame_2, dehumidification_frame_1
};

// 히터, 가습기 켜짐
uint8_t* flame_and_humidification_frames[] ={flame_frame_1, flame_frame_2, flame_frame_3,
		humidification_frame_3, humidification_frame_2, humidification_frame_1 };



const uint8_t fan_frame_count = 3;
const uint8_t flame_frame_count =3;
const uint8_t smile_frame_count =2;
const uint8_t dehumidification_frame_count =3;
const uint8_t humidification_frame_count=4;

//선풍기, 가습기
const uint8_t fan_and_humidification_frame_count = 9;
// 히터, 제습기
const uint8_t flame_and_dehumidification_frame_count = 6;
// 선풍기, 제습 켜짐
const uint8_t fan_and_dehumidification_frame_count = 8;

// 히터 가습기
const uint8_t flame_and_humidification_frame_count = 6;

uint8_t fan_frame_index = 0;
uint8_t flame_frame_index =0;
uint8_t humidification_frame_index =0;
uint8_t dehumidification_frame_index =0;
uint8_t smile_frame_index = 0;

//선풍기, 가습기
uint8_t fan_and_humidification_frame_index =0;
// 히터, 제습기
uint8_t flame_and_dehumidification_frame_index =0;
// 선풍기, 제습 켜짐
uint8_t fan_and_dehumidification_frame_index =0;

// 히터, 가습기
uint8_t flame_and_humidification_frame_index =0;


int current_row = 0;

void shiftOut(GPIO_TypeDef *port, uint16_t dataPin, uint16_t clockPin, uint8_t data) {
    for (int i = 7; i >= 0; i--) {
        HAL_GPIO_WritePin(port, dataPin, (data >> i) & 0x01 ? GPIO_PIN_SET : GPIO_PIN_RESET);
        HAL_GPIO_WritePin(port, clockPin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(port, clockPin, GPIO_PIN_RESET);
    }
}

void display_pattern_rowwise(uint8_t pattern[8]) {
    uint8_t col = ~(1 << current_row);
    uint8_t row = pattern[current_row];

    HAL_GPIO_WritePin(GPIO_PORT, LATCH_PIN, GPIO_PIN_RESET);
    shiftOut(GPIO_PORT, DATA_PIN, CLOCK_PIN, col);
    shiftOut(GPIO_PORT, DATA_PIN, CLOCK_PIN, ~row);
    HAL_GPIO_WritePin(GPIO_PORT, LATCH_PIN, GPIO_PIN_SET);

    current_row = (current_row + 1) % 8;
}

void animate_fan_pattern_rowwise(void) {
    display_pattern_rowwise(fan_frames[fan_frame_index]);
    static uint32_t last_frame_time = 0;
    if (HAL_GetTick() - last_frame_time >=300) {
        fan_frame_index = (fan_frame_index + 1) % fan_frame_count;
        last_frame_time = HAL_GetTick();
    }
}


void animate_flame_pattern_rowwise(void) {
    display_pattern_rowwise(flame_frames[flame_frame_index]);
    static uint32_t last_frame_time = 0;
    if (HAL_GetTick() - last_frame_time >= 300) {
        flame_frame_index = (flame_frame_index + 1) % flame_frame_count;
        last_frame_time = HAL_GetTick();
    }
}

void animate_smile_pattern_rowwise(void) {
    display_pattern_rowwise(smile_frames[smile_frame_index]);
    static uint32_t last_frame_time = 0;
    if (HAL_GetTick() - last_frame_time >=300) {
        smile_frame_index = (smile_frame_index + 1) % smile_frame_count;
        last_frame_time = HAL_GetTick();
    }
}

void animate_humidification_pattern_rowwise(void) {
    display_pattern_rowwise(humidification_frames[humidification_frame_index]);
    static uint32_t last_frame_time = 0;
    if (HAL_GetTick() - last_frame_time >= 300) {
    	humidification_frame_index = (humidification_frame_index + 1) % humidification_frame_count;
        last_frame_time = HAL_GetTick();
    }
}

void animate_dehumidification_pattern_rowwise(void) {
    display_pattern_rowwise(dehumidification_frames[dehumidification_frame_index]);
    static uint32_t last_frame_time = 0;
    if (HAL_GetTick() - last_frame_time >= 300) {
    	dehumidification_frame_index = (dehumidification_frame_index + 1) % dehumidification_frame_count;
        last_frame_time = HAL_GetTick();
    }
}
// 선풍기, 가습기
void animate_fan_and_humidification_pattern_rowwise(void) {
    display_pattern_rowwise(fan_and_humidification_frames[fan_and_humidification_frame_index]);
    static uint32_t last_frame_time = 0;
    if (HAL_GetTick() - last_frame_time >= 300)
    {
    	fan_and_humidification_frame_index = (fan_and_humidification_frame_index + 1) % fan_and_humidification_frame_count;
    	last_frame_time = HAL_GetTick();
    }
}

// 히터, 제습기
void animate_flame_and_dehumidification_pattern_rowwise(void)
{
    display_pattern_rowwise(flame_and_dehumidification_frames[flame_and_dehumidification_frame_index]);
    static uint32_t last_frame_time = 0;
    if (HAL_GetTick() - last_frame_time >= 300)
    {
    	flame_and_dehumidification_frame_index = (flame_and_dehumidification_frame_index + 1) % flame_and_dehumidification_frame_count;
    	last_frame_time = HAL_GetTick();
    }
}

// 선풍기, 제습기
void animate_fan_and_dehumidification_pattern_rowwise(void)
{
    display_pattern_rowwise(fan_and_dehumidification_frames[fan_and_dehumidification_frame_index]);
    static uint32_t last_frame_time = 0;
    if (HAL_GetTick() - last_frame_time >= 300)
    {
    	fan_and_dehumidification_frame_index = (fan_and_dehumidification_frame_index + 1) % fan_and_dehumidification_frame_count;
    	last_frame_time = HAL_GetTick();
    }

}

// 히터 가습기
void animate_flame_and_humidification_pattern_rowwise(void)
{
    display_pattern_rowwise(flame_and_humidification_frames[flame_and_humidification_frame_index]);
    static uint32_t last_frame_time = 0;
    if (HAL_GetTick() - last_frame_time >= 300)
    {
    	flame_and_humidification_frame_index = (flame_and_humidification_frame_index + 1) % flame_and_humidification_frame_count;
    	last_frame_time = HAL_GetTick();
    }
}





void test_columns_off(void) {
    HAL_GPIO_WritePin(GPIO_PORT, LATCH_PIN, GPIO_PIN_RESET);
    shiftOut(GPIO_PORT, DATA_PIN, CLOCK_PIN, 0xFF);
    shiftOut(GPIO_PORT, DATA_PIN, CLOCK_PIN, 0xFF);
    HAL_GPIO_WritePin(GPIO_PORT, LATCH_PIN, GPIO_PIN_SET);
}




