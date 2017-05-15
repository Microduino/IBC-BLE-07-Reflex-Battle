#define DEBUG       0       //串口调试功能  打开/关闭.
#define PIN_KEYA    6       //碰撞传感器A引脚.
#define PIN_KEYB    8       //碰撞传感器B引脚.
#define PIN_BUZZER  10      //蜂鸣器引脚.
#define PIN_LED     12      //彩灯引脚.
#define LED_NUM     2       //彩灯数量

#define LED_INTERVAL 800  //彩灯切换颜色的速率，数字1000即为1秒间隔，2000即为2秒间隔...
                           //减少这个数值，会增加游戏的难度，考验人的反应速度。
                           //尝试把这个值调整为200，挑战一下自己的反射神经
