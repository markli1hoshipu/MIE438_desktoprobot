============================================
Android端发送命令与Arduino处理对照表
============================================

1. 方向控制命令 (均不改变LCD显示的表情)
---------------- 
FORWARD    - 前进       
BACKWARD   - 后退
LEFT       - 逆时针转
RIGHT      - 顺时针转
STOP       - 急停
FLEFT      - 前左转
FRIGHT     - 前右转
BLEFT      - 后左转
BRIGHT     - 后右转

2. 速度控制命令
----------------
SPEED:[0-100]  (示例: SPEED:75)
- 速度百分比值，0=停止，100=全速

3. 菜单命令 
----------------
MENU:Smile (只改变LCD显示的表情)
MENU:Frown (只改变LCD显示的表情)
MENU:Sleep (只改变LCD显示的表情)
MENU:Default (回到初始状态)
MENU:SakiSakiSaki (回到特殊状态)

============================================
注意事项
============================================
1. 所有命令以换行符(\n)结尾
2. 命令区分大小写
3. 建议添加串口调试输出
4. 未识别的命令应忽略或返回错误

        case LCD_EMOJI_FROWN:
            // 皱眉表情
            // 眼睛
            _tft.fillCircle(x-size/3, y-size/5, size/10, TFT_BLUE);
            _tft.fillCircle(x+size/3, y-size/5, size/10, TFT_BLUE);
            // 嘴巴 - 皱眉
            _tft.drawLine(x-size/3, y+size/5, x+size/3, y+size/5, TFT_BLUE);
            _tft.drawLine(x-size/3, y+size/5, x-size/5, y, TFT_BLUE);
            _tft.drawLine(x+size/3, y+size/5, x+size/5, y, TFT_BLUE);
            _tft.drawLine(x-size/5, y, x+size/5, y, TFT_BLUE);
            break;
            
        case LCD_EMOJI_SLEEP:
            // 睡眠表情
            // 闭眼睡觉
            _tft.drawLine(x-size/2, y-size/5, x-size/6, y-size/5, TFT_BLUE);
            _tft.drawLine(x+size/2, y-size/5, x+size/6, y-size/5, TFT_BLUE);
            // Z字符号表示睡眠
            _tft.drawLine(x+size/3, y-size/2, x+size/6, y-size/3, TFT_BLUE);
            _tft.drawLine(x+size/6, y-size/3, x+size/3, y-size/3, TFT_BLUE);
            _tft.drawLine(x+size/3, y-size/3, x+size/6, y-size/6, TFT_BLUE);
            // 嘴巴 - 小o形状
            _tft.drawCircle(x, y+size/3, size/6, TFT_BLUE);
            break;
            
        case LCD_EMOJI_SPECIAL:
            // 特殊表情 - SakiSakiSaki!
            // 星星眼
            _tft.drawLine(x-size/3-size/8, y-size/5-size/8, x-size/3+size/8, y-size/5+size/8, TFT_BLUE);
            _tft.drawLine(x-size/3-size/8, y-size/5+size/8, x-size/3+size/8, y-size/5-size/8, TFT_BLUE);
            _tft.drawLine(x+size/3-size/8, y-size/5-size/8, x+size/3+size/8, y-size/5+size/8, TFT_BLUE);
            _tft.drawLine(x+size/3-size/8, y-size/5+size/8, x+size/3+size/8, y-size/5-size/8, TFT_BLUE);
            // 兴奋的嘴巴
            _tft.fillRect(x-size/3, y+size/5, size*2/3, size/5, TFT_RED);
            break;