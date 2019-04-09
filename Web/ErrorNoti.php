<!DOCTYPE html>
<html lang="en">
    <head>
        <title>IT_PROJECT_SOLUTION</title>
        <meta charset="utf-8">
        <meta name="viewport" content="width=device-width, initial-scale=1">
        <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.4.0/css/bootstrap.min.css">
        <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js"></script>
        <script src="https://maxcdn.bootstrapcdn.com/bootstrap/3.4.0/js/bootstrap.min.js"></script>
        <script src="https://cdn.datatables.net/1.10.19/js/jquery.dataTables.min.js"></script>
        <script src="https://cdn.datatables.net/1.10.19/js/dataTables.bootstrap.min.js"></script>
        <script src="https://cdn.datatables.net/1.10.19/css/dataTables.bootstrap.min.css"></script>
        <script>
            $(document).ready(function(){
                $('#noti').DataTable();
            });
         </script>
    </head>
    <body>
        <div class="container">
        <h2>Form Post Data</h2>
        <form action="#" method="POST">
            <div class="form-group">
                <label for="TIME">TIME:</label>
                <input type="datetime-local" class="form-control" id="TIME" placeholder="Enter TIME" name="TIME">
            </div>
            <div class="form-group">
                <label for="MACHINE_ID">MACHINE_ID:</label>
                <input type="text" class="form-control" id="MACHINE_ID" placeholder="Enter MACHINE_ID" name="MACHINE_ID">
            </div>
            <div class="form-group">
                <label for="MACHINE_INDEX">MACHINE_INDEX:</label>
                <input type="text" class="form-control" id="MACHINE_INDEX" placeholder="Enter MACHINE_INDEX" name="MACHINE_INDEX">
            </div>
            <div class="form-group">
                <label for="ERROR_INDEX">ERROR_INDEX:</label>
                <input type="text" class="form-control" id="ERROR_INDEX" placeholder="Enter ERROR_INDEX" name="ERROR_INDEX">
            </div>
            <div class="form-group">
                <button type="submit" class="btn btn-success">Submit</button>
            </div>
        </form>
        </div>
    </body>
</html>

<div class="container">
    <h2>History</h2>
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
        if ($_SERVER["REQUEST_METHOD"] == "POST") 
        {
            if ((isset($_POST["MACHINE_ID"])) && (isset($_POST["MACHINE_INDEX"])) && (isset($_POST["ERROR_INDEX"])))
            {
               //$time = $_POST["TIME"]; // Nếu dùng input thì comment set time bên dưới lại
               $MACHINE_ID = $_POST["MACHINE_ID"]; // ID máy
               $MACHINE_INDEX = $_POST["MACHINE_INDEX"]; // Máy số
               $ERROR_INDEX = $_POST["ERROR_INDEX"]; // 

               date_default_timezone_set("Asia/Ho_Chi_Minh"); // Set timezone
               $time = date('Y-m-d H:i:s');

               $sql = "INSERT INTO ERROR_NOTI (History_time, MACHINE_INDEX, ERROR_INDEX, MACHINE_ID) 
                     VALUES ('$time', $MACHINE_INDEX, $ERROR_INDEX, '$MACHINE_ID')";

               $message = "wrong answer";
               echo "<script type='text/javascript'>alert('$sql');</script>";
               if ($conn->query($sql) === TRUE) {
                  // echo "Record updated successfully";
               } else {
                  // echo "Error updating record: " . $conn->error;
               }
            }
        }
        
        // get data from table
        $sql = "SELECT * FROM ERROR_NOTI";
        $result = $conn->query($sql);
    ?>
    <table id="noti" class="table table-striped">
        <thead>
            <tr>
            <th>Time</th>
            <th>MACHINE_ID</th>
            <th>MACHINE_INDEX</th>
            <th>ERROR_INDEX</th>
            </tr>
        </thead>
        <tbody>
            <?php
                if ($result->num_rows > 0) {
                // output data of each row
                while($row = $result->fetch_assoc()) {
                    echo "<tr>";
                    echo "<td>".$row["History_time"]."</td>";
                    echo "<td>".$row["MACHINE_ID"]."</td>";
                    echo "<td>".$row["MACHINE_INDEX"]."</td>";
                    echo "<td>".$row["ERROR_INDEX"]."</td>";
                    echo "</tr>";
                    //echo "nodeid: " . $row["NODEID"]. " - temperature: " . $row["TEMPERATURE"]. " " . $row["HUMIDITY"]. "<br>";
                }
                } else {
                    echo "0 results";
                }
                $conn->close();
            ?>
        </tbody>
    </table>
    <div class="container">
        <h2>Functions</h2>
        <form action="DownloadExcel.php" method="POST">
            <button type="submit" class="btn btn-info">Download in Excel</button>
        </form>
    </div>
</div>