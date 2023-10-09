<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.2/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-T3c6CoIi6uLrA9TneNEoa7RxnatzjcDSCmG1MXxSR1GAsXEV/Dwwykc2MPK8M2HN" crossorigin="anonymous">
    <link rel="stylesheet" href="index.css">
    <title>Shoto</title>
</head>
<body>
    <div class="container text-center" id="main-div">
        <div class="row">
        <div class="mpu6050 mb-2">
            <h3>MPU6050 READING..</h3>
        </div>
            <div class="acceleration box shadow p-3 mb-2 bg-white rounded">
            <h4>Acceleration:</h4>
            <h6>X: <span id="ax">0 </span> ms<sup>-2</sup></h6>
            <h6>Y: <span id="ay">0 </span> ms<sup>-2</sup></h6>
            <h6>Z: <span id="az">0 </span> ms<sup>-2</sup></h6>
            </div>

            <div class="rotation box shadow p-3 mb-2 bg-white rounded">
            <h4>Gyroscope:</h4>
            <h6>X: <span id="rx">0 </span> rad s<sup>-1</sup></h6>
            <h6>Y: <span id="ry">0 </span> rad s<sup>-1</sup></h6>
            <h6>Z: <span id="rz">0 </span> rad s<sup>-1</sup></h6>
            </div>
            
            <div class="temperature box shadow p-3 mb-2 bg-white rounded">
            <h4>Temperature:</h4>
            <h6><span id="temperature">0</span><span> &#8451;</span></h6>
            <h6>Turn OFF/ON LED:</h6>
            <div><button id="ledButton" class="btn col" onclick="toggle()">OFF</button></div>
            </div>


        </div>
    </div>
    

    <script>
        var socket = new WebSocket('your local ip');
        socket.onmessage=function(event){
            console.log(event.data);
            const data =event.data.split(":");

            const msg=data[0] || "";
            const sensor=data[1] || "";

            if(sensor == "led"){
                var button = document.getElementById("ledButton");
                button.innerHTML = msg =="1" ? "ON" : "OFF";
            }
            else if(sensor=="MPU6050"){
                var parts = msg.split(",");
				var ax = parts[0];
				var ay = parts[1];
                var az = parts[2];
                var rx = parts[3];
				var ry = parts[4];
                var rz = parts[5];
                var temperature = parts[6];
				
				document.getElementById("ax").innerHTML = ax;
				document.getElementById("ay").innerHTML = ay;
                document.getElementById("az").innerHTML = az;
                document.getElementById("rx").innerHTML = rx;
				document.getElementById("ry").innerHTML = ry;
                document.getElementById("rz").innerHTML = rz;
                document.getElementById("temperature").innerHTML =  temperature;
            }
            

        };

        function toggle(){
            var button=document.getElementById("ledButton");
            var status=button.innerHTML === "OFF" ? "1" : "0";
            socket.send(status+":led:esp:localhost");

        }
        let button = document.getElementById("ledButton");
            button.addEventListener("click", () => {
            button.classList.toggle('clicked');
        })

    </script>
</body>
</html>

<?php
    echo 'Hasantha Karunachandra';
?>