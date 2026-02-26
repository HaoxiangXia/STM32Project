# GPIO_output

cubemx：

GPIO output level 设置初始电平

GPIO mode: 
- output push pull 推挽输出
- output open drain 开漏输出

>PC13采用开漏

Maximum output speed：
- low：led

```
HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET/*GPIO_PIN_RESET*/); GPIO分组，pin, 状态
HAL_Delay(500); 时间ms
```



---

>WARN: ST-Link is not in the DFU mode. Please restart it and retry.

fix: 重新插ST-Link