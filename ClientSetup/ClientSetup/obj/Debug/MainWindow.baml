   M S B A M L   `   `   `  ����F  BClientSetup, Version=1.0.0.0, Culture=neutral, PublicKeyToken=null)clr-namespace:ClientSetupClientSetup  5   	       ClientSetup.MainWindow   5   	   \ XPresentationFramework, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35R NWindowsBase, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35W SPresentationCore, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35D 9http://schemas.microsoft.com/winfx/2006/xaml/presentation    5   	   R NSystem.Xaml, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089:x,http://schemas.microsoft.com/winfx/2006/xaml     5   	   @d2http://schemas.microsoft.com/expression/blend/2008     5   	   Jmc;http://schemas.openxmlformats.org/markup-compatibility/2006     5   	   +localclr-namespace:ClientSetup     5   	     � Title$  
MainWindow��5   	   $	��450��6   $	��800��6)   ���	#FF11101D � 
ResizeMode$ NoResize=�6M    � Icon$, &Images/icon_transparent_background.png��6c   .��5	      � .+�5
   
   �� $
��Left=�6   $��61��6+   $��	10,10,0,0q�67   $	��Top=�6J   $��64��6b   $,��&Images/icon_transparent_background.png��6m   �� 5   
   $ ��Setup your weather station��6   $
��Left=�65   $��	98,10,0,0q�6P   $	��Top=�6c   
�������$��61��6�   $	��539��6�   $��36(�6�   �� 5   
   $$��Select your device serial port��6   $
��Left=�69   $��	81,89,0,0q�6T   $	��Top=�6g   
�������$��16(�6�   �� 5   
   $��Insert your WiFi SSID��6   $
��Left=�60   $��137,125,0,0q�6K   $	��Top=�6`   
�������$��16(�6�   �� 5   
   $��Insert your WiFi Password��6   $
��Left=�64   $��103,161,0,0q�6O   $	��Top=�6d   
�������$��16(�6�   �� 5   
   -   5         ��Name$ btn_go��5      $��GO��6!   $
��Left=�6.   $��309,351,0,0q�6I   $	��Top=�6^   $	��177��6v   $��28��6�   $<��6/Weather-station-setup-program;component/Font/#Poppins)�6�   $��18(�6�   
�������
�������   RoundButton#���   6  5      �� 5   
   
�������$
��Left=�6!   $��2��6<   $��
171,76,0,0q�6G   
�������$	��Top=�6j   $	��447��6�   �� 5   
   -   5         ��Name$ cmb_serial_port��5      $
��Left=�6,   $��
338,89,0,0q�6G   $	��Top=�6[   $	��225��6s   $��25��6   $<��6/Weather-station-setup-program;component/Font/#Poppins)�6�   �� 5   
   -   5         ��Name$ txt_ssid��5      $
��Left=�6$   $��23��6?   $��338,127,0,0q�6K    �� TextWrapping$
 Wrap=�6`   $�� ��6t   $	��Top=�6|   $	��225��6�   $<��6/Weather-station-setup-program;component/Font/#Poppins)�6�   G� 5   
   -   5         G�Name$ passbox_password��5      $
��Left=�60   $	��Top=�6K   $��338,163,0,0q�6c   $	��225��6x   $<��6/Weather-station-setup-program;component/Font/#Poppins)�6�   $��23��6�   �� 5   
   $��Select your sensor��6   $
��Left=�6-   $��103,197,0,0q�6H   $	��Top=�6]   
�������$��16(�6�   �� 5   
   $��Insert your min value��6   $
��Left=�60   $��103,233,0,0q�6K   $	��Top=�6`   
�������$��16(�6�   �� 5   
   -   5        $ 
cmb_sensor��5      $
��Left=�6'   $��338,200,0,0q�6B   $	��Top=�6W   $	��225��6o    �� SelectedIndex$ 0��6{   �� 5   
   -   5        $ txt_min_value��5      $
��Left=�6)   $��23��6D   $��338,238,0,0q�6P   $
 Wrap=�6e   $�� ��6y   $	��Top=�6�   $	��225��6�   $<��6/Weather-station-setup-program;component/Font/#Poppins)�6�   6�   �� 5   
   -   5        $ txt_max_value��5      $
��Left=�6)   $��23��6D   $��338,274,0,0q�6P   $
 Wrap=�6e   $�� ��6y   $	��Top=�6�   $	��225��6�   $<��6/Weather-station-setup-program;component/Font/#Poppins)�6�   6�   �� 5   
   $��Insert your max value��6   $
��Left=�60   $��103,269,0,0q�6K   $	��Top=�6`   
�������$��16(�6�   �� 5   
   $"��Insert your paired device id��6   $
��Left=�67   $��103,308,0,0q�6R   $	��Top=�6g   
�������$��16(�6�   �� 5   
   -   5        $ txt_paired_device��5      $
��Left=�6-   $��23��6H   $��338,308,0,0q�6T   $
 Wrap=�6i   $�� ��6}   $	��Top=�6�   $	��225��6�   $<��6/Weather-station-setup-program;component/Font/#Poppins)�6�   5      5       