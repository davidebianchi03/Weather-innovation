   M S B A M L   `   `   `  ���� X  TWeather-station-setup-program, Version=1.0.0.0, Culture=neutral, PublicKeyToken=nullM+clr-namespace:Weather_station_setup_programWeather_station_setup_program  .    (Weather_station_setup_program.MainWindow   \ XPresentationFramework, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35R NWindowsBase, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35W SPresentationCore, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35D 9http://schemas.microsoft.com/winfx/2006/xaml/presentation    R NSystem.Xaml, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089:x,http://schemas.microsoft.com/winfx/2006/xaml     @d2http://schemas.microsoft.com/expression/blend/2008     Jmc;http://schemas.openxmlformats.org/markup-compatibility/2006     =local+clr-namespace:Weather_station_setup_program       � Title$  
MainWindow��$	��450��$	��800�� � Icon$, &Images/icon_transparent_background.png�����	#FF11101D � 
ResizeMode$ NoResize=�.��� .+��� $
��Left=�$��61��$��	10,10,0,0q�$	��Top=�$��64��$,��&Images/icon_transparent_background.png���� $ ��Setup your weather station��$
��Left=�$��	98,10,0,0q�$	��Top=�
�������$��61��$	��539��$��36(��� $$��Select your device serial port��$
��Left=�$��
90,101,0,0q�$	��Top=�
�������$��16(��� $��Insert your WiFi SSID��$
��Left=�$��162,153,0,0q�$	��Top=�
�������$��16(��� $��Insert your WiFi Password��$
��Left=�$��119,207,0,0q�$	��Top=�
�������$��16(��� -    ��Name$ btn_go��$��GO��$
��Left=�$��309,297,0,0q�$	��Top=�$	��177��$��28��$<��6/Weather-station-setup-program;component/Font/#Poppins)�$��18(�
�������
�������   RoundButton#���   �� 
�������$
��Left=�$��2��$��
171,76,0,0q�
�������$	��Top=�$	��447���� -    ��Name$ cmb_serial_port��$
��Left=�$��351,105,0,0q�$	��Top=�$	��225��$��25��$<��6/Weather-station-setup-program;component/Font/#Poppins)��� -    ��Name$ txt_ssid��$
��Left=�$��23��$��351,157,0,0q� �� TextWrapping$
 Wrap=�$�� ��$	��Top=�$	��225��$<��6/Weather-station-setup-program;component/Font/#Poppins)�G� -    G�Name$ passbox_password��$
��Left=�$	��Top=�$��351,215,0,0q�$	��225��$<��6/Weather-station-setup-program;component/Font/#Poppins)