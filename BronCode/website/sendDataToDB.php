<?php

include "inc/db.php";

var_dump($_GET);

$sql = "INSERT INTO `watertemp` (`id`, `event`, `waterTemperature`, `Schaal`) VALUES (NULL, current_timestamp(), '".$_GET["airTemp"]."', '".$_GET["schaalSwitch"]."');";

// Execute SQL statement

connectDB($sql);

$sql = "INSERT INTO `luchttemp` (`id`, `event`, `buitenTemperature`, `Schaal`) VALUES (NULL, current_timestamp(), '".$_GET["airTemp"]."', '".$_GET["schaalSwitch"]."');";

// Execute SQL statement

connectDB($sql);


//xu84Ma@7 password db
?>