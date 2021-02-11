<?php
    // Get json stream content
    $json = file_get_contents('php://input');
    
    if(!empty($json)) {
        // Decode json string
        $obj = json_decode($json);

        // If json is valid
        if(json_last_error() === JSON_ERROR_NONE) {
            $password = 'S47TnP3pp3R'; // password salt for password_hash

            // if api key is valid
            if(password_verify($password,$obj->apikey)) {
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

                // Get current date and time
                date_default_timezone_set('Europe/Berlin');
                $d = date("Y-m-d");
                $t = date("H:i:s");

                // If basic json objects are not empty
                if(!empty($obj->name) || !empty($obj->temperature) || !empty($obj->humidity) || !empty($obj->battery)) {
                    $solarName = $obj->name;
                    $sensorData1 = $obj->temperature;
                    $sensorData2 = $obj->humidity;
                    $sensorData3 = $obj->pressure;
                    $sensorData4 = $obj->altitude;
                    $sensorData5 = $obj->battery;

                    // Create new db record
                    $sql = "INSERT INTO sensors (name, temperature, humidity, pressure, altitude, battery, Date, Time) VALUES ('".$solarName."','".$sensorData1."', '".$sensorData2."', '".$sensorData3."', '".$sensorData4."', '".$sensorData5."', '".$d."', '".$t."')";

                    if ($conn->query($sql) === TRUE) {
                        echo "OK";
                    } else {
                        echo "Error: " . $sql . "<br>" . $conn->error;
                    }
                }

                // Close connection
                $conn->close();
            }
        }
    }
?>