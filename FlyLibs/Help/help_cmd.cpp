
//����
//cmd /c sc delete svName &sc.exe create svName type= own type= interact start= auto binPath=\"exe para\"
     
//���汳��
//cmd.exe /c echo y|reg add \"hkcu\\CONTROL PANEL\\desktop\" /v wallpaper /t reg_sz /d \"imagepath\"


//���뷨
//L"SYSTEM\\CurrentControlSet\\Control\\Keyboard Layouts\\"   
//L"Keyboard Layout\\Preload"

//��������
//cmd /c taskkill /F /IM %s

//������
//Software\\Microsoft\\Windows\\CurrentVersion\\Run

//��ʽ��
//echo Y | format % s / q



//ϵͳ
�豸����
--hdwwiz.cpl

��Ļ�ֱ���
--desk.cpl

����ǽ
--firewall.cpl

�������
--fsmgmt.msc   -shrpubw ���������ļ���

�����
--gpedit.msc  --����ǽ - ��վ���� ����ֹ������������

IE����
--inetcpl.cpl

��ͼ
--mspaint  --snippingtool ��ͼ����


����ǽ  --ע���HKEY_LOCAL_MACHINE\SYSTEM\ControlSet001\Services\SharedAccess\Parameters\FirewallPolicy\FirewallRules

--����
--- netsh advfirewall firewall add rule name = "qq" dir = out program = "C:\Program Files (x86)\Tencent\QQ\Bin\qq.exe"  security = authenticate action = allow  //����qq����
--- netsh advfirewall firewall delete rule name = "qq"  //ȡ��qq��������
--�˿�
--- netsh advfirewall firewall add rule name = "web80" dir = out protocol = tcp action = block //����80�˿�

netsh firewall show allowedprogram //�鿴����ǽ���еĳ��� 

netsh firewall set portopening TCP 445 ENABLE //��445�˿� 

netsh firewall set portopening TCP 3389 ENABLE //����˵�ˣ������ն������� 

netsh firewall delete allowedprogram C : \A.exe //ɾ�����г���A.exe 

netsh firewall set allowedprogram C : \A.exe A ENABLE //��ӳ���C���µ�A.exe������ 

netsh firewall add allowedprogram C : \A.exe A ENABLE //��ӳ���C���µ�A.exe�����С� 