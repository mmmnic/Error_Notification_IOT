# ITProjectSolution
### Team: Hoang Anh Quoc, Ngo Quoc Nhu, Bui Anh Khoa
### ID github: mmmnic, ngonhu159, fxanhkhoa

__*How to add Database*__
  - Open phpMyadmin
  - Import sql file in database folder
  
__*How to post/get/reset data*__
  - 3 fields:
    + MACHINE_ID : ID of machine
    + MACHINE_INDEX: (1, 2 ,3) for 3 Machine in project
    + ERROR_INDEX: (1, 2 ,3) for 3 types of error in project
  - Just post to ErrorNoti.php
  - To find out history just open web page ErrorNoti.php
  - To Reset: use page Reset.php
    + Example: Reset.php?MACHINE_INDEX=3    (Reset machine 3)
