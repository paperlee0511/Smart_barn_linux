#ifndef INC_DOT_ANI_H_
#define INC_DOT_ANI_H_

#include "main.h"
#include "gpio.h"

#define DATA_PIN   GPIO_PIN_1
#define CLOCK_PIN  GPIO_PIN_2
#define LATCH_PIN  GPIO_PIN_3
#define GPIO_PORT  GPIOC



// ✅ 외부 패턴 선언
extern uint8_t smile[8];
extern uint8_t smile_1[8];
extern uint8_t sad[8];
extern uint8_t blank[8];

// 선풍기 프레임
extern uint8_t fan_frame_1[8];
extern uint8_t fan_frame_2[8];
extern uint8_t fan_frame_3[8];

// 불꽃 프레임
extern uint8_t flame_frame_1[8];
extern uint8_t flame_frame_2[8];
extern uint8_t flame_frame_3[8];

// 가습기 프레임
extern uint8_t humidification_frame_1[8];
extern uint8_t humidification_frame_2[8];
extern uint8_t humidification_frame_3[8];
extern uint8_t humidification_frame_4[8];

// 제습기 프레임
extern uint8_t dehumidification_frame_1[8];
extern uint8_t dehumidification_frame_2[8];
extern uint8_t dehumidification_frame_3[8];
extern uint8_t dehumidification_frame_4[8];

// 프레임 배열
extern uint8_t* fan_frames[];
extern uint8_t* flame_frames[];
extern uint8_t* smile_frames[];
extern uint8_t* humidification_frames[];
extern uint8_t* dehumidification_frames[];
extern uint8_t* fan_and_humidification_frames[];
extern uint8_t* flame_and_dehumidification_frames[];
extern uint8_t* fan_and_dehumidification_frames[];
extern uint8_t* flame_and_humidification_frames[];

// 프레임 개수
extern const uint8_t smile_frame_count;
extern const uint8_t fan_frame_count;
extern const uint8_t flame_frame_count;
extern const uint8_t humidification_count;
extern const uint8_t dehumidification_count;
extern const uint8_t fan_and_humidification_frame_count;
extern const uint8_t flame_and_dehumidification_frame_count;
extern const uint8_t fan_and_dehumidification_frame_count;
extern const uint8_t flame_and_humidification_frame_count;



// 기본 함수
void shiftOut(GPIO_TypeDef *port, uint16_t dataPin, uint16_t clockPin, uint8_t data);
void test_columns_off(void);

#endif /* INC_DOT_ANI_H_ */
