<!DOCTYPE html>
<html lang="en">
    <head>
        <title>IT_PROJECT_SOLUTION</title>
        <meta charset="utf-8">
        <meta name="viewport" content="width=device-width, initial-scale=1">
        <link rel="shortcut icon" type="image/x-icon" href="icon.ico" />
        <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.4.0/css/bootstrap.min.css">
        <link rel="stylesheet" href="css/text.css">
        <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js"></script>
        <script src="https://maxcdn.bootstrapcdn.com/bootstrap/3.4.0/js/bootstrap.min.js"></script>
        <script src="https://cdn.datatables.net/1.10.19/js/jquery.dataTables.min.js"></script>
        <script src="https://cdn.datatables.net/1.10.19/js/dataTables.bootstrap.min.js"></script>
        <script src="https://cdn.datatables.net/1.10.19/css/dataTables.bootstrap.min.css"></script>
        <link rel="icon" type="image/png" href="images/icons/favicon.ico"/>
        <link rel="stylesheet" type="text/css" href="vendor/bootstrap/css/bootstrap.min.css">
        <link rel="stylesheet" type="text/css" href="fonts/font-awesome-4.7.0/css/font-awesome.min.css">
        <link rel="stylesheet" type="text/css" href="vendor/animate/animate.css">
        <link rel="stylesheet" type="text/css" href="vendor/select2/select2.min.css">
        <link rel="stylesheet" type="text/css" href="vendor/perfect-scrollbar/perfect-scrollbar.css">
        <link rel="stylesheet" type="text/css" href="css/util.css">
        <link rel="stylesheet" type="text/css" href="css/main.css">
        <script>
            $(document).ready(function(){
                $('#noti').DataTable();
                setInterval(function(){ 
                    location.reload();
                    }, 20000);
            });
         </script>
    </head>
    <body>
        <!-- <div class="container">
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
        </div> -->
    </body>
