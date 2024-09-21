<!-- This is README.md for HotKeyProcessor -->
<!-- Made by Xinor-a                      -->

# HotKeyProcessor
## Before using...
These two files must be in the same directory.
- HotKey.txt
- HotKeyProcessor.exe

So you should copy `HotKey.txt(template file)` from `./template/` or create it to the same directory with `HotKeyProcessor.exe` at first.

## HotKey.txt
This is the example of HotKey.txt.
``` plaintext
CTRL,SHIFT,ALT,KEY,COMMAND
TRUE,TRUE,TRUE,A,notepad
```
Each line of HotKey.txt is a set of ***"HotKey"*** and ***"command"***, excluding the first line.
- ***"HotKey"*** is shortcut. It is combination of Ctrl, Shift, Alt key and one character.
- ***"Command"*** is the instruction sent to "cmd.exe" when its HotKey is pressed.

Each element in HotKey definition is separated by `,`.
Format is as below.
- the first element indicates if `Ctrl` key is used for the HotKey (TRUE/FALSE)
- the second element indicates if `Shift` key is used for the HotKey (TRUE/FALSE)
- the third element indicates if `Alt` key is used for the HotKey (TRUE/FALSE)
- the forth element specifies the character used for the HotKey
- the fifth element specifies the **command**

In the example above, the second line means that
- HotKey is `Ctrl + Shift + Alt + 'A'`.
- Command is `notepad`.

## 操作方法
If you want to reload HotKey.txt and apply new HotKey or reflect deletion some hotkey, you can use `Ctrl + Shift + Alt + 'R'`.
This is the system prepared hotkey, so you cannot use this HotKey combination for your custom command.

You can also `Ctrl + Shift + Alt + 'Q'` to quit this application, but cannot use for your custom command for the same reason.