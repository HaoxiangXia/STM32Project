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

---
---

>WARN: ST-Link is not in the DFU mode. Please restart it and retry.

fix: 重新插ST-Link