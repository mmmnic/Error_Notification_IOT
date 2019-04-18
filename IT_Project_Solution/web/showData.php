<!DOCTYPE html>
<html lang="en">
    <head>
        <title>IT_PROJECT_SOLUTION</title>
        <meta charset="utf-8">
        <meta name="viewport" content="width=device-width, initial-scale=1">
        <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.4.0/css/bootstrap.min.css">
        <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js"></script>
        <script src="https://maxcdn.bootstrapcdn.com/bootstrap/3.4.0/js/bootstrap.min.js"></script>
        <script>
            $(document).ready(function(){
                setInterval(function(){ 
                    location.reload();
                    }, 3000);
            });
            </script>
    </head>
    <body>
        <div class="container">
        <h2>Form Post Data</h2>
        <form action="#" method="POST">
            <div class="form-group">
                <label for="nodeid">NODE ID:</label>
                <input type="text" class="form-control" id="nodeid" placeholder="Enter NODE ID" name="nodeid">
            </div>
            <div class="form-group">
                <label for="temperature">TEMPERATURE:</label>
                <input type="text" class="form-control" id="temperature" placeholder="Enter temperature" name="temperature">
            </div>
            <div class="form-group">
                <label for="humidity">HUMIDITY:</label>
                <input type="text" class="form-control" id="humidity" placeholder="Enter humidity" name="humidity">
            </div>
            <button type="submit" class="btn btn-success">Submit</button>
        </form>
        </div>
    </body>
</html>

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
            if ((isset($_POST["nodeid"])) && (isset($_POST["temperature"])) && (isset($_POST["humidity"])))
            {
                $nodeid = $_POST["nodeid"];
                $temper = $_POST["temperature"];
                $humi = $_POST["humidity"];
                $sql = "UPDATE ENVIRONMENT set HUMIDITY = $humi , TEMPERATURE = $temper WHERE NODEID = '$nodeid'";
                if ($conn->query($sql) === TRUE) {
                    // echo "Record updated successfully";
                } else {
                    // echo "Error updating record: " . $conn->error;
                }
            }
        }
        
        // Insert to table
        $sql = "SELECT NODEID, TEMPERATURE, HUMIDITY FROM ENVIRONMENT";
        $result = $conn->query($sql);
    ?>
    <table class="table table-striped">
        <thead>
            <tr>
            <th>Node Name</th>
            <th>Temperature</th>
            <th>Humidity</th>
            </tr>
        </thead>
        <tbody>
            <?php
                if ($result->num_rows > 0) {
                // output data of each row
                while($row = $result->fetch_assoc()) {
                    echo "<tr>";
                    echo "<td>".$row["NODEID"]."</td>";
                    echo "<td>".$row["TEMPERATURE"]."</td>";
                    echo "<td>".$row["HUMIDITY"]."</td>";
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
</div>