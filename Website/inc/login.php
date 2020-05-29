<?php
include "db.php";

var_dump($_POST);

session_start();

$username = $_POST['username'];
$password = $_POST['pass'];

$login = false;

$i = connectDB("SELECT * FROM `users`");

foreach ($i as &$user) {
    if ($user['user'] == $username){
        if ($user['password'] == $password){
            $login = true;
        }
    }
}
if ($login = true) {
    $_SESSION['login'] = true;
    echo true;
}
elseif ($login = false){
    $_SESSION['login'] = false;
    echo false;
}