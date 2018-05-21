
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

//格式化
//echo Y | format % s / q



//系统
设备管理
--hdwwiz.cpl

屏幕分辨率
--desk.cpl

防火墙
--firewall.cpl

共享管理
--fsmgmt.msc   -shrpubw 创建共享文件夹

组策略
--gpedit.msc  --防火墙 - 出站规则 可阻止程序连接网络

IE属性
--inetcpl.cpl

画图
--mspaint  --snippingtool 截图工具