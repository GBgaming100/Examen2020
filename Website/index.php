<?php
include "inc/db.php";

//haal alle data uit de luchttemp database
//sorteert hem meteen op het laatste inkomende data zodat de meest recente temperatuur op het scherm komt
$luchtTemp = "SELECT * FROM `luchttemp` ORDER BY `luchttemp`.`event` DESC";

//database functie voor query te runnen
$luchtTemp = connectDB($luchtTemp);

//results komen in een array vorm hij verwijdert een stap in de array
$luchtTemp = $luchtTemp[0];

//haal alle data uit de watertemp database
//sorteert hem meteen op het laatste inkomende data zodat de meest recente temperatuur op het scherm komt
$waterTemp = "SELECT * FROM `watertemp` ORDER BY `watertemp`.`event` DESC";

//database functie voor query te runnen
$waterTemp = connectDB($waterTemp);

//results komen in een array vorm hij verwijdert een stap in de array
$waterTemp = $waterTemp[0];

?>
<html>
<head>
    <!--Bootstrap css-->
    <link rel="stylesheet" href="css/bootstrap.css">
    <link rel="stylesheet" href="css/bootstrap-grid.css">
    <link rel="stylesheet" href="css/bootstrap-reboot.css">
    <!--Custom css-->
    <link rel="stylesheet" href="css/style.css">

    <!-- Meta titels-->
    <meta charset="UTF-8">
    <meta name="description" content="">
    <meta name="keywords" content="">
    <meta name="theme-color" content="#000">
    <!--Title-->
    <title>Template</title>
    <!--Icon-->
    <link rel="icon" type="image/png" href="https://www.paaspop.nl/images/favicon/favicon.ico">
    <script
            src="https://code.jquery.com/jquery-3.4.1.js"
            integrity="sha256-WpOohJOqMqqyKL9FccASB9O0KwACQJpFTUBLTYOVvVU="
            crossorigin="anonymous"></script>
    <script src="js/bootstrap.js"></script>
    <script src="js/bootstrap.bundle.js"></script>

    <script src="js/main.js"></script>
</head>
<body>
<div class="bg-image"></div>
<div class="topleft">
    <p><a href="www.amxdev.nl"><img src="http://paaspop.amxdev.nl/img/AMXlogo.png" style="height: 125px"></a> </p>
</div>
<div class="container">
    <div class="row">
        <div class="main-content">
            <div class="col-lg-12 col-sm-12 text-center">
                <h1></h1>
            </div>
            <div class="col-lg-12 col-sm12 text-center">
                <h2><?= $waterTemp['waterTemperature'] ?> &#176;</h2>
                <h3>Water temperatuur</h3>
            </div>
            <div class="col-lg-12 col-sm12 text-center">
                <h2><?= $luchtTemp['buitenTemperature']?> &#176;</h2>
                <h3>Lucht temperatuur</h3>
            </div>
        </div>
    </div>
</div>
</body>
</html>