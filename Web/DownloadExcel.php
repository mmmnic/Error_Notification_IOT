<?php
   // Include PHP Excel Library
   require("Classes/PHPExcel.php");
   $servername = "localhost";
   $username = "IT_PROJECT";
   $password = "12345678";
   $dbname = "IT_SOLUTION";

   // Create connection
   $conn = new mysqli($servername, $username, $password, $dbname);
   // Check connection
   if ($conn->connect_error) {
       die("Connection failed: " . $conn->connect_error);
   } 
   //Khởi tạo đối tượng
   $excel = new PHPExcel();
   ///////////////////////////////// NODE 1 //////////////////////////////

   $sql = "SELECT * FROM ERROR_NOTI WHERE MACHINE_INDEX = 1 ORDER BY History_time DESC";
   $result = $conn->query($sql);

   if ($result->num_rows > 0) {
      // output data of each row
      $array_xls = array();
      while($row = $result->fetch_assoc()) {
         $element_array = array($row["History_time"], $row["MACHINE_INDEX"], $row["ERROR_INDEX"]);
         array_push($array_xls, $element_array);
      }
      // print_r($array_xls);

      // Tạo 1 sheet mới
      $excel->createSheet(0);

      //Chọn trang cần ghi (là số từ 0->n)
      $excel->setActiveSheetIndex(0);

      //Tạo tiêu đề cho trang. (có thể không cần)
      $excel->getActiveSheet()->setTitle('Node 1');

      //Xét chiều rộng cho từng, nếu muốn set height thì dùng setRowHeight()
      $excel->getActiveSheet()->getColumnDimension('A')->setWidth(20);
      $excel->getActiveSheet()->getColumnDimension('B')->setWidth(20);
      $excel->getActiveSheet()->getColumnDimension('C')->setWidth(20);
      $excel->getActiveSheet()->getColumnDimension('D')->setWidth(20);

      //Xét in đậm cho khoảng cột
      $excel->getActiveSheet()->getStyle('A1:D1')->getFont()->setBold(true);

      $excel->getActiveSheet()->setCellValue('A1', 'Time');
      // $excel->getActiveSheet()->setCellValue('B1', 'Machine ID');
      $excel->getActiveSheet()->setCellValue('B1', 'Machine Index');
      $excel->getActiveSheet()->setCellValue('C1', 'Error Index');

      // dòng bắt đầu
      $numRow = 2;
      foreach ($array_xls as $row){
         $excel->getActiveSheet()->setCellValue('A' . $numRow, $row[0]);
         // $excel->getActiveSheet()->setCellValue('B' . $numRow, $row[1]);
         $excel->getActiveSheet()->setCellValue('B' . $numRow, $row[1]);
         $excel->getActiveSheet()->setCellValue('C' . $numRow, $row[2]);
         $numRow++;
      }      
   }
   else{
      $message = "Something wrong";
      echo "<script type='text/javascript'>alert('$message');</script>";
   }

   ///////////////////////////////// NODE 2 //////////////////////////////

   $sql = "SELECT * FROM ERROR_NOTI WHERE MACHINE_INDEX = 2 ORDER BY History_time DESC";
   $result = $conn->query($sql);

   if ($result->num_rows > 0) {
      // output data of each row
      $array_xls = array();
      while($row = $result->fetch_assoc()) {
         $element_array = array($row["History_time"], $row["MACHINE_INDEX"], $row["ERROR_INDEX"]);
         array_push($array_xls, $element_array);
      }
      // print_r($array_xls);

      //Khởi tạo đối tượng
      // $excel = new PHPExcel();

      $excel->createSheet(1);
      $excel->setActiveSheetIndex(1);

      //Tạo tiêu đề cho trang. (có thể không cần)
      $excel->getActiveSheet()->setTitle('Node 2');

      //Xét chiều rộng cho từng, nếu muốn set height thì dùng setRowHeight()
      $excel->getActiveSheet()->getColumnDimension('A')->setWidth(20);
      $excel->getActiveSheet()->getColumnDimension('B')->setWidth(20);
      $excel->getActiveSheet()->getColumnDimension('C')->setWidth(20);
      $excel->getActiveSheet()->getColumnDimension('D')->setWidth(20);

      //Xét in đậm cho khoảng cột
      $excel->getActiveSheet()->getStyle('A1:D1')->getFont()->setBold(true);

      $excel->getActiveSheet()->setCellValue('A1', 'Time');
      // $excel->getActiveSheet()->setCellValue('B1', 'Machine ID');
      $excel->getActiveSheet()->setCellValue('B1', 'Machine Index');
      $excel->getActiveSheet()->setCellValue('C1', 'Error Index');

      // dòng bắt đầu
      $numRow = 2;
      foreach ($array_xls as $row){
         $excel->getActiveSheet()->setCellValue('A' . $numRow, $row[0]);
         // $excel->getActiveSheet()->setCellValue('B' . $numRow, $row[1]);
         $excel->getActiveSheet()->setCellValue('B' . $numRow, $row[1]);
         $excel->getActiveSheet()->setCellValue('C' . $numRow, $row[2]);
         $numRow++;
      }      
   }
   else{
      $message = "Something wrong";
      echo "<script type='text/javascript'>alert('$message');</script>";
   }

   ///////////////////////////////// NODE 3 //////////////////////////////

   $sql = "SELECT * FROM ERROR_NOTI WHERE MACHINE_INDEX = 3 ORDER BY History_time DESC";
   $result = $conn->query($sql);

   if ($result->num_rows > 0) {
      // output data of each row
      $array_xls = array();
      while($row = $result->fetch_assoc()) {
         $element_array = array($row["History_time"], $row["MACHINE_INDEX"], $row["ERROR_INDEX"]);
         array_push($array_xls, $element_array);
      }
      // print_r($array_xls);

      //Khởi tạo đối tượng
      // $excel = new PHPExcel();

      $excel->createSheet(2);
      $excel->setActiveSheetIndex(2);

      //Tạo tiêu đề cho trang. (có thể không cần)
      $excel->getActiveSheet()->setTitle('Node 3');

      //Xét chiều rộng cho từng, nếu muốn set height thì dùng setRowHeight()
      $excel->getActiveSheet()->getColumnDimension('A')->setWidth(20);
      $excel->getActiveSheet()->getColumnDimension('B')->setWidth(20);
      $excel->getActiveSheet()->getColumnDimension('C')->setWidth(20);
      $excel->getActiveSheet()->getColumnDimension('D')->setWidth(20);

      //Xét in đậm cho khoảng cột
      $excel->getActiveSheet()->getStyle('A1:D1')->getFont()->setBold(true);

      $excel->getActiveSheet()->setCellValue('A1', 'Time');
      // $excel->getActiveSheet()->setCellValue('B1', 'Machine ID');
      $excel->getActiveSheet()->setCellValue('B1', 'Machine Index');
      $excel->getActiveSheet()->setCellValue('C1', 'Error Index');

      // dòng bắt đầu
      $numRow = 2;
      foreach ($array_xls as $row){
         $excel->getActiveSheet()->setCellValue('A' . $numRow, $row[0]);
         // $excel->getActiveSheet()->setCellValue('B' . $numRow, $row[1]);
         $excel->getActiveSheet()->setCellValue('B' . $numRow, $row[1]);
         $excel->getActiveSheet()->setCellValue('C' . $numRow, $row[2]);
         $numRow++;
      }      
   }
   else{
      $message = "Something wrong";
      echo "<script type='text/javascript'>alert('$message');</script>";
   }

   // Khởi tạo đối tượng PHPExcel_IOFactory để thực hiện ghi file
   // ở đây mình lưu file dưới dạng excel2007
   PHPExcel_IOFactory::createWriter($excel, 'Excel2007')->save('data.xlsx');

   // for download, do not print anything to page if download
   header('Content-Type: application/vnd.openxmlformats-officedocument.spreadsheetml.sheet');
   header('Content-Disposition: attachment;filename="data.xlsx"');
   header('Cache-Control: max-age=0');
   
   PHPExcel_IOFactory::createWriter($excel, 'Excel2007')->save('php://output');
?>