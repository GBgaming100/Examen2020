<?php

function connectDB($sql)
{
    $connect = mysqli_connect("localhost:3306","barrekeverhoeven","zQ8g2p2_", "barrekeverhoeven"); //localhosts

    $resource = mysqli_query($connect, $sql);
    $retuning_array = array();
    while($result = mysqli_fetch_assoc($resource))
    {
        $retuning_array[] = $result;
    }
    return $retuning_array;
}
?>