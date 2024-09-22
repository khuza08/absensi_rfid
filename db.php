<?php
$servername = "localhost"; // Ganti jika perlu
$username = "root"; // Ganti jika perlu
$password = ""; // Ganti jika perlu
$dbname = "absensi";

// Buat koneksi
$conn = new mysqli($servername, $username, $password, $dbname);

// Periksa koneksi
if ($conn->connect_error) {
    die("Koneksi gagal: " . $conn->connect_error);
}
?>
