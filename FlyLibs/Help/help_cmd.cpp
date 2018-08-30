
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


防火墙  --注册表HKEY_LOCAL_MACHINE\SYSTEM\ControlSet001\Services\SharedAccess\Parameters\FirewallPolicy\FirewallRules

--程序
--- netsh advfirewall firewall add rule name = "qq" dir = out program = "C:\Program Files (x86)\Tencent\QQ\Bin\qq.exe"  security = authenticate action = allow  //禁用qq联网
--- netsh advfirewall firewall delete rule name = "qq"  //取消qq禁用网络
--端口
--- netsh advfirewall firewall add rule name = "web80" dir = out protocol = tcp action = block //禁用80端口

netsh firewall show allowedprogram //查看防火墙放行的程序 

netsh firewall set portopening TCP 445 ENABLE //打开445端口 

netsh firewall set portopening TCP 3389 ENABLE //不用说了，放行终端啦。。 

netsh firewall delete allowedprogram C : \A.exe //删除放行程序A.exe 

netsh firewall set allowedprogram C : \A.exe A ENABLE //添加程序C盘下的A.exe并放行 

netsh firewall add allowedprogram C : \A.exe A ENABLE //添加程序C盘下的A.exe并放行　 