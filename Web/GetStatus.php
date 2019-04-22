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
      $sql = "SELECT MACHINE_INDEX, ERROR_INDEX FROM ERROR_MACHINE_STATUS";

      $result = $conn->query($sql);
      if ($result->num_rows > 0) {
         // output data of each row
         $jsonArray = array();
         // $myObj->machine_index = new stdClass();
         // $myObj->error_index = new stdClass();
         while($row = $result->fetch_assoc()) {
            //  echo $row["MACHINE_INDEX"].$row["ERROR_INDEX"];
            $myObj = new stdClass();
            $myObj->machine_index = $row["MACHINE_INDEX"];
            $myObj->error_index = $row["ERROR_INDEX"];
            array_push($jsonArray, $myObj);
         }
         } else {
            echo "0 results";
         }
         $myJson = json_encode($jsonArray);
         echo $myJson;
         $conn->close();
   }
?>