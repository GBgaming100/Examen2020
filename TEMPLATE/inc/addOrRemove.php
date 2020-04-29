<?php

include "db.php";

$i = connectDB("SELECT * FROM `kratjescount` order by `kratjescount`.`id` DESC");

//var_dump($i);

$totaalKratjes = $i[0]['totaalKratjes'];

//$totaalKratjes = 1;

echo $_POST['addOrRemove'];

if ($_POST['addOrRemove'] == "true") {
    $kratjes = 1;
    $totaalKratjes = $totaalKratjes + $kratjes;

} else {
    $kratjes = 0;
    $totaalKratjes = $totaalKratjes - 1;

}

$date = date("Y-m-d H:i:s");

$query = "INSERT INTO `kratjescount` (`id`, `kratjes`, `added`, `totaalKratjes`, `date`) VALUES (NULL, '" . $kratjes . "', 'Max', '" . $totaalKratjes . "', '" . $date . "')";

$q = connectDB($query);


