<?php
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
   
   // Update value
   if ($_SERVER["REQUEST_METHOD"] == "GET") {
      if ((isset($_GET["MACHINE_INDEX"]))){
         $MACHINE_INDEX = $_GET["MACHINE_INDEX"];

         $sql = "UPDATE ERROR_MACHINE_STATUS SET ERROR_INDEX = 0 WHERE MACHINE_INDEX = $MACHINE_INDEX";

         $result = $conn->query($sql);
      }
   }
?>