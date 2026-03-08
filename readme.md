# GPIO_output

cubemx：

GPIO output level 设置初始电平

GPIO mode: 
- output push pull 推挽输出——可主动输出高电平和低电平  
- output open drain 开漏输出——只能主动输出低电平，高电平需要外部上拉

>PC13（板载LED）通常配置为开漏输出，因为其外部接有上拉电阻

Maximum output speed（控制GPIO输出驱动能力）：
- Low：适合LED等低速信号
- Medium / High：适合SPI、PWM等高速信号


```c
HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET/*GPIO_PIN_RESET*/); //GPIO分组，pin, 状态
HAL_Delay(500); //时间ms
```

# GPIO_input

### GPIO pull-up/down: 
pull up 上拉

- 松按键——高电平
- 按按键——低电平

pull down 下拉
- 反之

no pull up and pull down 浮空

```c
HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7); //GPIO分组，pin
HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_7); //GPIO分组，pin
HAL_GetTick(); //返回系统启动以来经过的毫秒数
```

### 非阻塞按键消抖
原理：检测到按键按下后等待10ms（机械按键抖动时间一般为 5~20ms），再次确认按键状态。
如果仍然按下，则认为按键有效。
```c
uint8_t key_processed = 0;
uint32_t debounce_time = 0;

while (1)
{
    if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12) == GPIO_PIN_RESET)
    {
        if (!key_processed)              // 如果还未处理
        {
            if (debounce_time == 0)      // 首次进入，记录时间
                debounce_time = HAL_GetTick();
            else if (HAL_GetTick() - debounce_time >= 10) // 已持续10ms
            {
                // 再次读取确认（可选，但推荐）
                if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12) == GPIO_PIN_RESET)
                {
                    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13); // 执行动作
                    key_processed = 1;                       // 标记已处理
                }
                debounce_time = 0; // 复位计时，准备下一次按下
            }
        }
    }
    else                                          // 按键释放
    {
        key_processed = 0;   // 清除标志，允许下一次按下
        debounce_time = 0;   // 计时器清零，避免残留值影响下次
    }

    // 其他任务...
}
```

# EXTI (External Interrupt) 外部中断

抢占优先级Preemption > 响应优先级Sub

Time base: System tick timer 要优先于 EXTI

GPIO mode: 
- External Interrupt Mode with Rising...            上升沿触发中断
- External Interrupt Mode with Falling...           下降沿触发中断
- External Interrupt Mode with Rising/Falling...    上升/下降都沿触发中断

```c
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) //中断回调函数
{
  if (GPIO_Pin == GPIO_PIN_?)
  {
    //执行动作
  }
}
```

# UART 串口

### USART

Mode: Asyn...异步模式

Baud Rate: 波特率（一般设为115200）

```c
HAL_UART_Transmit(&huart2, message, strlen(message), HAL_MAX_DELAY);  //串口阻塞发送函数
HAL_UART_Transmit_IT(&huart2, message, strlen(message));  //串口中断发送函数


```

# 定时器 Timers

## Base Timer

### TIM
- 高级定时器：TIM1, TIM8
- 通用定时器：TIM2 - TIM5
- 基本定时器：TIM6, TIM7

### 预分频器 Prescaler
- 0不分频
- 1二分频：每2个脉冲输出1个脉冲
- 2三分频：每3个脉冲输出1个脉冲
- n-1：每n个脉冲输出1个脉冲

### 计数器
计预分频后的脉冲数

### 自动重装载寄存器 Counter Period
m-1：定时m个脉冲（从0数到m-1）

```text
eg:
72MHz下
预分频器7200-1
72MHz / 7200 = 10000Hz
自动重装载寄存器10000-1
10000 / 10000 = 1Hz
定时1秒（^_^）
```
---
```c
HAL_TIM_Base_Start(&htim3);    //基本定时器启动函数
__HAL_TIM_GET_COUNTER(&htim3); //获取计数器值
/*双下划线开头全大写函数都是偏底层的*/
HAL_TIM_Base_Start_IT(&htim3); //基本定时器中断启动函数
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) //定时器中断回调函数
{
    if (htim == &htim3){

    }
}
```


## PWM_light

定时器设置为输出比较模式中的PWM模式，输出脉冲

```c
HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);        //PWM启动函数
__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, i); //设置比较寄存器
```

## Encoder 编码器

### TIM设置Combined Channels：
Encoder mode

默认使用 TI1 + TI2 双边沿检测，一次完整编码器脉冲可能计数 ±2 ,可以设置预分频器Prescaler为1,二分频后变化值为1

改变上升沿和下降沿可以变换方向

```c
HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_ALL); //编码器启动函数

__HAL_TIM_SET_COUNTER(&htim1, 100); //设置计数器
```

# 遇到的问题

>WARN: ST-Link is not in the DFU mode. Please restart it and retry.

how to fix: 重新插ST-Link