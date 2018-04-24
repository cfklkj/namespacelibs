
//服务
//cmd /c sc delete svName &sc.exe create svName type= own type= interact start= auto binPath=\"exe para\"
     
//桌面背景
//cmd.exe /c echo y|reg add \"hkcu\\CONTROL PANEL\\desktop\" /v wallpaper /t reg_sz /d \"imagepath\"


//输入法
//L"SYSTEM\\CurrentControlSet\\Control\\Keyboard Layouts\\"   
//L"Keyboard Layout\\Preload"

//结束进程
//cmd /c taskkill /F /IM %s

//启动项
//Software\\Microsoft\\Windows\\CurrentVersion\\Run