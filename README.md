# Error_Notification_IOT

This system is an IOT system. Using an NodeMCU ESP8266 (main Node) for supervisor and 3 NodeMCUs for employee.

Node for employee includes keypad for they to push in the error code.

Then it will send that code and the machine ID to the main Node.

The main Node will show that error to LED 7 segments and send the code to Sever for display on Web and android App.

Team: Hoang Anh Quoc, Ngo Quoc Nhu, Bui Anh Khoa

ID github: mmmnic, ngonhu159, fxanhkhoa


How to add Database:

Open phpMyadmin

Import sql file in database folder

How to post/get/reset data


3 fields:

MACHINE_ID : ID of machine

MACHINE_INDEX: (1, 2 ,3) for 3 Machine in project

ERROR_INDEX: (1, 2 ,3) for 3 types of error in project

Just post to ErrorNoti.php

To find out history just open web page ErrorNoti.php

To Reset: use page Reset.php

Example: Reset.php?MACHINE_INDEX=3 (Reset machine 3)

