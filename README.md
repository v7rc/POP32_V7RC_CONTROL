# POP32_V7RC_CONTROL

這個免費的開發碼是用在iNex POP32 SUMOKIT. 你可以使用Arduino IDE環境，去進行編譯並且下載到你的iNex SUMOBOT上面，接著準備好V7RC APP開始去控制你的SUMOBOT吧！

#開始之前
請先準備一片WIFI或是藍牙開發版，或是HC-08藍牙接收板，連結到POP32的TX/RX 插座上。之後我們會詳述連結方式。如果還沒有準備也沒有關係，可以先用Serial Command進行測試。

#編譯與燒錄

1. 打開Arduino IDE的開發環境，打開這個目錄下的POP32_V7RC_CONTROL.ino檔案。
2. 連結iNex POP32 開發板，記得要接上電池或是離電池，這樣才可以順利啟動POP32 開發板。 (也可以參考手冊說明)
3. 編譯與下載韌體到POP32開發板上。

#在Ardiono IDE上面測試
1. 開啟Serial Monitor.
2. 設定傳輸速率115200。
3. 在輸入框中輸入 'SRV1000200015001500#' 按下Enter.
4. 你會發現POP32開發板上面的燈號，亮起兩顆綠燈，兩顆紅燈。 （大約0.5秒鐘後會熄滅)
5. 在輸入框中輸入 'SRV2000100015001500#' 按下Enter.
6. 你會發現POP32開發板上面的燈號，亮起兩顆綠燈，兩顆紅燈，與步驟4的燈號相反。 （大約0.5秒鐘後會熄滅)
7. 如果有看到燈號，表示測試成功。

#與V7RC連結
1. 打開V7RC APP (請到Google Play或是ios app store下載。
2. 打開設定（左上方的齒輪符號)
3. 選擇BLE設定，進入BLE選擇視窗。
4. 在清單中選擇你的藍牙裝置。
5. 選擇坦克模式。
6. 回到主畫面。
7. 操作畫面左下方的搖桿。
8. 檢視SUMOBOT是否有移動。

---
# POP32_V7RC_CONTROL

This free development code is designed for use with the iNex POP32 SUMOKIT. You can compile and upload it to your iNex SUMOBOT using the Arduino IDE environment. Once that’s done, you’re ready to control your SUMOBOT with the V7RC APP!

### #Before You Begin

Please prepare a WiFi or Bluetooth development board, or an HC-08 Bluetooth receiver module, and connect it to the TX/RX port of the POP32. We’ll explain the connection method in more detail later. If you don’t have the hardware yet, that’s fine—you can test using Serial Commands for now.

### #Compiling and Uploading

1. Open the Arduino IDE.
2. Open the `POP32_V7RC_CONTROL.ino` file in this folder.
3. Connect your iNex POP32 development board and make sure the battery is connected (or use external power) to properly start the POP32 board. (Refer to the manual if needed.)
4. Compile and upload the firmware to the POP32 board.

### #Testing in Arduino IDE

1. Open the Serial Monitor.
2. Set the baud rate to **115200**.
3. In the input field, type: `SRV1000200015001500#` and press **Enter**.

   * You’ll see two green LEDs and two red LEDs turn on for about 0.5 seconds.
4. Now type: `SRV2000100015001500#` and press **Enter**.

   * The LEDs will turn on again, but in the opposite configuration from step 3.
5. If you see the LED pattern change as expected, the test is successful.

### #Connecting to V7RC

1. Open the **V7RC APP** (Download it from Google Play or the iOS App Store).
2. Tap the **Settings** icon (gear icon at the top left).
3. Select **BLE Settings** to open the Bluetooth selection window.
4. Choose your Bluetooth device from the list.
5. Select **Tank Mode**.
6. Return to the main screen.
7. Use the joystick at the bottom left of the control screen.
8. Check if your SUMOBOT moves accordingly.

## 


