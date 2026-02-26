# GPIO_output

cubemx：

GPIO output level 设置初始电平

GPIO mode: 
- output push pull 推挽输出
- output open drain 开漏输出

>PC13采用开漏

Maximum output speed：
- low：led

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

# exit

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

# 定时器 Timers

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
自动重装载寄存器10000-1
定时1秒（^_^）
```
---
```c
HAL_TIM_Base_Start(&htim3);    //基本定时器启动函数
__HAL_TIM_GET_COUNTER(&htim3); //获取计数器值
/*双下划线开头全大写函数都是偏底层的*/
HAL_TIM_Base_Start_IT(&htim3); //基本定时器中断启动函数
void HAL_TIM_PeriodElapsedCallback(***Def *htim) //定时器中断回调函数
{
    if (htim == &htim3){

    }
}
```


# PWM_light
```c
HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);        //PWM启动函数
__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, i); //
```

---
---

>WARN: ST-Link is not in the DFU mode. Please restart it and retry.

fix: 重新插ST-Link