</html>
<body>
<h1 class='elegantshadow'>Error Notification Page</h1>
    <div class="container">
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
                //    echo "<script type='text/javascript'>alert('$sql');</script>";
                if ($conn->query($sql) == TRUE) {
                    // echo "Record updated successfully";
                } else {
                    // echo "Error updating record: " . $conn->error;
                }

                /////////////// Update Status /////////////////
                $sql = "UPDATE ERROR_MACHINE_STATUS SET ERROR_INDEX = $ERROR_INDEX WHERE MACHINE_INDEX = $MACHINE_INDEX";
                if ($conn->query($sql) == TRUE) {
                    // echo "Record updated successfully";
                    } else {
                        // echo "Error updating record: " . $conn->error;
                    }
                }
            }
            
            // get data from table
            $sql = "SELECT * FROM ERROR_NOTI ORDER BY History_time DESC";
            $result = $conn->query($sql);

            $sql_stt = "SELECT * FROM ERROR_MACHINE_STATUS";
            $result_stt = $conn->query($sql_stt);

            while($row_stt = mysqli_fetch_assoc($result_stt)){
                $data[] = $row_stt;
           }
        ?>
        <!-- STATUS -->
        <h2 style="text-align: center;"> STATUS </h2>
        <div class="col-sm-4">
            <?php
                $text_noti = "";
                // print_r($data[0]["ERROR_INDEX"]);
                if ($data[0]["ERROR_INDEX"] == 0){
                    echo "<div class=\"panel panel-success\"> \n";
                    $text_noti = "Working";
                }
                else if ($data[0]["ERROR_INDEX"] == 1){
                    echo "<div class=\"panel panel-warning\"> \n";
                    $text_noti = "Fixing";
                }
                else{
                    echo "<div class=\"panel panel-danger\"> \n";
                    $text_noti = "Error";
                }
            ?>
            <!-- <div class="panel panel-default"> -->
                <div class="panel-heading">Node 1</div>
                <div class="panel-body"><?php echo $text_noti ?></div>
            </div>
        </div>
        <div class="col-sm-4">
            <?php
                $text_noti = "";
                // print_r($data[0]["ERROR_INDEX"]);
                if ($data[1]["ERROR_INDEX"] == 0){
                    echo "<div class=\"panel panel-success\"> \n";
                    $text_noti = "Working";
                }
                else if ($data[1]["ERROR_INDEX"] == 1){
                    echo "<div class=\"panel panel-warning\"> \n";
                    $text_noti = "Fixing";
                }
                else{
                    echo "<div class=\"panel panel-danger\"> \n";
                    $text_noti = "Error";
                }
            ?>
            <!-- <div class="panel panel-default"> -->
                <div class="panel-heading">Node 2</div>
                <div class="panel-body"><?php echo $text_noti ?></div>
            </div>
        </div>
        <div class="col-sm-4">
            <?php
                $text_noti = "";
                // print_r($data[0]["ERROR_INDEX"]);
                if ($data[2]["ERROR_INDEX"] == 0){
                    echo "<div class=\"panel panel-success\"> \n";
                    $text_noti = "Working";
                }
                else if ($data[2]["ERROR_INDEX"] == 1){
                    echo "<div class=\"panel panel-warning\"> \n";
                    $text_noti = "Fixing";
                }
                else{
                    echo "<div class=\"panel panel-danger\"> \n";
                    $text_noti = "Error";
                }
            ?>
            <!-- <div class="panel panel-default"> -->
                <div class="panel-heading">Node 3</div>
                <div class="panel-body"><?php echo $text_noti ?></div>
            </div>
        </div>

        <!-- HISTORY TABLE -->
        <h2 style="text-align: center;"> HISTORY </h2>
        <div class="table100 ver2 m-b-110">
            <div class="table100-head">
                <table>
                    <thead>
                        <tr class="row100 head">
                            <th class="cell100 column1">Time</th>
                            <th class="cell100 column2">Node 1</th>
                            <th class="cell100 column3">Node 2</th>
                            <th class="cell100 column4">Node 3</th>
                        </tr>
                    </thead>
                </table>
            </div>
            <!-- BODY OF TABLE -->
            <div class="table100-body js-pscroll">
                <table>
                    <tbody>
                    <?php
                        if ($result->num_rows > 0) {
                        // output data of each row
                        while($row = $result->fetch_assoc()) {
                            
                            echo "<tr class=\"row100 body\"> \n";
                            echo "<td class=\"cell100 column1\">".$row["History_time"]."</td> \n";
                            if ($row["MACHINE_INDEX"] == "1"){
                                echo "<td class=\"cell100 column2\">".$row["ERROR_INDEX"]."</td> \n";
                                echo "<td class=\"cell100 column3\">"."0"."</td> \n";
                                echo "<td class=\"cell100 column4\">"."0"."</td> \n";
                            }
                            else if ($row["MACHINE_INDEX"] == "2"){
                                echo "<td class=\"cell100 column2\">"."0"."</td> \n";
                                echo "<td class=\"cell100 column3\">".$row["ERROR_INDEX"]."</td> \n";
                                echo "<td class=\"cell100 column4\">"."0"."</td> \n";
                            }
                            else if ($row["MACHINE_INDEX"] == "3"){
                                echo "<td class=\"cell100 column2\">"."0"."</td> \n";
                                echo "<td class=\"cell100 column3\">"."0"."</td> \n";
                                echo "<td class=\"cell100 column4\">".$row["ERROR_INDEX"]."</td> \n";
                            }
                            // echo "<td>".$row["MACHINE_ID"]."</td>";
                            
                            echo "</tr> \n";
                            //echo "nodeid: " . $row["NODEID"]. " - temperature: " . $row["TEMPERATURE"]. " " . $row["HUMIDITY"]. "<br>";
                        }
                        } else {
                            echo "0 results";
                        }
                        $conn->close();
                    ?>
                    </tbody>
                </table>
            </div>
        </div>
        <!-- Chú Thích -->
        <h2 style="text-align: center;"> Chú Thích </h2>
        <div class="table100 ver5 m-b-110">
            <div class="table100-head">
                <table>
                    <thead>
                        <tr class="row100 head">
                            <th class="cell100 column1">Error No.</th>
                            <th class="cell100 column2">Name Of Error</th>
                        </tr>
                    </thead>
                </table>
            </div>
            <div class="table100-body js-pscroll">
                <table>
                    <tbody>
                        <tr class="row100 body">
                            <td class="cell100 column1">0</td>
                            <td class="cell100 column2">Not Error</td>
                        </tr>
                        <tr class="row100 body">
                            <td class="cell100 column1">1</td>
                            <td class="cell100 column2">Is Being Fixed</td>
                        </tr>
                        <tr class="row100 body">
                            <td class="cell100 column1">2</td>
                            <td class="cell100 column2">Is Broken</td>
                        </tr>
                    </tbody>
                <table>
            </div>
        </div>
        <!-- FOR DOWNLOAD EXCEL -->
        <div class="container">
            <h2>Functions</h2>
            <form action="DownloadExcel.php" method="POST">
                <button type="submit" class="btn btn-info">Download in Excel</button>
            </form>
        </div>
    </div>
</body>