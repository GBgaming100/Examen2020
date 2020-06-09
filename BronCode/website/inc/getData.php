<?php
include "db.php";
//haal alle data uit de luchttemp database
//sorteert hem meteen op het laatste inkomende data zodat de meest recente temperatuur op het scherm komt
$luchtTemp = "SELECT * FROM `luchttemp` ORDER BY `luchttemp`.`event` DESC";

//database functie voor query te runnen
$luchtTemp = connectDB($luchtTemp);

//results komen in een array vorm hij verwijdert een stap in de array
$luchtTemp = $luchtTemp[0]['buitenTemperature'];

//haal alle data uit de watertemp database
//sorteert hem meteen op het laatste inkomende data zodat de meest recente temperatuur op het scherm komt
$waterTemp = "SELECT * FROM `watertemp` ORDER BY `watertemp`.`event` DESC";

//database functie voor query te runnen
$waterTemp = connectDB($waterTemp);

//results komen in een array vorm hij verwijdert een stap in de array
$waterTemp = $waterTemp[0]['waterTemperature'];

$SchaalSwitch = "SELECT `Schaal` FROM `watertemp` ORDER BY `watertemp`.`event` DESC ";

$SchaalSwitch = connectDB($SchaalSwitch);

$SchaalSwitch = $SchaalSwitch[0]['Schaal'];

echo json_encode(Array(
    'luchtTemp' => $luchtTemp,
    'schaal' => $SchaalSwitch,
    'waterTemp' => "$waterTemp"
));
?>