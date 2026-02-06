<?php

$conn = new mysqli("localhost", "admin", "admin", "scanner_db");
if($conn->connect_error){
    die("DB CONNECTION FAILED");
}else{
    echo "Connection was established";
}
?>
