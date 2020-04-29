<?php
include "inc/db.php";

$i = connectDB("SELECT * FROM `kratjescount` ORDER BY `kratjescount`.`id` DESC");

$totaalKratjes = $i[0]['totaalKratjes'];

$flesjes = $totaalKratjes * 12;
$liter = $totaalKratjes * 12 * 0.33;
$kcal = $totaalKratjes * 12 * 129;
$prijs = $totaalKratjes * 12 * 0.33 * 1.67;
$prijs = number_format($prijs, 2, '.', '');
$flesjesPerWeek = $flesjes / 52;
$flesjesPerWeek = number_format($flesjesPerWeek, 0, '.', '');
$flesjesPerDag = $flesjes / 356;
$flesjesPerDag = number_format($flesjesPerDag, 1, '.', '');


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

    <script src="js/main.js"></script>
</head>
<body>
<div class="bg-image"></div>
<div class="container">
    <div class="row">
        <div class="main-content">
            <div class="col-lg-12 col-sm-12 text-center">
                <h1><?= $totaalKratjes ?></h1>
            </div>
            <div class="col-lg-12 col-sm12 text-center">
                <h2>Kratjes</h2>
            </div>
        </div>
    </div>

</div>


<div class="stats">
    <h3>Statestieken</h3>


    <h4>Er zijn <span class="highlite"><?= $flesjes ?></span> flesjes Bier gedronken.</h4>
    <h4>Dat is <span class="highlite"><?= $liter ?></span> liter.</h4>
    <h4>Dat houdt in dat we <span class="highlite"><?= $kcal ?></span> kcal hebben geconsumeerd.</h4>
    <h4> Het komt er dus op neer dat we ongeveer <br><span class="highlite">&euro;<?= $prijs ?> </span> hebben
        uitgegeven aan bier.</h4>
    <h4>Er wordt momenteel gemiddeld <span class="highlite"><?= $flesjesPerWeek ?></span> flesjes per week gedronken.
        Dat houdt in dat er ongeveer <span class="highlite"><?= $flesjesPerDag ?></span> flesjes per dag worden
        gedronken (en ja dat moeten er meer worden).</h4>
</div>

<section class="faq-section">
    <div class="faq">

        <h2>Frequently Asked Questions</h2>

        <div class="accordion">
            <div class="accordion-item">
                <a>Why is the moon sometimes out during the day?</a>
                <div class="content">
                    <p>Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut
                        labore et dolore magna aliqua. Elementum sagittis vitae et leo duis ut. Ut tortor pretium
                        viverra suspendisse potenti.</p>
                </div>
            </div>
            <div class="accordion-item">
                <a>Why is the sky blue?</a>
                <div class="content">
                    <p>Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut
                        labore et dolore magna aliqua. Elementum sagittis vitae et leo duis ut. Ut tortor pretium
                        viverra suspendisse potenti.</p>
                </div>
            </div>
            <div class="accordion-item">
                <a>Will we ever discover aliens?</a>
                <div class="content">
                    <p>Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut
                        labore et dolore magna aliqua. Elementum sagittis vitae et leo duis ut. Ut tortor pretium
                        viverra suspendisse potenti.</p>
                </div>
            </div>
            <div class="accordion-item">
                <a>How much does the Earth weigh?</a>
                <div class="content">
                    <p>Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut
                        labore et dolore magna aliqua. Elementum sagittis vitae et leo duis ut. Ut tortor pretium
                        viverra suspendisse potenti.</p>
                </div>
            </div>
            <div class="accordion-item">
                <a>How do airplanes stay up?</a>
                <div class="content">
                    <p>Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut
                        labore et dolore magna aliqua. Elementum sagittis vitae et leo duis ut. Ut tortor pretium
                        viverra suspendisse potenti.</p>
                </div>
            </div>
        </div>

    </div>
</section>

</body>
</html>