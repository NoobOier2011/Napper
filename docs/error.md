# This is the guide about Error


### Module Code
| Code | Module |
| ---- | ------ |
| 001 to 100  | Window Create |
| 101 to 200  | Webview 2 |
| 201 to 300  | File IO |
| 301 to 400  | Batch File |

----

## 001-100: Window Creation

### Error 001
**Description**: Failed to register window class

**Solutions**:
1. Restart the application
2. Check if another Napper instance is running
3. Reboot your system and try again

---

### Error 002
**Description**: Failed to create window

**Solutions**:
1. Close other programs to free resources
2. Restart Napper
3. Update graphics drivers

---

### Error 003
**Description**: Failed to initialize COM

**Solutions**:
1. Restart your computer
2. Run `sfc /scannow` to repair system files
3. Reinstall Windows updates

---

## 101-200: WebView2

### Error 101
**Description**: Failed to create WebView2 environment

**Solutions**:
1. Download and install WebView2 Runtime:
   https://developer.microsoft.com/microsoft-edge/webview2/
2. Run Napper as administrator
3. Update Windows 10/11 to the latest version

---

### Error 102
**Description**: Failed to create WebView2 controller

**Solutions**:
1. Restart Napper
2. Reinstall WebView2 Runtime
3. Clear WebView2 cache: `%LOCALAPPDATA%\Microsoft\Edge WebView`