<?php
include "inc/db.php";

$i = connectDB("SELECT * FROM `kratjescount` ORDER BY `kratjescount`.`id` DESC");

$totaalKratjes = $i[0]['totaalKratjes'];

$flesjes = $totaalKratjes * 12;
$liter = $totaalKratjes * 12 * 0.33;
$kcal = $totaalKratjes * 12 * 129;
$prijs = $totaalKratjes * 12 * 0.33 * 1.67;
$prijs = number_format($prijs, 2, '.', '');


//$i = $i[1]['totaalKratjes'];

//var_dump($totaalKratjes);

//var_dump($i);
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

    <script src="https://kit.fontawesome.com/51ee248d46.js" crossorigin="anonymous"></script>

    <script src="js/main.js"></script>
</head>
<body>
<div class="bg-image"></div>
<div class="container">
    <div class="row">
        <div class="main-content">
            <div class="col-lg-12 col-sm-12 h1-add text-center">
                <h1><i class="fas fa-arrow-circle-up js-addKratje"></i></h1>
            </div>
            <div class="col-lg-12 col-sm12 text-center">
                <h2> <?= $totaalKratjes ?> Kratjes</h2>
            </div>
            <div class="col-lg-12 col-sm-12 h1-add text-center">
                <h1><i class="fas fa-arrow-circle-down js-removeKratje"></i></h1>
            </div>
        </div>
    </div>

</div>

</body>
</html>