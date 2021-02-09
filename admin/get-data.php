<?php
    // Get device
    $device = '';
    if(!empty($_GET['device'])){
        $device = $_GET['device'];
    }
    
    // Connect to database
    $servername = "localhost";
    $username = "solartherm";
    $password = "solartherm";
    $dbname = "solartherm";

    // Create connection
    $conn = new mysqli($servername, $username, $password, $dbname);
    
    // Check connection
    if ($conn->connect_error) {
        die("Database Connection failed: " . $conn->connect_error);
    }

    // SQL query
    $sql = 'SELECT * FROM `sensors` WHERE `name`="' . $device . '" AND `TimeStamp` >= NOW() - INTERVAL 1 DAY ORDER BY TimeStamp ASC';

    $result = $conn->query($sql);

    $row = '';
    if ($result->num_rows > 0) {
        $counter = $result->num_rows;

        $dataarray = ['lastupdate'=>'','name'=>'','temperature'=>'','humidity'=>'','pressure'=>'','altitude'=>'','battery'=>'','temphistory'=>[],'humhistory'=>[]];
        
        $i = 1;
        // output data of each row
        while($row = $result->fetch_assoc()) {
            //var_dump($row);
            if($i == $counter) {
                $dataarray['lastupdate'] = $row["TimeStamp"];
                $dataarray['name'] = $row["name"];
                $dataarray['temperature'] = $row["temperature"];
                $dataarray['humidity'] = $row["humidity"];
                $dataarray['pressure'] = $row["pressure"];
                $dataarray['altitude'] = $row["altitude"];
                $dataarray['battery'] = $row["battery"];
            }

            array_push($dataarray['temphistory'],$row["temperature"]);
            array_push($dataarray['humhistory'],$row["humidity"]);

            $i++;
        }
    }/* else {
        echo "0 results";
    }*/

    $conn->close();

    if(!empty($dataarray)) {
        echo json_encode($dataarray);
    }
?>