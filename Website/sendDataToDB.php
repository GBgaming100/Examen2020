<?php

include "inc/db.php";

var_dump($_GET);

$sql = "INSERT INTO `watertemp` (`id`, `event`, `waterTemperature`) VALUES (NULL, current_timestamp(), '".$_GET["waterTemp"]."');";

// Execute SQL statement

connectDB($sql);

$sql = "INSERT INTO `luchttemp` (`id`, `event`, `buitenTemperature`) VALUES (NULL, current_timestamp(), '".$_GET["airTemp"]."');";

// Execute SQL statement

connectDB($sql);


//xu84Ma@7 password db
?>